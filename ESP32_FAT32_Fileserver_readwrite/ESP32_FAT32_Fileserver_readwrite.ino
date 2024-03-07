//for ESP32
//Write ssid & Password
/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#ifdef  UseWiFi
  #include <WiFi.h>
#else
  #include <Ethernet.h>
#endif

#include <SD.h>
#include <FS.h>
#include "CheckAndResponse.h"

#define Blue_LED 2 //BlueLED on DevKit(Need changes when using other boards)

WiFiServer server(80);

const char* ssid = "Write your SSID";
const char* password = "Write your Password";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Blue_LED, OUTPUT);

  // connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if(digitalRead(Blue_LED)){
      digitalWrite(Blue_LED, HIGH);
    }
    else{
      digitalWrite(Blue_LED, LOW);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  int count = 0;
  // Initialize SDcard
  if (!SD.begin()) {
    Serial.println("SD Card Mount Failed");
    while (count < 100){
      digitalWrite(Blue_LED, HIGH);
      delay(500);
      digitalWrite(Blue_LED, LOW);
      delay(500);
      count++;
    }
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
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
  server.begin(); //start the server
  Serial.print("\nHTTP server started at: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  CheckAndResponse(client);
}