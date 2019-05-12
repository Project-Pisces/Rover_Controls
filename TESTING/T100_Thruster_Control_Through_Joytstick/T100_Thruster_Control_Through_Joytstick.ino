#include <Servo.h>

byte servoPin_LF = 6; //Left Forward Thruster
byte servoPin_RF = 7; //Right Forward Thruster
byte servoPin_LL = 8; //Left Lateral Thruster
byte servoPin_RL = 9; //Right Lateral Thruster

Servo Left_Side_Forward_Thruster; //Orange Flag
Servo Right_Side_Forward_Thruster; //Pink Flag
Servo Left_Lateral_Thruster;  //Red Flag
Servo Right_Lateral_Thruster; //Yellow Flag

//For Joystick
int y_key = A0;
int y_pos;
int x_key = A1;
int x_pos;

//For Serial Command
char rx_byte = 0;

//For Motor Control
int stop_signal = 1500;

void setup() {
  Serial.begin(9600);  //Set up Serial Monitor for reading

  //Motor Setup
  Left_Side_Forward_Thruster.attach(servoPin_LF);
  Right_Side_Forward_Thruster.attach(servoPin_RF); 
  Left_Lateral_Thruster.attach(servoPin_LL);
  Right_Lateral_Thruster.attach(servoPin_RL); 

  //Set motor speed to zero
  Left_Side_Forward_Thruster.writeMicroseconds(stop_signal);
  Right_Side_Forward_Thruster.writeMicroseconds(stop_signal); 
  Left_Lateral_Thruster.writeMicroseconds(stop_signal);
  Right_Lateral_Thruster.writeMicroseconds(stop_signal); 
  delay(7000); // delay to allow the ESC to recognize the stopped signal

  //Joystick Control
  pinMode(y_key,INPUT);
  pinMode(x_key,INPUT);
  
}

void loop() {
   // Set signal value, which should be between 1100 and 1900
  Read_Serial_Monitor();
  y_pos = analogRead(y_key);
  x_pos = analogRead(x_key);
//  switch (rx_byte){
//    case '+':
//      signal = signal + 100;
//      if(signal >= 1900){
//        signal = 1900;
//      }
//    break;
//    case '-':
//      signal = signal - 100;
//      if(signal <= 1100){
//        signal = 1100;
//      }
//    break;
//  }
  
int y_signal = map(y_pos,0,1023,1100,1900);
int x_signal = map(x_pos,0,1023,1100,1900);
int right_signal;
int left_signal;

  Limit_Motor_Signal(y_signal);
  Limit_Motor_Signal(x_signal);
  if(x_signal > 1489){
      int right_signal = map(x_signal,1489,1900,1500,1900);
      Right_Lateral_Thruster.writeMicroseconds(right_signal); 
  }
  if(x_signal <= 1489){
      int left_signal = map(x_signal,1489,1100,1500,1900);
       Serial.println(left_signal);
      Left_Lateral_Thruster.writeMicroseconds(left_signal);  
  }
  Left_Side_Forward_Thruster.writeMicroseconds(y_signal);
  Right_Side_Forward_Thruster.writeMicroseconds(y_signal); 



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
