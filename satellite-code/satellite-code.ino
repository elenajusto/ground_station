// ************ Description *****************
// Program:     Satellite Code
// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4

// ************ Libraries ********************
#include <Servo.h>

// ************ Variables ********************

int satLED = 4;         // Digital output pin for LED
Servo satArm;           // Servo object. Pin assignment  in setup() loop. Pin = 9


void setup() {

  // Initialize the serial port:
  Serial.begin(9600);

  // Servo motor setup
  satArm.attach(9);                             // Set pin 9 for servo control

  satArm.write(0);

  digitalWrite(satLED, HIGH); 
}

void loop() {
  

}
