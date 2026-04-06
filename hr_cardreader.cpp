#include "hr_cardreader.h"

namespace hrCardReader {

unsigned int lastBitCount = 0;
volatile unsigned long lastMicrosPulseWidth = 0;
volatile unsigned long lastMicrosInterleave = 0;
volatile unsigned long wiegandLearnPulseWidth = 0;
volatile unsigned long wiegandLearnInterleave = 0;
volatile unsigned int bitCount = 0;
volatile bool processingData = false;
volatile unsigned int cardData[256];
const String LINE = "|";
const String END = ";";
const String NONE = "";

//Auth Data
String bin, auth, parity;
uint16_t ep1, ep2, ep3, op1, op2, op3;
uint64_t auth0, auth1, auth2, auth3, auth4, auth5;
uint64_t auth6, auth7, auth8, auth9, auth10, auth11;
uint64_t auth12, auth13, auth14, auth15, auth16, auth17;

void CardReaderInit(void) {
  ClearAuthData();
  for (int i = 0; i < 256; i++) { cardData[i] = 0; }

  if (hrSettings::wiegandLearnMode) {
    attachInterrupt(hrSettings::D0_PIN, OnCardD0Learn, CHANGE);
    attachInterrupt(hrSettings::D1_PIN, OnCardD1Learn, CHANGE);
    return;
  }

  attachInterrupt(hrSettings::D0_PIN, OnCardD0, FALLING);
  attachInterrupt(hrSettings::D1_PIN, OnCardD1, FALLING);
}

void ParseCard(void) {
  if (hrSettings::replayBin != "") {
    processingData = true;
    hrUtil::SendWiegand(hrSettings::replayBin);
    hrSettings::replayBin = "";
    bitCount = 0;
    lastBitCount = 0;
    processingData = false;
  }

  if (bitCount == 0) { return; }
  if (bitCount > lastBitCount) {
    lastBitCount = bitCount;
  } else {
    delay(50);
    if (bitCount > lastBitCount) { return; }
    processingData = true;
    ClearAuthData();
    if (bitCount == 26) { Parse26bit(); }
    if (bitCount == 27) { Parse27bit(); }
    if (bitCount == 28) { Parse28bit(); }
    if (bitCount == 29) { Parse29bit(); }
    if (bitCount == 30) { Parse30bit(); }
    if (bitCount == 31) { Parse31bit(); }
    if (bitCount == 32) { Parse32bit(); }
    if (bitCount == 33) { Parse33bit(); }
    if (bitCount == 34) { Parse34bit(); }
    if (bitCount == 35) { Parse35bit(); }
    if (bitCount == 36) { Parse36bit(); }
    if (bitCount == 37) { Parse37bit(); }
    if (bitCount == 38) { Parse38bit(); }
    if (bitCount == 39) { Parse39bit(); }
    if (bitCount == 40) { Parse40bit(); }
    if (bitCount == 42) { Parse42bit(); }
    if (bitCount == 46) { Parse46bit(); }
    if (bitCount == 48) { Parse48bit(); }
    if (bitCount == 56) { Parse56bit(); }
    if (bitCount == 58) { Parse58bit(); }
    if (bitCount == 64) { Parse64bit(); }
    if (bitCount == 75) { Parse75bit(); }
    if (bitCount == 80) { Parse80bit(); }
    if (bitCount == 83) { Parse83bit(); }
    if (bitCount == 91) { Parse91bit(); }
    if (bitCount == 107) { Parse107bit(); }
    if (bitCount == 128) { Parse128bit(); }
    if (bitCount == 200) { Parse200bit(); }
    if (bitCount == 245) { Parse245bit(); }
    if (hrSettings::captureUnknownBitLengths) {
      if (bin == "" && bitCount >= 12) {
        ParseXbits();
      }
    }
    if (hrSettings::wiegandLearnMode) {
      WiegandLearn();
    }
    bitCount = 0;
    lastBitCount = 0;
    processingData = false;
  }
}

static void IRAM_ATTR OnCardD0Learn(void) {
  if (processingData) { return; }
  unsigned long n = micros();

  if (digitalRead(hrSettings::D0_PIN) == 0) {
    if (lastMicrosPulseWidth == 0) {
      lastMicrosPulseWidth = n;
    }

    if (lastMicrosInterleave == 0) {
      lastMicrosInterleave = n;
    } else {
      if (wiegandLearnInterleave == 0) {
        wiegandLearnInterleave = n - lastMicrosInterleave;
      }
    }

    cardData[bitCount] = 0;
    if (bitCount < 256) {
      bitCount = bitCount + 1;
    }

    return;
  }

  if (wiegandLearnPulseWidth == 0) {
    wiegandLearnPulseWidth = n - lastMicrosPulseWidth;
  }
}

static void IRAM_ATTR OnCardD1Learn(void) {
  if (processingData) { return; }
  unsigned long n = micros();

  if (digitalRead(hrSettings::D1_PIN) == 0) {
    if (lastMicrosInterleave == 0) {
      lastMicrosInterleave = n;
    } else {
      if (wiegandLearnInterleave == 0) {
        wiegandLearnInterleave = n - lastMicrosInterleave;
      }
    }

    cardData[bitCount] = 1;
    if (bitCount < 256) {
      bitCount = bitCount + 1;
    }
  }
}

static void IRAM_ATTR OnCardD0(void) {
  if (!processingData) {
    cardData[bitCount] = 0;
    if (bitCount < 256) {
      bitCount = bitCount + 1;
    }
  }
}

static void IRAM_ATTR OnCardD1(void) {
  if (!processingData) {
    cardData[bitCount] = 1;
    if (bitCount < 256) {
      bitCount = bitCount + 1;
    }
  }
}

static void ClearAuthData(void) {
  bin = "";
  auth = "";
  parity = "Pass";
  ep1 = 0;
  ep2 = 0;
  ep3 = 0;
  op1 = 0;
  op2 = 0;
  op3 = 0;
  auth0 = 0;
  auth1 = 0;
  auth2 = 0;
  auth3 = 0;
  auth4 = 0;
  auth5 = 0;
  auth6 = 0;
  auth7 = 0;
  auth8 = 0;
  auth9 = 0;
  auth10 = 0;
  auth11 = 0;
  auth12 = 0;
  auth13 = 0;
  auth14 = 0;
  auth15 = 0;
  auth16 = 0;
  auth17 = 0;
}

static void ClearParity(void) {
  parity = "Pass";
}

static void WiegandLearn(void) {
  if (hrSettings::SERIAL_CONSOLE_ONLY_DEBUG_MODE) {
    Serial.print("Pulse Width: ");
    Serial.println(wiegandLearnPulseWidth);
    Serial.print("Interleave: ");
    Serial.println(wiegandLearnInterleave - wiegandLearnPulseWidth);
    Serial.println("");
  }
  hrSettings::wiegandPulseWidth = wiegandLearnPulseWidth;
  hrSettings::wiegandInterleave = wiegandLearnInterleave - wiegandLearnPulseWidth;
  lastMicrosPulseWidth = 0;
  lastMicrosInterleave = 0;
  wiegandLearnPulseWidth = 0;
  wiegandLearnInterleave = 0;
}

static bool IsFunctionCard(void) {
  bool fc = false;
  if (bin == hrSettings::FC_DEFAULT) { fc = true; }
  if (bin == hrSettings::FC_CONNECT_TO_AP) { fc = true; }
  if (bin == hrSettings::FC_SHOW_AP_INFO) { fc = true; }
  if (fc == false) { return false; }

  if (bin == hrSettings::FC_DEFAULT) {
    hrSettings::lastCardData = "Function card: Default settings";
    hrUtil::OLEDShowMessage(":Function Card:", "Default settings", "", "", "", "");
    hrUtil::LedControl(true, false, false, false);
    hrUtil::ClearSettings();
    hrUtil::ClearSpecialCards();
    hrUtil::RequestReboot(1000);
  }

  if (bin == hrSettings::FC_CONNECT_TO_AP) {
    if (hrSettings::oneShotConnectToAP) {
      hrSettings::lastCardData = "Function card: Create AP";
      hrUtil::OLEDShowMessage(":Function Card:", "Create AP", "", "", "", "");
      hrUtil::LedControl(true, false, false, false);
      hrUtil::RequestReboot(1000);
    } else {
      hrSettings::lastCardData = "Function card: Connect to AP";
      hrUtil::OLEDShowMessage(":Function Card:", "Connect to AP", "", "", "", "");
      hrUtil::LedControl(true, false, false, false);
      hrUtil::RequestRebootAndConnectToAP(1000);
    }
  }

  if (bin == hrSettings::FC_SHOW_AP_INFO) {
    hrSettings::lastCardData = "<a href=\"/selectSpecialCard?page=1&b="
                               + bin + "\"><u>Function card: Show AP Info on OLED</u></a>";
    hrUtil::OLEDShowAPInfo();
    hrUtil::LedControl(true, false, false, false);
  }

  return true;
}

static void SaveCardData(void) {
  if (hrSettings::SERIAL_CONSOLE_ONLY_DEBUG_MODE) {
    Serial.print("Bits: ");
    Serial.println(bitCount);
    Serial.print("Bin: ");
    Serial.println(bin);
    Serial.print("Auth Data: ");
    Serial.println(auth);
    Serial.println("");
    return;
  }

  File logFile = SPIFFS.open("/card_data.txt", "a");
  if (!logFile) {
    hrSettings::lastCardData = "Card save error";
    hrUtil::OLEDShowMessage("Card save error", "", "", "", "", "");
    hrUtil::LedControl(false, true, false, false);
    return;
  }
  logFile.print(String(bitCount));
  logFile.print(",");
  logFile.print(bin);
  logFile.print(",");
  logFile.println(auth);
  logFile.close();
  hrSettings::lastCardData = "Bits: " + String(bitCount) + "<br>";
  hrSettings::lastCardData += "Bin: " + bin + "<br>";
  hrUtil::OLEDShowMessage("Card Saved", "", "Bits: " + String(bitCount), "", "Bin: " + bin, "");
  hrUtil::LedControl(true, false, false, true);
}

static void AddAuthHeader(String header) {
  auth += header + "|";
}

static void AddAuthItem(String key, String value, String separator) {
  auth += key + ": " + value + separator;
}

static uint64_t BitOp(int i, uint64_t n) {
  n <<= 1;
  n |= (cardData[i] & 1);
  return n;
}

static String GetAuth(uint64_t n) {
  char buf[8 * sizeof(uint64_t) + 1];
  char *str = &buf[sizeof(buf) - 1];
  *str = '\0';
  do {
    char c = n % 10;
    n /= 10;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);
  return String(str);
}

static void ParseXbits(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Unknown");
  AddAuthItem("Note", "Logging raw bits only", NONE);
  SaveCardData();
}

static void Parse26bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 24) { auth0 = BitOp(i, auth0); }     //Full Card Number
    if (i >= 1 && i <= 8) { auth1 = BitOp(i, auth1); }      //H10301 Facility Code
    if (i >= 9 && i <= 24) { auth2 = BitOp(i, auth2); }     //H10301 Card Number
    if (i >= 1 && i <= 12) { auth3 = BitOp(i, auth3); }     //Indala Facility Code
    if (i >= 13 && i <= 24) { auth4 = BitOp(i, auth4); }    //Indala Card Number
    if (i >= 1 && i <= 12 && cardData[i] == 1) { ep1++; }   //Even Parity (All)
    if (i >= 13 && i <= 24 && cardData[i] == 1) { op1++; }  //Odd Parity (All)
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[25] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("H10301");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("Indala");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse27bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 0 && i <= 12) { auth0 = BitOp(i, auth0); }   //Indala Facility Code
    if (i >= 13 && i <= 26) { auth1 = BitOp(i, auth1); }  //Indala Card Number
  }
  int fcPatternIndalaASC[13] = { 9, 4, 6, 5, 0, 7, 19, 8, 10, 16, 24, 12, 22 };
  for (int i = 0; i < 13; i++) {
    auth2 = BitOp(fcPatternIndalaASC[i], auth2);  //Indala ASC Facility Code
  }
  int cnPatternIndalaASC[14] = { 26, 1, 3, 15, 14, 17, 20, 13, 25, 2, 18, 21, 11, 23 };
  for (int i = 0; i < 14; i++) {
    auth3 = BitOp(cnPatternIndalaASC[i], auth3);  //Indala ASC Card Number
  }
  int fcPatternTecom[11] = { 15, 19, 24, 23, 22, 18, 6, 10, 14, 3, 2 };
  for (int i = 0; i < 11; i++) {
    auth4 = BitOp(fcPatternTecom[i], auth4);  //Tecom Facility Code
  }
  int cnPatternTecom[16] = { 0, 1, 13, 12, 9, 26, 20, 16, 17, 21, 25, 7, 8, 11, 4, 5 };
  for (int i = 0; i < 16; i++) {
    auth5 = BitOp(cnPatternTecom[i], auth5);  //Tecom Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Indala");
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), END);
  AddAuthHeader("Indala ASC");
  AddAuthItem("Facility Code", GetAuth(auth2), LINE);
  AddAuthItem("Card Number", GetAuth(auth3), END);
  AddAuthHeader("Tecom");
  AddAuthItem("Facility Code", GetAuth(auth4), LINE);
  AddAuthItem("Card Number", GetAuth(auth5), NONE);
  SaveCardData();
}

