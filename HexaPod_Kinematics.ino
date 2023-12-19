[11:03 PM, 12/17/2023] محمود وائل: #include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Define the PCA9685 PWM controller
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// PWM parameters
#define SERVOMIN  150  // Minimum pulse length
#define SERVOMAX  600  // Maximum pulse length
#define SERVO_FREQ 60   // Analog servos run at 60 Hz updates

void setPWM(int channel, int on, int off) {
  pwm.setPWM(channel, on, off);
}

void setServoAngle(int servoNum, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  setPWM(servoNum, 0, pulse);
}

int defaultAngle = 90, riseAngle = 150, rollAngle1 = 130, rollAngle2 = 50,
timeDelay = 2000;

class Leg{
  public:
  int roll, rise;

  void prepare(int rollBin, int riseBin){
  rise = riseBin;
  roll = rollBin;
  // setServoAngle…
[11:04 PM, 12/17/2023] محمود وائل: #include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Define the PCA9685 PWM controller
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// PWM parameters
#define SERVOMIN  150  // Minimum pulse length
#define SERVOMAX  600  // Maximum pulse length
#define SERVO_FREQ 60   // Analog servos run at 60 Hz updates

void setPWM(int channel, int on, int off) {
  pwm.setPWM(channel, on, off);
}

void setServoAngle(int servoNum, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  setPWM(servoNum, 0, pulse);
}

int defaultAngle = 90, riseAngle = 150, rollAngle1 = 130, rollAngle2 = 50,
timeDelay = 2000;

class Leg{
  public:
  int roll, rise;

  void prepare(int rollBin, int riseBin){
  rise = riseBin;
  roll = rollBin;
  }
  
};

class HexaPod{
  public:
  Leg leg1, leg2, leg3, leg4, leg5, leg6;

void prepare(int riseBin1, int rollBin1, int riseBin2, int rollBin2,
  int riseBin3, int rollBin3, int riseBin4, int rollBin4,
  int riseBin5, int rollBin5, int riseBin6, int rollBin6){
    leg1.prepare(riseBin1, rollBin1);
    leg2.prepare(riseBin2, rollBin2);
    leg3.prepare(riseBin3, rollBin3);
    leg4.prepare(riseBin4, rollBin4);
    leg5.prepare(riseBin5, rollBin5);
    leg6.prepare(riseBin6, rollBin6);
  }
};

HexaPod hexa;

void stepBackward(){
  // stepForward pt1
  // Rise even legs 
  setServoAngle(hexa.leg2.rise, riseAngle);
  setServoAngle(hexa.leg4.rise, riseAngle);
  setServoAngle(hexa.leg6.rise, riseAngle);
  delay(timeDelay);

  // Roll even legs back
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  
  // Roll odd legs front
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  delay(timeDelay);
  
  // stepForward pt2
  // Rise odd legs 
  setServoAngle(hexa.leg1.rise, riseAngle);
  setServoAngle(hexa.leg3.rise, riseAngle);
  setServoAngle(hexa.leg5.rise, riseAngle);
  delay(timeDelay);

  // Roll even legs front
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);

  // Roll odd legs back
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  delay(timeDelay);
}

void stepForward(){
  // stepForward pt1
  // Rise even legs 
  setServoAngle(hexa.leg2.rise, 30);
  setServoAngle(hexa.leg4.rise, riseAngle);
  setServoAngle(hexa.leg6.rise, riseAngle);
  delay(timeDelay);

  // Roll even legs back
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);

  // Roll odd legs front
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  delay(timeDelay);
  
  // stepForward pt2
  // Rise odd legs 
  setServoAngle(hexa.leg1.rise, 30);
  setServoAngle(hexa.leg3.rise, 30);
  setServoAngle(hexa.leg5.rise, riseAngle);
  delay(timeDelay);

  // Roll even legs front
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);

  // Roll odd legs back
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull odd legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);

  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
}

void spinRight(){
  // spinRight pt1
  // Rise even legs
  setServoAngle(hexa.leg2.rise, riseAngle);
  setServoAngle(hexa.leg4.rise, riseAngle);
  setServoAngle(hexa.leg6.rise, riseAngle);
  delay(timeDelay);

  // Roll right legs
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);

  // Roll left legs
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
  
  // spinRight pt2
  // Rise odd legs 
  setServoAngle(hexa.leg1.rise, riseAngle);
  setServoAngle(hexa.leg3.rise, riseAngle);
  setServoAngle(hexa.leg5.rise, riseAngle);
  delay(timeDelay);

  // Roll right legs
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);

  // Roll left legs
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);
  delay(timeDelay);

  // Put down odd legss
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
}

void spinLeft(){
  // spinRight pt1
  // Rise even legs 
  setServoAngle(hexa.leg2.rise, riseAngle);
  setServoAngle(hexa.leg4.rise, riseAngle);
  setServoAngle(hexa.leg6.rise, riseAngle);
  delay(timeDelay);

  // Roll right legss
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);

  // Roll left legs
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  delay(timeDelay);

  // Put down even legss
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
  
  // spinRight pt2
  // Rise odd legs 
  setServoAngle(hexa.leg1.rise, riseAngle);
  setServoAngle(hexa.leg3.rise, riseAngle);
  setServoAngle(hexa.leg5.rise, riseAngle);
  delay(timeDelay);

  // Roll right legs
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);

  // Roll left legs
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  delay(timeDelay);
}


void setup() {
pwm.begin();	
pwm.setPWMFreq(SERVO_FREQ);
Serial.begin(9600);
delay(timeDelay);
hexa.prepare(0, 1, 2, 3, 4, 5, 
            6, 7, 8, 9, 10, 11);
}

void loop() {
  stepForward();
}