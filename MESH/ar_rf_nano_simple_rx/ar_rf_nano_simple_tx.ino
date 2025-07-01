



/*


🛠 Tips

    Make sure both modules use same channel and address.

    Try different RF24_PA_LEVELs (MIN, LOW, HIGH, MAX) if signal is weak.

    If you want to debug, connect both to USB and open Serial Monitors.

*/



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

