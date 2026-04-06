#ifndef HR_UTIL_H
#define HR_UTIL_H

#include <Arduino.h>
#include <Preferences.h>
#include "hr_settings.h"

namespace hrUtil {

void SettingsInit(void);
void GPIOInit(void);
void SaveSettings(void);
void ClearSettings(void);
void StatusLED(String state, uint32_t flashRate);

}  //namespace hrUtil

#endif
