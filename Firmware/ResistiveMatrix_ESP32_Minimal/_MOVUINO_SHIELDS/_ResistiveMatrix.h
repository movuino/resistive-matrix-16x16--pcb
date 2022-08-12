#if !defined(_MOVUINOESP32_RESISTIVEMATRIX_)
#define _MOVUINOESP32_RESISTIVEMATRIX_

//COL MUX pins
#define S0  33
#define S1  32 
#define S2  26 
#define S3  27 
int controlPin[] = {S0, S1, S2, S3};
#define SIG_pin  25

//ROW MUX pins
#define  A   13 
#define  B 	  4 
#define  C 	 15 
#define  D 	 14  
#define COM  12 

int muxChannel[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};

class MovuinoResistiveMatrix
{
private:
	int _rows = 0; //Number of rows
	int _cols = 0; //Number of columns
	
	int _sensorValue = 0; 
	
	int** _lastUpdate;
	
public:
	MovuinoResistiveMatrix(int rows, int cols);
	~MovuinoResistiveMatrix();
	
	void begin();
	void update();
	void printData();
	int readMux(int channel);
	int getValue(int row, int col);
};

MovuinoResistiveMatrix::MovuinoResistiveMatrix(int rows, int cols)
{
	this->_rows = rows;
	this->_cols = cols;
	this->_lastUpdate = new int*[rows];
	for (int i=0; i<rows; i++){
		_lastUpdate[i] = new int[cols];
	}	
}
	
MovuinoResistiveMatrix::~MovuinoResistiveMatrix(){
}

void MovuinoResistiveMatrix::begin()
{
  //--------------------------------
  //-------MULTIPLEXER COL----------
  //--------------------------------
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SIG_pin, INPUT);
  digitalWrite(SIG_pin,0); //Pull down
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  //--------------------------------
  //------MULTIPLEXER ROW-----------
  //--------------------------------
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(COM, OUTPUT);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);;
  digitalWrite(COM, LOW);
}

void MovuinoResistiveMatrix::update()
{
   for (int row = 0; row < this->_rows; row++) {
    
	digitalWrite(A,muxChannel[row][0]);
    digitalWrite(B,muxChannel[row][1]);
    digitalWrite(C,muxChannel[row][2]);
    digitalWrite(D,muxChannel[row][3]);
    
	//ACTIVATE ROW
    digitalWrite(COM, 1);

    // READ LINE VALUES
    for (int col = 0; col < this->_cols; col++) {
      _lastUpdate[row][col] = readMux(col);
    }
	
    //DEACTIVATE ROW
    digitalWrite(COM, 0);
  }
}

int MovuinoResistiveMatrix::readMux(int channel) {
  for (int i = 0; i < 4; i ++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  } //read the value at the SIG pin

  int val = analogRead(SIG_pin); //return the value
  return val;
}

void MovuinoResistiveMatrix::printData()
{
  this->update();
  for (int i=0; i< this->_rows; i++){
	Serial.print("z");
	Serial.print(i);
	for (int j=0; j< this->_cols; j++){
		Serial.print("x");
		Serial.print(this->_lastUpdate[i][j]);
	}
	Serial.println();
  }
}

int MovuinoResistiveMatrix::getValue(int row, int col)
{
	this->update();
	return _lastUpdate[row][col];
}

#endif