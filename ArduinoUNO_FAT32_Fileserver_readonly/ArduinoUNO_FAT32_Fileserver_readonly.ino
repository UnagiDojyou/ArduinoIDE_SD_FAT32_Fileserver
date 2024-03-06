//only read
//for Arduino UNO with shield
//Write MAC address & IP adress*2
//This was created by modifying ESP32_FAT32_Filserver_readwrite.
//Commenting out points to features that were cut due to lack of RAM.

#include <Ethernet.h>
#include <SD.h>
#include <SPI.h>

#define chipSelect 4
#define Blue_LED 13 //LED on EthernetShield

EthernetServer server(80);

const byte mac[] = { Write your board MAC address }; //ex.{ 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D }
const byte ip[] = { Write IP address }; //ex.{ 192, 168, 0, 200 }
const char IPaddr[] = "Write the same IP address as you wrote above"; //ex."192.168.0.200"

// decode %URL
String urlDecode(String str) {
  String decoded = "";
  char temp[] = "0x00";
  int str_len = str.length();

  for (int i = 0; i < str_len; i++) {
    if (str[i] == '%') {
      if (i + 2 < str_len) {
        temp[2] = str[i + 1];
        temp[3] = str[i + 2];
        decoded += (char) strtol(temp, NULL, 16);
        i += 2;
      }
    } else if (str[i] == '+') {
      decoded += ' ';
    } else {
      decoded += str[i];
    }
  }
  return decoded;
}

/*// ecode %URL
String urlEncode(String str) {
  String encoded = "";
  char temp[] = "0x00";
  
  for (int i = 0; i < str.length(); i++) {
    char c = str[i];
    
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '/') {
      encoded += c;
    //} else if (c == ' ') {
    //  encoded += '+';
    } else {
      sprintf(temp, "%02X", c);
      encoded += '%' + String(temp);
    }
  }
  return encoded;
}*/

//defines for client
#define BUFFLEN 30 //length of the receive buffer
char buff[BUFFLEN]; //buffer
int count = 0; //counter for the buffer
bool isBlankLine = false; //if the last line is empty, the end of request
String path;

//put received char in buffer and check the GET command and empty line
void processReequest(char c) { //Changed to void because it includes the function of process_request()
  if(c == '\r'){ //if the code is CR, ignore it
    isBlankLine = false;
    //return "";
  }
  else if(c == '\n') {  //if the code is NL, read the GET request
    buff[count]='\0'; //put null character at the end
    //String request = buff; //convert to String
    isBlankLine = (count == 0); //and check if the line is empty
    count=0;
    if(strncmp(buff, "GET", 3) == 0){
      path = buff;
      //Serial.println(path);
      path.replace("GET ","");
      path.replace(" HTTP/1.1","");
    }
    //return request;
  }
  else { //if the code is not control code, record it
    isBlankLine = false;
    if(count >= (BUFFLEN - 1) ) count=0; //if almost overflow, reset the counter
      buff[count++]=c; //add char at the end of buffer
    //return "";
  }
}

/*// https://qiita.com/dojyorin/items/ac56a1c2c620782d90a6
String ipToString(uint32_t ip){
    String result = "";
    result += String((ip & 0xFF), 10);
    result += ".";
    result += String((ip & 0xFF00) >> 8, 10);
    result += ".";
    result += String((ip & 0xFF0000) >> 16, 10);
    result += ".";
    result += String((ip & 0xFF000000) >> 24, 10);
    return result;
}*/

/*String getExtension(const String& filename) {
  int dotIndex = filename.lastIndexOf('.');
  if (dotIndex == -1 || dotIndex == filename.length() - 1) {
    return "";
  }
  return filename.substring(dotIndex + 1);
}*/

/*String getFilename(const String& filename) {
  int dotIndex = filename.lastIndexOf('/');
  if (dotIndex == -1 || dotIndex == filename.length() - 1) {
    return "";
  }
  return filename.substring(dotIndex + 1);
}*/

/*//decide header by extension
String getType(const String& extension) {
  if (extension.equalsIgnoreCase("txt")) {
    return "text/plain";
  } else if (extension.equalsIgnoreCase("csv")) {
    return "text/csv";
  } else if (extension.equalsIgnoreCase("html") || extension.equalsIgnoreCase("htm")) {
    return "text/html";
  } else if (extension.equalsIgnoreCase("css")) {
    return "text/css";
  } else if (extension.equalsIgnoreCase("js")) {
    return "text/javascript";
  } else if (extension.equalsIgnoreCase("json")) {
    return "application/json";
  } else if (extension.equalsIgnoreCase("pdf")) {
    return "application/pdf";
  } else if (extension.equalsIgnoreCase("jpg") || extension.equalsIgnoreCase("jpeg")) {
    return "image/jpeg";
  } else if (extension.equalsIgnoreCase("png")) {
    return "image/png";
  } else if (extension.equalsIgnoreCase("gif")) {
    return "image/gif";
  } else if (extension.equalsIgnoreCase("svg")) {
    return "image/svg+xml";
  } else if (extension.equalsIgnoreCase("zip")) {
    return "application/zip";
  } else if (extension.equalsIgnoreCase("mpeg") || extension.equalsIgnoreCase("mpg")) {
    return "video/mpeg";
  } else {
    return "other";
  }
}*/

