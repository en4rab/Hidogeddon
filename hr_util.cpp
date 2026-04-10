#include "hr_util.h"

namespace hrUtil {

uint32_t systemRebootDelay = 5000;
bool systemReboot = false;
Preferences preferences;
Adafruit_SSD1306 display(hrSettings::SCREEN_WIDTH, hrSettings::SCREEN_HEIGHT, &Wire, hrSettings::OLED_RESET);

//Inits
void SettingsInit(void) {
  if (hrSettings::SERIAL_CONSOLE_ONLY_DEBUG_MODE) {
    Serial.begin(115200);
  }

  preferences.begin("prOneShot", false);
  if (preferences.getBool("CONNECT_AP", false)) {
    preferences.putBool("CONNECT_AP", false);
    hrSettings::oneShotConnectToAP = true;
  }
  preferences.end();

  preferences.begin("hrSettings", false);
  if (preferences.getBool("hrs_ok", false)) {
    hrSettings::apSSID = preferences.getString("AP_SSID", hrSettings::apSSID);
    hrSettings::apPassword = preferences.getString("AP_PW", hrSettings::apPassword);
    String ipAddr = preferences.getString("AP_IP", hrSettings::apIP.toString());
    hrSettings::apIP.fromString(ipAddr);
    hrSettings::wifiSSID = preferences.getString("WIFI_SSID", hrSettings::wifiSSID);
    hrSettings::wifiPassword = preferences.getString("WIFI_PW", hrSettings::wifiPassword);
    hrSettings::allowCaptivePortal = preferences.getBool("ALLOW_CP", hrSettings::allowCaptivePortal);
    hrSettings::webPollSeconds = preferences.getString("WEB_POLL", hrSettings::webPollSeconds);
    hrSettings::oledConnected = preferences.getBool("OLED_CON", hrSettings::oledConnected);
    hrSettings::wiegandPulseWidth = preferences.getInt("W_PULSE", hrSettings::wiegandPulseWidth);
    hrSettings::wiegandInterleave = preferences.getInt("W_INTER", hrSettings::wiegandInterleave);
    hrSettings::wiegandLearnMode = preferences.getBool("W_LMODE", hrSettings::wiegandLearnMode);
    hrSettings::captureUnknownBitLengths = preferences.getBool("CAP_UBL", hrSettings::captureUnknownBitLengths);
    hrSettings::bleEnabled  = preferences.getBool("BLE_EN",    hrSettings::bleEnabled);
    hrSettings::bleMAC      = preferences.getString("BLE_MAC",  hrSettings::bleMAC);
    hrSettings::bleAddrType = preferences.getUChar("BLE_ATYPE", hrSettings::bleAddrType);
  }
  preferences.end();

  if (hrSettings::bleEnabled && hrSettings::bleMAC == "") {
    hrSettings::bleScanMode = true;
  }

  preferences.begin("hrSpecCards", false);
  if (preferences.getBool("hrs_ok", false)) {
    hrSettings::TEST_CARD = preferences.getString("TEST_CARD", hrSettings::TEST_CARD);
    hrSettings::FC_DEFAULT = preferences.getString("FC_DEF", hrSettings::FC_DEFAULT);
    hrSettings::FC_CONNECT_TO_AP = preferences.getString("FC_CTAP", hrSettings::FC_CONNECT_TO_AP);
    hrSettings::FC_SHOW_AP_INFO = preferences.getString("FC_SAPI", hrSettings::FC_SHOW_AP_INFO);
  }
  preferences.end();
}

void GPIOInit(void) {
  pinMode(hrSettings::D1_PIN, INPUT);
  pinMode(hrSettings::D0_PIN, INPUT);
  pinMode(hrSettings::NOT_USED_PIN_A, OUTPUT);
  pinMode(hrSettings::NOT_USED_PIN_B, OUTPUT);
  pinMode(hrSettings::GREEN_LED_PIN, OUTPUT);
  pinMode(hrSettings::YELLOW_LED_PIN, OUTPUT);
  pinMode(hrSettings::RED_LED_PIN, OUTPUT);
  pinMode(hrSettings::HEARTBEAT_LED_PIN, OUTPUT);

  digitalWrite(hrSettings::NOT_USED_PIN_A, 0);
  digitalWrite(hrSettings::NOT_USED_PIN_B, 0);
  digitalWrite(hrSettings::GREEN_LED_PIN, 0);
  digitalWrite(hrSettings::YELLOW_LED_PIN, 0);
  digitalWrite(hrSettings::RED_LED_PIN, 0);
  digitalWrite(hrSettings::HEARTBEAT_LED_PIN, 1);

  if (hrSettings::EXT_PIN < 255) {
    pinMode(hrSettings::EXT_PIN, OUTPUT);
    digitalWrite(hrSettings::EXT_PIN, 0);
  }

  delay(500);
  hrUtil::LedControl(true, true, true, false);
}

void OLEDInit(void) {
  if (hrSettings::OLED_SDA_PIN == 255) { hrSettings::oledConnected = false; }
  if (hrSettings::OLED_SCL_PIN == 255) { hrSettings::oledConnected = false; }

  if (hrSettings::oledConnected) {
    Wire.setPins(hrSettings::OLED_SDA_PIN, hrSettings::OLED_SCL_PIN);
    display.begin(SSD1306_SWITCHCAPVCC, hrSettings::SCREEN_ADDRESS);
    OLEDShowLogo();
    delay(2000);
    OLEDShowMessage("Hidogeddon Reader", "", ":Version:", hrSettings::VERSION_NUMBER, ":Build Date:", hrSettings::BUILD_DATE);
  }
}

void SPIFFSInit(void) {
  if (SPIFFS.begin(true)) {
    return;
  }

  if (SPIFFS.format()) {
    if (SPIFFS.begin(true)) {
      return;
    }
  }

  if (hrSettings::oledConnected) {
    delay(2000);
    OLEDShowMessage("SPIFFS init failed", "", "", "", "", "");
  }

  while (true) {
    LedControl(false, false, true, false);
  }
}


//Utils
void HeartBeat(void) {
  digitalWrite(hrSettings::HEARTBEAT_LED_PIN, 0);
  delay(100);
  digitalWrite(hrSettings::HEARTBEAT_LED_PIN, 1);
  delay(100);
}

void SaveSettings(void) {
  preferences.begin("hrSettings", false);
  preferences.putString("AP_SSID", hrSettings::apSSID);
  preferences.putString("AP_PW", hrSettings::apPassword);
  preferences.putString("AP_IP", hrSettings::apIP.toString());
  preferences.putString("WIFI_SSID", hrSettings::wifiSSID);
  preferences.putString("WIFI_PW", hrSettings::wifiPassword);
  preferences.putBool("ALLOW_CP", hrSettings::allowCaptivePortal);
  preferences.putString("WEB_POLL", hrSettings::webPollSeconds);
  preferences.putBool("OLED_CON", hrSettings::oledConnected);
  preferences.putInt("W_PULSE", hrSettings::wiegandPulseWidth);
  preferences.putInt("W_INTER", hrSettings::wiegandInterleave);
  preferences.putBool("W_LMODE", hrSettings::wiegandLearnMode);
  preferences.putBool("CAP_UBL", hrSettings::captureUnknownBitLengths);
  preferences.putBool("BLE_EN",     hrSettings::bleEnabled);
  preferences.putString("BLE_MAC",  hrSettings::bleMAC);
  preferences.putUChar("BLE_ATYPE", hrSettings::bleAddrType);
  preferences.putBool("hrs_ok", true);
  preferences.end();
}

void SaveSpecialCards(void) {
  preferences.begin("hrSpecCards", false);
  preferences.putString("TEST_CARD", hrSettings::TEST_CARD);
  preferences.putString("FC_DEF", hrSettings::FC_DEFAULT);
  preferences.putString("FC_CTAP", hrSettings::FC_CONNECT_TO_AP);
  preferences.putString("FC_SAPI", hrSettings::FC_SHOW_AP_INFO);
  preferences.putBool("hrs_ok", true);
  preferences.end();
}

void ClearSettings(void) {
  preferences.begin("hrSettings", false);
  preferences.clear();
  preferences.end();
  preferences.begin("prOneShot", false);
  preferences.putBool("CONNECT_AP", false);
  preferences.end();
}

void ClearSpecialCards(void) {
  hrSettings::TEST_CARD = "n";
  hrSettings::FC_DEFAULT = "n";
  hrSettings::FC_CONNECT_TO_AP = "n";
  hrSettings::FC_SHOW_AP_INFO = "n";
  preferences.begin("hrSpecCards", false);
  preferences.clear();
  preferences.end();
}

void OLEDShowLogo(void) {
  if (hrSettings::oledConnected) {
    display.clearDisplay();
    display.drawBitmap(0, 0, hrSettings::LOGO_OLED, hrSettings::SCREEN_WIDTH, hrSettings::SCREEN_HEIGHT, WHITE);
    display.display();
  }
}

void OLEDShowAPInfo(void) {
  if (hrSettings::oledConnected) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (hrSettings::oneShotConnectToAP) {
      display.setCursor(0, 0);
      display.println("Connected to WiFi");
      display.setCursor(0, 20);
      display.println(":SSID:");
      display.setCursor(0, 30);
      display.println(hrSettings::wifiSSID);
      display.setCursor(0, 40);
      display.println(":IP:");
      display.setCursor(0, 50);
      display.println(hrSettings::wifiIP);
    } else {
      display.setCursor(0, 0);
      display.println(":SSID:");
      display.setCursor(0, 10);
      display.println(hrSettings::apSSID);
      display.setCursor(0, 20);
      display.println(":PASS:");
      display.setCursor(0, 30);
      display.println(hrSettings::apPassword);
      display.setCursor(0, 40);
      display.println(":IP:");
      display.setCursor(0, 50);
      display.println(hrSettings::apIP);
    }

    display.display();
  }
}

