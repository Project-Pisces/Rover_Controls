int LED = 8;
int H2O = A4;

int Max_Water_Signal = 300;
int ledState = LOW;  
unsigned long previousMillis = 0; 
const long interval = 250;     
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  
  pinMode(LED, OUTPUT);
  pinMode(H2O, INPUT);
  
}

// the loop function runs over and over again forever
void loop() {

  int Water_Reading = analogRead(H2O);
  Serial.println(Water_Reading);
  Serial.println(ledState);
  if(Water_Reading > Max_Water_Signal){
    
//  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(500);                       // wait for a second
//  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
//  delay(500);                       // wait for a second
  
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
  if(Water_Reading <= Max_Water_Signal){
    digitalWrite(LED,LOW);
  }
  }

}
