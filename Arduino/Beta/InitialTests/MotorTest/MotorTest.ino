#define LDIR 12
#define LPWM 3
#define RDIR 13
#define RPWM 11



void setup() {
  // put your setup code here, to run once:
  pinMode(LDIR, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);

  digitalWrite(LDIR, HIGH);
  digitalWrite(RDIR, HIGH);
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, LOW);

  
  analogWrite(LPWM, 1023);
  analogWrite(RPWM, 1023);
}

void loop() {
  // put your main code here, to run repeatedly:

}
