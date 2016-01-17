/*
  IR Sensor test (for line follower)
 
 
 */


// motor configuration
// Given these motor pins, it is important NOT to use
// analog input 0, as it gets fouled by the DIR output
// pins somehow...
#define VEL 60 // 65 followed the line! 70 did too V=65/gd=10  V=70/gd=10 (V=80/g=8 does not work-- timing loop is too slow...)
// So, I think we need to speed up the analog2digital conversion rate -- the 
// 60/gd10 wobbled a lot.
// 60/gd20 missed the line so did 12
// 60 / 8 -- ?
#define PGain_DIV 8
#define IGain_DIV 10000
#define DGain_DIV(a) ((a * 3) >> 1)

const int Dir1A = 12;

const int Left_Motor = 11; 
const int Dir2A = 13;

const int Right_Motor = 3;

// do this for debug...
#include <LiquidCrystal.h>

#define NUM_SENSORS 3
#define HIST_SIZE 256
#define HIST_LIM (HIST_SIZE - 1)

char needs_calibration; 

LiquidCrystal lcd(8,9,4,5,6,7);
static FILE lcdout = {
  0};

class Sensor {
public:
  Sensor() {
    clearHist();
    ch = -1;
    sense_range = 0; 
  }

  void init(int analog_chan) {
    ch = analog_chan;
    sense_range = 0; 
  }

  void collectCalInfo() {
    // readings are 10 bits... 
    hist[(cur_reading >> 2) & 0xff]++; 
  }

  bool completeCal() {
    int i; 
    int top_peak = 0; 
    int top_idx = 0; 
    int bot_peak = 0; 
    int bot_idx = 0; 
    int found_top = 0; 
    int found_bot = 0; 

    for(i = 0; i < HIST_SIZE; i++) {
      if(found_top == 0) {
        // look for top value
        if(hist[HIST_LIM - i] > top_peak) {
          top_peak = hist[HIST_LIM - i];
          top_idx = HIST_LIM - i; 
        }
        else if(hist[HIST_LIM - i] < (top_peak >> 2)) {
          found_top = 1; 
        }
      }

      if(found_bot == 0) {
        // look for bottom value
        if(hist[i] > bot_peak) {
          bot_peak = hist[i];
          bot_idx = i;
        }
        else if(hist[i] < (bot_peak >> 2)) {
          found_bot = 1; 
        }
      }
    }
    Serial.print("FT,FB,TI,TP  ");
    Serial.print(found_top); Serial.print(" ");
    Serial.print(found_bot); Serial.print(" ");
    Serial.print(top_idx); Serial.print(" ");
    Serial.println(top_peak);
    
    if(found_top && found_bot) {
      sense_low = (bot_idx << 2);
      sense_range = (top_idx - bot_idx) << 2;
    }
    
    for(i = 0; i < HIST_SIZE; i++) hist[i] = 0; 
    
    return true; 
  }

  bool calOK() {
    return (sense_range > 10);
  }

  long getVal() {
    long v = (signed long) cur_reading; 
    v = (v - sense_low) * 1000; 
    v = v / sense_range;
    if(v < 0) return 0; 
    else if(v > 1000) return 1000; 
    else return v; 
  }

  unsigned int read() {
    cur_reading = analogRead(ch); 
    return cur_reading;
  }

private:
  void clearHist() {
    int i; 
    for(i = 0; i < HIST_SIZE; i++) hist[i] = 0;
  }
public:
  int ch; 
  unsigned int hist[HIST_SIZE]; 
  unsigned int cur_reading; 
  unsigned int sense_range; 
  unsigned int sense_low; 
}; 

