#ifndef HR_BLE_H
#define HR_BLE_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include "hr_settings.h"

namespace hrBLE {

void BLEInit(void);
bool BLEAction(String action);
bool BLEisConnected(void);
bool BLEisScanComplete(void);
static void BLETasks(void *p);
static void BLESearch(void);
static void BLEConnect(void);
static void BLESendCommand(int cmd);

}  //namespace hrBLE

#endif
