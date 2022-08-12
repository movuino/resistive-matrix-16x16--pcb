#include "_MOVUINO_SHIELDS/_ResistiveMatrix.h"

// Command for serial messages
#define CMD_STOP_RECORD 'S'  // Stop the record
#define CMD_START_RECORD 'G' // Start the record

//Force Sensitive Resistor Matrix dimensions
#define ROWS 4
#define COLS 6

MovuinoResistiveMatrix matrix = MovuinoResistiveMatrix(ROWS, COLS);

long timerRecord0;

bool isRecording = false;

void setup()
{
  Serial.begin(115200);
  matrix.begin();
}

void loop()
{
   if(isRecording){
    matrix.update();
    matrix.printData();
   }
 
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
    case CMD_STOP_RECORD:
      isRecording = false;
      break;
    case CMD_START_RECORD:
      isRecording = true;
      break;
    default: 
      break;
    }
  }
}
