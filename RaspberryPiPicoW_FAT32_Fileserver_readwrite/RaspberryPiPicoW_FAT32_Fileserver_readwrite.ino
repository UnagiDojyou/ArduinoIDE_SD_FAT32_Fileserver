//for RaspberryPiPicoW
//Write ssid & Password
/*
 * Connect the SD card to the following pins:
   ************ SPI0 ************
   ** MISO (AKA RX) - pin 0, 4, or 16
   ** MOSI (AKA TX) - pin 3, 7, or 19
   ** CS            - pin 1, 5, or 17
   ** SCK           - pin 2, 6, or 18
 */

#define UseWiFi

#ifdef  UseWiFi
  #include <WiFi.h>
#else
  #include <Ethernet.h>
#endif

#include <SdFat.h>
#include <SPI.h>
#include "CheckAndResponse.h"

SdFat sd;

WiFiServer server(80);

const char* ssid = "Write your SSID";
const char* password = "Write your Password";

const int _MISO = 4;   // AKA SPI RX
const int _MOSI = 7;  // AKA SPI TX
const int _CS = 5;
const int _SCK = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (digitalRead(LED_BUILTIN)) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  int count = 0;
  // Initialize SDcard
  SPI.setRX(_MISO);
  SPI.setTX(_MOSI);
  SPI.setSCK(_SCK);

  //sd.begin(_CS, SD_SCK_MHZ(50));
  if (!sd.begin(_CS, SD_SCK_MHZ(50))){
    Serial.println("SD Card Mount Failed");
    while (count < 100) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      count++;
    }
    return;
  }

  /*int type = SD.type();
  if(type > 3){
    Serial.println("No SD card attached");
    while (count < 100) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      count++;
    }
    return;
  }*/

  Serial.println("SD Card initialized.");
  server.begin();  //start the server
  Serial.print("\nHTTP server started at: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  CheckAndResponse(client);
}