static void Parse28bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 4 && i <= 11) { auth0 = BitOp(i, auth0); }    //2804 Facility Code
    if (i >= 12 && i <= 26) { auth1 = BitOp(i, auth1); }   //2804 Issue Number
    if (i >= 1 && i <= 13 && cardData[i] == 1) { ep1++; }  //Even Parity
    if (i >= 0 && i <= 26 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[27] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("2804");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Issue Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse29bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 0 && i <= 12) { auth0 = BitOp(i, auth0); }   //Indala Facility Code
    if (i >= 13 && i <= 28) { auth1 = BitOp(i, auth1); }  //Indala Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Indala");
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse30bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 12) { auth0 = BitOp(i, auth0); }     //ATS Facility Code
    if (i >= 13 && i <= 28) { auth1 = BitOp(i, auth1); }    //ATS Card Number
    if (i >= 1 && i <= 12 && cardData[i] == 1) { ep1++; }   //Even Parity
    if (i >= 13 && i <= 28 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[29] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("ATS");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse31bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 4) { auth0 = BitOp(i, auth0); }   //HID ADT Facility Code
    if (i >= 5 && i <= 27) { auth1 = BitOp(i, auth1); }  //HID ADT Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID ADT");
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse32bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 0 && i <= 32) { auth0 = BitOp(i, auth0); }     //Full Card Number
    if (i >= 1 && i <= 13) { auth1 = BitOp(i, auth1); }     //ATS Facility Code
    if (i >= 14 && i <= 30) { auth2 = BitOp(i, auth2); }    //ATS Card Number
    if (i >= 7 && i <= 14) { auth3 = BitOp(i, auth3); }     //Indala/Kantech KSF Facility Code
    if (i >= 15 && i <= 30) { auth4 = BitOp(i, auth4); }    //Indala/Kantech KSF Card Number
    if (i >= 7 && i <= 14) { auth5 = BitOp(i, auth5); }     //K32 Facility Code
    if (i >= 15 && i <= 30) { auth6 = BitOp(i, auth6); }    //K32 Card Number
    if (i >= 1 && i <= 6) { auth7 = BitOp(i, auth7); }      //K32 Issue Level
    if (i >= 4 && i <= 15) { auth8 = BitOp(i, auth8); }     //W32 Facility Code
    if (i >= 16 && i <= 31) { auth9 = BitOp(i, auth9); }    //W32 Card Number
    if (i >= 1 && i <= 12) { auth10 = BitOp(i, auth10); }   //HID Facility Code
    if (i >= 13 && i <= 30) { auth11 = BitOp(i, auth11); }  //HID Card Number
    if (i >= 1 && i <= 24) { auth12 = BitOp(i, auth12); }   //HID Check Point Card Number
    if (i >= 1 && i <= 14) { auth13 = BitOp(i, auth13); }   //32-B Facility Code
    if (i >= 15 && i <= 30) { auth14 = BitOp(i, auth14); }  //32-B Card Number
    if (i >= 7 && i <= 14) { auth15 = BitOp(i, auth15); }   //Kastle Facility Code
    if (i >= 15 && i <= 30) { auth16 = BitOp(i, auth16); }  //Kastle Card Number
    if (i >= 2 && i <= 6) { auth17 = BitOp(i, auth17); }    //Kastle Issue Level
    if (i >= 1 && i <= 13 && cardData[i] == 1) { ep1++; }   //ATS/K32 Even Parity
    if (i >= 14 && i <= 30 && cardData[i] == 1) { op1++; }  //ATS/K32 Odd Parity
    if (i >= 1 && i <= 16 && cardData[i] == 1) { ep2++; }   //32-B Even Parity
    if (i >= 15 && i <= 30 && cardData[i] == 1) { op2++; }  //32-B Odd Parity
    if (i >= 1 && i <= 16 && cardData[i] == 1) { ep3++; }   //Kastle Even Parity
    if (i >= 14 && i <= 30 && cardData[i] == 1) { op3++; }  //Kastle Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[31] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("ATS");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("Indala/Kantech KSF");
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("K32");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth5), LINE);
  AddAuthItem("Card Number", GetAuth(auth6), LINE);
  AddAuthItem("Issue Level", GetAuth(auth7), END);
  AddAuthHeader("W32");
  AddAuthItem("Facility Code", GetAuth(auth8), LINE);
  AddAuthItem("Card Number", GetAuth(auth9), END);
  AddAuthHeader("HID");
  AddAuthItem("Facility Code", GetAuth(auth10), LINE);
  AddAuthItem("Card Number", GetAuth(auth11), END);
  AddAuthHeader("HID Check Point");
  AddAuthItem("Card Number", GetAuth(auth12), END);
  ClearParity();
  if (cardData[0] != ep2 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[31] != op2 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("32-B");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth13), LINE);
  AddAuthItem("Card Number", GetAuth(auth14), END);
  ClearParity();
  if (cardData[0] != ep3 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[31] != op3 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("Kastle");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth15), LINE);
  AddAuthItem("Card Number", GetAuth(auth16), LINE);
  AddAuthItem("Issue Level", GetAuth(auth17), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse33bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 7) { auth0 = BitOp(i, auth0); }      //Indala DSX/HID D10202 Facility Code
    if (i >= 8 && i <= 31) { auth1 = BitOp(i, auth1); }     //Indala DSX/HID D10202 Card Number
    if (i >= 1 && i <= 16 && cardData[i] == 1) { ep1++; }   //HID D10202 Even Parity
    if (i >= 16 && i <= 31 && cardData[i] == 1) { op1++; }  //HID D10202 Odd Parity
    if (i >= 2 && i <= 3) { auth2 = BitOp(i, auth2); }      //RS2-HID R901592C Facility Code
    if (i >= 4 && i <= 30) { auth3 = BitOp(i, auth3); }     //RS2-HID R901592C Card Number
    if (i >= 2 && i <= 30 && cardData[i] == 1) { ep2++; }   //RS2-HID R901592C Even Parity
    if (i >= 2 && i <= 30 && cardData[i] == 1) { op2++; }   //RS2-HID R901592C Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[32] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Indala DSX");
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), END);
  AddAuthHeader("HID D10202");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), END);
  ClearParity();
  if (cardData[1] != ep2 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[31] != op2 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("RS2-HID R901592C");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth2), LINE);
  AddAuthItem("Card Number", GetAuth(auth3), NONE);
  SaveCardData();
}

