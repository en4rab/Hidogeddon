#include "hr_util.h"

namespace hrUtil {

uint32_t systemRebootDelay = 5000;
bool systemReboot = false;
Preferences preferences;

void SettingsInit(void) {
  preferences.begin("hrSettings", false);
  if (preferences.getBool("hrs_ok", false)) {
    hrSettings::bleMAC = preferences.getString("BLE_MAC", "");
  }
  preferences.end();

  if (hrSettings::bleMAC == "") {
    hrSettings::bleScanMode = true;
  }
}

void GPIOInit(void) {
  pinMode(hrSettings::TRIGGER_PIN, INPUT_PULLUP);
  pinMode(hrSettings::STATUS_LED_PIN, OUTPUT);
  digitalWrite(hrSettings::STATUS_LED_PIN, hrSettings::STATUS_LED_PIN_INVERTED ? 1 : 0);
  while (digitalRead(hrSettings::TRIGGER_PIN) == 0) {
    StatusLED("flash", 50);
  }
}

void SaveSettings() {
  preferences.begin("hrSettings", false);
  preferences.putString("BLE_MAC", hrSettings::bleMAC);
  preferences.putBool("hrs_ok", true);
  preferences.end();
}

void ClearSettings(void) {
  preferences.begin("hrSettings", false);
  preferences.clear();
  preferences.end();
}

void StatusLED(String state, uint32_t flashRate) {
  if (state == "flash" && flashRate < 50) {
    flashRate = 50;
  }

  if (state == "on" || state == "flash") {
    digitalWrite(hrSettings::STATUS_LED_PIN, hrSettings::STATUS_LED_PIN_INVERTED ? 0 : 1);
    if (flashRate > 0) { delay(flashRate); }
  }

  if (state == "off" || state == "flash") {
    digitalWrite(hrSettings::STATUS_LED_PIN, hrSettings::STATUS_LED_PIN_INVERTED ? 1 : 0);
    if (flashRate > 0) { delay(flashRate); }
  }
}

}  //namespace hrUtil
