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

int max_thrust_signal = 1600;    //Signal for ESC for maximum forward thrust NOTE:MAX SIGNAL = 1900, MIN = 1100
int min_thrust_signal = 1400;    //Signal for ESC for maximum reverse thrust
int stop_thrust_signal = 1500;  //Signal for ESC to stop the thrusters

//Variables to store various signals for each thruster.
int FRT_Signal; //Front Right Thruster Signal
int FLT_Signal; //Front Left Thruster Signal
int BRT_Signal; //Back Right Thruster Signal
int BLT_Signal; //Back Left Thruster Signal

//Radio Frequency Receiver Initialization Setup

float Input_ReceivedMessage[6] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input
float Output_SentMessage[2] = {000};  //Used to send water sensor data back to transmitter/controller
RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte addresses[][6] = {"00001","00002"};  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

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
int Mid_Point_Angle = 180;

// Water Sensor Initialization and Setup

int H2O_Sensor = A0;


// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  //pinMode(led, OUTPUT);
  Serial.begin(19200);

  //H20 Sensor Setup
  pinMode(H2O_Sensor,INPUT);
  
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

  delay(7000); //Delay Required to allow the ESC to recognize the stop signal
  
  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openWritingPipe(addresses[0]); // 00002 Allows NRF24L01 to send data via Pipe 2
  radio.openReadingPipe(1, addresses[1]); // 00001 Allows the NRF24L01 ready to receive data via Pipe 1
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level,for low power consumption
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  //radio.startListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01
    
}