static void Parse34bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 32) { auth0 = BitOp(i, auth0); }     //Full Card Number
    if (i >= 1 && i <= 16) { auth1 = BitOp(i, auth1); }     //HID H10306 Facility Code
    if (i >= 17 && i <= 32) { auth2 = BitOp(i, auth2); }    //HID H10306 Card Number
    if (i >= 22 && i <= 32) { auth3 = BitOp(i, auth3); }    //Indala Optus Facility Code
    if (i >= 1 && i <= 16) { auth4 = BitOp(i, auth4); }     //Indala Optus Card Number
    if (i >= 1 && i <= 13) { auth5 = BitOp(i, auth5); }     //Cardkey Smartpass Facility Code
    if (i >= 17 && i <= 32) { auth6 = BitOp(i, auth6); }    //Cardkey Smartpass Card Number
    if (i >= 14 && i <= 16) { auth7 = BitOp(i, auth7); }    //Cardkey Smartpass Issue Level
    if (i >= 9 && i <= 16) { auth8 = BitOp(i, auth8); }     //HID N1002 Facility Code
    if (i >= 17 && i <= 32) { auth9 = BitOp(i, auth9); }    //HID N1002 Card Number
    if (i >= 1 && i <= 8) { auth10 = BitOp(i, auth10); }    //BQT Facility Code
    if (i >= 9 && i <= 32) { auth11 = BitOp(i, auth11); }   //BQT Card Number
    if (i >= 1 && i <= 16 && cardData[i] == 1) { ep1++; }   //Even Parity (All)
    if (i >= 17 && i <= 32 && cardData[i] == 1) { op1++; }  //Odd Parity (All)
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[33] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID H10306");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("Indala Optus");
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("Cardkey Smartpass");
  AddAuthItem("Facility Code", GetAuth(auth5), LINE);
  AddAuthItem("Card Number", GetAuth(auth6), LINE);
  AddAuthItem("Issue Level", GetAuth(auth7), END);
  AddAuthHeader("HID N1002");
  AddAuthItem("Facility Code", GetAuth(auth8), LINE);
  AddAuthItem("Card Number", GetAuth(auth9), END);
  AddAuthHeader("BQT");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth10), LINE);
  AddAuthItem("Card Number", GetAuth(auth11), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse35bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 33) { auth0 = BitOp(i, auth0); }   //Full Card Number
    if (i >= 2 && i <= 13) { auth1 = BitOp(i, auth1); }   //HID Corporate 1000 Company ID Code
    if (i >= 14 && i <= 33) { auth2 = BitOp(i, auth2); }  //HID Corporate 1000 Card Number
    if (i >= 3 && i % 3 == 0) {                           //Even Parity
      if (cardData[i] == 1) { ep1++; }
      if (cardData[i - 1] == 1) { ep1++; }
    }
    if (i >= 1 && i <= 34 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[1] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[0] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID Corporate 1000");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Company ID Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse36bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 14) { auth1 = BitOp(i, auth1); }     //Chubb Facility Code
    if (i >= 19 && i <= 34) { auth2 = BitOp(i, auth2); }    //Chubb Card Number
    if (i >= 21 && i <= 32) { auth3 = BitOp(i, auth3); }    //HID Inner Range Facility Code
    if (i >= 1 && i <= 16) { auth4 = BitOp(i, auth4); }     //HID Inner Range Card Number
    if (i >= 1 && i <= 8) { auth5 = BitOp(i, auth5); }      //HID Simplex Facility Code
    if (i >= 11 && i <= 34) { auth6 = BitOp(i, auth6); }    //HID Simplex Card Number
    if (i >= 9 && i <= 10) { auth7 = BitOp(i, auth7); }     //HID Simplex Issue Level
    if (i >= 1 && i <= 17 && cardData[i] == 1) { op1++; }   //HID Simplex Odd Parity1
    if (i >= 17 && i <= 34 && cardData[i] == 1) { op2++; }  //HID Simplex Odd Parity2
    if (i >= 1 && i <= 18) { auth8 = BitOp(i, auth8); }     //HID Siemens Facility Code
    if (i >= 19 && i <= 34) { auth9 = BitOp(i, auth9); }    //HID Siemens Card Number
    if (i >= 1 && i <= 2 && cardData[i] == 1) { ep3++; }    //HID Siemens Even Parity
    if (i == 34 && cardData[i] == 1) { ep3++; }
    if (i >= 3 && i <= 30 && i % 3 == 0) {
      if (cardData[i + 1] == 1) { ep3++; }
      if (cardData[i + 2] == 1) { ep3++; }
    }
    if (i == 1 && cardData[i] == 1) { op3++; }  //HID Siemens Odd Parity
    if (i >= 3 && i % 3 == 0) {
      if (cardData[i] == 1) { op3++; }
      if (cardData[i + 1] == 1) { op3++; }
    }
  }
  if (cardData[0] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (cardData[35] != op2 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Chubb");
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("HID Inner Range");
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("HID Simplex");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth5), LINE);
  AddAuthItem("Card Number", GetAuth(auth6), LINE);
  AddAuthItem("Issue Level", GetAuth(auth7), END);
  ClearParity();
  if (cardData[35] != ep3 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[0] != op3 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("HID Siemens");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth8), LINE);
  AddAuthItem("Card Number", GetAuth(auth9), NONE);
  SaveCardData();
}

