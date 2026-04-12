#include "hr_settings.h"

namespace hrSettings {

//Product
const char VERSION_NUMBER[] = "1.1";
const char BUILD_DATE[] = "2026-04-12";

//Debug Mode - ONLY FOR FIRMWARE DEV/TEST -> !!! This should be set to false for normal operation !!!
const bool SERIAL_CONSOLE_ONLY_DEBUG_MODE = false;  //Writes SaveCardData() in hr_cardreader.cpp to the serial console rather than SPIFFS card_data.txt

//----------------------------------------------------------------------------------------------------------------------------
//PIN Assignments [ !!! SELECT ONLY ONE !!! ]
#define GPIO_ESP32_C3_SUPER_MINI_PCB    //ESP32 C3 Super Mini (PCB) [ en4rab ] [ https://github.com/en4rab/Hidogeddon        ]
//#define GPIO_ESP32_C3_SUPER_MINI_DIY  //ESP32 C3 Super Mini (DIY) [ 00Waz  ] [ https://github.com/00Waz/Hidogeddon         ]
//----------------------------------------------------------------------------------------------------------------------------

#ifdef GPIO_ESP32_C3_SUPER_MINI_PCB
const uint8_t HEARTBEAT_LED_PIN = 8;  //[ OUTPUT ] -> Built-in LED
const uint8_t D1_PIN = 0;             //[ INPUT  ] -> Wiegand D1 PIN (Clock on OLED PCB)
const uint8_t D0_PIN = 1;             //[ INPUT  ] -> Wiegand D0 PIN (Data on OLED PCB)
const uint8_t NOT_USED_PIN_A = 20;    //[ OUTPUT ] -> NO LONGER USED - SENDS WIEGAND ON SAME PINS
const uint8_t NOT_USED_PIN_B = 21;    //[ OUTPUT ] -> NO LONGER USED - SENDS WIEGAND ON SAME PINS
const uint8_t GREEN_LED_PIN = 4;      //[ OUTPUT ] -> Green LED
const uint8_t YELLOW_LED_PIN = 3;     //[ OUTPUT ] -> Yellow LED
const uint8_t RED_LED_PIN = 2;        //[ OUTPUT ] -> Red LED
const uint8_t EXT_PIN = 5;            //[ OUTPUT ] -> Pulse output when a card is both read and logged successfully
const uint8_t OLED_SDA_PIN = 6;       //I2C Serial Data
const uint8_t OLED_SCL_PIN = 7;       //I2C Serial Clock
#endif

#ifdef GPIO_ESP32_C3_SUPER_MINI_DIY
const uint8_t HEARTBEAT_LED_PIN = 8;  //[ OUTPUT ] -> Built-in LED
const uint8_t CLOCK_PIN = 3;          //[ INPUT  ] -> Wiegand D1 PIN
const uint8_t DATA_PIN = 4;           //[ INPUT  ] -> Wiegand D0 PIN
const uint8_t NOT_USED_PIN_A = 5;     //[ OUTPUT ] -> NO LONGER USED - SENDS WIEGAND ON SAME PINS
const uint8_t NOT_USED_PIN_B = 6;     //[ OUTPUT ] -> NO LONGER USED - SENDS WIEGAND ON SAME PINS
const uint8_t GREEN_LED_PIN = 0;      //[ OUTPUT ] -> Green LED
const uint8_t YELLOW_LED_PIN = 1;     //[ OUTPUT ] -> Yellow LED
const uint8_t RED_LED_PIN = 2;        //[ OUTPUT ] -> Red LED
const uint8_t EXT_PIN = 255;          //[ OUTPUT ] -> 255 = NOT USED
const uint8_t OLED_SDA_PIN = 255;     //I2C Serial Data -> 255 = NOT USED
const uint8_t OLED_SCL_PIN = 255;     //I2C Serial Clock -> 255 = NOT USED
#endif

//OLED
const uint8_t SCREEN_WIDTH = 128;     //OLED display width, in pixels
const uint8_t SCREEN_HEIGHT = 64;     //OLED display height, in pixels
const uint8_t OLED_RESET = -1;        //Reset pin (-1 if sharing Arduino reset pin)
const uint8_t SCREEN_ADDRESS = 0x3C;  //0x3D for 128x64, 0x3C for 128x32 (see datasheet)

//User Config Defaults - Set by HTML Settings Page
String apSSID = "Hidogeddon";
String apPassword = "13371337";
IPAddress apIP(192, 168, 3, 10);
String wifiSSID = "";
String wifiPassword = "";
IPAddress wifiIP(0, 0, 0, 0);
bool allowCaptivePortal = true;
String webPollSeconds = "5";
#ifdef GPIO_ESP32_C3_SUPER_MINI_PCB
bool oledConnected = true;
#endif
#ifdef GPIO_ESP32_C3_SUPER_MINI_DIY
bool oledConnected = false;
#endif
int wiegandPulseWidth = 50;
int wiegandInterleave = 2000;
bool wiegandLearnMode = false;
bool captureUnknownBitLengths = false;
bool bleEnabled = false;

//BLE Settings
//DO NOT CHANGE THESE
const char *bleManufacturerTarget = "716";
uint8_t bleAddrType = 0;
String bleMAC = "";

//User Config Defaults - Set by HTML Special Cards Page
//DO NOT CHANGE THESE
String TEST_CARD = "n";         //Test Card - works as normal but will log the type as "TEST CARD"
String FC_DEFAULT = "n";        //Function Card - Defaults the settings and Reboots the ESP32 whilst retaining the log
String FC_CONNECT_TO_AP = "n";  //Function Card - Reboots the ESP32 and connects to an access point rather than creating one, if SSID and PW are set
String FC_SHOW_AP_INFO = "n";   //Function Card - Shows AP info on the OLED if the OLED is set as connected

//General
//DO NOT CHANGE THESE
bool oneShotConnectToAP = false;
String lastCardData = "Ready";
String replayBin = "";
int logCount = 0;

}  //namespace hrSettings
