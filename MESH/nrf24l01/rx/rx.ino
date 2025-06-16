
/*
 3 arduino-cli lib install "RF24"
 4
 5 ../../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano tx.ino
 6 ../../../../bow/arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano tx.ino
 7
 8 */





#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openReadingPipe(0, address);
  radio.startListening(); // Set as receiver
}

void loop() {
  if (radio.available()) {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
  }
}