void OLEDShowMessage(String L1, String L2, String L3, String L4, String L5, String L6) {
  if (hrSettings::oledConnected) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(L1);
    display.setCursor(0, 10);
    display.println(L2);
    display.setCursor(0, 20);
    display.println(L3);
    display.setCursor(0, 30);
    display.println(L4);
    display.setCursor(0, 40);
    display.println(L5);
    display.setCursor(0, 50);
    display.println(L6);
    display.display();
  }
}

void RequestRebootAndConnectToAP(uint32_t rebootDelay) {
  preferences.begin("prOneShot", false);
  preferences.putBool("CONNECT_AP", true);
  preferences.end();
  systemRebootDelay = rebootDelay;
  systemReboot = true;
}

void RequestReboot(uint32_t rebootDelay) {
  systemRebootDelay = rebootDelay;
  systemReboot = true;
}

void CheckReboot(void) {
  if (!systemReboot) { return; }
  systemReboot = false;
  digitalWrite(hrSettings::HEARTBEAT_LED_PIN, 1);
  delay(systemRebootDelay);
  ESP.restart();
}

void RequestTriggerBLEScan(void) {
  hrSettings::bleTriggerPending = true;
}

void CheckTriggerBLEScan(void) {
  // Handled by BLE task monitoring bleTriggerPending; no-op here
}

