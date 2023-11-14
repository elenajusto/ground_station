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
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


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

int IR_RECEIVE_PIN = 4;                                       // Signal pin for IR receiver 
IRrecv irrecv(IR_RECEIVE_PIN);                                // Create an IRrecv object
decode_results decodedSignal;                                 // Stores results from IR detector

const uint8_t PING_COMMAND = 0x01;                            // Command for ping - Hex value for "1"
const uint8_t ACK_COMMAND = 0x02;                             // Command for ack - Hex value for "2"

const int ledPin = 3;                                         // Use LED to signal if IR message is received
bool lightState = LOW;                                        // Keep track of whether the LED is on
unsigned long last = millis();                                // Remember when we last received an IR message

// LCD Variables
LiquidCrystal_I2C lcd(0x27, 16, 2);                           // Initialise the LCD with I2C address 0x27 for a 16x2 character display

// Tracking
bool connectionState = false;  // Tracks the connection state
int currentSearchRadius = 2;  // Current search radius in degrees


// ************ Functions ********************


// Function for LCD to display any given string
void displayString(String message) {
  lcd.clear();                                                  // Clear the LCD
  lcd.setCursor(0, 0);                                          // Set the cursor to the first line
  int displayLength = 16;                                       // Maximum number of characters per line

  // Display the message in chunks of 'displayLength' characters
  for (int i = 0; i < message.length(); i += displayLength) {
    String chunk = message.substring(i, min(i + displayLength, message.length()));
    lcd.print(chunk);                                           // Print the chunk of the message

    // Move to the next line or exit if the message ends
    if (i + displayLength < message.length()) {
      lcd.setCursor(0, 1);                                      // Move cursor to the second line
    } else {
      break;                                                    // Exit the loop if the end of the message is reached
    }
  }
  delay(100);
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


// Function to receive ASCII characters
String receivedString = "";  // Global variable to hold the received string
const uint8_t MAX_STRING_LENGTH = 11;  // Length of "EEEEEEEEEEE"
const char TERMINATION_CHARACTER = '\n';  // Termination character
const int stabilizationDelay = 100; // Delay in milliseconds for stabilization at the beginning of signal check

bool checkSignalChar() {
  delay(stabilizationDelay); // Wait for stabilization before starting signal check

  if (IrReceiver.decode()) {
    digitalWrite(ledPin, HIGH);

    if (IrReceiver.decodedIRData.protocol == NEC) {
      uint8_t receivedValue = IrReceiver.decodedIRData.command;

      // Append the received character to the string
      receivedString += (char)receivedValue;

      // Check for string completion by length or termination character
      if (receivedString.length() >= MAX_STRING_LENGTH || receivedValue == TERMINATION_CHARACTER) {
        Serial.print(F("Received String: "));
        Serial.println(receivedString);

        // Check if the received string matches "EEEEEEEEEEE"
        bool isMatch = (receivedString == "EEEEEEEEEEE");
        receivedString = ""; // Reset the string for the next message
        digitalWrite(ledPin, LOW);

        if (isMatch) {
          displayString("Signal Found!");
          delay(5000);
          displayString("EEEEEEEEEEE"); // Display the string if it matches
          return true; // Return true if the string matches "EEEEEEEEEEE"
        } else {
          return false; // Return false if the string doesn't match
        }
      }
    } else {
      Serial.println("Noise");
      displayString("Noise");
      receivedString = ""; // Reset the string if noise is detected
    }

    IrReceiver.resume();
    digitalWrite(ledPin, LOW);
  } else {
    Serial.println("No signal, Searching...");
    displayString("Searching...");
  }

  return false; // Return false if no signal is received or if the signal is not NEC
}


// Vertical Scan Function
bool verticalScan() {
  displayString("Vertical Scan Start");
  delay(1000);
  const int stabilizationDelay = 100; // Delay in milliseconds to allow for stabilization

  // Scan upwards from 0 to 180 degrees
  for (int degree = 0; degree <= 90; degree += 5) {
    servoArm.write(degree);
    delay(stabilizationDelay); // Wait for stabilization after moving

    // Check for signal at this vertical degree
    if (checkSignalChar()) {
      Serial.println("Signal found during initial vertical scan");
      delay(3000); // Hold position for a while (for debugging)
      displayString("Vertical Scan Complete");
      return true; // Signal found
    }
  }

  // Scan downwards from 180 to 0 degrees
  for (int degree = 90; degree >= 0; degree -= 5) {
    servoArm.write(degree); // Position arm to the next degree
    delay(stabilizationDelay); // Wait for stabilization after moving

    // Check for signal at this vertical degree
    if (checkSignalChar()) {
      Serial.println("Signal found during final vertical scan");
      delay(3000); // Hold position for a while (for debugging)
      displayString("Vertical Scan Complete");
      return true; // Signal found
    }
  }

  Serial.println("No signal found in vertical scan");
  displayString("Vertical Scan Complete");
  return false; // No signal found in the entire vertical scan
}


// Horizontal Scan Function - Scans the whole sky, using vertical scan and checkSignal downstream.
void horizontalScan() {
  displayString("Horizontal Scan Start");
  delay(1000);
  Serial.println("Starting horizontal scan");                 // Debug statement

  int stepsToCover = 1000;                                    // Number of degrees to cover in steps: 360 = 2038, 180 = 1019

  for (int step = 0; step <= stepsToCover; step += 100){      // Move in steps of 300

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
  displayString("Horizontal Scan Complete");
};


// ************ Main Program ********************


void setup() {

  // Initialise LCD
  lcd.init();                                    // Initialise the LCD
  lcd.backlight();                               // Turn on backlight
  
  // Initialise the serial port:
  displayString("Starting Serial");
  Serial.begin(9600);

  // Servo motor setup
  displayString("Starting Servo");
  servoArm.attach(6);                             // Set pin 6 for servo control
  servoArm.write(0);                              // Start arm at a horizontal right angle
  
	// Stepper motor setup
  displayString("Starting Stepper");
	towerStepper.setMaxSpeed(200.0);
	towerStepper.setAcceleration(80.0);
	towerStepper.setSpeed(200);       

  displayString("Starting LED");
  pinMode(ledPin, OUTPUT);                       // Initialise LED
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);

  // IR Receiver setup
  displayString("Starting IR");
  irsend.begin(IR_LED_PIN);                                         // Start the transmitter
  //irrecv.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, ledPin);        // Start the receiver object - Incorrect way of starting?
  irrecv.enableIRIn();                                            // Start the receiver object

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);

  displayString("Ready to go!");

};

void loop() {  

  // Clear the LCD every loop
  lcd.clear(); 

  // Continously scan the sky
  horizontalScan();     

};