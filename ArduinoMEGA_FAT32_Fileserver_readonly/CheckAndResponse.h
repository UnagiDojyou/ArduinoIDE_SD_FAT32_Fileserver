#ifndef CHECHANDRESPONSE_H
#define CHECHANDRESPONSE_H

//you could chose readwrite\readonly
#define readonly

//you could chose UseWiFi/UseEthernet
#define UseEthernet

//Enable/Disable Rename
#define DisableRename


#ifdef UseWiFi
  #include <WiFi.h>
  #define WiFiEthernet WiFi
  #define WiFiEthernetClient WiFiClient
#else
  #include <Ethernet.h>
  #define WiFiEthernet Ethernet
  #define WiFiEthernetClient EthernetClient
#endif

#include <SD.h>
#include <SPI.h>

bool CheckAndResponse(WiFiEthernetClient &client);


void process_request(WiFiEthernetClient& client, String request);

void sendHTTP(WiFiEthernetClient& client, const String& request);
/*
bool checkfilename(String checkstr);

String kmgt(unsigned long bytes);

String getType(const String& extension);

String getFilename(const String& filename);

String getExtension(const String& filename);

String ipToString(uint32_t ip);

String processReequest(char c);

String urlEncode(String str);

String urlDecode(String str);
*/

#endif