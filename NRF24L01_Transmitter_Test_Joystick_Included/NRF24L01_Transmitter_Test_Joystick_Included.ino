#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

int x = 0;

//Radio Frequency Transmitter Initialization Setup

float Input_SentMessage[6] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input

RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte address[6] = "00001";  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

//For Joystick
int y_key = A0;
int y_pos;
int x_key = A1;
int x_pos;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);  //For Transmitter value Testing
  
  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openWritingPipe(address[1]);  //  Allows the NRF24L01 ready to receive data
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  radio.stopListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01
  
  //Joystick Control
  pinMode(y_key,INPUT);
  pinMode(x_key,INPUT);
  
}

// the loop routine runs over and over again forever:
void loop() {
  y_pos = analogRead(y_key);  //Reads the Y value on the joystick and assigns that value to a variable
  x_pos = analogRead(x_key);  //Reads the X value on the joystick and assigns that value to a variable
//  Serial.println("\nThe X Position is ");
//  Serial.println(x_pos);
//  Serial.println("\nThe Y Position is");
//  Serial.println(y_pos);
  Input_SentMessage[0] = x_pos;  //Assigns the first element of the message being sent with the x position on the joystick
  Input_SentMessage[1] = y_pos;  //Assigns the second element of the message being sent with the y position on the joystick
  radio.write(&Input_SentMessage,sizeof(Input_SentMessage)); 
  delay(10);
    
}