void LedControl(bool greenLed, bool yellowLed, bool redLed, bool extOut) {
  //Controls the EXT PIN if extOut is true and the pin has been assigned
  if (extOut && hrSettings::EXT_PIN < 255) {
    digitalWrite(hrSettings::EXT_PIN, 1);
  }

  //Controls the Green, Yellow, and Red LED's on the P50/P200 reader
  //Using 2N3904 NPN transistors with BASE connected to GPIO via 1K resistor
  //P50/P200 LED wire connected to COLLECTOR
  //0v/GND connected to EMITTER
  for (int i = 0; i < 2; i++) {
    digitalWrite(hrSettings::GREEN_LED_PIN, 0);
    digitalWrite(hrSettings::YELLOW_LED_PIN, 0);
    digitalWrite(hrSettings::RED_LED_PIN, 0);

    delay(100);

    if (greenLed) {
      digitalWrite(hrSettings::GREEN_LED_PIN, 1);
    }

    if (yellowLed) {
      digitalWrite(hrSettings::YELLOW_LED_PIN, 1);
    }

    if (redLed) {
      digitalWrite(hrSettings::RED_LED_PIN, 1);
    }

    delay(100);
  }

  digitalWrite(hrSettings::GREEN_LED_PIN, 1);
  digitalWrite(hrSettings::YELLOW_LED_PIN, 1);
  digitalWrite(hrSettings::RED_LED_PIN, 1);

  if (extOut && hrSettings::EXT_PIN < 255) {
    digitalWrite(hrSettings::EXT_PIN, 0);
  }
}

