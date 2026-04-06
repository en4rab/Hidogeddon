/*
_   _ ___________ _____ _____  _________________ _____ _   _  
| | | |_   _|  _  \  _  |  __ \|  ___|  _  \  _  \  _  | \ | | 
| |_| | | | | | | | | | | |  \/| |__ | | | | | | | | | |  \| | 
|  _  | | | | | | | | | | | __ |  __|| | | | | | | | | | . ` | 
| | | |_| |_| |/ /\ \_/ / |_\ \| |___| |/ /| |/ /\ \_/ / |\  | 
\_| |_/\___/|___/  \___/ \____/\____/|___/ |___/  \___/\_| \_/

Product Name:  Vibrate BLE for Hidogeddon Reader
Version:       1.0
Build Date:    2026-04-06
Author:        Daniel Raines [ Dr0pR00t ] [ https://www.linkedin.com/in/danielraines ]

::CREDITS::
Cardinal_Black: Original BLE lib/commands for FitPro M4/M5/M6
Craigsblackie & Iceman: ASCII art

::FITPRO SHOPPING LINKS::
https://www.aliexpress.com/item/1005008971783153.html
https://www.aliexpress.com/item/1005009086572716.html

::HARDWARE::
This firmware was tested using a FireBeetle 2 ESP32-E
https://www.dfrobot.com/product-2195.html

::GITHUB::HIDOGEDDON::
The home for this project, OLED-PCB and DIY versions, respectively
https://github.com/en4rab/Hidogeddon
https://github.com/00Waz/Hidogeddon

::PIN ASSIGNMENTS::
You can define PIN assignments in hr_settings.cpp

::IDE::
This firmware was written, compiled and tested using Arduino IDE v2.3.6

*/

#include <Arduino.h>
#include "hr_settings.h"
#include "hr_util.h"
#include "hr_ble.h"

void setup() {
  hrUtil::SettingsInit();
  hrUtil::GPIOInit();
  hrBLE::FitProM4Init();
}

void loop() {
  if (hrSettings::bleScanMode) {
    hrBLE::ScanDevices();
  } else {
    hrBLE::MaintainConnection();
  }
  hrBLE::InputTriggerHandler();
}