// the loop routine runs over and over again forever:
void loop() {
    radio.startListening();  //Start the NRF24L01 to listen for any data from the transmitter
    radio.read(&Input_ReceivedMessage,sizeof(Input_ReceivedMessage));  // "&" sign takes in the all of the data being transferred, sizeof makes sure to capture the whole data and not just specific parts
    Xaverage = Input_ReceivedMessage[0];  //Assigns input received value as the x position which should relay from transmitter code.
    Yaverage = Input_ReceivedMessage[1];  //Assigns input received value as the y position which should relay from transmitter code.
    Current_Angle = Input_ReceivedMessage[2];  //Assigns input received value as the y position which should relay from transmitter code.
    X_Signal = Input_ReceivedMessage[3];  //Assigns input received value as the y position which should relay from transmitter code.
    Y_Signal = Input_ReceivedMessage[4];  //Assigns input received value as the y position which should relay from transmitter code.
    X_Signal = Limit_Motor_Signal(X_Signal);
    Y_Signal = Limit_Motor_Signal(Y_Signal);
//    int x_signal = map(x_pos, 0, 1023,min_thrust_signal,max_thrust_signal);  //Maps the value of the analog signal to the correct ESC desired powered Signal for the X-direction
//    int y_signal = map(y_pos, 0, 1023,min_thrust_signal,max_thrust_signal);  //Maps the value of the analog signal to the correct ESC desired powered Signal for the Y-direction
//    Limit_Motor_Signal(x_signal);  //Limits the motor signals to make sure that the ESC doesn't run the motor at an unexpected PWM rate
//    Limit_Motor_Signal(y_signal);

  if(Current_Angle > AngleBound8 && Current_Angle <=AngleBound1 && Xaverage > 250){  // Clockwise (CW) Movement Quadrant, Joystick is moved to the right
//    Front_Right_Thruster.writeMicroseconds(map(X_Signal,min_thrust_signal,max_thrust_signal,max_thrust_signal,min_thrust_signal));//Needs to go opposite direction at same magnitude
//    Front_Left_Thruster.writeMicroseconds(X_Signal);
//    Back_Right_Thruster.writeMicroseconds(map(X_Signal,min_thrust_signal,max_thrust_signal,max_thrust_signal,min_thrust_signal));//Needs to go opposite direction at same magnitude
//    Back_Left_Thruster.writeMicroseconds(X_Signal);

      //Map Function Used to Flip the signal for thruster to "push" in the opposite direction
      int FRT_Signal = min_thrust_signal; //Front Right Thruster Signal
      int FLT_Signal = max_thrust_signal; //Front Left Thruster Signal
      int BRT_Signal = min_thrust_signal; //Back Right Thruster Signal
      int BLT_Signal = max_thrust_signal; //Back Left Thruster Signal
      Serial.println("Moving Right");   
      Serial.println(BLT_Signal); 
      Front_Right_Thruster.writeMicroseconds(FRT_Signal);
      Front_Left_Thruster.writeMicroseconds(FLT_Signal);
      Back_Right_Thruster.writeMicroseconds(BLT_Signal);
      Back_Left_Thruster.writeMicroseconds(BRT_Signal);
            
  }
////  else if(Current_Angle > AngleBound1 && Current_Angle <=AngleBound2){  // Drift Right (DR) Movement Quadrant (CURRENTLY NOT USED)
////    //TBD
////  }
  else if(Current_Angle > AngleBound2 && Current_Angle <=AngleBound3 && Yaverage > 250){  // Forward (FW) Movement Quadrant
//    Front_Right_Thruster.writeMicroseconds(Y_Signal);
//    Front_Left_Thruster.writeMicroseconds(Y_Signal);
//    Back_Right_Thruster.writeMicroseconds(Y_Signal);
//    Back_Left_Thruster.writeMicroseconds(Y_Signal);

      int FRT_Signal = max_thrust_signal; //Front Right Thruster Signal
      int FLT_Signal = max_thrust_signal; //Front Left Thruster Signal
      int BRT_Signal = max_thrust_signal; //Back Right Thruster Signal
      int BLT_Signal = max_thrust_signal; //Back Left Thruster Signal
      Serial.println("Moving Forward");
      Serial.println(BLT_Signal);
      Front_Right_Thruster.writeMicroseconds(FRT_Signal);
      Front_Left_Thruster.writeMicroseconds(FLT_Signal);
      Back_Right_Thruster.writeMicroseconds(BLT_Signal);
      Back_Left_Thruster.writeMicroseconds(BRT_Signal);

  }
////  else if(Current_Angle > AngleBound3 && Current_Angle <=AngleBound4){  // Drift Left (DL) Movement Quadrant (CURRENTLY NOT USED)
////    //TBD
////  }
  else if((Current_Angle > AngleBound4 && Current_Angle <= Mid_Point_Angle && Xaverage < -250) ){  // Counter-Clockwise (CCW) Movement Quadrant
//    Front_Right_Thruster.writeMicroseconds(map(X_Signal,min_thrust_signal,max_thrust_signal,max_thrust_signal,min_thrust_signal));//Needs to go opposite direction at same magnitude
//    Front_Left_Thruster.writeMicroseconds(X_Signal);
//    Back_Right_Thruster.writeMicroseconds(map(X_Signal,min_thrust_signal,max_thrust_signal,max_thrust_signal,min_thrust_signal));//Needs to go opposite direction at same magnitude
//    Back_Left_Thruster.writeMicroseconds(X_Signal);
      
      //Map Function Used to Flip the signal for thruster to "push" in the opposite direction
      int FRT_Signal = max_thrust_signal; //Front Right Thruster Signal
      int FLT_Signal = min_thrust_signal; //Front Left Thruster Signal
      int BRT_Signal = max_thrust_signal; //Back Right Thruster Signal
      int BLT_Signal = min_thrust_signal; //Back Left Thruster Signal
      Serial.println("Moving Left");
      Serial.println(BLT_Signal);
          Front_Right_Thruster.writeMicroseconds(FRT_Signal);
      Front_Left_Thruster.writeMicroseconds(FLT_Signal);
      Back_Right_Thruster.writeMicroseconds(BLT_Signal);
      Back_Left_Thruster.writeMicroseconds(BRT_Signal);
  }
////  else if(Current_Angle > AngleBound5 && Current_Angle <=AngleBound6){  // Drift Backwards Left (DBL) Movement Quadrant (CURRENTLY NOT USED)
////    //TBD
////  }
  else if(Current_Angle > AngleBound6 && Current_Angle <=AngleBound7 && Yaverage < -250){  // Backwards (BW) Movement Quadrant
//    Front_Right_Thruster.writeMicroseconds(Y_Signal);
//    Front_Left_Thruster.writeMicroseconds(Y_Signal);
//    Back_Right_Thruster.writeMicroseconds(Y_Signal);
//    Back_Left_Thruster.writeMicroseconds(Y_Signal);
      
      int FRT_Signal = min_thrust_signal; //Front Right Thruster Signal
      int FLT_Signal = min_thrust_signal; //Front Left Thruster Signal
      int BRT_Signal = min_thrust_signal; //Back Right Thruster Signal
      int BLT_Signal = min_thrust_signal; //Back Left Thruster Signal
      Serial.println("Moving Backwards");
      Serial.println(BLT_Signal);
      Front_Right_Thruster.writeMicroseconds(FRT_Signal);
      Front_Left_Thruster.writeMicroseconds(FLT_Signal);
      Back_Right_Thruster.writeMicroseconds(BLT_Signal);
      Back_Left_Thruster.writeMicroseconds(BRT_Signal);
  }
////  else if(Current_Angle > AngleBound7 && Current_Angle <=AngleBound8){  // Drift Backwards Right (DBR) Movement Quadrant (CURRENTLY NOT USED)
////    //TBD
////  }
//   
////    Serial.println("The X Position is ");
////    Serial.println(Input_ReceivedMessage[4]);
////    Serial.println("The Y Position is");
//    Serial.println(BLT_Signal);
  
//    else  //if none of the actions above are being used the tell the thrusters that they should not be moving.
//    {
//      int FRT_Signal = stop_thrust_signal; //Front Right Thruster Signal
//      int FLT_Signal = stop_thrust_signal; //Front Left Thruster Signal
//      int BRT_Signal = stop_thrust_signal; //Back Right Thruster Signal
//      int BLT_Signal = stop_thrust_signal; //Back Left Thruster Signal  
//    }
  else if(Xaverage < 250 && Xaverage > -250 && Yaverage < 250 && Yaverage > -250)
  {
      int FRT_Signal = stop_thrust_signal; //Front Right Thruster Signal
      int FLT_Signal = stop_thrust_signal; //Front Left Thruster Signal
      int BRT_Signal = stop_thrust_signal; //Back Right Thruster Signal
      int BLT_Signal = stop_thrust_signal; //Back Left Thruster Signal
      Serial.println(BLT_Signal);
      Front_Right_Thruster.writeMicroseconds(FRT_Signal);
      Front_Left_Thruster.writeMicroseconds(FLT_Signal);
      Back_Right_Thruster.writeMicroseconds(BLT_Signal);
      Back_Left_Thruster.writeMicroseconds(BRT_Signal);
  }
    Serial.println(Xaverage);
    delay(5);
    
    radio.stopListening();  //Gives the NRF24L01 the ability to send data back to the controller
    int Water_Reading = analogRead(H2O_Sensor);
    Output_SentMessage[0] = Water_Reading;
    Serial.println(Water_Reading);
    radio.write(&Output_SentMessage,sizeof(Output_SentMessage));
    delay(50);
    
  }


//////// FUNCTION PROTOTYPES  ///////// 

int Limit_Motor_Signal(int ESC_Signal){  //Limits the motor signal of maximum and minimum thrust values
  int x;
  if(x >= max_thrust_signal){
    x = max_thrust_signal;
  }
  if(x <= min_thrust_signal){
    x = min_thrust_signal;
  }
  return x;
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
