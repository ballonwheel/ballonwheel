
/*
arduino-cli lib install "RF24"

../../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano tx.ino
../../../../bow/arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano tx.ino

*/




#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW); // Power level: try HIGH if range is short
  radio.setChannel(108);         // Avoid WiFi channels (optional)
  radio.openWritingPipe(address);
  radio.stopListening(); // Set as transmitter
}

void loop() {
  const char text[] = "Hello NRF!";
  radio.write(&text, sizeof(text));
  Serial.println("Sent: Hello NRF!");
  delay(1000);
}


