#ifndef CHECHANDRESPONSE_H
#define CHECHANDRESPONSE_H
#include <WiFi.h>
#include <SD.h>
#include <SPI.h>

bool CheckAndResponse(WiFiClient &client);


void process_request(WiFiClient& client, String request);

void sendHTTP(WiFiClient& client, const String& request);
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