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


// IR  Variables
int IR_LED_PIN = 13;                                          // Signal out pin for transmitter
IRsend irsend;                                                // Initialise the IRsend object
int delayTime = 100;                                          // Time to wait between each ping
int messageDelay = 50;                                        // Time to wait between each character being sent

int IR_RECEIVE_PIN = 11;                                      // Signal pin for IR receiver 
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

const uint8_t PING_COMMAND = 0x01;                            // Command for ping - Hex value for "1"
const uint8_t ACK_COMMAND = 0x02;                             // Command for ack - Hex value for "2"


// ************ Functions ********************


// Function to send an ACK
void sendAck() {
  irsend.sendNEC(ACK_COMMAND, 32);  // Sending ACK command using the NEC protocol
}

// Function to listen for a ping
void listenForPing() {
  if (irrecv.decode(&results)) {
    if (results.value == PING_COMMAND) {
      Serial.println(F("Ping received! Sending ACK."));
      sendAck();
    }
    irrecv.resume();                 // Receive the next value
  }
}


// ************ Program ********************


void setup() {

// Initialise the serial port:
  Serial.begin(9600);

  // IR Receiver setup
  irsend.begin(IR_LED_PIN);                                // Start the transmitter
  irrecv.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);       // Start the receiver
}

void loop() {

  // Continously listen for pings
  listenForPing();
}