static void Parse37bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 35) { auth0 = BitOp(i, auth0); }     //HID H10302 Card Number
    if (i >= 1 && i <= 16) { auth1 = BitOp(i, auth1); }     //HID H10304 Facility Code
    if (i >= 17 && i <= 35) { auth2 = BitOp(i, auth2); }    //HID H10304 Card Number
    if (i >= 3 && i <= 6) { auth3 = BitOp(i, auth3); }      //PointGuard MID Facility Code
    if (i >= 7 && i <= 35) { auth4 = BitOp(i, auth4); }     //PointGuard MID Card Number
    if (i >= 1 && i <= 18 && cardData[i] == 1) { ep1++; }   //HID H10302/H10304 & PointGuard MDI Even Parity
    if (i >= 18 && i <= 35 && cardData[i] == 1) { op1++; }  //HID H10302/H10302 & PointGuard MDI Odd Parity
    if (i >= 1 && i <= 8) { auth5 = BitOp(i, auth5); }      //AWID RS2 34 Facility Code
    if (i >= 9 && i <= 32) { auth6 = BitOp(i, auth6); }     //AWID RS2 34 Card Number
    if (i >= 1 && i <= 16 && cardData[i] == 1) { ep2++; }   //AWID RS2 34 Even Parity
    if (i >= 17 && i <= 32 && cardData[i] == 1) { op2++; }  //AWID RS2 34 Odd Parity
    if (i >= 4 && i <= 35) { auth7 = BitOp(i, auth7); }     //HID Generic Card Number
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[36] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID H10302 (CSN)");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Card Number", GetAuth(auth0), END);
  AddAuthHeader("HID H10304 (FARPOINTE)");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Faciltiy Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("GuardPoint MDI");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Faciltiy Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  ClearParity();
  if (cardData[0] != ep2 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[33] != op2 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("AWID RS2 34");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Faciltiy Code", GetAuth(auth5), LINE);
  AddAuthItem("Card Number", GetAuth(auth6), END);
  AddAuthHeader("HID Generic");
  AddAuthItem("Card Number", GetAuth(auth7), NONE);
  SaveCardData();
}

