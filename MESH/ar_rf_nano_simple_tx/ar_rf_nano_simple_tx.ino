


/*

https://github.com/keywish/keywish-nano-plus/blob/master/RF-Nano/Schematic/RF-Nano-Schematic.pdf
https://github.com/emakefun/rf-nano/blob/master/schematic/rf-nano_sch_v3.0.pdf

arduino-cli lib install "RF24"

 11 Arduino nano CLI
 12 https://github.com/arduino/arduino-cli
 13 https://arduino.github.io/arduino-cli/0.21/commands/arduino-cli/
 14
 15   551 mkdir rpi_deb
 16   552 cd rpi_deb
 17 https://github.com/arduino/arduino-cli/releases
 18   553 wget https://github.com/arduino/arduino-cli/releases/download/0.22.0/arduino-cli_0.22.0_Linux_64bit.tar.gz
 19 Kicsomagol ide
 20   554  ./arduino-cli
 21   555  ./arduino-cli config init
 22   556  ./arduino-cli sketch new blink
 23   557  nano blink/blink.ino
 24   558  ./arduino-cli core update-index
 25   559  ./arduino-cli board list    //→nem ismerte fel a nano-t
 26   560  ./arduino-cli board listall
 27   562  ./arduino-cli core search nano
 28   564  ./arduino-cli core install arduino:avr   // →  avr telepites
 29   565  ./arduino-cli core update-index
 30   566  ./arduino-cli board list   //→ most se latja
 31   567  ./arduino-cli board listall
 32   570  sudo chmod 777 /dev/ttyUSB0    //→ ez kell
 33   571  ./arduino-cli board list   //most se latja  577  ./arduino-cli compile --verbose --fqbn arduino:avr:nano blink   //forditani azert lehet
 34   582  ./arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano blink  // azt irjak azert letolti es tenyleg !!!
 35 ../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
 36 ../../../bow/arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano arduino.ino


../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano ./ar_rf_nano_simple_tx.ino
../../../bow/arduino-cli lib install "RF24"

$ ARDUINO_DATA_DIR=$HOME/.arduino15 ../../../bow/arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328 ./ar_rf_nano_simple_tx.ino





🛠 Tips
    Make sure both modules use same channel and address.
    Try different RF24_PA_LEVELs (MIN, LOW, HIGH, MAX) if signal is weak.
*/

/* tx code */

#if 1

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9);  // CE, CSN for Keywish RF-Nano

const byte address[6] = "1Node";
unsigned long lastPingTime = 0;
int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("TX ready");
  if (!radio.begin()) {
    Serial.println("NRF24 init failed");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(108);
  radio.setAutoAck(false); //debug
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  unsigned long now = millis();
  if (now - lastPingTime >= 1000) {
    lastPingTime = now;
    counter++;
    bool ok = radio.write(&counter, sizeof(counter));
    if (ok) {
      Serial.print("Sent ping: ");
      Serial.println(counter);
    } else {
      Serial.println("Send failed");
    }
  }
}



#endif

#if 0


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "1Node";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();

  if (!radio.isChipConnected()) {
    Serial.println("NRF24 not connected!");
  } else {
    Serial.println("NRF24 is connected."); 
  }

  radio.printDetails();  // Print radio config

}

void loop() {
  const char text[] = "ping";
  bool ok = radio.write(&text, sizeof(text));
  if (ok) Serial.println("Sent: ping");
  else Serial.println("Send failed");
  delay(1000);
}





#endif

#if 0
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "1Node";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello from TX!";
  radio.write(&text, sizeof(text));
  Serial.println("Sent: Hello from TX!");
  delay(1000);
}

#endif




/*

https://www.hestore.hu/prod_10047913.html#


AR-RF-NANO
Fejlesztői modul ATmega328, NRF24L01 és CH340-nel (Arduino IDE kompatibilis), USB-C 


Microcontroller: Atmel ATmega328
Operating Voltage (logic level): 5 V
Input Voltage (recommended): 5 V
Input Voltage (limits): 5 V
Digital I/O Pins: 14 (of which 6 provide PWM output)
Analog Input Pins: 8
DC Current per I/O Pin: 40 mA
Flash Memory: 32 KB of which 2 KB used by bootloader
SRAM: 2 KB
EEPROM: 1 KB
Clock Speed: 16 MHz
Length: 50 mm
Width: 19 mm

Cooperative IC: NRF24L01
328p - nrf24l01:
D10 - CE
D9 - CSN
D13 - SCK
D11 - MOSI
D12 - MISO

Arduino IDE környezetben való programozás esetén előfordulhat, hogy a Processzornál az ATmega328P (Old Bootloader) opciót kell választani. 

*/



