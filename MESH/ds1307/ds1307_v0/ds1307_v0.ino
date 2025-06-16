


/*
arduino-cli lib install "OneWire"
arduino-cli lib install "DallasTemperature"
arduino-cli lib install "RTClib"
arduino-cli lib install "Wire"



*/



#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 data pin (connected to "DS" pin)
#define ONE_WIRE_BUS 2

// EEPROM I2C address (AT24C32)
#define EEPROM_ADDR 0x50

RTC_DS1307 rtc;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// EEPROM helpers
void eepromWriteByte(int addr, byte val) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((addr >> 8) & 0xFF);   // MSB
  Wire.write(addr & 0xFF);          // LSB
  Wire.write(val);
  Wire.endTransmission();
  delay(5);  // EEPROM write delay
}

byte eepromReadByte(int addr) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((addr >> 8) & 0xFF);   // MSB
  Wire.write(addr & 0xFF);          // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(EEPROM_ADDR, 1);
  return Wire.available() ? Wire.read() : 0xFF;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  rtc.begin();
  sensors.begin();

  if (!rtc.isrunning()) {
    Serial.println("RTC not running. Setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set to compile time
  }

  // Test EEPROM write
  eepromWriteByte(0x0000, 0xAB);
  byte test = eepromReadByte(0x0000);
  Serial.print("EEPROM Test Byte: 0x");
  Serial.println(test, HEX);
}

void loop() {
  // === Get Time ===
  DateTime now = rtc.now();
  Serial.print("Time: ");
  Serial.print(now.timestamp(DateTime::TIMESTAMP_FULL));
  Serial.println();

  // === Read Temperature ===
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  // === Store Temp in EEPROM (2 bytes: int16_t scaled *100) ===
  int addr = 0x0010;
  int16_t temp_scaled = (int16_t)(tempC * 100);
  eepromWriteByte(addr, (temp_scaled >> 8) & 0xFF);
  eepromWriteByte(addr + 1, temp_scaled & 0xFF);

  // === Read Back Temp ===
  byte high = eepromReadByte(addr);
  byte low = eepromReadByte(addr + 1);
  int16_t read_temp = (int16_t)((high << 8) | low);
  Serial.print("EEPROM Temp Readback: ");
  Serial.print(read_temp / 100.0);
  Serial.println(" °C");

  delay(5000);
}