static void Parse38bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 24 && i <= 36) { auth0 = BitOp(i, auth0); }    //BQT Facility Code
    if (i >= 1 && i <= 19) { auth1 = BitOp(i, auth1); }     //BQT Access Code
    if (i >= 20 && i <= 23) { auth2 = BitOp(i, auth2); }    //BQT Issue Code
    if (i >= 5 && i <= 14) { auth3 = BitOp(i, auth3); }     //ISCS Facility Code
    if (i >= 15 && i <= 36) { auth4 = BitOp(i, auth4); }    //ISCS Card Number
    if (i >= 1 && i <= 18 && cardData[i] == 1) { ep1++; }   //Even Parity (All)
    if (i >= 19 && i <= 36 && cardData[i] == 1) { op1++; }  //Odd Parity (All)
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[37] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("BQT");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Access Code", GetAuth(auth1), LINE);
  AddAuthItem("Issue Code", GetAuth(auth2), END);
  AddAuthHeader("ISCS");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), NONE);
  SaveCardData();
}

static void Parse39bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 17) { auth0 = BitOp(i, auth0); }     //Pyramid Facility Code
    if (i >= 18 && i <= 37) { auth1 = BitOp(i, auth1); }    //Pyramid Card Number
    if (i >= 1 && i <= 18 && cardData[i] == 1) { ep1++; }   //Even Parity
    if (i >= 19 && i <= 37 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[38] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Pyramid");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse40bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 38) { auth0 = BitOp(i, auth0); }    //Casi-Rusco Card Number
    if (i >= 4 && i <= 15) { auth1 = BitOp(i, auth1); }    //HID Honeywell Facility Code
    if (i >= 16 && i <= 31) { auth2 = BitOp(i, auth2); }   //HID Honeywell Card Number
    if (i >= 1 && i <= 10) { auth3 = BitOp(i, auth3); }    //XceedID RS2 Facility Code
    if (i >= 11 && i <= 38) { auth4 = BitOp(i, auth4); }   //XceedID RS2 Card Number
    if (i >= 1 && i <= 19 && cardData[i] == 1) { ep1++; }  //XceedID RS2 Even Parity
    if (i >= 0 && i <= 38 && cardData[i] == 1) { op1++; }  //XceedID RS2 Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[39] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID Honeywell");
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("XceedID RS2");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("Casi-Rusco");
  AddAuthItem("Card Number", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse42bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 0 && i <= 13) { auth0 = BitOp(i, auth0); }     //Lenel Facility Code
    if (i >= 14 && i <= 25) { auth1 = BitOp(i, auth1); }    //Lenel Card Number
    if (i >= 1 && i <= 20 && cardData[i] == 1) { ep1++; }   //Even Parity
    if (i >= 21 && i <= 40 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[41] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Lenel");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse46bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 7 && i <= 20) { auth0 = BitOp(i, auth0); }   //DCAC Facility Code
    if (i >= 21 && i <= 44) { auth1 = BitOp(i, auth1); }  //DCAC Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("DCAC");
  AddAuthItem("Facility Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), NONE);
  SaveCardData();
}

static void Parse48bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 46) { auth0 = BitOp(i, auth0); }   //Full Card Number
    if (i >= 2 && i <= 23) { auth1 = BitOp(i, auth1); }   //HID H2004064 Company ID Code
    if (i >= 24 && i <= 46) { auth2 = BitOp(i, auth2); }  //HID H2004064 Card Number
    if (i >= 3 && i % 3 == 0) {                           //HID H2004064 Even Parity
      if (cardData[i] == 1) { ep1++; }
      if (cardData[i + 1] == 1) { ep1++; }
    }
    if (i >= 1 && i <= 47 && cardData[i] == 1) { op1++; }   //HID H2004064 Odd Parity
    if (i >= 12 && i <= 27) { auth3 = BitOp(i, auth3); }    //HID H10304 Facility Code
    if (i >= 28 && i <= 46) { auth4 = BitOp(i, auth4); }    //HID H10304 Card Number
    if (i >= 12 && i <= 46) { auth5 = BitOp(i, auth5); }    //HUGHES ID H10302 Card Number
    if (i >= 12 && i <= 29 && cardData[i] == 1) { ep2++; }  //HID H10304/HUGHES ID H10302 Even Parity
    if (i >= 29 && i <= 46 && cardData[i] == 1) { op2++; }  //HID H10304/HUGHES ID H10302 Odd Parity
  }
  if (cardData[1] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[0] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("HID H2004064 (Corporate 1000)");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Company ID Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  ClearParity();
  if (cardData[11] != ep2 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[47] != op2 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  AddAuthHeader("HID H10304 (FC/CN 37)");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth4), END);
  AddAuthHeader("HUGHES ID 37 H10302");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Card Number", GetAuth(auth5), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse56bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 0 && i <= 55) { auth0 = BitOp(i, auth0); }   //Full Card Number
    if (i >= 0 && i <= 23) { auth1 = BitOp(i, auth1); }   //IR SIFER Facility Code
    if (i >= 24 && i <= 55) { auth2 = BitOp(i, auth2); }  //IR SIFER Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("Inner Range SIFER");
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse58bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 56) { auth0 = BitOp(i, auth0); }     //Full Card Number
    if (i >= 1 && i <= 14) { auth1 = BitOp(i, auth1); }     //TWIC/CAC 58 Agency Code
    if (i >= 15 && i <= 28) { auth2 = BitOp(i, auth2); }    //TWIC/CAC 58 System Code
    if (i >= 29 && i <= 48) { auth3 = BitOp(i, auth3); }    //TWIC/CAC 58 Credential Code
    if (i >= 49 && i <= 52) { auth4 = BitOp(i, auth4); }    //TWIC/CAC 58 Credential Series
    if (i >= 53 && i <= 56) { auth5 = BitOp(i, auth5); }    //TWIC/CAC 58 Individual Series Issue
    if (i >= 1 && i <= 28 && cardData[i] == 1) { ep1++; }   //Even Parity
    if (i >= 29 && i <= 56 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[57] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("TWIC/CAC 58");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Agency Code", GetAuth(auth1), LINE);
  AddAuthItem("System Code", GetAuth(auth2), LINE);
  AddAuthItem("Credential Code", GetAuth(auth3), LINE);
  AddAuthItem("Credential Series", GetAuth(auth4), LINE);
  AddAuthItem("Individual Series Issue", GetAuth(auth5), END);
  AddAuthHeader("Full Card Number");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse64bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    auth0 = BitOp(i, auth0);                                //Full Card Number
    if (i >= 0 && i <= 15) { auth1 = BitOp(i, auth1); }     //TWIC/CAC 64 BCD Agency Code
    if (i >= 16 && i <= 31) { auth2 = BitOp(i, auth2); }    //TWIC/CAC 64 BCD System Code
    if (i >= 32 && i <= 55) { auth3 = BitOp(i, auth3); }    //TWIC/CAC 64 BCD Credential Code
    if (i >= 56 && i <= 59) { auth4 = BitOp(i, auth4); }    //TWIC/CAC 64 BCD Credential Series
    if (i >= 60 && i <= 63) { auth5 = BitOp(i, auth5); }    //TWIC/CAC 64 BCD Individual Series Issue
    if (i >= 0 && i <= 13) { auth6 = BitOp(i, auth6); }     //TWIC/CAC 56 TSM Agency Code
    if (i >= 14 && i <= 27) { auth7 = BitOp(i, auth7); }    //TWIC/CAC 56 TSM System Code
    if (i >= 28 && i <= 47) { auth8 = BitOp(i, auth8); }    //TWIC/CAC 56 TSM Credential Code
    if (i >= 48 && i <= 51) { auth9 = BitOp(i, auth9); }    //TWIC/CAC 56 TSM Credential Series
    if (i >= 52 && i <= 55) { auth10 = BitOp(i, auth10); }  //TWIC/CAC 56 TSM Individual Series Issue
    if (i >= 57 && i <= 63) { auth11 = BitOp(i, auth11); }  //TWIC/CAC 56 TSM Transaction Status Messages
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("TWIC/CAC 64 BCD");
  AddAuthItem("Agency Code", GetAuth(auth1), LINE);
  AddAuthItem("System Code", GetAuth(auth2), LINE);
  AddAuthItem("Credential Code", GetAuth(auth3), LINE);
  AddAuthItem("Credential Series", GetAuth(auth4), LINE);
  AddAuthItem("Individual Series Issue", GetAuth(auth5), END);
  AddAuthHeader("TWIC/CAC 56 TSM");
  AddAuthItem("Agency Code", GetAuth(auth6), LINE);
  AddAuthItem("System Code", GetAuth(auth7), LINE);
  AddAuthItem("Credential Code", GetAuth(auth8), LINE);
  AddAuthItem("Credential Series", GetAuth(auth9), LINE);
  AddAuthItem("Individual Series Issue", GetAuth(auth10), LINE);
  AddAuthItem("Transaction Status Messages", GetAuth(auth11), END);
  AddAuthHeader("8byte iCLASS");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse75bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 15 && i <= 28) { auth0 = BitOp(i, auth0); }    //PIV Site Code
    if (i >= 29 && i <= 48) { auth1 = BitOp(i, auth1); }    //PIV Card Number
    if (i >= 1 && i <= 14) { auth2 = BitOp(i, auth2); }     //PIV Agency Code
    if (i >= 49 && i <= 73) { auth3 = BitOp(i, auth3); }    //PIV Expiration Date
    if (i >= 1 && i <= 37 && cardData[i] == 1) { ep1++; }   //PIV Even Parity
    if (i >= 38 && i <= 73 && cardData[i] == 1) { op1++; }  //PIV Odd Parity
    if (i >= 1 && i <= 48) { auth4 = BitOp(i, auth4); }     //PIV Class Card Number
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[74] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("PIV");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Site Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), LINE);
  AddAuthItem("Agency Code", GetAuth(auth2), LINE);
  AddAuthItem("Expiration Date", GetAuth(auth3), END);
  AddAuthHeader("PIV Class");
  AddAuthItem("Card Number", GetAuth(auth4), NONE);
  SaveCardData();
}

