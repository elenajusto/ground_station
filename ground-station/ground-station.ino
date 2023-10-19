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

  //for degree in horizontalDegrees {

    // Position to degree

    // Call vertical scan
    
    // Check what vertical scan says

      // If verticalScan found satellite - End state

      // If verticalScan has not found satellite

        //Continue to next degree
  //};
};

// State - Vertical Scan
bool verticalScan() {

  // Position to starting degree (90)
  servoArm.write(180);

  // Debug - Adjust as preferred
  delay(1000);

  for (int degree = 180; degree >= 0; degree -=1) {
    
    servoArm.write(degree);                           // Position arm to next degree

    delay(15);                                        // Incremental delays - Adjust as preferred

    // Call IR transmitter [checkSignal]
    bool signalFound = checkSignalDummy();            // Using dummy function!!            

    // Check what checkSignal returns
    if (signalFound == true) {

      // If checkSignal found satellite - Return True
      Serial.println("verticalScan == true");           // DEBUG
      delay(3000);                                      // DEBUG - Holds arm position for this amount of time
      return true;

    }  
    
    // checkSignal did not find satellite 
    // Continue to next degree   

  };

  Serial.println("verticalScan == false"); // DEBUG

  // Debug - Adjust as preferred
  delay(1000); 

  // Return to horizontalScan
  return false;

};

// Simulates checkSignal() function
bool checkSignalDummy(){

  // Simulate the sending of a ping and waiting for a receipt
  delay(50);

  // Randomly decide whether a ping receipt is received or not
  int chanceOfSuccess = 90;                                     // Adjust chance of success as needed - Currenlty 0
  if (random(0, 100) > chanceOfSuccess) {        

    // Simulate a successful ping receipt
    Serial.println("Signal received!"); // Debug
    return true;

  } else {
    // Simulate a failed ping receipt
    Serial.println("NO SIGNAL :("); // Debug
    return false;

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
  verticalScan();
}