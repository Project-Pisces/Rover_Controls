#include <Servo.h>

byte servoPin = 9;
Servo servo;

float Force = 0;
float Steps = 0.1;
char rx_byte = 0;

void setup() {
  
  Serial.begin(9600);
  servo.attach(servoPin);

  servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  // Max Thrust in one direction is 5, so opposite direction is negative 5.
  rx_byte = Command();  //Calling for an input from serial monitor.
  switch(rx_byte){
    case 'w': {
      Force = Force + Steps;
      if(Force >= 5){
        Force = 5;
      }
      break;    
    }
    case 's': {
      Force = Force - Steps;
        if(Force <= -5){
          Force = -5;
        }
      break;    
    }
  }

  float Corresponding_Value = map(Force,-5,5,1100,1900);  //1500 is equal to zero, not on
  //Serial.print(Corresponding_Value);
  Serial.print(Corresponding_Value);
  Serial.print('\n');
  float signal = Corresponding_Value; // Set signal value, which should be between 1100 and 1900

  servo.writeMicroseconds(signal); // Send signal to ESC.
  
}

char Command() {        //Asks for  a serial input in the serial monitor.
    if (Serial.available() > 0) {
      rx_byte = Serial.read();
    }
    return rx_byte;
}
