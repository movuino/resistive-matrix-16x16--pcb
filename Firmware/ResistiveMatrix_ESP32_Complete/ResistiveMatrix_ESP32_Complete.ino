//#include "_MOVUINO_ESP32/_MPU9250.h"
//#include "_MOVUINO_ESP32/_WifiOSC.h"
#include "_MOVUINO_ESP32/_Button.h"
#include "_MOVUINO_ESP32/_Recorder.h"
#include "_MOVUINO_ESP32/_Neopixel.h"
#include "_MOVUINO_ESP32/_BatteryManager.h"
#include "_MOVUINO_SHIELDS/_ResistiveMatrix.h"

// Color swap
#define WHITE255 ((255 << 16) | (255 << 8) | 255)
#define RED ((255 << 16) | (0 << 8) | 0)
#define GREEN ((0 << 16) | (250 << 8) | 0)
#define YELLOW ((200 << 16) | (175 << 8) | 0)
#define BLUE ((0 << 16) | (0 << 8) | 255)
#define MAGENTA ((255 << 16) | (0 << 8) | 255) 

// Command for serial messages
#define CMD_FORMAT_SPIFF 'f' // Format the SPIFF
#define CMD_CREATE_FILE 'c'  // Create a new file in the SPIFF
#define CMD_READ_FILE 'r'    // Read the file
#define CMD_ADD_LINE 'a'     // Add a new line in the SPIFFS (usefull for debugging)
#define CMD_STOP_RECORD 's'  // Stop the record
#define CMD_LISTING_DIR 'l'  // List files in the directory
#define CMD_SPIFF_INFO 'i'   // Get informations about the spiff
#define CMD_START_RECORD 'g' // Start the record
#define CMD_DELETE_ALL_FILES 'd' //Delete all files in the SPIFFS

// Wifi configuration
char ssid[] = "my-wifi";
char pass[] = "password";
int port = 5555;
int ip[4] = {192, 168, 1, 18};

//Force Sensitive Resistor Matrix dimensions
#define ROWS 4
#define COLS 6

//MovuinoMPU9250 mpu = MovuinoMPU9250();
// MovuinoWifiOSC osc = MovuinoWifiOSC(ssid, pass, ip, port);
//MovuinoButton button = MovuinoButton();
MovuinoRecorder recorder = MovuinoRecorder();
MovuinoNeopixel neopix = MovuinoNeopixel();
BatteryManager batt_mana = BatteryManager();
MovuinoResistiveMatrix matrix = MovuinoResistiveMatrix(ROWS, COLS);

bool isBtnHold = false;

long timerRecord0;

String recordId = "ResistiveMatrix";
String colsId = "col_1" ;
char colNum[4];
bool colsDone = false;

int timeHoldCallib = 1800;

uint32_t colOn = BLUE;
uint32_t colRec = RED;
uint32_t colCallib = MAGENTA;
uint32_t colReadFiles = YELLOW;
uint32_t colFormat = RED;

void setup()
{
  Serial.begin(115200);
  /*pinMode(BATTERY_PIN, INPUT);
  
  // Neopixel
  neopix.begin();
  neopix.setBrightness(5);
  showBatteryLevel();
  normalMode();
  freezBlink(2);
  neopix.update();
  
  // Other
  //mpu.begin();
  // osc.begin();
  button.begin();
  recorder.begin();*/
  matrix.begin();
  //freezBlink(4);
  //Serial.println("Setup Done!");
}

