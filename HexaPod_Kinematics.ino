#include <Wire.h>
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

int defaultAngle = 90, riseAngle_4_6 = 150, riseAngle_1_3 = 30, rollAngle1 = 130, rollAngle2 = 50,
timeDelay = 2000;
// Variables definitions:
// defaultAngle --> The default angle for all legs, don't change it!

// riseAngle_1_3 --> The rise anlge for the legs from 1 - 3 

// riseAngle_4_6 --> The rise anlge for the legs from 4 - 6 

// rollAngle1 --> The rolling anlge to move the leg to front (in case of looking at the front of the Hexa) 
// it could move the leg to the back in case your're looking at the back of the Hexa

// rollAngle2 --> The rolling anlge to move the leg to back (opposite effect for rollAngle1)

// timeDelay --> The time gap between each part of the movement 

// A class defining the leg of the HexaPod part, made for easy use, edits, and useability 
class Leg{
  public:
  int roll, rise;

  // Set the pins for rise joint and roll joint of the leg 
  void prepare(int rollBin, int riseBin){
  rise = riseBin;
  roll = rollBin;
  }
  
};

// A class defining the the HexaPod object, made for easy use, edits, and useability. Easy setup for HexaPod legs and controlling functionality. 
class HexaPod{
  public:
  Leg leg1, leg2, leg3, leg4, leg5, leg6;

  // set the Pins for all the 6 legs of the Hexa
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

// Create HexaPod object 
HexaPod hexa;

void stepBackward(){
  // StepForward pt1

  // Rise even legs of the ground
  setServoAngle(hexa.leg2.rise, riseAngle_1_3);
  setServoAngle(hexa.leg4.rise, riseAngle_4_6);
  setServoAngle(hexa.leg6.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll even legs to the front where should land later 
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  
  // Roll odd legs to the back, to generate a pushing effect for the HexaPod 
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs to their starting point 
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  delay(timeDelay);
  
  // stepForward pt2

  // Rise odd legs of the ground
  setServoAngle(hexa.leg1.rise, riseAngle_1_3);
  setServoAngle(hexa.leg3.rise, riseAngle_1_3);
  setServoAngle(hexa.leg5.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll odd legs to the front where should land later 
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);

  // Roll even legs to the back, to generate a pushing effect for the HexaPod  
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);

  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs to their starting point 
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
  
  // Rise even legs of the ground
  setServoAngle(hexa.leg2.rise, riseAngle_1_3);
  setServoAngle(hexa.leg4.rise, riseAngle_4_6);
  setServoAngle(hexa.leg6.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll even legs to the back where should land later 
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);

  // Roll odd legs to the front, to generate a pushing effect for the HexaPod 
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs to their starting point 
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  delay(timeDelay);
  
  // stepForward pt2

  // Rise odd legs of the ground 
  setServoAngle(hexa.leg1.rise, riseAngle_1_3);
  setServoAngle(hexa.leg3.rise, riseAngle_1_3);
  setServoAngle(hexa.leg5.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll even legs to the front where should land later 
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);

  // Roll odd legs to the back, to generate a pushing effect for the HexaPod 
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull legs to their starting point 
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

  // Rise even legs of the ground
  setServoAngle(hexa.leg2.rise, riseAngle_1_3);
  setServoAngle(hexa.leg4.rise, riseAngle_4_6);
  setServoAngle(hexa.leg6.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll right side legs
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);

  // Roll left side legs
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);
  delay(timeDelay);

  // Put down even legs
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull all the legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
  
  // spinRight pt2