static void Parse80bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    auth0 = BitOp(i, auth0);  //Full Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("10byte CSN");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse83bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 14) { auth0 = BitOp(i, auth0); }     //TWIC/CAC 83 Agency Code
    if (i >= 15 && i <= 28) { auth1 = BitOp(i, auth1); }    //TWIC/CAC 83 System Code
    if (i >= 29 && i <= 48) { auth2 = BitOp(i, auth2); }    //TWIC/CAC 83 Credential Code
    if (i >= 49 && i <= 52) { auth3 = BitOp(i, auth3); }    //TWIC/CAC 83 Credential Series
    if (i >= 53 && i <= 56) { auth4 = BitOp(i, auth4); }    //TWIC/CAC 83 Individual Series Issue
    if (i >= 57 && i <= 81) { auth5 = BitOp(i, auth5); }    //TWIC/CAC 83 Expiration Date
    if (i >= 1 && i <= 40 && cardData[i] == 1) { ep1++; }   //Even Parity
    if (i >= 41 && i <= 81 && cardData[i] == 1) { op1++; }  //Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[82] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("TWIC/CAC 83");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Agency Code", GetAuth(auth0), LINE);
  AddAuthItem("System Code", GetAuth(auth1), LINE);
  AddAuthItem("Credential Code", GetAuth(auth2), LINE);
  AddAuthItem("Credential Series", GetAuth(auth3), LINE);
  AddAuthItem("Individual Series Issue", GetAuth(auth4), LINE);
  AddAuthItem("Expiration Date", GetAuth(auth5), NONE);
  SaveCardData();
}

