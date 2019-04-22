#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//READ REFERENCE PAGE ATTACHED TO GOOGLE DRIVE FOR UROV3 AND MOTOR CONTROL INSTRUCTIONS AND CODE DETAILS

//Motor Setup
byte Thruster1 = 3;  //Front Right Thruster
byte Thruster2 = 4;  //Front Left Thruster
byte Thruster3 = 5;  //Back Right Thruster
byte Thruster4 = 6;  //Back Left Thruster
Servo Front_Right_Thruster;
Servo Front_Left_Thruster;
Servo Back_Right_Thruster;
Servo Back_Left_Thruster;

int max_thrust_signal = 1900;    //Signal for ESC for maximum forward thrust
int min_thrust_signal = 1100;    //Signal for ESC for maximum reverse thrust
int stop_thrust_signal = 1500;  //Signal for ESC to stop the thrusters
//Radio Frequency Receiver Initialization Setup

float Input_ReceivedMessage[6] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input
RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte address[6] = "00001";  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

//Variable Initialization
int Yaverage;  //Y position from analog stick input
int Xaverage;  //X position from analog stick input
int Current_Angle; //Angle used to determine which movement control should be used.
int X_Signal; //ESC Signal from average X joystick value
int Y_Signal; //ESC Signal from average Y joystick value

////Movement Intervals used for controlling the 8 different actions and motions of the ROV.  REFERENCED ON REFERENCE PAGE.
//The Angles are used to figure out the X and Y positional values for each specific direction movement regions.
int AngleBound1 = 23;
int AngleBound2 = 68;
int AngleBound3 = 113;
int AngleBound4 = 158;
int AngleBound5 = -157;
int AngleBound6 = -112;
int AngleBound7 = -67;
int AngleBound8 = -22;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  //pinMode(led, OUTPUT);
  Serial.begin(9600);

  //Attaching Various Servos to Pin Signal Assignments
  Front_Right_Thruster.attach(Thruster1);
  Front_Left_Thruster.attach(Thruster2);
  Back_Right_Thruster.attach(Thruster3);
  Back_Left_Thruster.attach(Thruster4);

  //Set all motor speeds to zero during start up
  Front_Right_Thruster.writeMicroseconds(stop_thrust_signal);
  Front_Left_Thruster.writeMicroseconds(stop_thrust_signal);
  Back_Right_Thruster.writeMicroseconds(stop_thrust_signal);
  Back_Left_Thruster.writeMicroseconds(stop_thrust_signal);
  
  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openReadingPipe(1,address[0]);  //  Allows the NRF24L01 ready to receive data
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level,for low power consumption
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  radio.startListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01
    
}

// the loop routine runs over and over again forever:
void loop() 
   {
    radio.read(&Input_ReceivedMessage,sizeof(Input_ReceivedMessage));  // "&" sign takes in the all of the data being transferred, sizeof makes sure to capture the whole data and not just specific parts
    Xaverage = Input_ReceivedMessage[0];  //Assigns input received value as the x position which should relay from transmitter code.
    Yaverage = Input_ReceivedMessage[1];  //Assigns input received value as the y position which should relay from transmitter code.
    Current_Angle = Input_ReceivedMessage[2];  //Assigns input received value as the y position which should relay from transmitter code.
    X_Signal = Input_ReceivedMessage[3];  //Assigns input received value as the y position which should relay from transmitter code.
    Y_Signal = Input_ReceivedMessage[4];  //Assigns input received value as the y position which should relay from transmitter code.
    
//    int x_signal = map(x_pos, 0, 1023,min_thrust_signal,max_thrust_signal);  //Maps the value of the analog signal to the correct ESC desired powered Signal for the X-direction
//    int y_signal = map(y_pos, 0, 1023,min_thrust_signal,max_thrust_signal);  //Maps the value of the analog signal to the correct ESC desired powered Signal for the Y-direction
//    Limit_Motor_Signal(x_signal);  //Limits the motor signals to make sure that the ESC doesn't run the motor at an unexpected PWM rate
//    Limit_Motor_Signal(y_signal);


    if( 






    
//    Serial.println("The X Position is ");
//    Serial.println(Input_ReceivedMessage[0]);
//    Serial.println("The Y Position is");
//    Serial.println(Input_ReceivedMessage[1]);
    delay(10);
    
  }


//////// FUNCTION PROTOTYPES  ///////// 

void Limit_Motor_Signal(int x){  //Limits the motor signal of maximum and minimum thrust values
  if(x >= max_thrust_signal){
    x = 1900;
  }
  if(x <= min_thrust_signal){
    x = 1100;
  }
}

//Function Prototype
//int Quadrant_Calculation_X(int Degree){  //Function to calculate the X positions of the movement interval bounds based on the angle.
//  int x;
//  if(Degree >= 0 && Degree <=90){
//    x = 512 + 512*cos(Degree*2*PI/360);
//  }
//  else if(Degree > 90 && Degree <= 270){
//    x = 512 + 512*cos(Degree*2*PI/360);
//  }
//  else if(Degree > 270 && Degree <= 360){
//    x = 512 + 512*cos(Degree*2*PI/360); 
//  }
//  return x;
//}
//
//int Quadrant_Calculation_Y(int Degree){
//  int y;
//  if(Degree >= 0 && Degree <=180){
//    y = 512 + 512*sin(Degree*2*PI/360);
//  }  
//  else if(Degree > 180 && Degree <= 360){
//    y = 512 + 512*sin(Degree*2*PI/360);
//  }
//  return y; 
//}
