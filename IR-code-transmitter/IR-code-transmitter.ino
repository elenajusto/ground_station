// ************ Description *****************
// Program:     IR Communication Code - Ground Station
// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4

// This program will send a ping to the satellite and output the acknowledgement message
// that it is expected to receive from the satellite into the Serial monitor.

// ************ Libraries ********************
#include <IRremote.h>

// ************ Variables ********************
int IR_LED_PIN = 13;   
IRsend irsend;             // Initialise the IRsend object

String message = "ping";   // Message being sent

int delayTime = 1000;      // Time to wait between each ping
int messageDelay = 50;     // Time to wait between each character being sent

// ************ Functions ********************

void sendPing(const String &message){
  for (int i = 0; i < message.length(); i++) {  // Loop for each character in the message
    irsend.sendNEC((uint32_t)message[i], 32);   // Send each character's ASCII value as a unique NEC code 
    delay(messageDelay);                        // Wait between chars to avoid mix-up
  }
  delay(delayTime); 
};

// ************ Program Loops ********************

void setup() {
  Serial.begin(9600);                 // Initialise serial communications
  pinMode(IR_LED_PIN, OUTPUT);        // Set the IR LED pin as output
}

void loop() {
  Serial.print("Sending");
  sendPing(message);                  // Sends ping to satellite
}
