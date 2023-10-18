// ************ Description *****************

// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4


// ************ Libraries ********************
#include <Stepper.h>
#include <Servo.h>

// ********** Stepper Motor **********
const int stepsPerRevolution = 500;                     // change this to fit the number of steps per revolution

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// ********** Servo Motor **********
Servo servoArm;                                        // Servo object. Pin assignment  in setup() loop. Pin = 6

// Return servo to starting position
void lowerServoArm() {
    servoArm.write(0);                                // Lower servoArm
};

// Test function - "Flex" tower's servoArm
void flexServoArm(){
  servoArm.write(90);
  delay(500);
  servoArm.write(120);
  delay(500);
  servoArm.write(180);
  delay(500);
  servoArm.write(270);
};

void setup() {

  // Set pin 9 for servo control
  servoArm.attach(6);

  // set the speed at 60 rpm:
  myStepper.setSpeed(60);

  // initialize the serial port:
  Serial.begin(9600);

}

void loop() {

  myStepper.step(500);
  delay(500);


}