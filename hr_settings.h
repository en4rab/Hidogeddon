#ifndef HR_SETTINGS_H
#define HR_SETTINGS_H

#include <Arduino.h>

namespace hrSettings {

//Product
const extern char VERSION_NUMBER[];
const extern char BUILD_DATE[];

//Debug Mode
const extern bool SERIAL_CONSOLE_ONLY_DEBUG_MODE;

//PIN Assignments
const extern uint8_t HEARTBEAT_LED_PIN;
const extern uint8_t D1_PIN;
const extern uint8_t D0_PIN;
const extern uint8_t NOT_USED_PIN_A;
const extern uint8_t NOT_USED_PIN_B;
const extern uint8_t GREEN_LED_PIN;
const extern uint8_t YELLOW_LED_PIN;
const extern uint8_t RED_LED_PIN;
const extern uint8_t EXT_PIN;
const extern uint8_t OLED_SDA_PIN;
const extern uint8_t OLED_SCL_PIN;

//OLED
const extern uint8_t SCREEN_WIDTH;
const extern uint8_t SCREEN_HEIGHT;
const extern uint8_t OLED_RESET;
const extern uint8_t SCREEN_ADDRESS;

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
extern bool bleEnabled;

//BLE Settings
const extern char *bleManufacturerTarget;
extern uint8_t bleAddrType;
extern String bleMAC;

//User Config Defaults (HTML Special Cards page)
extern String TEST_CARD;
extern String FC_DEFAULT;
extern String FC_CONNECT_TO_AP;
extern String FC_SHOW_AP_INFO;

//General
extern bool oneShotConnectToAP;
extern String lastCardData;
extern String replayBin;
extern int logCount;

}  //namespace hrSettings

#endif
