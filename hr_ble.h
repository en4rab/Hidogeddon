#ifndef HR_BLE_H
#define HR_BLE_H

#include <Arduino.h>
#include <NimBLEDevice.h>

#include "hr_settings.h"

namespace hrBLE {

void FitProM4Init(void);
void MaintainConnection(void);
void ScanDevices(void);
void TriggerVibrate(void);

}  //namespace hrBLE

#endif
