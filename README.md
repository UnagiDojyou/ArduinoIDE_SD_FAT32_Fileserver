# ArduinoIDE SD FAT32 Fileserver
Connect an SD card via SPI and browse, edit (delete, rename), and upload the contents of the SD card via HTTP.<br>
<br>
![sample](samplevideo.gif)

## Feature
* __View SD card contetnts on browser.__
* __Download contents.__
* __File upload.__
* __Wide compatibility.__<br>
Easy using to other boards.
* __No additional libraries required.__<br>
Use only SD.h (& FS.h) & WiFi.h (or Ethernet.h)

## About each version
* __ESP32_FAT32_Fileserver_readwrite__<br>
Files can be viewed, uploaded, deleted, and renamed.<br>
Enter your WiFi SSID and password into the sketch.<br>
This is the most basic version for boards with WiFi.<br>
* __ESP32_FAT32_Fileserver_readonly__<br>
Files can be only viewed.<br>
Enter your WiFi SSID and password into the sketch.<br>
* __RaspberryPiPicoW_FAT32_Fileserver_readwrite__<br>
Files can be viewed, uploaded, deleted, and renamed.<br>
Enter your WiFi SSID and password into the sketch.<br>
* __RaspberryPiPicoW_FAT32_Fileserver_readonly__<br>
Files can be only viewed.<br>
Enter your WiFi SSID and password into the sketch.<br>
* __ArduinoUNO_FAT32_Fileserver_readonly__<br>
For ArduinoUNO with Ethernet shield.<br>
Files can be only viewed. Limited functionality to work with less RAM and Flash.<br>
Enter the MAC address and IP address in the sketch.<br>
* __ArduinoMEGA_FAT32_Fileserver_readwrite__<br>
For ArduinoMEGA with Ethernet shield.<br>
Files can be viewed, uploaded, and deleted.<br>
SD.h for Arduino does not support rename, so there is no renaming function.<br>
Enter the MAC address and IP address in the sketch.<br>
* __ArduinoMEGA_FAT32_Fileserver_readonly__<br>
For ArduinoMEGA with Ethernet shield.<br>
Files can be only viewed.<br>
Enter the MAC address and IP address in the sketch.<br>

## Use with other boards
Include CheckAndResponse.h.<br>
SD.bigin() and server.begin() have to done in the setup, and CheckAndResponse() have to be written in the loop. <br>
Select readwrite/readonly in CheckAndResponse.h.<br>
Select UseWiFi/UseEthernet in CheckAndResponse.h.<br>
Select enable/disable Rename function in CheckAndResponse.h. (Some boards' SD.h do not support rename.)<br>