static void Parse91bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 1 && i <= 14) { auth0 = BitOp(i, auth0); }   //TWIC/CAC 83 TSM Agency Code
    if (i >= 15 && i <= 28) { auth1 = BitOp(i, auth1); }  //TWIC/CAC 83 TSM System Code
    if (i >= 29 && i <= 48) { auth2 = BitOp(i, auth2); }  //TWIC/CAC 83 TSM Credential Code
    if (i >= 49 && i <= 52) { auth3 = BitOp(i, auth3); }  //TWIC/CAC 83 TSM Credential Series
    if (i >= 53 && i <= 56) { auth4 = BitOp(i, auth4); }  //TWIC/CAC 83 TSM Individual Series Issue
    if (i >= 57 && i <= 81) { auth5 = BitOp(i, auth5); }  //TWIC/CAC 83 TSM Expiration Date
    if (i >= 82 && i <= 89) { auth6 = BitOp(i, auth6); }  //TWIC/CAC 83 TSM Transaction Status Messages
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("TWIC/CAC 83 TSM");
  AddAuthItem("Agency Code", GetAuth(auth0), LINE);
  AddAuthItem("System Code", GetAuth(auth1), LINE);
  AddAuthItem("Credential Code", GetAuth(auth2), LINE);
  AddAuthItem("Credential Series", GetAuth(auth3), LINE);
  AddAuthItem("Individual Series Issue", GetAuth(auth4), LINE);
  AddAuthItem("Expiration Date", GetAuth(auth5), LINE);
  AddAuthItem("Transaction Status Messages", GetAuth(auth6), NONE);
  SaveCardData();
}

