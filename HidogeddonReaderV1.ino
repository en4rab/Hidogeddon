/*
_   _ ___________ _____ _____  _________________ _____ _   _  
| | | |_   _|  _  \  _  |  __ \|  ___|  _  \  _  \  _  | \ | | 
| |_| | | | | | | | | | | |  \/| |__ | | | | | | | | | |  \| | 
|  _  | | | | | | | | | | | __ |  __|| | | | | | | | | | . ` | 
| | | |_| |_| |/ /\ \_/ / |_\ \| |___| |/ /| |/ /\ \_/ / |\  | 
\_| |_/\___/|___/  \___/ \____/\____/|___/ |___/  \___/\_| \_/

Product Name:  Hidogeddon Reader
Version:       1.0
Build Date:    2026-04-06
Author:        Daniel Raines [ Dr0pR00t ] [ https://www.linkedin.com/in/danielraines ]

::CREDITS::
ALL: Moral support + just being an awesome dudes!
0xFFFF and 00Waz: Data Formats [see below references]
En4rab: OLED Hidogeddon Logo
Cardinal_Black: Original BLE lib/commands for FitPro M4/M5/M6 > VibrateBLE.ino
Craigsblackie & Iceman: ASCII art

::REFERENCES::
https://web.archive.org/web/20181108040155/http://cardinfo.barkweb.com.au
https://www.everythingid.com.au/hid-card-formats-i-15

::HARDWARE::
This firmware was tested using the OLED-PCB
https://github.com/en4rab/Paxtogeddon-OLED-PCB

::GITHUB::
The home for this project, OLED-PCB and DIY versions, respectively
https://github.com/en4rab/Hidogeddon
https://github.com/00Waz/Hidogeddon

::PIN ASSIGNMENTS::
You can define PIN assignments in hr_settings.cpp 
by simply uncommenting your desired board
Default is #define GPIO_ESP32_C3_SUPER_MINI_PCB

::IDE::
This firmware was written, compiled and tested using Arduino IDE v2.3.6

::LIBS::
The following additional/add-on libraries are required by this firmware
ESPAsyncWebSrv by dvarrel v1.2.9
AsyncTCP by dvarrel v1.1.4
Adafruit SSD1306 by Adafruit v2.5.14
Adafruit GFX Library by Adafruit v1.12.1
Adafruit BusIO by Adafruit v1.17.1

*/

#include <Arduino.h>
#include "hr_settings.h"
#include "hr_util.h"
#include "hr_server.h"
#include "hr_html.h"
#include "hr_cardreader.h"

void setup() {
  hrUtil::SettingsInit();
  hrUtil::GPIOInit();
  hrUtil::OLEDInit();
  hrUtil::SPIFFSInit();
  hrCardReader::CardReaderInit();
  hrServer::ServerInit();
}

void loop() {
  hrUtil::HeartBeat();
  hrUtil::CheckReboot();
  hrUtil::CheckTriggerBLEScan();
  hrCardReader::ParseCard();
}
