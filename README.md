# ArduinoIDE SD FAT32 Fileserver
Connect an SD card via SPI and browse, edit (delete, rename), and upload the contents of the SD card via HTTP.
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
* __ArduinoUNO_FAT32_Fileserver_readonly__<br>
For ArduinoUNO with Ethernet shield.<br>
Files can be only viewed. Limited functionality to work with less RAM and Flash.<br>
Enter the MAC address and IP address in the sketch.<br>
* __ArduinoMEGA_FAT32_Fileserver_readwrite__<br>
__NOT TESTED__<br>
For ArduinoMEGA with Ethernet shield.<br>
Files can be viewed, uploaded, and deleted.<br>
SD.h for Arduino does not support rename, so there is no renaming function.<br>
Enter the MAC address and IP address in the sketch.<br>
* __ArduinoMEGA_FAT32_Fileserver_readonly__<br>
__NOT TESTED__<br>
For ArduinoMEGA with Ethernet shield.<br>
Files can be only viewed.<br>
Enter the MAC address and IP address in the sketch.<br>

## Use with other boards
### For boards with WiFi
* __Use the one for ESP32 as a base.__<br>
* __Rewrite 2 in "#define Blue_LED 2".__ <br>
Blue_LED serves as an indicator during WiFi and SD card initialization. Therefore, it should be a GPIO with an LED.<br>
* __FS.h should be erased if necessary.__<br>
* __It is believed that Rename is not available on many non-ESP boards.__<br>
* __uint8_t cardType = SD.cardType(); may not work, delete as appropriate.__<br>
### For boards with Ethernet
* __Use the one for ArduinoMEGA as a base.__<br>
* __Rewrite 4 in "#define chipSelect 4".__<br>
* __If SD.h supports rename, refer to the one for ESP32 and re-enable it.__<br>