String kmgt(unsigned long bytes){
  if(bytes < 1000){
    return String(bytes)+"B";
  }
  else if(1000 <= bytes && bytes < 1000000){
    return String(int(bytes/1000))+"KB";
  }
  else if(1000000 <= bytes && bytes < 1000000000){
    return String(int(bytes/1000000))+"MB";
  }
  else{
    return String(int(bytes/1000000000))+"GB";
  }
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

  // Initialize SDcard
  if (!SD.begin(chipSelect)) {
    //Serial.println("SD Card Mount Failed");
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
  //Serial.println("SD Card initialized.");
  
  //connect to Ethernet
  Ethernet.begin( mac, ip );
  server.begin(); //start the server
  //Serial.print("HTTP server started at: ");
  //Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    while(client.connected()) {
      if(client.available()) {
        /*char c = client.read();
        //Serial.write(c); //Output Request from client
        String request = processReequest(c);*/
        processReequest(client.read());
        /*if(!request.equals("")){
          process_request(client,request);
        }*/
        //if the line is blank, the request has ended.
        if(isBlankLine){
          sendHTTP(client); //send HTTP response
          break;
        }
      }
    }
  }
}

/*void process_request(WiFiClient &client,String request){
  if(request.startsWith("GET")){
    //String path = request.substring(4).toInt();
    path = request;
    path.replace("GET ","");
    path.replace(" HTTP/1.1","");
    path = urlDecode(path); //decode%
  }
}*/

void sendHTTP(EthernetClient &client) {
  //Serial.println("");
  //Serial.print("path:");
  //Serial.println(path);
  if(path.equals("")){
    path = "/";
  }
  if(path.endsWith("/")){ //Directory
    if(!path.equals("/")){
      path.remove(path.length()-1); //delet last "/"
    }
    if(SD.exists(path) || path.equals("/")){ //check path is exist
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("");
      client.println("");
      //Serial.print("Directroy:");
      //Serial.println(path);
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>SD Reader</title>");
      client.println("<meta charset=\"UTF-8\">");
      client.println("</head>");
      client.println("<body>");
      //display current path
      client.print("<h1>");
      client.print(path);
      if(!path.equals("/")){ //add "/"
        client.print("/");
      }
      client.println("</h1>");
      //Parent Directory
      //String IPaddr = ipToString(Ethernet.localIP());
      if(!path.equals("/")){
        client.print("<p><a href=\"");
        client.print("http://");
        client.print(IPaddr);
        client.print(path.substring(0, path.lastIndexOf("/")+1)); //Parent Directory path(not encoded)
        client.print("\" >");
        client.print("Parent Directory");
        client.print("</a>");
        client.println("</p>");
      }
      client.println("<hr>");
      //list up directory contents
      File dir = SD.open(path);
      while(true){
        File entry = dir.openNextFile();
        if (!entry){
          break;
        }
        /*String filename = entry.name();
        if (entry.isDirectory()) {
          filename += "/";
        }
        String fileurl = "http://" + IPaddr + urlEncode(path);
        if(path.equals("/")){
           fileurl = fileurl + filename;
        }
        else{
          fileurl = fileurl + "/" + urlEncode(filename);
        }*/
        client.print("<p>");
        client.print("<a href=\"");
        //client.print(fileurl);
        client.print("http://");
        client.print(IPaddr);
        client.print(path);
        if(!path.equals("/")){
          client.print("/");
        }
        client.print(entry.name());
        if (entry.isDirectory()) {
          client.print("/");
        }
        client.print("\" >");
        //client.print(filename);
        client.print(entry.name());
        if (entry.isDirectory()) {
          client.print("/");
        }
        client.println("</a>");
        if(!entry.isDirectory()){ //display file size
          client.print(" ");
          client.println(kmgt(entry.size()));
        }
        client.println("</p>");
        entry.close();
      }
      dir.close();
      client.println("<hr>");
      client.print("<p>");
      client.print("Powered by ");
      client.print("<a href=\"https://github.com/UnagiDojyou/ArduinoIDE_SD_FAT32_Fileserver\">ArduinoIDE_SD_FAT32_Fileserver</a>");
      client.println("</p>");
      client.println("</body>");
      client.println("</html>");
    }
    else{ //Directory not Found
      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
    }
  }
  else{ //File
    if(SD.exists(path)){
      //Serial.print("File:");
      //Serial.println(path);
      //String extension = getExtension(getFilename(path));
      File file = SD.open(path);
      //unsigned long filesize = file.size();
      client.println("HTTP/1.1 200 OK");
      client.print("Content-Length: ");
      client.println(file.size());
      /*if(extension.equals("other")){
        client.print("Content-Disposition");
      }
      else{
        client.print("Content-Type: ");
        client.println(getType(extension));
      }*/
      client.println("Connection: close");
      client.println();
      //send file to client with 30 buffer.
      const size_t bufferSize = 30; //buffer size
      byte buffe[bufferSize]; //buffe
      while (file.available()) {
        size_t bytesRead = file.read(buffe, bufferSize);
        client.write(buffe, bytesRead);
      }
      file.close();
    }
    else{ //File not Found
      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
    }
  }
  client.println("");
  client.stop();
  //Serial.println("Response finish");
  return;
}