void SendWiegand(String binaryData) {
  pinMode(hrSettings::D0_PIN, OUTPUT);
  pinMode(hrSettings::D1_PIN, OUTPUT);
  delay(500);

  for (int i = 0; i < binaryData.length(); i++) {
    if (binaryData[i] == '0') {
      digitalWrite(hrSettings::D0_PIN, 0);
      delayMicroseconds(hrSettings::wiegandPulseWidth);
      digitalWrite(hrSettings::D0_PIN, 1);
    } else {
      digitalWrite(hrSettings::D1_PIN, 0);
      delayMicroseconds(hrSettings::wiegandPulseWidth);
      digitalWrite(hrSettings::D1_PIN, 1);
    }

    delayMicroseconds(hrSettings::wiegandInterleave);
  }

  delay(50);
  pinMode(hrSettings::D0_PIN, INPUT);
  pinMode(hrSettings::D1_PIN, INPUT);
}

String CalculateBinary(String cardNo, int bits) {
  String binaryOut = "";
  int num = cardNo.toInt();
  int ep = 0;
  int op = 0;
  uint64_t mask = 8388608;
  int bitIterator = 24;
  int evenParityStart = 1;
  int evenParityEnd = 12;
  int oddParityStart = 13;
  int oddParityEnd = 24;
  bool addParityBits = true;

  if (bits == 32 || bits == 34) {
    mask = 2147483648;
    bitIterator = 32;
    evenParityStart = 1;
    evenParityEnd = 16;
    oddParityStart = 17;
    oddParityEnd = 32;
    if (bits == 32) {
      addParityBits = false;
    }
  }

  for (int i = 1; i <= bitIterator; i++) {
    binaryOut += (mask & num) == 0 ? "0" : "1";
    if (i >= evenParityStart && i <= evenParityEnd && (mask & num) != 0) { ep++; }
    if (i >= oddParityStart && i <= oddParityEnd && (mask & num) != 0) { op++; }
    mask >>= 1;
  }

  if (addParityBits) {
    binaryOut = (ep % 2 == 0 ? "0" : "1") + binaryOut;
    binaryOut += op % 2 == 0 ? "1" : "0";
  }

  return binaryOut;
}

String ConvertBaseToString(uint64_t n, bool base16) {
  uint8_t base = base16 == true ? 16 : 10;
  String ret = "";
  char buf[8 * sizeof(uint64_t) + 1];
  char *str = &buf[sizeof(buf) - 1];
  *str = '\0';
  do {
    char c = n % base;
    n /= base;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);
  ret = String(str);
  if (base16 && ret.length() % 2 == 1) {
    ret = "0" + ret;
  }
  return ret;
}

String SplitString(String s, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = s.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (s.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? s.substring(strIndex[0], strIndex[1]) : "";
}

bool IsNumeric(String s) {
  if (s.length() == 0) { return false; }
  for (int i = 0; i < s.length(); i++) {
    if (!isDigit(s.charAt(i))) {
      return false;
    }
  }
  return true;
}

bool IsIpAddress(String s) {
  if (s.length() == 0) { return false; }
  if (s.charAt(s.length() - 1) == '.') { return false; }
  String octet1 = SplitString(s, '.', 0);
  if (!IsNumeric(octet1)) { return false; }
  int o1 = octet1.toInt();
  if (o1 < 1 || o1 > 254) { return false; }
  String octet2 = SplitString(s, '.', 1);
  if (!IsNumeric(octet2)) { return false; }
  int o2 = octet2.toInt();
  if (o2 > 255) { return false; }
  String octet3 = SplitString(s, '.', 2);
  if (!IsNumeric(octet3)) { return false; }
  int o3 = octet3.toInt();
  if (o3 > 255) { return false; }
  String octet4 = SplitString(s, '.', 3);
  if (!IsNumeric(octet4)) { return false; }
  int o4 = octet4.toInt();
  if (o4 < 1 || o4 > 254) { return false; }
  String trailing = SplitString(s, '.', 4);
  if (trailing != "") { return false; }
  return true;
}

}  //namespace hrUtil
