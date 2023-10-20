// ************ Description *****************

// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4


// ************ Libraries ********************
#include <AccelStepper.h>
#include <Servo.h>

// ************ Variables ********************

#define MotorInterfaceType 4                                  // Step constant for stepper motor
AccelStepper towerStepper(MotorInterfaceType, 8, 10, 9, 11);  // Step sequence for motor is IN1-IN3-IN2-IN4

Servo servoArm;                                               // Servo object. Pin assignment  in setup() loop. Pin = 6

int lastKnownServoPosition = 0;                                    // Last known vertical position
int lastKnownStepperPosition = 0;                                  // Last known horizontal position

// ************ Functions ********************

// Horizontal Scan Function - Scans the whole sky, using vertical scan and checkSignal downstream.
void horizontalScan() {
  
  Serial.println("Starting horizontal scan");                 // Debug statement

  int stepsToCover = 1019;                                    // Number of degrees to cover in steps: 360 = 2038, 180 = 1019

  for (int step = 0; step <= stepsToCover; step += 100){      // Move in steps of 100

    // Position to next step of stepsToCover
    towerStepper.moveTo(step);                                
    while (towerStepper.distanceToGo() != 0) {
      towerStepper.run();
    }

    // Calls vertical scan
    if (verticalScan()) {
      Serial.println("SIGNAL FOUND - EXITING SCAN");
      break;                                                  // Exits horizontal scan if signal is detected from vertical scan
    };

    Serial.print("Step: ");                                   // Debug statement
    Serial.print(step);                                       // Debug statement
    Serial.print(" of ");                                     // Debug statement
    Serial.print(stepsToCover);                               // Debug statement
    Serial.println(" steps");                                 // Debug statement
  };

  Serial.println("Completed horizontal scan");                 // Debug statement

};

// Vertical Scan Function
bool verticalScan() {

  // Position to starting degree (90)
  servoArm.write(180);

  // Debug - Adjust as preferred
  delay(1000);

  for (int degree = 180; degree >= 0; degree -=10) {
    
    servoArm.write(degree);                           // Position arm to next degree

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

// ************** DUMMMY *************** Simulates checkSignal() function  
bool checkSignalDummy(){

  // Simulate the sending of a ping and waiting for a receipt
  delay(50);

  // Randomly decide whether a ping receipt is received or not
  int chanceOfSuccess = 100;                                     // Adjust chance of success as needed - Currenlty 0%
  if (random(0, 100) > chanceOfSuccess) {        

    // Simulate a successful ping receipt
    Serial.println("Signal received!"); // Debug
    lastKnownServoPosition = servoArm.read();                    // Saves servo angle of last known sat position
    lastKnownStepperPosition = towerStepper.currentPosition();   // Saves stepper angle of last known sat position
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

// Tracking state
bool trackingState() {

  Serial.println("Starting track");                        // Debug statement
  int degreePerStep = 50;                                  // Move this amount of steps per check for stepper
  int degreeForServo = 25;                                 // Servo arm checks this vertical value up and down

  Serial.println("Moving left of last known position.");   // Debug statement
  // Stepper go left by degreePerStep
  towerStepper.moveTo(lastKnownStepperPosition - degreePerStep);                                
  while (towerStepper.distanceToGo() != 0) {
    towerStepper.run();
  };
  delay(2000);
  // Call checkSignal()
  if (checkSignalDummy()) {
    // Yes Signal? Exit
    return true;
  } else {
    // Return to original position - degreePerStep to the right 
    Serial.println("Nothing detected on left. Continuing track...");   // Debug statement
    delay(2000);
    towerStepper.moveTo(lastKnownStepperPosition + degreePerStep);                                
    while (towerStepper.distanceToGo() != 0) {
    towerStepper.run();
    }
  };
  
  Serial.println("Moving right of last known position.");   // Debug statement
  // Stepper go right by degreePerStep
  towerStepper.moveTo(lastKnownStepperPosition + degreePerStep);                                
  while (towerStepper.distanceToGo() != 0) {
    towerStepper.run();
    };
  delay(2000);
  // Call checkSignal()
  if (checkSignalDummy()) {
    // Yes Signal? Exit
    return true;
  } else {
    // No Signal? Return to original position - degreePerStep to the left
    Serial.println("Nothing detected on right. Continuing track...");     // Debug statement
    delay(2000);
    towerStepper.moveTo(lastKnownStepperPosition - degreePerStep);                                
    while (towerStepper.distanceToGo() != 0) {
      towerStepper.run();
      };
    };

  // Servo arm go up by degreePerStep
  Serial.println("Moving up of last known position.");
  servoArm.write(lastKnownServoPosition - degreeForServo);

  delay(2000);

  // Call checkSignal()
  if (checkSignalDummy()){
    // Yes Signal? Exit
    return true;
  } else {
    // No Signal? Return to original position - degreePerStep down
    Serial.println("Nothing detected up. Continuing track...");
    delay(2000);
    servoArm.write(lastKnownServoPosition);
    delay(2000);
  };
  
  // Servo arm go down by degreePerStep
  Serial.println("Moving down of last known position.");
  servoArm.write(lastKnownServoPosition + degreeForServo);

  delay(2000);

  // Call checkSignal()
  if (checkSignalDummy()){
    // Yes Signal? Exit
    return true;
  } else {
    // No Signal? Return to original position - degreePerStep up
    Serial.println("Nothing detected down. Continuing track...");
    delay(2000);
    servoArm.write(lastKnownServoPosition);
    delay(2000);
  };

  Serial.println("Tracking state completed. Nothing found");
  return false;
}

void setup() {

  // Initialize the serial port:
  Serial.begin(9600);

  // Servo motor setup
  servoArm.attach(6);                             // Set pin 9 for servo control
  
	// Stepper motor setup
	towerStepper.setMaxSpeed(1000.0);
	towerStepper.setAcceleration(50.0);
	towerStepper.setSpeed(200);            
  
  // TESTING AREA
  lastKnownServoPosition = 90;
  lastKnownStepperPosition = 0;

  servoArm.write(lastKnownServoPosition);        // Moves servo to a 90 degree starting position
};

void loop() {  

  // Continously tries to track satellite from last known position. Commences full sky scan if not found from track
  if (!trackingState()){
    horizontalScan();
  };

};