import processing.serial.*;

// Serial communication
Serial myPort; // Serial communication with touch sensor
ArduinoSerial arduinoSerial;
Thread serialThread;
String serialData = "";

// Colors
color red = color(245, 91, 85);
color blue = color(125, 222, 227);
color yellow = color(243, 240, 114);
color green = color(0, 255, 127);
color purple = color(73, 81, 208);
color white = color(255, 255, 255);

// Size
int maxDiameter = 75;       // Max range to display data point

// Sensors
DataPoint[][] pointGrid;    // Array of data points from the touch surface
int ROWS = 4;              // number of data point on X axis for touch surface
int COLS = 6;              // number of data point on Y axis for touch surface
int dataCounter = 0;        // count number of incoming data
long timerDataCounter0 = 0; // timer to compute incoming data rate

Table table; //For writing to the .csv file
TableRow newRow;
String colId = "";
boolean isRecording = false;
boolean isOnOff = false;
float time_sec = 0;
int recordCount = 0;

//Record button parameters
int button_d = 30;
int button_x1 = 10 + button_d/2;
int button_y1 = 10 + button_d/2;
int button_x2 = button_x1 + 3*button_d/2;
int button_y2 = button_y1;
RecordButton  play = new RecordButton(button_x1, button_y1, button_d); 
RecordButton rec = new RecordButton(button_x2, button_y2, button_d);

int w = COLS*100;
int h = ROWS*100 + 2*button_d + 10;

void setup()
{
  // Set size
  size(600, 450);
  maxDiameter = int(width / float(COLS) - 10);

  // Set data point grid
  pointGrid = new DataPoint[ROWS][COLS];
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      pointGrid[i][j] = new DataPoint(i, j);
    }
  }

  // Set Table 
  table = new Table();
  table.addColumn("time");
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLS; j++){
      colId = "x" + str(j) + "y" + str(i);
      table.addColumn(colId);
    }
  }

  // Set serial communication with touch sensors
  printArray(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200); // initialize serial communication
  arduinoSerial = new ArduinoSerial();
  serialThread = new Thread(arduinoSerial);
  serialThread.start();                       // start serial thread
 
  timerDataCounter0 = millis();

  myPort.clear();
}

void draw()
{
  background(purple);

  play.displayOnOff(isOnOff);
  rec.displayRecord(isRecording);
  
  // Update values at each draw even if no new values are coming to generate smooth animation
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      pointGrid[i][j].shiftRawVal();
    }
  }

  // Remap and display data points
  for (int i = 0; i < ROWS; i++) {
    // Get row data range
    float minRow_ = 1000.0;
    float maxRow_ = 0.0;
    int sumRow_ = 0;
    for (int j = 0; j < COLS; j++) {
      sumRow_ += pointGrid[i][j].curSRelativeVal;

      if (minRow_ > pointGrid[i][j].curSRelativeVal) {
        minRow_ = pointGrid[i][j].curSRelativeVal;
      }
      if (maxRow_ < pointGrid[i][j].curSRelativeVal) {
        maxRow_ = pointGrid[i][j].curSRelativeVal;
      }
    }

    // Get remap values for the current row and display data point
    for (int j = 0; j < COLS; j++) {
      pointGrid[i][j].curRemapVal = (pointGrid[i][j].curSRelativeVal - minRow_) / (maxRow_ - minRow_);
      pointGrid[i][j].curRemapVal *= sumRow_;
      //pointGrid[i][j].curRemapVal 4096.0; // 12-bit ADC (for full range values)
      pointGrid[i][j].curRemapVal /= 500.0; // for finger testing
      pointGrid[i][j].curRemapVal = constrain(pointGrid[i][j].curRemapVal, 0.0, 1.0);

      pointGrid[i][j].display(maxDiameter); // display data point
      
      //Recording the dataPoints' smooth values
      if(isRecording){
        if (i == 0  && j == 0){
          newRow = table.addRow();
          newRow.setFloat("time",(float)(millis() - time_sec)/1000);
        }
        String id = "x" + str(j) + "y" + str(i);
        newRow.setInt(id, (int)pointGrid[i][j].getSmoothVal());
      }       
    }
  }

  if (millis() - timerDataCounter0 > 1000) {
    println("serial speed = ", int(1000 * dataCounter / (millis() - timerDataCounter0)), "data/seconde");
    timerDataCounter0 = millis();
    dataCounter = 0;
  }
}

void mouseClicked(){
  if (mouseX > button_x1 - button_d/2 && mouseX < button_x1 + button_d/2 && 
      mouseY > button_y1 - button_d/2 && mouseY < button_y1 + button_d/2){ 
    if(isOnOff){
      isOnOff = false;
      play.displayOnOff(isOnOff);
      myPort.write('S'); //Stop sending data
    }
    else{
      isOnOff = true;
      play.displayOnOff(isOnOff);
      myPort.write('G'); //Go and start sending data
    }
  } 
  if (mouseX > button_x2 - button_d/2 && mouseX < button_x2 + button_d/2 && 
      mouseY > button_y2 - button_d/2 && mouseY < button_y2 + button_d/2){ 
    if(isRecording){
      recordCount++;
      isRecording = false;
      rec.displayRecord(isRecording);
      String fileName = "data/record" + str(recordCount) + ".csv";
      saveTable(table, fileName);
      table.clearRows();
    }
    else{
      isRecording = true;
      time_sec = millis();
      rec.displayRecord(isRecording);
    }
  } 
}

void serialEvent(Serial myPort) {
  String message = myPort.readStringUntil(13);
  
  if (message != null)
  {
    //println(message);
    serialData = message;
    //serialData = message.substring( 0, message.length()-1 ); // remove 'q' character
    myPort.clear();
  }
}
