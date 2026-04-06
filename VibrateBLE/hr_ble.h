#ifndef HR_BLE_H
#define HR_BLE_H

#include <Arduino.h>
#include "BLEDevice.h"
#include "BLEClient.h"
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "hr_settings.h"
#include "hr_util.h"

namespace hrBLE {

void FitProM4Init(void);
void MaintainConnection(void);
void InputTriggerHandler(void);
void ScanDevices(void);
static void Vibrate(void);

}  //namespace hrBLE

#endif
