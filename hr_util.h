#ifndef HR_UTIL_H
#define HR_UTIL_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "hr_settings.h"

namespace hrUtil {

void SettingsInit(void);
void GPIOInit(void);
void OLEDInit(void);
void SPIFFSInit(void);
void HeartBeat(void);
int TotalCardsInLog(void);
void SaveSettings(void);
void SaveSpecialCards(void);
void SaveBLESettings(void);
void ClearSettings(void);
void ClearSpecialCards(void);
void ClearBLESettings(void);
void OLEDShowLogo(void);
void OLEDShowAPInfo(void);
void OLEDShowMessage(String L1, String L2, String L3, String L4, String L5, String L6);
void RequestRebootAndConnectToAP(uint32_t rebootDelay);
void RequestReboot(uint32_t rebootDelay);
void CheckReboot(void);
void LedControl(bool greenLed, bool yellowLed, bool redLed, bool extOut);
void SendWiegand(String binaryData);
String CalculateBinary(String cardNo, int bits);
String ConvertBaseToString(uint64_t n, bool base16);
String SplitString(String s, char separator, int index);
bool IsNumeric(String s);
bool IsIpAddress(String s);

}  //namespace hrUtil

#endif
