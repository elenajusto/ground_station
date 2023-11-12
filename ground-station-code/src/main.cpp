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

// IR  Variables
int IR_LED_PIN = 13;                                          // Signal out pin for transmitter
IRsend irsend;                                                // Initialise the IRsend object
int delayTime = 100;                                          // Time to wait between each ping
int messageDelay = 50;                                        // Time to wait between each character being sent

int IR_RECEIVE_PIN = 3;                                       // Signal pin for IR receiver 
IRrecv irrecv(IR_RECEIVE_PIN);                                // Create an IRrecv object
decode_results decodedSignal;                                 // Stores results from IR detector

const uint8_t PING_COMMAND = 0x01;                            // Command for ping - Hex value for "1"
const uint8_t ACK_COMMAND = 0x02;                             // Command for ack - Hex value for "2"

const int ledPin = 4;                                         // Use LED to signal if IR message is received
bool lightState = LOW;                                        // Keep track of whether the LED is on
unsigned long last = millis();                                // Remember when we last received an IR message

// ************ Functions ********************


// Check signal function
bool checkSignal(){

  /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
  */
  if (IrReceiver.decode()) {                 // Checks if signal is received by IR receiver
    Serial.println("Signal Received!");      // Serial monitor status when signal is received
    digitalWrite(ledPin, HIGH);              // Ground station LED lights up when signal received
  
  /*
      * Print a short summary of received data
  */
  IrReceiver.printIRResultShort(&Serial);
  IrReceiver.printIRSendUsage(&Serial);
  
  if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
    
    // We have an unknown protocol here, print more info (Raw data)
    IrReceiver.printIRResultRawFormatted(&Serial, true);

     return false;                            // Function returns false if raw signal is received
    }
    
    Serial.println();                         // Outputs above information to serial monitor
    
    /*
    * !!!Important!!! Enable receiving of the next value,
    * since receiving has stopped after the end of the current received data packet.
    */
  
  IrReceiver.resume();                        // Enable receiving of the next value
  digitalWrite(ledPin, LOW);                  // Ground station LED turns off after signal is received
  
  return true;                                // Function returns true if signal is received
  } else {                                   
    return false;                             // Function returns false if no signal is received
  }
}

// Function to receive ASCII characters
String receivedString = "";  // Global variable to hold the received string
const uint8_t MAX_STRING_LENGTH = 11;  // Length of "XYZ.XYZ.XYZ"
const char TERMINATION_CHARACTER = '\n';  // Termination character

bool checkSignalChar() {
  if (IrReceiver.decode()) {
    digitalWrite(ledPin, HIGH);

    // Check if the received data is part of a string
    if (IrReceiver.decodedIRData.protocol == NEC) {
      // Assuming NEC protocol, adjust as necessary
      uint8_t receivedValue = IrReceiver.decodedIRData.command;

      // Append the received character to the string
      receivedString += (char)receivedValue;

      // Check for string completion by length or termination character
      if (receivedString.length() >= MAX_STRING_LENGTH || receivedValue == TERMINATION_CHARACTER) {
        // Handle the completed string
        Serial.print(F("Received String: "));
        Serial.println(receivedString);

        // Reset the string for the next message
        receivedString = "";
      }
    } else {
      // Handle unknown protocol or noise
      Serial.println(F("Received unknown or noisy signal"));
    }

    IrReceiver.resume();
    digitalWrite(ledPin, LOW);

    return true;
  } else {
    return false;
  }
}


// Simulates checkSignal() function  
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
  //servoArm.write(180);
  for (int degree = 0; degree != 180; degree += 5) {
    servoArm.write(degree);
    delay(100);
  }

  // Debug - Adjust as preferred
  delay(1000);

  for (int degree = 180; degree >= 0; degree -=5) {
    
    servoArm.write(degree);                           // Position arm to next degree

    // Call IR transmitter [checkSignal]
    bool signalFound = checkSignal();

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

  Serial.println("Completed horizontal scan");                // Debug statement

};

void setup() {

  // Initialise the serial port:
  Serial.begin(9600);

  // Servo motor setup
  servoArm.attach(6);                             // Set pin 6 for servo control
  servoArm.write(0);                              // Start arm at a horizontal right angle
  
	// Stepper motor setup
	towerStepper.setMaxSpeed(1000.0);
	towerStepper.setAcceleration(50.0);
	towerStepper.setSpeed(200);       

  pinMode(ledPin, OUTPUT);                       // Initialise LED
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);

  // IR Receiver setup
  irsend.begin(IR_LED_PIN);                                         // Start the transmitter
  irrecv.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, ledPin);        // Start the receiver object - Incorrect way of starting?
  //irrecv.enableIRIn();                                            // Start the receiver object

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
};

void loop() {  

  // Continously scan the sky
  //horizontalScan();     

  //checkSignal();     


  checkSignalChar();
};