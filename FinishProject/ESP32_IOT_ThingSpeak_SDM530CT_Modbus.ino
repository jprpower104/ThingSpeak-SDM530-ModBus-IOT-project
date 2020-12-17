#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <algorithm>  // for std::reverse
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>


esp32ModbusRTU modbus(&Serial1, 4);  // use Serial1 and pin 4 as RTS

#define TIMEOUT_MS 1000
#define QUEUE_SIZE 20
static uint32_t lastMillis = 0;
static uint32_t delay_ms = 50;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
float Vac = 0;
float IacPh1 = 0;
float IacPh2 = 0;
float IacPh3 = 0;
float Pac = 0;
float PF = 0;
float Phi = 0;
float Freq = 0;
String myStatus = "";


void setup() {
  Serial.begin(115200);  // Serial output
  Serial1.begin(9600, SERIAL_8N1, 16, 17, false);  // Modbus connection
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) {
    //Serial.printf("Address id 0x%03x function code 0x%02x address reg: 0x%02x len %u: 0x%04x", serverAddress, fc, address,length);
    /*for (size_t i = 0; i < length; ++i) {
      Serial.printf("%02x", data[i]);
    }*/
    std::reverse(data, data + 4);  // fix endianness
    //Serial.printf("\n val: %.6f", *reinterpret_cast<float*>(data));
    if(address==0x00)
      {
        Vac = float(*reinterpret_cast<float*>(data));
        Serial.printf("\n Voltage all phases: %.6f V_ca", Vac);
      }
    else if(address==0x06)
      {
        IacPh1 = float(*reinterpret_cast<float*>(data))*1000;
        Serial.printf("\n Current phase1: %.6f A_ca", IacPh1);
      }
    else if(address==0x08)
      {
        IacPh2 = float(*reinterpret_cast<float*>(data))*1000;
        Serial.printf("\n Current phase2: %.6f A_ca", IacPh2);
      }
    else if(address==0x0A)
      {
        IacPh3 = float(*reinterpret_cast<float*>(data))*1000;
        Serial.printf("\n Current phase3: %.6f A_ca", IacPh3);
      }
    else if(address==0x34)
      {
        Pac = float(*reinterpret_cast<float*>(data));
        Serial.printf("\n Power: %.6f W", Pac);
      }
    else if(address==0x3E)
      {
        PF = float(*reinterpret_cast<float*>(data))*100;  
        Serial.printf("\n PF: %.6f", PF);
      }
    else if(address==0x42)
      {
        Phi = float(*reinterpret_cast<float*>(data));
        Serial.printf("\n Phase Angle: %.6f DEG", Phi);
      }
    else if(address==0x46)
      {
        Freq = float(*reinterpret_cast<float*>(data));
        Serial.printf("\n Frequency: %.6f hz", Freq);
      }
    else
      {
      Serial.printf("\n val: %.6f address reg: 0x%02x", *reinterpret_cast<float*>(data), address);
      }
    //Serial.print("\n\n");
  });
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("\n error!");
  });
  modbus.begin();
  delay(700);
}

void loop() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  if (millis() - lastMillis > 20000) {
    lastMillis = millis();
    Serial.print("\n\nsending Modbus request...\n");
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x00, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x06, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x08, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x0A, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x34, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x3E, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x42, 0x02);
    delay(delay_ms);
    modbus.readInputRegisters(0x002, 0x46, 0x02);
    delay(delay_ms);
  
 
    // set the fields with the values
    ThingSpeak.setField(1, Vac);
    ThingSpeak.setField(2, IacPh1);
    ThingSpeak.setField(3, IacPh2);
    ThingSpeak.setField(4, IacPh3);
    ThingSpeak.setField(5, Pac);
    ThingSpeak.setField(6, PF);
    ThingSpeak.setField(7, Phi);
    ThingSpeak.setField(8, Freq);
  
    // set the status
    ThingSpeak.setStatus(myStatus);
    
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("\nChannel update successful.");
    }
    else{
      Serial.println("\nProblem updating channel. HTTP error code " + String(x));
    } 
  }
}
