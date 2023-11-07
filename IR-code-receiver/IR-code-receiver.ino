// ************ Description *****************
// Program:     IR Communication Code - Satellite
// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 4

// This program will listen for pings from the ground station and then return
// an acknowledgement (ack) packet to confirm it has received a ping.

// ************ Libraries ********************
#include <IRremote.h>

// ************ Variables ********************
int IR_LED_PIN = 13;   
IRsend irsend;             // Initialise the IRsend object

String message = "ack";    // Message being sent

int delayTime = 1000;      // Time to wait between each ping
int messageDelay = 50;     // Time to wait between each character being sent

int IR_RECEIVE_PIN = 11;         // The pin the IR receiver is connected to
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

// ************ Functions ********************

void sendAck(const String &message){
  for (int i = 0; i < message.length(); i++) {  // Loop for each character in the message
    irsend.sendNEC((uint32_t)message[i], 32);   // Send each character's ASCII value as a unique NEC code 
    delay(messageDelay);                        // Wait between chars to avoid mix-up
  }
  delay(delayTime); 
};

void printReceive(){
  if (irrecv.decode(&results)) {
      if (results.decode_type == NEC) {
        Serial.print((char)results.value);  // Print the received character as ASCII
      }
      irrecv.resume();                      // Prepare to receive the next value
  }
} 

// ************ Program Loops ********************

void setup() {
  Serial.begin(9600);                 // Initialise serial communications
  pinMode(IR_LED_PIN, OUTPUT);        // Set the IR LED pin as output
  irrecv.enableIRIn();                // Start the receiver
}

void loop() {
  Serial.println("Listening");
  printReceive();                     // Print received messages to serial monitor
}