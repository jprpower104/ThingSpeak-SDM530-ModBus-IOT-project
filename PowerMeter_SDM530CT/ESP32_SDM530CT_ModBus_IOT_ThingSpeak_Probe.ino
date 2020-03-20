/*
Copyright 2018 Bert Melis
This example reads 2 words (4 bytes) from address 52 of a server with id 1.
address 52 = register 30053 (Eastron SDM630 Total system power)
The ESP is connected to a max3485 with pins 17 (RX), 4 (TX) and 16 as RTS.
*/

#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <algorithm>  // for std::reverse

esp32ModbusRTU modbus(&Serial1, 4);  // use Serial1 and pin 16 as RTS

#define TIMEOUT_MS 1000
#define QUEUE_SIZE 20
static uint32_t lastMillis = 0;

void setup() {
  Serial.begin(115200);  // Serial output
  Serial1.begin(9600, SERIAL_8N1, 16, 17, false);  // Modbus connection

  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) {
    //Serial.printf("Address id 0x%03x function code 0x%02x address reg: 0x%02x len %u: 0x%04x", serverAddress, fc, address,length);
    /*for (size_t i = 0; i < length; ++i) {
      Serial.printf("%02x", data[i]);
    }*/
    std::reverse(data, data + 4);  // fix endianness
    //Serial.printf("\n val: %.6f", *reinterpret_cast<float*>(data));
    if(address==0x00)
    Serial.printf("\n Voltage: %.6f V_ca", *reinterpret_cast<float*>(data));
    else if(address==0x06)
    Serial.printf("\n Current: %.6f A_ca", *reinterpret_cast<float*>(data));
    else if(address==0x0C)
    Serial.printf("\n Power: %.6f W", *reinterpret_cast<float*>(data));
    else if(address==0x1E)
    Serial.printf("\n PF: %.6f", *reinterpret_cast<float*>(data));
    else if(address==0x24)
    Serial.printf("\n Phase Angle: %.6f DEG", *reinterpret_cast<float*>(data));
    else if(address==0x46)
    Serial.printf("\n Frequency: %.6f hz", *reinterpret_cast<float*>(data));
    else
    Serial.printf("\n val: %.6f address reg: 0x%02x", *reinterpret_cast<float*>(data), address);
    
    //Serial.print("\n\n");
  });
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();

}

void loop() {
  if (millis() - lastMillis > 30000) {
    lastMillis = millis();
    Serial.print("sending Modbus request...\n");
    delay(500);
    modbus.readInputRegisters(0x002, 0x00, 0x02);
    delay(500);
    modbus.readInputRegisters(0x002, 0x06, 0x02);
    delay(500);
    modbus.readInputRegisters(0x002, 0x0C, 0x02);
    delay(500);
    modbus.readInputRegisters(0x002, 0x1E, 0x02);
    delay(500);
    modbus.readInputRegisters(0x002, 0x24, 0x02);
    delay(500);
    modbus.readInputRegisters(0x002, 0x46, 0x02);
    delay(500);
  }
}
