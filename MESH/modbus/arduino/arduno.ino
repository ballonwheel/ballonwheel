

#include <ModbusRTUSlave.h>

ModbusRTUSlave modbus;

uint16_t inputRegs[4];    // 30001-30004
uint16_t holdingRegs[3];  // 40001-40003

void setup() {
  Serial.begin(9600);
  modbus.begin(9600, SERIAL_8N1);
  modbus.configureSlave(1);  // <-- Ez az állomás címe (1–5)
}

void loop() {
  inputRegs[0] = (int)(readTemp() * 100);
  inputRegs[1] = (int)(readHumidity() * 100);
  inputRegs[2] = readSoil();
  inputRegs[3] = (int)(readBattery() * 100);

  modbus.inputRegisters(inputRegs, 4);
  modbus.holdingRegisters(holdingRegs, 3);

  modbus.poll();
}

// dummy functions
float readTemp() { return 21.5; }
float readHumidity() { return 62.3; }
int readSoil() { return 650; }
float readBattery() { return 4.11; }