static void Parse107bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 15 && i <= 28) { auth0 = BitOp(i, auth0); }    //PIV Site Code
    if (i >= 29 && i <= 48) { auth1 = BitOp(i, auth1); }    //PIV Card Number
    if (i >= 1 && i <= 14) { auth2 = BitOp(i, auth2); }     //PIV Agency Code
    if (i >= 49 && i <= 73) { auth3 = BitOp(i, auth3); }    //PIV Expiration Date
    if (i >= 1 && i <= 37 && cardData[i] == 1) { ep1++; }   //PIV Even Parity
    if (i >= 38 && i <= 73 && cardData[i] == 1) { op1++; }  //PIV Odd Parity
  }
  if (cardData[0] != ep1 % 2 == 0 ? 0 : 1) { parity = "Fail"; }
  if (cardData[74] != op1 % 2 == 0 ? 1 : 0) { parity = "Fail"; }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("PIV 75 HMAC 32");
  AddAuthItem("Parity", parity, LINE);
  AddAuthItem("Site Code", GetAuth(auth0), LINE);
  AddAuthItem("Card Number", GetAuth(auth1), LINE);
  AddAuthItem("Agency Code", GetAuth(auth2), LINE);
  AddAuthItem("Expiration Date", GetAuth(auth3), NONE);
  SaveCardData();
}

static void Parse128bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    auth0 = BitOp(i, auth0);  //Full Card Number
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("PIV-I");
  AddAuthItem("CSN", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse200bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 29 && i <= 48) { auth0 = BitOp(i, auth0); }  //FASC-N Embedded HMA/EXP Card Number
    if (i >= 30 && i <= 49) { auth1 = BitOp(i, auth1); }  //PIV Facility Code
    if (i >= 55 && i <= 84) { auth2 = BitOp(i, auth2); }  //PIV Card Number
  }
  for (int i = 49; i > 29; i--) {
    auth3 = BitOp(i, auth3);  //FASC-N Embedded HMA/EXP Facility Code
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("PIV");
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth2), END);
  AddAuthHeader("FASC-N Embedded HMA/EXP");
  AddAuthItem("Facility Code", GetAuth(auth3), LINE);
  AddAuthItem("Card Number", GetAuth(auth0), NONE);
  SaveCardData();
}

static void Parse245bit(void) {
  for (int i = 0; i < bitCount; i++) {
    bin += cardData[i];
    if (i >= 29 && i <= 48) { auth0 = BitOp(i, auth0); }  //FASC-N Appended EXP Card Number
  }
  for (int i = 49; i > 29; i--) {
    auth1 = BitOp(i, auth1);  //FASC-N Appended EXP Facility Code
  }
  if (IsFunctionCard()) { return; }
  AddAuthHeader("FASC-N Appended EXP");
  AddAuthItem("Facility Code", GetAuth(auth1), LINE);
  AddAuthItem("Card Number", GetAuth(auth0), NONE);
  SaveCardData();
}

}  //namespace hrCardReader