void loop()
{
  /*if (!colsDone){
    for (int i=1; i<COLS; i++){
      colsId += ",col_";
      sprintf(colNum,"%d",i+1);
      colsId += colNum;
    }
    colsDone = true;
  }*/
  // -----------------------------------------
  //                TEST
  // -----------------------------------------
   matrix.update();
   matrix.printData();
   //Serial.println(matrix.getValue(2,3));
   /*if(matrix.getValue(2,3)>200) {
     neopix.turnOn();
     neopix.setColor((uint32_t)MAGENTA);
     neopix.forceUpdate();
    } 
    else {
     neopix.turnOn();
     neopix.setColor((uint32_t)BLUE);
     neopix.forceUpdate();
    }
   neopix.turnOff();

  // -----------------------------------------
  //                UPDATES
  // -----------------------------------------
  neopix.update();
  button.update();

  
  // -----------------------------------------
  //                SERIAL
  // -----------------------------------------
  if (Serial.available() > 0)
  {
    char serialMessage = Serial.read();
    Serial.print("\n");
    Serial.print("Message received : ");
    Serial.println(serialMessage);

    //--------- Serial command -------------
    switch (serialMessage)
    {
    case CMD_CREATE_FILE:
      Serial.println("Creation of ");
      recorder.newRecord(recordId);
      break;
    case CMD_READ_FILE:
      Serial.println("reading all recorded files ");
      readAllfiles();
      break;
    case CMD_FORMAT_SPIFF:
      Serial.println("Formating the SPIFFS (data files)...");
      freezColorStrob(5, colFormat);
      neopix.setColor(colFormat);
      neopix.forceUpdate();
      recorder.formatSPIFFS();
      normalMode();
      break;
    case CMD_LISTING_DIR:
      Serial.println("Listing directory");
      neopix.blinkOn(50, 2);
      recorder.listDirectory();
      break;
    case CMD_SPIFF_INFO:
      Serial.println("Print info SPIFFS");
      neopix.blinkOn(50, 2);
      recorder.printStateSPIFFS();
      break;
    case CMD_ADD_LINE:
      neopix.blinkOn(50, 2);
      recorder.addRow();
      break;
    case CMD_STOP_RECORD:
      stopRecord();
      break;
    case CMD_START_RECORD:
      startRecord();
      break;
    case CMD_DELETE_ALL_FILES:
      recorder.deleteAllFiles();
    default: 
      break;
    }
  }

  // -----------------------------------------
  //                RECORDER
  // -----------------------------------------
  if (button.isReleased())
  {
    if (!isBtnHold)
    {
      if (!recorder.isRecording())
      {
        startRecord();
      }
      else
      {
        stopRecord();
      }
    }
    isBtnHold = false;
  }

  if (recorder.isRecording())
  {
    if (millis() - timerRecord0 > 10)
    {
      matrix.update();  
      for(int i=0; i<ROWS; i++){
        recorder.addRow();
        for(int j=0; j<COLS; j++){
          recorder.pushData<float>(matrix.getValue(i,j));
        }
      }
    }
  }*/
  /*
  // -----------------------------------------
  //               CALLIBRATION
  // -----------------------------------------
  if (button.timeHold())
  {
    // Color shade
    float r_ = (button.timeHold() - 400) / (float)timeHoldCallib;
    neopix.lerpTo(colCallib, r_);

    if (button.timeHold() > timeHoldCallib)
    {
      neopix.setColor(colCallib); // lock color
      if (button.timeHold() > timeHoldCallib + 20)
      {
        isBtnHold = true;
        freezBlink(2);
        if (!recorder.isRecording())
        {
          mpu.magnometerCalibration();
          button.reset(); // force reset
          neopix.blinkOn(100, 2);
          normalMode();
        }
      }
    }
  }*/
}

void normalMode() {
  neopix.setColor(colOn);
}

void startRecord()
{
  recorder.newRecord(recordId);
  recorder.defineColumns(colsId);
  
  freezColorStrob(2, RED);
  neopix.rainbowOn();
  neopix.breathOn(1000, 0.8);
}

void stopRecord()
{
  freezColorStrob(2, GREEN);
  neopix.rainbowOff();
  neopix.breathOff();
  recorder.stop();
  normalMode();
}

void readAllfiles()
{
  freezColorStrob(2, colReadFiles);
  neopix.setColor(colReadFiles);
  neopix.forceUpdate();

  recorder.readAllRecords();
  
  freezBlink(3);
  normalMode();
}

void freezBlink(int nblink_)
{
  for (int i = 0; i < nblink_; i++)
  {
    neopix.turnOff();
    neopix.forceUpdate();
    delay(50);
    neopix.turnOn();
    neopix.forceUpdate();
    delay(50);
  }
}

void freezColorStrob(int nblink_, uint32_t color_)
{
  uint32_t curCol_ = neopix.getColor();
  for (int i = 0; i < nblink_; i++)
  {
    neopix.setColor(color_);
    neopix.forceUpdate();
    delay(100);
    neopix.setColor(curCol_);
    neopix.forceUpdate();
    delay(100);
  }
}

void showBatteryLevel(void)
{
  int level;
   
  level = batt_mana.getBatteryLevel();
  
  if (level >= 50)
    neopix.setColor((uint32_t)GREEN);
  else if (level >= 25)
    neopix.setColor((uint32_t)YELLOW);
  else
    neopix.setColor((uint32_t)RED);
    
  neopix.forceUpdate();
  delay(2500);
  Serial.printf("Battery Level: %d%%\n", level);
  delay(2500);
}
