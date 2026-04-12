#include "hr_BLE.h"

namespace hrBLE {

NimBLEClient *pClient = nullptr;
NimBLEScan *pScan = nullptr;
NimBLERemoteCharacteristic *pCharacteristic = nullptr;
bool bleScanComplete = true;
bool bleBusy = false;
bool bleInitComplete = false;
uint8_t bleAction = 0;

void BLEInit(void) {
  if (!hrSettings::bleEnabled) { return; }
  if (hrSettings::SERIAL_CONSOLE_ONLY_DEBUG_MODE) { return; }
  //Method called, Name, Stack size (bytes), param passed, priority, handle, core
  xTaskCreatePinnedToCore(BLETasks, "BLETasks", 8192, NULL, 1, NULL, 0);
}

bool BLEAction(String action) {
  if (!hrSettings::bleEnabled) { return false; }
  if (pClient == nullptr) { return false; }
  if (!bleInitComplete) { return false; }
  if (bleBusy) { return false; }
  if (action == "Scan") {
    bleScanComplete = false;
    hrSettings::bleMAC = "";
    hrSettings::bleAddrType = 0;
    bleAction = 1;
  }
  if (action == "Connect") { bleAction = 2; }
  if (action == "Vibrate") { bleAction = 3; }
  if (bleAction == 0) { return false; }
  return true;
}

bool BLEisConnected(void) {
  if (pClient == nullptr) { return false; }
  if (!pClient->isConnected()) { return false; }
  return true;
}

bool BLEisScanComplete(void) {
  return bleScanComplete;
}

class ScanCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice *bleDevice) override {
    if (bleDevice->getName() == hrSettings::bleManufacturerTarget && hrSettings::bleMAC == "") {
      NimBLEDevice::getScan()->stop();
      hrSettings::bleAddrType = bleDevice->getAddress().getType();
      hrSettings::bleMAC = bleDevice->getAddress().toString().c_str();
      pScan->clearResults();
      bleBusy = false;
      bleAction = 2;
    }
  }

  void onScanEnd(const NimBLEScanResults &results, int reason) override {
    pScan->clearResults();
    bleBusy = false;
    bleAction = 4;
  }
} scanCallbacks;

static void BLETasks(void *p) {
  NimBLEDevice::init("");
  NimBLEDevice::setPower(9);
  pClient = NimBLEDevice::createClient();
  pClient->setConnectionParams(12, 12, 0, 150);
  pClient->setConnectTimeout(5000);
  pScan = NimBLEDevice::getScan();
  pScan->setScanCallbacks(&scanCallbacks, false);
  pScan->setActiveScan(true);
  pScan->setInterval(100);
  pScan->setWindow(50);
  BLEConnect();
  bleInitComplete = true;

  while (true) {
    if (bleAction == 1) {
      vTaskDelay(pdMS_TO_TICKS(2000));
      bleAction = 0;
      BLESearch();
    }

    if (bleAction == 2) {
      bleAction = 0;
      BLEConnect();
      vTaskDelay(pdMS_TO_TICKS(3000));
      bleScanComplete = true;
    }

    if (bleAction == 3) {
      bleAction = 0;
      bleBusy = true;
      BLESendCommand(1);
      vTaskDelay(pdMS_TO_TICKS(1000));
      BLESendCommand(0);
      bleBusy = false;
    }

    if (bleAction == 4) {
      bleAction = 0;
      vTaskDelay(pdMS_TO_TICKS(3000));
      bleScanComplete = true;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void BLESearch(void) {
  if (pClient == nullptr) { return; }
  if (bleBusy) { return; }
  bleBusy = true;
  pClient->disconnect();
  pCharacteristic = nullptr;
  pScan->start(15000, false, false);
}

static void BLEConnect(void) {
  if (pClient == nullptr) { return; }
  if (bleBusy) { return; }
  if (hrSettings::bleMAC == "") { return; }
  if (pClient->isConnected()) { return; }
  bleBusy = true;
  pCharacteristic = nullptr;
  NimBLEAddress bleAddress(hrSettings::bleMAC.c_str(), hrSettings::bleAddrType);
  pClient->connect(bleAddress);
  if (pClient->isConnected()) {
    NimBLERemoteService *pService = pClient->getService("6e400001-b5a3-f393-e0a9-e50e24dcca9d");
    if (pService) {
      pCharacteristic = pService->getCharacteristic("6e400002-b5a3-f393-e0a9-e50e24dcca9d");
      if (pCharacteristic != nullptr) {
        bleBusy = false;
        bleAction = 3;
        return;
      }
    }
    pClient->disconnect();
  }
  bleBusy = false;
}

static void BLESendCommand(int cmd) {
  if (pClient == nullptr) { return; }
  if (!pClient->isConnected()) { return; }
  if (cmd == 0) {
    uint8_t stopCmd[8] = { 220, 0, 5, 21, 1, 0, 20, 1 };
    pCharacteristic->writeValue(stopCmd, 8);
    return;
  }
  uint8_t vibrateCmd[9] = { 205, 0, 6, 18, 1, 11, 0, 1, 1 };
  pCharacteristic->writeValue(vibrateCmd, 9);
}


}  //namespace hrBLE
