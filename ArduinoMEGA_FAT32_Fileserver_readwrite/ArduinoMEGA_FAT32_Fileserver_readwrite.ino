//for Arduino MEGA with shield
//Write MAC address & IP adress
//This was created by modifying ESP32_FAT32_Filserver_readwrite.
//Arduino's SD.h don't support filerename. Therefore, the rename function is commented out.

#ifdef  UseWiFi
  #include <WiFi.h>
#else
  #include <Ethernet.h>
#endif

#include <SD.h>
#include <SPI.h>
#include "CheckAndResponse.h"

#define chipSelect 4
#define Blue_LED 13 //LED on EthernetShield

EthernetServer server(80);

const byte mac[] = { Write your board MAC address }; //ex.{ 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D }
const byte ip[] = { Write IP address }; //ex.{ 192, 168, 0, 200 }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  int count = 0;
  // Initialize SDcard
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card Mount Failed");
    SPI.end();
    pinMode(Blue_LED, OUTPUT);
    while (count < 100){
      digitalWrite(Blue_LED, HIGH);
      delay(500);
      digitalWrite(Blue_LED, LOW);
      delay(500);
      count++;
    }
    return;
  }
  Serial.println("SD Card initialized.");
  
  // connect to Ethernet
  Ethernet.begin( mac, ip );
  server.begin(); //start the server
  Serial.print("HTTP server started at: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  CheckAndResponse(client);
}