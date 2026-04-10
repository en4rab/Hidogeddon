#ifndef HR_SETTINGS_H
#define HR_SETTINGS_H

#include <Arduino.h>

namespace hrSettings {

//Product
const extern String VERSION_NUMBER;
const extern String BUILD_DATE;

//Debug Mode
const extern bool SERIAL_CONSOLE_ONLY_DEBUG_MODE;

//PIN Assignments
const extern byte HEARTBEAT_LED_PIN;
const extern byte D1_PIN;
const extern byte D0_PIN;
const extern byte NOT_USED_PIN_A;
const extern byte NOT_USED_PIN_B;
const extern byte GREEN_LED_PIN;
const extern byte YELLOW_LED_PIN;
const extern byte RED_LED_PIN;
const extern byte EXT_PIN;
const extern byte OLED_SDA_PIN;
const extern byte OLED_SCL_PIN;

//OLED
const extern byte SCREEN_WIDTH;
const extern byte SCREEN_HEIGHT;
const extern byte OLED_RESET;
const extern byte SCREEN_ADDRESS;

//User Config Defaults (HTML Settings Page)
extern String apSSID;
extern String apPassword;
extern IPAddress apIP;
extern String wifiSSID;
extern String wifiPassword;
extern IPAddress wifiIP;
extern bool allowCaptivePortal;
extern String webPollSeconds;
extern bool oledConnected;
extern int wiegandPulseWidth;
extern int wiegandInterleave;
extern bool wiegandLearnMode;
extern bool captureUnknownBitLengths;

//User Config Defaults (HTML Special Cards page)
extern String TEST_CARD;
extern String FC_DEFAULT;
extern String FC_CONNECT_TO_AP;
extern String FC_SHOW_AP_INFO;

//BLE Settings
const extern String bleManufacturerTarget;
extern uint8_t bleAddrType;
extern bool bleEnabled;
extern String bleMAC;
extern bool bleScanMode;
extern volatile bool bleTriggerPending;

//General
extern bool oneShotConnectToAP;
extern String lastCardData;
extern String replayBin;

//Hidogeddon Logo
const extern unsigned char LOGO_OLED[];
const extern char LOGO_HTML[];

}  //namespace hrSettings

#endif
