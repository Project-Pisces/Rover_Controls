#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

//Radio Frequency Receiver Initialization Setup

int Input_ReceivedMessage[2] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input
RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte address[6] = "00001";  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

//LED SETUP

int led = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
//  // declare pin 9 to be an output:
//  pinMode(led, OUTPUT);
  Serial.begin(9600);
  
  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openReadingPipe(1,address[0]);  //  Allows the NRF24L01 ready to receive data
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  radio.startListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01
    
}

// the loop routine runs over and over again forever:
void loop() 
   {
    radio.read(&Input_ReceivedMessage,sizeof(Input_ReceivedMessage));  // "&" sign takes in the all of the data being transferred, sizeof makes sure to capture the whole data and not just specific parts
    Serial.println(Input_ReceivedMessage[0]);
    
  }
