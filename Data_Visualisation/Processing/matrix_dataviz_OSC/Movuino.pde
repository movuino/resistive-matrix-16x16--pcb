import processing.serial.*;
import oscP5.*;
import netP5.*;
//Thread
MovuinoOSC movuinoOSC;
Thread movuinoThread;

void initMovuinoOSC(String ip_, int portin_, int portout_) {
  movuinoOSC = new MovuinoOSC(ip_, portin_, portout_);
  movuinoThread = new Thread(movuinoOSC);
  movuinoThread.start();
  movuinoOSC.printRawDataCollect();
}

//-----------------------------
//------- MOVUINO THREAD ------
//-----------------------------
public class MovuinoOSC implements Runnable {
  int[] rawData; // array list to store each data sent from the Movuino. The size of the list correspond to the size of the moving mean
  int nDat = COLS * ROWS + 1; // number of data received from Movuino 
  
  Thread thread;
  // OSC communication parameters
  OscP5 oscP5Movuino;
  NetAddress myMovuinoLocation;
  int portIn = 7400;
  int portOut = 7401;
  String ip;

  public MovuinoOSC(String ip_, int portin_, int portout_) {
    this.ip = ip_;
    this.portIn = portin_;
    this.portOut = portout_;
    oscP5Movuino = new OscP5(this, this.portIn); // this port must be the same than the port on the Movuino
    myMovuinoLocation = new NetAddress(this.ip, this.portOut);
    NetInfo.print();
    this.rawData = new int[nDat];
  }

  public void start() {
    thread = new Thread(this);
    thread.start();
  }

  public void run() {
    while (true) {   
      // Update display continuously
      time_sec = rawData[0];
      for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
           pointGrid[i][j].pushNewRawVal(rawData[i+1]);
        } 
      }
      delay(1); // regulation
    }
  }

  public void stop() {
    thread = null;
  }
  
  //-----------------------------
  //--------- DATA PRINT --------
  //-----------------------------

  void printInfo() {
    println("-------------------------");
  }

  void printRawDataCollect() {
    // Print raw data store from the Movuino
    for (int j=0; j < this.nDat; j++) {
      if (this.rawData != null) {
        print(this.rawData[j] + " ");
      }
      if (j==this.nDat-1) {
        println();
      }
    }
  }
  
  //-----------------------------
  //----- OSC COMMUNICATION -----
  //-----------------------------

  void sendToMovuino(String addr_, String mess_) {
    // Send messages to Movuino through OSC protocol
    OscMessage myOscMessage = new OscMessage("/" + addr_); // create a new OscMessage with an address pattern
    myOscMessage.add(mess_); // add a value to the OscMessage
    oscP5Movuino.send(myOscMessage, myMovuinoLocation); // send the OscMessage to a remote location specified in myNetAddress
  }
  
  void oscEvent(OscMessage theOscMessage) {
    // Receive data from Movuino on the channel /data
    if (theOscMessage.checkAddrPattern("/data")) {
      if (theOscMessage.checkTypetag("sfffffffffii")) {
        for (int i=0; i<nDat; i++) {
          this.rawData[i] = theOscMessage.get(i).intValue();
        }
        return;
      }
    }
    println("### received an osc message. with address pattern "+theOscMessage.addrPattern()+" and typetag "+theOscMessage.typetag());
  }
}
