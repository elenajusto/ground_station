// ************ Description *****************
// Program:     Satellite Code
// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4

// ************ Libraries ********************
#include <Servo.h>
#include <Wire.h>         
#include <SparkFun_MMA8452Q.h>                    // Includes the SFE_MMA8452Q library

// ************ Variables ********************

int satLED = 4;                                   // Digital output pin for LED

Servo satArm;                                     // Servo object. Pin assignment  in setup() loop. Pin = 9

MMA8452Q accel;                                   // Instance of the MMA8452Q (accelero) class

// ************ Functions ********************

void moveSatArm(int position) {
  switch(position) {
    case 0:
      satArm.write(0);
      break;
    case 90:
      satArm.write(90);
      break;
    case 180:
      satArm.write(180);
      break;
  }
}

void setup() {

  // Initialise the serial port:
  Serial.begin(9600);

  // Initialise accelerometer class - Default init. Full-scale range of +/-2g, and an output data rate of 800 Hz (fastest).
  accel.init();

  // Servo motor setup
  satArm.attach(9);                             // Set pin 9 for servo control

  // LED turns on upon start
  digitalWrite(satLED, HIGH);                   
}

void loop() {

  digitalWrite(satLED, LOW);
  Serial.println("Move arm to 0");
  moveSatArm(0);
  delay(1000);

  digitalWrite(satLED, HIGH);
  Serial.println("Move arm to 90");
  moveSatArm(90);
  delay(1000);

  digitalWrite(satLED, LOW);
  Serial.println("Move arm to 180");
  moveSatArm(180);
  delay(1000);

  digitalWrite(satLED, HIGH);
  Serial.println("Move arm to 90");
  moveSatArm(90);
  delay(1000);
}
