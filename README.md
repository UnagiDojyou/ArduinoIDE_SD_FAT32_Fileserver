# ArduinoIDE SD FAT32 Fileserver
Connect an SD card via SPI and browse, edit (delete, rename), and upload the contents of the SD card via HTTP.
## feature
* View SD card contetnts on browser.
* Download contents.
* File upload.
* Wide compatibility.<br>
Easy using to other boards.
* No additional libraries required.<br>
only use SD.h (& FS.h) & WiFi.h (or Ethernet.h)

## Use with other boards
### For boards with WiFi
* Rewrite 2 in "#define Blue_LED 2". <br>
Blue_LED serves as an indicator during WiFi and SD card initialization. Therefore, it should be a GPIO with an LED.<br>
* FS.h should be erased if necessary.<br>
* It is believed that Rename is not available on many non-ESP boards.<br>
* uint8_t cardType = SD.cardType(); may not work, delete as appropriate.<br>
### For boards with Ethernet instead of WiFi
* Rewrite 2 in "#define Blue_LED 2".<br>
Blue_LED serves as an indicator during WiFi and SD card initialization. Therefore, it should be a GPIO with an LED.<br>
* FS.h should be erased if necessary.<br>
* Include Ethernet.h instead of WiFi.h.<br>
And Please define MAC address, change WiFi to Ethernet, and change setup. Especially the setup will most likely change.<br>
* It is believed that Rename is not available on many non-ESP boards.<br>
* uint8_t cardType = SD.cardType(); may not work, delete as appropriate.<br>
