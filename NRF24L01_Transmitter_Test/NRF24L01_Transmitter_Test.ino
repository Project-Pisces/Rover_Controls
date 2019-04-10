#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

int x = 0;

//Radio Frequency Transmitter Initialization Setup

int Input_SentMessage[2] = {000};  //Used to store value received by NRF24L01 for X and Y input, Element 0 = X Input, Element 1 = Y Input

RF24 radio(7,8); //Setup of the RF24 radio on SPI pin 9 and 10 on the Arduino Uno
const byte address[6] = "00001";  //Radio Channel that is used for communicating the 2 NRF24L01 Transmitter and Receiver

int brightness1 = 0;    // how bright the LED is
int brightness2 = 255;

// the setup routine runs once when you press reset:
void setup() {

  //Radio Setup
  radio.begin();  // Start up the NRF24L01
  radio.openWritingPipe(address[1]);  //  Allows the NRF24L01 ready to receive data
  radio.setPALevel(RF24_PA_MIN); // Lowest Possible Power Level
  radio.setDataRate(RF24_1MBPS); //Set to the middle transmission speed
  radio.setChannel(124);  //Sets RF Channel pick one that is not similar to Wi-Fi and Microwave Ovens
  radio.stopListening();   // Start the NRF24L01 to begin listening for any data being transmitted from the Transmitting NRF24L01

}

// the loop routine runs over and over again forever:
void loop() {
    
  if(x = 0)
  {
    Input_SentMessage[0] = brightness1;
    radio.write(&Input_SentMessage,sizeof(Input_SentMessage));
    x = 1;
  }
  if(x = 1)
  {
    Input_SentMessage[0] = brightness2;
    x = 0;
    radio.write(&Input_SentMessage,sizeof(Input_SentMessage));
  }
  delay(1000);
  
  
  

  
}
