// ************ Description *****************

/*
* Created by Elena Justo
*
* This file is responsible for controlling the satellite, including:
* - The onboard servo arm 
* - The onboard LED 
* - The IR transmitter code 
* - The IR receiver code 
* - Reading accelerometer data 
*/


// ************ Libraries ********************


#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>
#include <IRremote.h>


// ************ Variables ********************


// Other Variables
int satLED = 4;                                               // Digital output pin for LED

// IR  Variables
int IR_LED_PIN = 13;                                          // Signal out pin for transmitter
IRsend irsend;                                                // Initialise the IRsend object
int delayTime = 100;                                          // Time to wait between each ping
int messageDelay = 50;                                        // Time to wait between each character being sent

int IR_RECEIVE_PIN = 11;                                      // Signal pin for IR receiver 
IRrecv irrecv(IR_RECEIVE_PIN);                                // Create an IRrecv object
decode_results decodedSignal;                                 // Stores results from IR detector

const uint8_t PING_COMMAND = 0x01;                            // Command for ping - Hex value for "1"
const uint8_t ACK_COMMAND = 0x02;                             // Command for ack - Hex value for "2"

/*
 * Set up the data to be sent.
 * For most protocols, the data is build up with a constant 8 (or 16 byte) address
 * and a variable 8 bit command.
 * There are exceptions like Sony and Denon, which have 5 bit address.
*/
uint8_t sCommand = 0x34;
uint8_t sRepeats = 0;



// ************ Functions ********************


// Function to send an ACK
void sendAck() {
  irsend.sendNEC(ACK_COMMAND, 32);  // Sending ACK command using the NEC protocol
}


// Simple send function from IR library
void simpleSend() {
  /*
     * Print current send values
  */
  Serial.println();
  digitalWrite(satLED, HIGH);                                // LED turns on when signal is being sent
  Serial.print(F("Send now: address=0x00, command=0x"));
  Serial.print(sCommand, HEX);
  Serial.print(F(", repeats="));
  Serial.print(sRepeats);
  Serial.println();

  Serial.println(F("Send standard NEC with 8 bit address"));
  Serial.flush();
  digitalWrite(satLED, LOW);
  
  // Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
  IrSender.sendNEC(0x00, sCommand, sRepeats);

  /*
    * Increment send values
  */
  sCommand += 0x11;
  sRepeats++;
  // clip repeats at 4
  if (sRepeats > 4) {
      sRepeats = 4;
  }

  delay(1000);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}

// Simple string sending function
void sendString(String str) {
  Serial.println(F("Sending String via IR:"));
  for (int i = 0; i < str.length(); i++) {
    // Convert each character to its ASCII value
    uint8_t asciiValue = str.charAt(i);

    // Output to Serial Monitor
    Serial.print(F("Sending character '"));
    Serial.print(str.charAt(i));
    Serial.print(F("' (ASCII: 0x"));
    Serial.print(asciiValue, HEX);
    Serial.println(F(")"));

    // Send the ASCII value via IR
    IrSender.sendNEC(0x00, asciiValue, 0); // Adjust based on your IR library

    // Delay between characters
    delay(100); 
  }
  Serial.println(F("String transmission complete."));
}


// ************ Program ********************


void setup() {
  // Initialise the serial port:
  Serial.begin(9600);

  // Initialise LED
  pinMode(satLED, OUTPUT);

  // Flash the Red LED for 1 second to visually indicate that the program is turning on 
  digitalWrite(satLED, HIGH);
  delay(1000);
  digitalWrite(satLED, LOW);

  // IR Receiver setup
  irrecv.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);        // Start the receiver object

  // IR Transmitter setup
  IrSender.begin(DISABLE_LED_FEEDBACK);                      // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_LED_PIN);
}


void loop() {
  //simpleSend();

  String myString = "123.123.123";
  sendString(myString);
  delay(5000);            // Delay before sending the string again
}