  // Rise odd legs of the ground 
  setServoAngle(hexa.leg1.rise, riseAngle_1_3);
  setServoAngle(hexa.leg3.rise, riseAngle_1_3);
  setServoAngle(hexa.leg5.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll right side legs
  setServoAngle(hexa.leg1.roll, rollAngle1);
  setServoAngle(hexa.leg2.roll, rollAngle1);
  setServoAngle(hexa.leg3.roll, rollAngle1);

  // Roll left side legs
  setServoAngle(hexa.leg4.roll, rollAngle2);
  setServoAngle(hexa.leg5.roll, rollAngle2);
  setServoAngle(hexa.leg6.roll, rollAngle2);
  delay(timeDelay);

  // Put down odd legs
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull all the legs
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

  // Rise even legs of the ground
  setServoAngle(hexa.leg2.rise, riseAngle_1_3);
  setServoAngle(hexa.leg4.rise, riseAngle_4_6);
  setServoAngle(hexa.leg6.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll right side legs
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);

  // Roll left side legs
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  delay(timeDelay);

  // Put down even legs of the ground
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
  delay(timeDelay);

  // Pull all the legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);
  delay(timeDelay);
  
  // spinRight pt2

  // Rise odd legs of the ground 
  setServoAngle(hexa.leg1.rise, riseAngle_1_3);
  setServoAngle(hexa.leg3.rise, riseAngle_1_3);
  setServoAngle(hexa.leg5.rise, riseAngle_4_6);
  delay(timeDelay);

  // Roll right side legs
  setServoAngle(hexa.leg1.roll, rollAngle2);
  setServoAngle(hexa.leg2.roll, rollAngle2);
  setServoAngle(hexa.leg3.roll, rollAngle2);

  // Roll left side legs
  setServoAngle(hexa.leg4.roll, rollAngle1);
  setServoAngle(hexa.leg5.roll, rollAngle1);
  setServoAngle(hexa.leg6.roll, rollAngle1);
  delay(timeDelay);

  // Put down odd legs 
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);
  delay(timeDelay);

  // Pull all the legs
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  delay(timeDelay);
}

// use to reset all angles to the defaults and finout the indices for each leg.
// Should be used only once at the first, then reset the legs on this angle 
void resetAngles(){
  setServoAngle(0, defaultAngle);
  delay(timeDelay);

  setServoAngle(1, defaultAngle);
  delay(timeDelay);

  setServoAngle(2, defaultAngle);
  delay(timeDelay);

  setServoAngle(3, defaultAngle);
  delay(timeDelay);

  setServoAngle(4, defaultAngle);
  delay(timeDelay);

  etServoAngle(5, defaultAngle);
  delay(timeDelay);

  setServoAngle(6, defaultAngle);
  delay(timeDelay);

  setServoAngle(7, defaultAngle);
  delay(timeDelay);

  setServoAngle(8, defaultAngle);
  delay(timeDelay);

  setServoAngle(9, defaultAngle);
  delay(timeDelay);

  setServoAngle(10, defaultAngle);
  delay(timeDelay);

  setServoAngle(11, defaultAngle);
  delay(timeDelay);
}

// use this to make the HexaPod Stand up and wake up :)
// Should only be used when starting the Bot from sleeping (idle)
// Will make the Hexa stand up and set all angles to its starting location to start working fine later 
void standUp(){
  // Stand Up Pt1

  // Rise the odd legs of the ground 
  setServoAngle(hexa.leg1.rise, riseAngle_1_3);
  setServoAngle(hexa.leg3.rise, riseAngle_1_3);
  setServoAngle(hexa.leg5.rise, riseAngle_4_6);

  // Roll the odd legs to where they should be (default location)
  setServoAngle(hexa.leg1.roll, defaultAngle);
  setServoAngle(hexa.leg3.roll, defaultAngle);
  setServoAngle(hexa.leg5.roll, defaultAngle);

  // Put the odd legs on the ground in order to make the Hexa stand up
  setServoAngle(hexa.leg1.rise, defaultAngle);
  setServoAngle(hexa.leg3.rise, defaultAngle);
  setServoAngle(hexa.leg5.rise, defaultAngle);

  // Stand Up Pt2

  // Rise the even legs of the ground 
  setServoAngle(hexa.leg2.rise, riseAngle_1_3);
  setServoAngle(hexa.leg4.rise, riseAngle_4_6);
  setServoAngle(hexa.leg6.rise, riseAngle_4_6);

  // Roll the even legs to where they should be (default location)
  setServoAngle(hexa.leg2.roll, defaultAngle);
  setServoAngle(hexa.leg4.roll, defaultAngle);
  setServoAngle(hexa.leg6.roll, defaultAngle);

  // Put the even legs on the ground in order to make the Hexa stand up
  setServoAngle(hexa.leg2.rise, defaultAngle);
  setServoAngle(hexa.leg4.rise, defaultAngle);
  setServoAngle(hexa.leg6.rise, defaultAngle);
}

void setup() {
  pwm.begin();	
  pwm.setPWMFreq(SERVO_FREQ);

  Serial.begin(9600);

  // Prepare the HexaPod, pass the indices as pairs for each leg (rise joint bin , roll joint bin) from leg 1 to leg 6
  hexa.prepare(0, 1, 2, 3, 4, 5, 
              6, 7, 8, 9, 10, 11);

  // Wake your kid up :)
  standUp();
}

void loop() {
  // Work is done here 
  stepForward();
}