
#include <SoftwareSerial.h>

#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4
#define SFX_ACT 2
SoftwareSerial ss(SFX_TX, SFX_RX); 

char lb[128]; 
void readLine()
{
  int ep = ss.readBytesUntil('\n', lb, 128);
  lb[ep] = '\000';
}
int setVolume(int tv)
{
  while(ss.available()) ss.read();
  int i; 

  for(i = 0; i < 256; i++) {
    ss.println("-");
    readLine();
    int v = atoi(lb);
    if(v <= tv) return v; 
  }
  return 1024; 
}
void setup() {

  ss.begin(9600);

  Serial.begin(115200);
    // put your setup code here, to run once:
  pinMode(SFX_RST, OUTPUT);
  digitalWrite(SFX_RST, LOW);
  digitalWrite(SFX_RST, HIGH);

  pinMode(SFX_ACT, INPUT);
  
  delay(1500);
  while(ss.available() > 0) {
    char c = ss.read();
    Serial.print(c);
  }
  Serial.println("Setting volume to 150");
  setVolume(120);
  while(ss.available() > 0) {
    char c = ss.read();
    Serial.print(c);
  }

  int k; 
  for(k = 0; k < 20; k++) {
  Serial.println("About to send a song request.");
  
  ss.println("PDIGIT999WAV");
  int dlycount;
  dlycount = 0;
  while(digitalRead(SFX_ACT)) {
    //delay(10);
    dlycount++; 
  }
  Serial.print(dlycount); Serial.println(" counts before falling edge.");
  dlycount = 0; 
  while(!digitalRead(SFX_ACT)) {
    //delay(10);
    dlycount++; 
  }
  Serial.print(dlycount); Serial.println(" counts before rising edge.");

  Serial.println("About to request a second.");
  ss.println("PDIGIT000WAV");
  dlycount = 0;
  while(digitalRead(SFX_ACT)) {
    //delay(10);
    dlycount++; 
  }
  Serial.print(dlycount); Serial.println(" counts before falling edge.");
  dlycount = 0; 
  while(!digitalRead(SFX_ACT)) {
    //delay(10);
    dlycount++; 
  }
  Serial.print(dlycount); Serial.println(" counts before rising edge.");
  } 
  //ss.println("PDIGIT222WAV");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (ss.available() > 0) {
    char c = ss.read();
    Serial.print(c);
  }
}
