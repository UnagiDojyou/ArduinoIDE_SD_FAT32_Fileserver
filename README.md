
# ArduinoIDE SdFat Fileserver
Use Sdfat.h instead of SD.h.<br>
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
Use only SdFat.h & WiFi.h (or Ethernet.h)

## About each version
* __RaspberryPiPicoW_FAT32_Fileserver_readwrite__<br>
Files can be viewed, uploaded, deleted, and renamed.<br>
Enter your WiFi SSID and password into the sketch.<br>

## Use with other boards
Include CheckAndResponse.h.<br>
sd.bigin() and server.begin() have to done in the setup, and CheckAndResponse() have to be written in the loop. <br>
Select readwrite/readonly in CheckAndResponse.h.<br>
Select UseWiFi/UseEthernet in CheckAndResponse.h.<br>