template < int NumSensors > class SensorArray {
public:
  SensorArray(int * ad_ids) {
    int i; 
    for(i = 0; i < NumSensors; i++) sensors[i].init(ad_ids[i]);
    last_position = 2000;
    cal_step_count = 0; 
  }

  void scanSensors() {
    int i; 
    for(i = 0; i < NumSensors; i++) sensors[i].read(); 
  }

  void collectCalInfo() {
    int i; 
    for(i = 0; i < NumSensors; i++) {
      sensors[i].collectCalInfo();
    }
    cal_step_count++; 
    if(cal_step_count >= 256) {
      completeCal(); 
    }
  }

  bool completeCal() {
    int i;  
    for(i = 0; i < NumSensors; i++) {
      sensors[i].completeCal();
      Serial.print(i); Serial.print(" "); Serial.print(sensors[i].sense_low); Serial.print(" "); Serial.println(sensors[i].sense_range);
    }
    cal_step_count = 0; 
    return calOK(); 
  }

  bool calOK() {
    int i; 
    bool flag = true; 
    for(i = 0; i < NumSensors; i++) {
      if(sensors[i].calOK()) { Serial.print(i); Serial.println("OK"); }
      flag = flag && sensors[i].calOK(); 
    }
    return flag; 
  }

  // returns a value in range 0..4000
  long calcPosition() {
    int i; 
    long v = 0; 
    long sum = 0; 
    long wsum = 0; 
    bool found_line = false;
    for(i = 0; i < NumSensors; i++) {
      v = sensors[i].getVal();
      if(v > 800) found_line = true; 
      sum += v; 
      wsum += i * 1000 * v; 
    }

    lcd.setCursor(0,0);
    fprintf(&lcdout, "P %4ld %4ld %4ld", sensors[0].getVal(), sensors[1].getVal(), sensors[2].getVal());

    if(!found_line) {
      if(last_position < 100) {
        return 0; 
      }
      else if(last_position > ((NumSensors - 1) * 1000 - 100)){
        return (NumSensors - 1) * 1000;
      }
      else {
        return last_position;
      }
    }
    else {
      last_position = wsum / sum;
      return last_position;      
    }
  }

  Sensor sensors[NumSensors]; 
  long last_position;
  int cal_step_count; 
};

#define Rbase 10.0
#define Rup 100.0
#define Rdown  75.0
#define Rleft  51.0 
#define Rright 39.0 
#define Rselect  15.0 

#define B_UP 1
#define B_DOWN 2
#define B_LEFT 4
#define B_RIGHT 8
#define B_SEL 16

int sensor_ads[] = {
  A1, A2, A3};
SensorArray<3> sensors(sensor_ads); 

//
// This is a bit elaborate, but the code here
// generates a lookup table for A/D readings from the
// LCD button matrix output.  The table contains the
// expected A/D output for each of the 32 possible
// combinations of keypress.  
//
// This code was inspired by the example code at
// http://linksprite.com/wiki/index.php5?title=16_X_2_LCD_Keypad_Shield_for_Arduino_V2
// but substantially reorganized to provide a more useful output from
// getKey().
// 

int KeyTable[32];

void generateKeyTable(int vcc,int* array)
{
  float resistor;

  int i; 
  array[0] = vcc; 
  for(i = 1; i < 32; i++) {
    float rt = 0.0; 
    float rb = 0.0; 
    rb = Rbase;
    if(i & B_UP) rt += 1.0 / Rup;
    if(i & B_DOWN) rt += 1.0 / Rdown;
    if(i & B_LEFT) rt += 1.0 / Rleft;
    if(i & B_RIGHT) rt += 1.0 / Rright;
    if(i & B_SEL) rt += 1.0 / Rselect;

    if(rt != 0.0) rt = 1.0 / rt; 

    resistor = vcc * rt / (rb + rt); 
    array[i] = (int) resistor;  
  }

}

//
// Using the table generated by GenerateKeyTable, find
// the entry whose value is closest to the output of the A/D
// converter.   If this entry is X, then
// UP    was pressed if bit 0 of X is set
// DOWN  was pressed if bit 1 of X is set
// LEFT  was pressed if bit 2 of X is set
// RIGHT was pressed if bit 3 of X is set
// SEL   was pressed if bit 4 of X is set.
// 
unsigned char getKey(int value)
{
  char tmpChar;
  unsigned int Rst;

  // find the nearest 

    unsigned char res = 0; 
  unsigned int nearest = 1023; 
  int i; 
  for(i = 0; i < 32; i++) {
    int diff; 
    if(value > KeyTable[i]) diff = value - KeyTable[i];
    else diff = KeyTable[i] - value; 

    if(diff < nearest) {
      res = i; 
      nearest = diff;
    }
  }

  return (unsigned char) res; 
}

