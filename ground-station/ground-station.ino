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

// ************ Functions ********************

// Horizontal Scan Function
void horizontalScan() {
  
  int horizontalDegrees = 180;  // Number of degrees to cover in scan
  int startingDegree = 0;       // Starting degree for horizontal scan
  int endingDegree = 1019;      // Ending degree for horizontal scan (180 degrees)

  // Position to starting degree (0)
  towerStepper.moveTo(startingDegree);
  while (towerStepper.distanceToGo() != 0) {
    towerStepper.run();
  }

  // Start the scan
  for (int step = startingDegree; step <= endingDegree; step += 100) {

    // Position to degree
    towerStepper.moveTo(step);
    while (towerStepper.distanceToGo() != 0) {
      towerStepper.run();
    }

    // Call vertical scan
    bool foundSatellite = verticalScan();

    // Check what vertical scan says
    if (foundSatellite) {
      // If verticalScan found satellite - End state
      Serial.println("Satellite found at horizontal degree: " + String(step * 180 / endingDegree));
      return;
    }

    // If verticalScan has not found satellite, continue to next degree
  }
  Serial.println("Horizontal scan complete. Satellite not found.");
};

// Vertical Scan Function
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

// ************** DUMMMY *************** Simulates checkSignal() function  
bool checkSignalDummy(){

  // Simulate the sending of a ping and waiting for a receipt
  delay(50);

  // Randomly decide whether a ping receipt is received or not
  int chanceOfSuccess = 100;                                     // Adjust chance of success as needed - Currenlty 0
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

  // Initialize the serial port:
  Serial.begin(9600);

  // Servo motor setup
  servoArm.attach(6);                             // Set pin 9 for servo control
  
	// Stepper motor setup
	towerStepper.setMaxSpeed(1000.0);
	towerStepper.setAcceleration(50.0);
	towerStepper.setSpeed(200);            
	towerStepper.moveTo(1019);                      // Sets position that stepper will move to (in steps) - 360 = 2038, 180 = 1019
};

void loop() {
  //verticalScan();              // Testing purposes
  
	//towerStepper.run();          // Testing purposes

  horizontalScan();        

};