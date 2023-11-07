// ************ Description *****************

/*
* Created by Elena Justo
*
* This file is responsible for controlling the ground station, including:
* - The servo arm carrying the IR sensors
* - The stepper motor that rotates the tower
* - The IR transmitter code for the ground station
* - The IR receiver code for the ground station
*/

// ************ Libraries ********************
#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>
#include <IRremote.h>


// ************ Variables ********************

// Stepper Motor Variables
#define MotorInterfaceType 4                                  // Step constant for stepper motor
AccelStepper towerStepper(MotorInterfaceType, 8, 10, 9, 11);  // Step sequence for motor is IN1-IN3-IN2-IN4

// Servo Motor Variables
Servo servoArm;                                               // Servo object. Pin assignment  in setup() loop. Pin = 6

// IR Transmission Variables
int IR_LED_PIN = 13;   
IRsend irsend;                                                // Initialise the IRsend object
String message = "ping";                                      // Message being sent
int delayTime = 1000;                                         // Time to wait between each ping
int messageDelay = 50;                                        // Time to wait between each character being sent

// IR Receiving Variables
int IR_RECEIVE_PIN = 3;                                       // The pin the IR receiver is connected to
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

// ************ Functions ********************

// ************** DUMMMY *************** Simulates checkSignal() function  
bool checkSignalDummy(){

  // Simulate the sending of a ping and waiting for a receipt
  delay(50);

  // Randomly decide whether a ping receipt is received or not
  int chanceOfSuccess = 100;                                     // Adjust chance of success as needed - Currenlty 0%
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

void setup() {

  // Initialize the serial port:
  Serial.begin(9600);

  // Servo motor setup
  servoArm.attach(6);                             // Set pin 6 for servo control
  
	// Stepper motor setup
	towerStepper.setMaxSpeed(1000.0);
	towerStepper.setAcceleration(50.0);
	towerStepper.setSpeed(200);            
};

void loop() {  

  // Continously scan the sky
  horizontalScan();          

};