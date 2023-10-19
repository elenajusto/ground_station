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
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

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

// State - Horizontal Scan
void horizontalScan() {
  
  int horizontalDegrees = 180;          // Number of degrees to cover in scan

  // Position to starting degree (0)

  for degree in horizontalDegrees {

    // Position to degree

    // Call vertical scan
    
    // Check what vertical scan says

      // If verticalScan found satellite - End state

      // If verticalScan has not found satellite

        //Continue to next degree
  };
};

// State - Vertical Scan
bool verticalScan() {

  int verticalDegrees = 180;           // Number of degrees to cover in scan

  // Position to starting degree (90)

  for degree in verticalDegrees {
    // Position to degree

    // Call IR transmitter [checkSignal]

    // Check what checkSignal returns

      // If checkSignal found satellite - Return True

      // If checkSignal did not find satellite 

        // Continue to next degree
  
  // Return to horizontalScan

  };
};

// Sends a ping to satellite and awaits confirmation signal
bool checkSignal(){
  // Send ping

  // Wait for ping receipt

  // Is ping receipt received?

    // No
      // Return False

    // Yes
      // Return True
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

}