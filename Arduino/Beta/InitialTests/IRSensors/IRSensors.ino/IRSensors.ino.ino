/*
 * Test the IR sensors ... note first channel is a little odd. ...  A1--.
 * Otherwise sensing appears to work. 
 */

int sensor_names[5]; 
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  sensor_names[0] = A1;
  sensor_names[1] = A0;
  sensor_names[2] = A2; 
  sensor_names[3] = A6;
  sensor_names[4] = A7;
}

int sensors[5]; 
// the loop routine runs over and over again forever:
void loop() {
  int i;
  delay(10);
  for(i = 0; i < 5; i++) {
    sensors[i] = analogRead(sensor_names[i]);
    delay(1);
  }

  for(i = 0; i < 5; i++) {
    Serial.print(sensors[i]); Serial.print(" ");
  }
  Serial.println(" ");
  delay(1000);   
}
