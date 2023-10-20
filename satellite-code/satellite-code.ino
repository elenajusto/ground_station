// ************ Description *****************
// Program:     Satellite Code
// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4

// ************ Libraries ********************
#include <Servo.h>
#include <Wire.h>                                    // I2C communication for accelerometer
#include <SparkFun_MMA8452Q.h>                       // Accelerometer library

// ************ Variables ********************

int satLED = 4;                                      // Digital output pin for LED

Servo satArm;                                        // Servo object. Pin assignment  in setup() loop. Pin = 9

MMA8452Q accel;                                      // Instance of the MMA8452Q (accelero) class


int ledState = LOW;                                  // Setting LED state
const long delayInterval = 1000;                     // Blink interval for LED
unsigned long flashLEDPreviousMillis = 0;            // Last time LED was updated

// ************ Functions ********************

// Moves the arm of the satellite to a preset position (0 or 180 for horizontal, 90 for vertical)
void moveSatArm(int position) {
  switch(position) {
    case 0:
      satArm.write(0);
      break;
    case 90:
      satArm.write(90);
      break;
    case 180:
      satArm.write(180);
      break;
  }
}

// Flash the onboard LED based on flashInterval variable
void flashLED(){
  
  unsigned long flashLEDCurrentMillis = millis();                           // Crrent millis time

  if (flashLEDCurrentMillis - flashLEDPreviousMillis >= delayInterval) {
    flashLEDPreviousMillis =  flashLEDCurrentMillis;                        // Updates last LED update time
   
    // Toggle LED state
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // Write toggled LED state to LED
    digitalWrite(satLED, ledState);                                         
  }
}

void setup() {

  // Initialise the serial port:
  Serial.begin(9600);

  // Initialise accelerometer class - Default init. Full-scale range of +/-2g, and an output data rate of 800 Hz (fastest).
  accel.init();

  // Servo motor setup
  satArm.attach(9);                             // Set pin 9 for servo control

  // Position servo arm to be horizontal upon start
  satArm.write(0);
  
  // LED turns on upon start
  digitalWrite(satLED, HIGH);                   
}

void loop() {

}
