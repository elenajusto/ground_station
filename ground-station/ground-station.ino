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
  
  Serial.println("Starting horizontal scan");                 // Debug statement

  int stepsToCover = 1019;                                    // Number of degrees to cover in steps: 360 = 2038, 180 = 1019

  for (int step = 0; step <= stepsToCover; step += 100){      // Move in steps of 100

    // Position to next step of stepsToCover
    towerStepper.moveTo(step);                                
    while (towerStepper.distanceToGo() != 0) {
      towerStepper.run();
    }

    Serial.print("Step: ");                                   // Debug statement
    Serial.print(step);                                       // Debug statement
    Serial.print(" of ");                                     // Debug statement
    Serial.print(stepsToCover);                               // Debug statement
    Serial.println(" steps");                                 // Debug statement

    // Call vertical scan here
    delay(1000);                                              // 1 second delay - Placeholder for calling scan
  };

  Serial.println("Completed horizontal scan");                 // Debug statement

 // for degree in horizontalDegrees 

    // Position to degree

    // Call vertical scan

    // Check what vertical scan says

      // If verticalScan found satellite - End state

      // If verticalScan has not found satellite

        //Continue to next degree
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
	//towerStepper.moveTo(-500);                      // Sets position that stepper will move to (in steps) - 360 = 2038, 180 = 1019
};

void loop() {
  //verticalScan();              // Testing purposes
  
	//towerStepper.run();          // Testing purposes

  horizontalScan();        
  //towerStepper.run();

};