// helper routine for formatted LCD output
static int lcd_putchar(char ch, FILE*stream)
{
  lcd.write(ch);
  return(0);
}

int integral; 
int last_proportional; 

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);

  Serial.print("Starting up... ");
  Serial.println("");

  generateKeyTable(analogRead(A0), KeyTable);

  lcd.begin(16,2);
  lcd.print("Hello");
  // setup formatted IO for the LCD and serial streams.
  fdev_setup_stream(&lcdout, lcd_putchar, NULL, _FDEV_SETUP_WRITE);

  pinMode(Dir1A, OUTPUT);

  pinMode(Dir2A, OUTPUT);


  pinMode(Left_Motor, OUTPUT);
  pinMode(Right_Motor, OUTPUT);

  needs_calibration = 1; 
  
  // set PWM rate to 4kHz or so. 
  TCCR2B = (TCCR2B & 0xF8) | 2;
  // whould ou believe 122 Hz
  TCCR2B = (TCCR2B & 0xF8) | 6; // 122 is good, is 30 better? 
  
  motorsStop();
  analogWrite(Left_Motor, 0);
  analogWrite(Right_Motor, 0);
  
  integral = 0; 
  last_proportional = 0; 

}

float last_err = 0.0; 
void motorsForward()
{

  digitalWrite(Dir1A, HIGH);
  digitalWrite(Dir2A, HIGH);

}

void motorsStop()
{    
  digitalWrite(Dir1A, LOW);
  digitalWrite(Dir2A, LOW);
}

long err; 

void handle_keys()
{
#if 0
  unsigned char key = getKey(analogRead(A0));
  if(key != 0) {
    if((key & B_UP) != 0) {
      Kp += 10; 
    }
    else if((key & B_DOWN) != 0) {
      Kp -= 10;
    }
    else if((key & B_LEFT) != 0) {
      Kp += 100;
    }
    else if((key & B_RIGHT) != 0) {
      Kp -= 100;
    }
    lcd.setCursor(0,0);
    long iKp = (long) Kp; 
    fprintf(&lcdout, "Kp = %ld ", iKp);
    delay(1000);
  }
#endif
}

void chase_line()
{
  // lifted from 3pi linefollower code. 
  long pos = sensors.calcPosition(); 
  int proportional = ((int) pos) - 1000;
  int derivative = proportional - last_proportional;
  integral += proportional; 
  last_proportional = proportional; 
  int power_difference;
  //power_difference = proportional / 8 + integral / 10000 + derivative * 3 / 2;
  power_difference = proportional / PGain_DIV + DGain_DIV(derivative) + integral / IGain_DIV;
  
  const int max = 40;
  if(power_difference > max) power_difference = max; 
  if(power_difference < -max) power_difference = -max; 
  lcd.setCursor(0,1);
  fprintf(&lcdout, "pp %4ld %4d", pos, power_difference);
  
  if(power_difference > 0) {
    analogWrite(Left_Motor, VEL);
    analogWrite(Right_Motor, VEL - power_difference);
  }
  else {
    analogWrite(Left_Motor, VEL + power_difference);
    analogWrite(Right_Motor, VEL);
  }
}

void loop()
{
  handle_keys();

  sensors.scanSensors();

  if(!needs_calibration || sensors.calOK()) {
    if(needs_calibration) {
      delay(5000);
    }
    chase_line();
    needs_calibration = 0; 
    motorsForward(); 
  }
  else {
    sensors.collectCalInfo(); 
    lcd.setCursor(0,0);
    int v = (sensors.sensors[2].calOK() ? 4 : 0) + (sensors.sensors[1].calOK() ? 2 : 0) + (sensors.sensors[0].calOK() ? 1 : 0);
    fprintf(&lcdout, "M%4d%4d%4d %d", sensors.sensors[0].cur_reading, sensors.sensors[1].cur_reading, sensors.sensors[2].cur_reading, v);
    delay(20);
  }

}


