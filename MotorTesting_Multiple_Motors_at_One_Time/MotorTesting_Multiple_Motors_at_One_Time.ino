#include <Servo.h>

byte FRONT_LEFT_THRUSTER_PIN = 9;
byte FRONT_RIGHT_THRUSTER_PIN = 8;
Servo F_Right_Thruster;
Servo F_Left_Thruster;

float Zero_Thrust = 1500;  //Equivalent of 0 on ESC
float Force = 0;    //Initialization for Force Value
float Steps = 0.5;  //Amount of Force Increase when increasing the Thrust Value.
char rx_byte = 0;   //For Serial Input Control
float MAX_FORCE_VALUE = 5;  //Max Thrust Value of Motor
float MIN_FORCE_VALUE = -5; //Min Thurst Value of Motor

void setup() {
  
  Serial.begin(9600);  //Sets up Serial Monitor
  F_Left_Thruster.attach(FRONT_LEFT_THRUSTER_PIN);
  F_Right_Thruster.attach(FRONT_RIGHT_THRUSTER_PIN);

  F_Left_Thruster.writeMicroseconds(Zero_Thrust); // send "stop" signal to ESC.
  F_Right_Thruster.writeMicroseconds(Zero_Thrust); // send "stop" signal to ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  // Max Thrust in one direction is 5, so opposite direction is negative 5.
  rx_byte = Command();  //Calling for an input from serial monitor.
  switch(rx_byte){
    case 'w': {
      Force = Force + Steps;
      if(Force >= MAX_FORCE_VALUE){
        Force = MAX_FORCE_VALUE;
      }
      break;    
    }
    case 's': {
      Force = Force - Steps;
        if(Force <= MIN_FORCE_VALUE){
          Force = MIN_FORCE_VALUE;
        }
      break;    
    }
  }
  ActivateThrusters(Force);

}

char Command() {        //Asks for  a serial input in the serial monitor.
    if (Serial.available() > 0) {
      rx_byte = Serial.read();
    }
    return rx_byte;
}

void ActivateThrusters(float Force){
  
  float Input_Value = map(Force,-5,5,1100,1900);  //1500 is equal to zero, not on
  //Serial.print(Corresponding_Value);
  Serial.print(Input_Value);
  float signal = Input_Value; // Set signal value, which should be between 1100 and 1900

// This is where the distributing will go when multiple thrusters are being used.

  F_Right_Thruster.writeMicroseconds(signal); // Combined moves the ROV forward.
  F_Left_Thruster.writeMicroseconds(signal);
}
