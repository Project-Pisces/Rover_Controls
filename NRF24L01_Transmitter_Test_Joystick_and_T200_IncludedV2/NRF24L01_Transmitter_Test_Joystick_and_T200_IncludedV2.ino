#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//Radio Frequency Transmitter Initialization Setu
float Input_SentMessage[6] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input
float Output_ReceivedMessage[2] = {000};  //Used to store datavalues that is recevied from ROV for Water Sensor
RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte addresses[][6] = {"00001", "00002"};  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

//For Joystick
int y_key = A0;
int y_pos;
int x_key = A2;
int x_pos;

int Neutral_Bias_X = 500;
int Neutral_Bias_Y = 505;

// Smoothing out the data Setup
const int numReadings = 5;

int x_pos_readings[numReadings];      // the readings from the analog input for x position
int y_pos_readings[numReadings];      // the readings from the analog input for y position
int readIndex = 0;              // the index of the current reading
int Xtotal = 0;                  // the running total for X position Values
int Ytotal = 0;                  // the running total for Y position Values
int Xaverage = 0;                // the average for X Position
int Yaverage = 0;                // the average for Y Position
int Current_Angle;


// ESC Readable Signal
int max_thrust_signal = 1700;  //Signal for ESC for maximum forward thrust NOTE:MAX SIGNAL = 1900, MIN = 1100
int min_thrust_signal = 1300;  //Signal for ESC for maximum reverse thrust
int stop_thrust_signal = 1500; //Signal for ESC to stop the thrusters
int X_Signal;
int Y_Signal;

// LED SETUP
int LED = 6;
int Max_Water_Signal = 300;  //Max Signal for Water Sensor
int ledState = LOW;  
unsigned long previousMillis = 0; 
const long interval = 250; //LED Blinking Time Interval

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);  //For Transmitter value Testing

  //LED Setup
  pinMode(LED,OUTPUT);

  //Initialize all readings to 0 for smoothing of input data
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    x_pos_readings[thisReading] = 0;
  }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    y_pos_readings[thisReading] = 0;
  }  
    
  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openWritingPipe(addresses[1]); // 00001  Allows Transmitter to send data via PIPE 1
  radio.openReadingPipe(1, addresses[0]); // 00002  Allows the NRF24L01 ready to receive data via PIPE 2
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  //radio.stopListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01
  
  //Joystick Control Set Up
  pinMode(y_key,INPUT);
  pinMode(x_key,INPUT);
  
}

// the loop routine runs over and over again forever:
void loop() {
  radio.stopListening();
  
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

  Current_Angle = (180/PI)*atan2(Yaverage,Xaverage);
  
 ////////// Conversion of Input Data into Readable Signal for ESC Motor Control ////////

 X_Signal = map(Xaverage,-Neutral_Bias_X,Neutral_Bias_X,min_thrust_signal,max_thrust_signal);
 Y_Signal = map(Yaverage,-Neutral_Bias_Y,Neutral_Bias_Y,min_thrust_signal,max_thrust_signal);
// X_Signal = Limit_Motor_Signal(X_Signal);
// Y_Signal = Limit_Motor_Signal(Y_Signal);
  
  ////////// Formatting of Data and Sending of Data via RadioWaves  /////////
  Input_SentMessage[0] = Xaverage;  //Assigns the first element of the message being sent with the x position on the joystick
  Input_SentMessage[1] = Yaverage;  //Assigns the second element of the message being sent with the y position on the joystick
  Input_SentMessage[2] = Current_Angle;  //Assigns the second element of the message being sent with the y position on the joystick
  Input_SentMessage[3] = X_Signal;  //Assigns the second element of the message being sent with the y position on the joystick
  Input_SentMessage[4] = Y_Signal;  //Assigns the second element of the message being sent with the y position on the joystick
  radio.write(&Input_SentMessage,sizeof(Input_SentMessage)); 
  //Serial.println(Input_SentMessage[3]);
  delay(5);

  radio.startListening();
  radio.read(&Output_ReceivedMessage,sizeof(Output_ReceivedMessage));
  int Water_Reading = Output_ReceivedMessage[0];  //Stores the element of data sent from the ROV to Water_Reading
  //Serial.println(Water_Reading);
  
    if(Water_Reading > Max_Water_Signal){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
          // save the last time you blinked the LED
          previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
            ledState = HIGH;
        } 
        else {
            ledState = LOW;
        }
        // set the LED with the ledState of the variable:
        digitalWrite(LED, ledState);
      }
    }
    Serial.println(ledState);
  delay(5);
}
//////////  FUNCTION PROTOTYPES  /////////

int Limit_Motor_Signal(int x){  //Limits the motor signal of maximum and minimum thrust values
  if(x >= max_thrust_signal){
    x = max_thrust_signal;
  }
  else if(x <= min_thrust_signal){
    x = min_thrust_signal;
  }
  return x;
}
