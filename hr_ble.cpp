#include "hr_ble.h"
#include "hr_util.h"

namespace hrBLE {

BLEClient *pClient = nullptr;
BLERemoteCharacteristic *pCharacteristic = nullptr;

void FitProM4Init(void) {
  BLEDevice::init("");
  BLEDevice::setPower(ESP_PWR_LVL_P9);
  pClient = BLEDevice::createClient();
}

void MaintainConnection(void) {
  if (pClient == nullptr) { return; }
  if (pClient->isConnected()) { return; }
  if (hrSettings::bleMAC == "") { return; }

  pClient->disconnect();
  pCharacteristic = nullptr;

  BLEAddress bleAddress(hrSettings::bleMAC.c_str());
  pClient->connect(bleAddress, BLE_ADDR_TYPE_PUBLIC, 10000);
  if (pClient->isConnected()) {
    BLERemoteService *pService = pClient->getService("6e400001-b5a3-f393-e0a9-e50e24dcca9d");
    if (pService) {
      pCharacteristic = pService->getCharacteristic("6e400002-b5a3-f393-e0a9-e50e24dcca9d");
    }
  }
}

class BLECallBack : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice bleDevice) {
    if (bleDevice.getName() == hrSettings::bleManufacturerTarget
        && hrSettings::bleMAC == "") {
      hrSettings::bleMAC = bleDevice.getAddress().toString();
    }
  }
};

void ScanDevices(void) {
  if (pClient == nullptr) { return; }
  pClient->disconnect();
  pCharacteristic = nullptr;
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new BLECallBack());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  pBLEScan->start(5, false);
  pBLEScan->clearResults();
  if (hrSettings::bleMAC != "") {
    hrUtil::SaveSettings();
    hrSettings::bleScanMode = false;
  }
}

void TriggerVibrate(void) {
  if (pClient == nullptr) { return; }
  if (!pClient->isConnected()) { return; }
  if (pCharacteristic == nullptr) { return; }
  uint8_t vibrateCmd[9] = { 205, 0, 6, 18, 1, 11, 0, 1, 1 };
  uint8_t stopCmd[8] = { 220, 0, 5, 21, 1, 0, 20, 1 };
  pCharacteristic->writeValue(vibrateCmd, 9);
  delay(500);
  pCharacteristic->writeValue(stopCmd, 8);
}

}  //namespace hrBLE
