
//For Joystick
int y_key = A0;
int y_pos;
int x_key = A1;
int x_pos;

int Neutral_Bias_X = 500;
int Neutral_Bias_Y = 505;
//For Serial Command
char rx_byte = 0;

// Smoothing out the data Setup
const int numReadings = 10;

int x_pos_readings[numReadings];      // the readings from the analog input for x position
int y_pos_readings[numReadings];      // the readings from the analog input for y position
int readIndex = 0;              // the index of the current reading
int Xtotal = 0;                  // the running total for X position Values
int Ytotal = 0;                  // the running total for Y position Values
int Xaverage = 0;                // the average for X Position
int Yaverage = 0;                // the average for Y Position

void setup() {
  Serial.begin(9600);  //Set up Serial Monitor for reading
  //Initialize all readings to 0 for smoothing of input data
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    x_pos_readings[thisReading] = 0;
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    y_pos_readings[thisReading] = 0;
  }  
  //Joystick Control
  pinMode(y_key,INPUT);
  pinMode(x_key,INPUT);

}

void loop() {
  ////////// Acquisition and Smoothing of Joystick Data ////////
  //  Subtract the last reading
  Xtotal = Xtotal - x_pos_readings[readIndex];
  Ytotal = Ytotal - y_pos_readings[readIndex];
  
  //  Take a reading from the sensor
  y_pos_readings[readIndex] = analogRead(y_key)- Neutral_Bias_Y;
  x_pos_readings[readIndex] = analogRead(x_key)- Neutral_Bias_X;
  
  //  Add the newest reading to the total for both X and Y components
  Xtotal = Xtotal + x_pos_readings[readIndex];
  Ytotal = Ytotal + y_pos_readings[readIndex];
  
  //Move forward to the next element in the array
  readIndex = readIndex + 1;
  
  // if at the end of the array
  if(readIndex >= numReadings){
    //Start back again at the beginning
    readIndex = 0;
  }
  
  //Calclate the running averages for X and Y positions on Joystick
  Xaverage = Xtotal / numReadings;
  Yaverage = Ytotal / numReadings;
  int angle = (180/PI)*atan2(Yaverage,Xaverage);
//  Serial.println(y_pos);
//  Serial.println(x_pos);
  Serial.println(angle);
//int y_signal = map(y_pos,0,1023,1100,1900);
//int x_signal = map(x_pos,0,1023,1100,1900);
//int right_signal;
//int left_signal;
//
//  Limit_Motor_Signal(y_signal);
//  Limit_Motor_Signal(x_signal);



}

void Read_Serial_Monitor(){
    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
    }
}

void Limit_Motor_Signal(int x){
  if(x >= 1900){
    x = 1900;
  }
  if(x <= 1100){
    x = 1100;
  }

}
