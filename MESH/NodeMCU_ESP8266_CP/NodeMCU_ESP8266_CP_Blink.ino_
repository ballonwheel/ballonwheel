


#if 0

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

arduino-cli core update-index
arduino-cli config add board_manager.additional_urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli core install esp8266:esp8266

 ../../../bow/arduino-cli compile -v --fqbn esp8266:esp8266:nodemcuv2 NodeMCU_ESP8266_CP.ino
../../../bow/arduino-cli upload -p /dev/ttyUSB1 -v --fqbn esp8266:esp8266:nodemcuv2


#endif


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED is usually on GPIO2 (D4)
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (active low)
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off
  delay(500);
}
