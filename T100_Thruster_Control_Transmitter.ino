/*
  Source https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
*/
#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// Pin Designation for each Thruster

byte servoPin_LF = 6; //Left Forward Thruster
byte servoPin_RF = 7; //Right Forward Thruster
byte servoPin_LL = 8; //Left Back Thruster
byte servoPin_RL = 9; //Right Back Thruster

//Servo Setup for Each Thruster

Servo Left_Side_Forward_Thruster; //Orange Flag
Servo Right_Side_Forward_Thruster; //Pink Flag
Servo Left_Back_Thruster;  //Red Flag
Servo Right_Back_Thruster; //Yellow Flag

//Radio Frequency Receiver Initialization Setup

int Input_SentMessage[2] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input

RF24 radio(9,10); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const uint64_t pipe = 0xE6E6E6E6E6E6;  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

//For Joystick
int y_key = A0;
int y_pos;
int x_key = A1;
int x_pos;

//For Motor Control
int stop_signal = 1500;

void setup() {
  Serial.begin(9600);  //Set up Serial Monitor for reading

  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openReadingPipe(1,pipe);  //  Allows the NRF24L01 ready to receive data
  radio.startListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01

  //Joystick Set Up Control
  pinMode(y_key,INPUT);
  pinMode(x_key,INPUT);
  
}

void loop() {
   // Set signal value, which should be between 1100 and 1900
//  Read_Serial_Monitor();
  y_pos = analogRead(y_key);
  x_pos = analogRead(x_key);
  
int y_signal = map(y_pos,0,1023,1100,1900);
int x_signal = map(x_pos,0,1023,1100,1900);
int right_signal;
int left_signal;

Limit_Motor_Signal(y_signal);
Limit_Motor_Signal(x_signal);
  if(x_signal > 1489){
      int right_signal = map(x_signal,1489,1900,1500,1900);
      Right_Back_Thruster.writeMicroseconds(right_signal); 
  }
  if(x_signal <= 1489){
      int left_signal = map(x_signal,1489,1100,1500,1900);
       Serial.println(left_signal);
      Left_Back_Thruster.writeMicroseconds(left_signal);  
  }
  Left_Side_Forward_Thruster.writeMicroseconds(y_signal);
  Right_Side_Forward_Thruster.writeMicroseconds(y_signal); 
}

void Limit_Motor_Signal(int x){
  if(x >= 1900){
    x = 1900;
  }
  if(x <= 1100){
    x = 1100;
  }

}
