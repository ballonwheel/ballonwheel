



/*


🛠 Tips

    Make sure both modules use same channel and address.

    Try different RF24_PA_LEVELs (MIN, LOW, HIGH, MAX) if signal is weak.

    If you want to debug, connect both to USB and open Serial Monitors.

*/

#if 1

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9);  // CE, CSN for Keywish RF-Nano

const byte address[6] = "1Node";
int receivedCounter;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("RX ready");
  if (!radio.begin()) {
    Serial.println("NRF24 init failed");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(108);
  radio.openReadingPipe(1, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedCounter, sizeof(receivedCounter));
    Serial.print("Received ping: ");
    Serial.println(receivedCounter);
  }
}




#else


#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "1Node";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
  }
}

#endif
