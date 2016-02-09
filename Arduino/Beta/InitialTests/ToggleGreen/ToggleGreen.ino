/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin GREEN_LIGHT. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#define GREEN_LIGHT 9
#define GO_BUTTON 10
int lamp_state; 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin GREEN_LIGHT as an output.
  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(GO_BUTTON, INPUT_PULLUP);
  lamp_state = LOW; 
}


// the loop function runs over and over again forever
void loop() {
  int button_state = digitalRead(GO_BUTTON);
  if(button_state == 0) {
    lamp_state = (lamp_state == LOW) ? HIGH : LOW; 
    digitalWrite(GREEN_LIGHT, lamp_state);   // toggle the lamp
    delay(1000);              // wait for a second
  }
}
