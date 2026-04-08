#include "hr_ble.h"
#include "hr_util.h"

namespace hrBLE {

NimBLEClient *pClient = nullptr;
NimBLERemoteCharacteristic *pCharacteristic = nullptr;

void FitProM4Init(void) {
  NimBLEDevice::init("");
  NimBLEDevice::setPower(9);
  pClient = NimBLEDevice::createClient();
  pClient->setConnectTimeout(5);  // seconds
}

void MaintainConnection(void) {
  if (pClient == nullptr) { return; }
  if (pClient->isConnected()) { return; }
  if (hrSettings::bleMAC == "") { return; }

  pClient->disconnect();
  pCharacteristic = nullptr;

  NimBLEAddress bleAddress(hrSettings::bleMAC.c_str(), BLE_ADDR_PUBLIC);
  pClient->connect(bleAddress);
  if (pClient->isConnected()) {
    NimBLERemoteService *pService = pClient->getService("6e400001-b5a3-f393-e0a9-e50e24dcca9d");
    if (pService) {
      pCharacteristic = pService->getCharacteristic("6e400002-b5a3-f393-e0a9-e50e24dcca9d");
    }
  } else {
    // Connection failed - back off before retrying to avoid starving WiFi
    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}

class BLECallBack : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* bleDevice) {
    if (bleDevice->getName() == hrSettings::bleManufacturerTarget.c_str()
        && hrSettings::bleMAC == "") {
      hrSettings::bleMAC = bleDevice->getAddress().toString().c_str();
    }
  }
};

void ScanDevices(void) {
  if (pClient == nullptr) { return; }
  pClient->disconnect();
  pCharacteristic = nullptr;
  NimBLEScan *pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setScanCallbacks(new BLECallBack());
  pBLEScan->setActiveScan(true);
  // interval 100ms / window 50ms = 50% BLE duty cycle, leaves WiFi adequate airtime
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(50);
  pBLEScan->start(5, false);
  pBLEScan->clearResults();
  if (hrSettings::bleMAC != "") {
    hrUtil::SaveSettings();
    hrSettings::bleScanMode = false;
  } else {
    // Device not found - back off before next scan to avoid starving WiFi
    vTaskDelay(pdMS_TO_TICKS(30000));
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
