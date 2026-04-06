const char *logo = R"(
_   _ ___________ _____ _____  _________________ _____ _   _  
| | | |_   _|  _  \  _  |  __ \|  ___|  _  \  _  \  _  | \ | | 
| |_| | | | | | | | | | | |  \/| |__ | | | | | | | | | |  \| | 
|  _  | | | | | | | | | | | __ |  __|| | | | | | | | | | . ` | 
| | | |_| |_| |/ /\ \_/ / |_\ \| |___| |/ /| |/ /\ \_/ / |\  | 
\_| |_/\___/|___/  \___/ \____/\____/|___/ |___/  \___/\_| \_/)";

/*

Product Name:  Wiegand Send for Hidogeddon Reader
Version:       1.0
Build Date:    2026-04-06
Author:        Daniel Raines [ Dr0pR00t ] [ https://www.linkedin.com/in/danielraines ]

::NOTE::
This firmware is designed to run on another ESP32 
and output Wiegand to the Hidogeddon Reader ESP32
It was designed purely for testing.

::CREDITS::
Craigsblackie & Iceman: ASCII art

::HARDWARE::
This firmware was tested using a FireBeetle 2 ESP32-E
https://www.dfrobot.com/product-2195.html

::GITHUB::HIDOGEDDON::
The home for this project, OLED-PCB and DIY versions, respectively
https://github.com/en4rab/Hidogeddon
https://github.com/00Waz/Hidogeddon

::IDE::
This firmware was written, compiled and tested using Arduino IDE v2.3.6

*/

#include <Arduino.h>
const byte D0_PIN = 17; //Wiegand D0
const byte D1_PIN = 16; //Wiegand D1
String bin = "";
String data = "";

void SendWiegand() {
  if (bin.length() == 0) { return; }
  Serial.println("");
  Serial.print("Bits: ");
  Serial.println(bin.length());
  Serial.print("Bin: ");
  Serial.println(bin);
  Serial.println("Auth Data...");
  Serial.println(data);
  Serial.println("");
  Serial.println("--------------------------------------------------------------");

  for (int i = 0; i < bin.length(); i++) {
    if (bin[i] == '0') {
      digitalWrite(D0_PIN, 0);
      delayMicroseconds(50);
      digitalWrite(D0_PIN, 1);
    } else {
      digitalWrite(D1_PIN, 0);
      delayMicroseconds(50);
      digitalWrite(D1_PIN, 1);
    }
    delayMicroseconds(2000);
  }

  bin = "";
}

void SerialPrintMenu() {
  Serial.println(logo);
  Serial.println("");
  Serial.println("--------------------------------------------------------------");
  Serial.println("Hidogeddon Reader -> Wiegand Send Tests");
  Serial.println("--------------------------------------------------------------");
  Serial.println("");
  Serial.println("0: This menu");
  Serial.println("26: Send 26 bit Wiegand");
  Serial.println("27: Send 27 bit Wiegand");
  Serial.println("28: Send 28 bit Wiegand");
  Serial.println("29: Send 29 bit Wiegand");
  Serial.println("30: Send 20 bit Wiegand");
  Serial.println("31: Send 31 bit Wiegand");
  Serial.println("32: Send 32 bit Wiegand");
  Serial.println("33: Send 33 bit Wiegand");
  Serial.println("34: Send 34 bit Wiegand");
  Serial.println("35: Send 35 bit Wiegand");
  Serial.println("36: Send 36 bit Wiegand");
  Serial.println("37: Send 37 bit Wiegand");
  Serial.println("38: Send 38 bit Wiegand");
  Serial.println("39: Send 39 bit Wiegand");
  Serial.println("40: Send 40 bit Wiegand");
  Serial.println("42: Send 42 bit Wiegand");
  Serial.println("46: Send 46 bit Wiegand");
  Serial.println("48: Send 48 bit Wiegand");
  Serial.println("56: Send 56 bit Wiegand");
  Serial.println("58: Send 58 bit Wiegand");
  Serial.println("64: Send 64 bit Wiegand");
  Serial.println("75: Send 75 bit Wiegand");
  Serial.println("80: Send 80 bit Wiegand");
  Serial.println("83: Send 83 bit Wiegand");
  Serial.println("91: Send 91 bit Wiegand");
  Serial.println("107: Send 107 bit Wiegand");
  Serial.println("128: Send 128 bit Wiegand");
  Serial.println("200: Send 200 bit Wiegand");
  Serial.println("245: Send 245 bit Wiegand");
  Serial.println("");
  Serial.println("--------------------------------------------------------------");
}

void Send(String bitCount) {
  if (bitCount == "26") {
    data = "H10301\nParity: Pass\nFacility Code: 15\nCard Number: 31872\n\nIndala\nParity: Pass\nFacility Code: 247\nCard Number: 3200\n\nFull Card Number\nCSN: 1014912";
    bin = "10000111101111100100000000";
  }

  if (bitCount == "27") {
    data = "Indala\nFacility Code: 454\nCard Number: 8785\n\nIndala ASC\nFacility Code: 3601\nCard Number: 8642\n\nTecom\nFacility Code: 88\nCard Number: 9863";
    bin = "000011100011010001001010001";
  }

  if (bitCount == "28") {
    data = "2804\nParity: Pass\nFacility Code: 208\nIssue Number: 29813";
    bin = "0010110100001110100011101010";
  }

  if (bitCount == "29") {
    data = "Indala\nFacility Code: 2087\nCard Number: 18665";
    bin = "01000001001110100100011101001";
  }

  if (bitCount == "30") {
    data = "ATS\nParity: Pass\nFacility Code: 3645\nCard Number: 29321";
    bin = "011100011110101110010100010010";
  }

  if (bitCount == "31") {
    data = "HID ADT\nFacility Code: 12\nCard Number: 588893";
    bin = "0110000010001111110001011101000";
  }

  if (bitCount == "32") {
    data = "ATS\nParity: Pass\nFacility Code: 617\nCard Number: 2160\n\nIndala/Kantech KSF\nFacility Code: 210\nCard Number: 2160\n\nK32\nParity: Pass\nFacility Code: 210\nCard Number: 2160\nIssue Level: 4\n\nW32\nFacility Code: 2468\nCard Number: 4321\n\nHID\nFacility Code: 308\nCard Number: 133232\n\nHID Check Point\nCard Number: 1263649\n\n32-B\nParity: Pass\nFacility Code: 1234\nCard Number: 2160\n\nKastle\nParity: Pass\nFacility Code: 210\nCard Number: 2160\nIssue Level: 4\n\nFull Card Number\nCSN: 2309230817";
    bin = "10001001101001000001000011100001";
  }

  if (bitCount == "33") {
    data = "Indala DSX\nFacility Code: 45\nCard Number: 7539517\n\nHID D1_PIN0202\nParity: Pass\nFacility Code: 45\nCard Number: 7539517\n\nRS2-HID R901592C\nParity: Pass\nFacility Code: 2\nCard Number: 112821662";
    bin = "101011010111001100001011001111011";
  }

  if (bitCount == "34") {
    data = "HID H10306\nParity: Pass\nFacility Code: 38243\nCard Number: 43862\n\nIndala Optus\nFacility Code: 854\nCard Number: 38243\n\nCardkey Smartpass\nFacility Code: 4780\nCard Number: 43862\nIssue Level: 3\n\nHID N1002\nFacility Code: 99\nCard Number: 43862\n\nBQT\nParity: Pass\nFacility Code: 149\nCard Number: 6531926\n\nFull Card Number\nCSN: 2506337110";
    bin = "0100101010110001110101011010101100";
  }

  if (bitCount == "35") {
    data = "HID Corporate 1000\nParity: Pass\nCompany ID Code: 800\nCard Number: 1014912\n\nFull Card Number\nCSN: 5134843008";
    bin = "01001100100000111101111100100000001";
  }

  if (bitCount == "36") {
    data = "Chubb\nFacility Code: 5972\nCard Number: 21821\n\nHID Inner Range\nFacility Code: 1359\nCard Number: 23889\n\nHID Simplex\nParity: Pass\nFacility Code: 93\nCard Number: 4674877\nIssue Level: 1\n\nHID Siemens\nParity: Pass\nFacility Code: 95559\nCard Number: 21821";
    bin = "001011101010100011101010101001111010";
  }

  if (bitCount == "37") {
    data = "HID H10302 (CSN)\nParity: Pass\nCard Number: 18019744896\n\nHID H10304 (FARPOINTE)\nParity: Pass\nFaciltiy Code: 34369\nCard Number: 490624\n\nGuardPoint MDI\nParity: Pass\nFaciltiy Code: 1\nCard Number: 303004800\n\nAWID RS2 34\nParity: Pass\nFaciltiy Code: 134\nCard Number: 4321168\n\nHID Generic\nCard Number: 839875712";
    bin = "1100001100100000111101111100100000001";
  }

  if (bitCount == "38") {
    data = "BQT\nParity: Pass\nFacility Code: 5595\nAccess Code: 452334\nIssue Code: 2\n\nISCS\nParity: Pass\nFacility Code: 823\nCard Number: 1856987";
    bin = "11101110011011101110001010101110110111";
  }

  if (bitCount == "39") {
    data = "Pyramid\nParity: Pass\nFacility Code: 46631\nCard Number: 951753";
    bin = "001011011000100111111010000101110010010";
  }

  if (bitCount == "40") {
    data = "HID Honeywell\nFacility Code: 3568\nCard Number: 39211\n\nXceedID RS2\nParity: Pass\nFacility Code: 111\nCard Number: 139236736\n\nCasi-Rusco\nCard Number: 29935572352";
    bin = "1000110111110000100110010010101100000001";
  }

  if (bitCount == "42") {
    data = "Lenel\nParity: Pass\nFacility Code: 2430\nCard Number: 555";
    bin = "001001011111100010001010110000000000000000";
  }

  if (bitCount == "46") {
    data = "DCAC\nFacility Code: 10622\nCard Number: 6675219";
    bin = "0000000101001011111100110010111011011000100110";
  }

  if (bitCount == "48") {
    data = "HID H2004064 (Corporate 1000)\nParity: Pass\nCompany ID Code: 260\nCard Number: 75458\n\nHID H10304 (FC/CN 37)\nParity: Pass\nFacility Code: 4160\nCard Number: 75458\n\nHUGHES ID 37 H10302\nParity: Pass\nCard Number: 2181113538\n\nFull Card Number\nCSN: 35186553202370";
    bin = "110000000000000100000100000000100100110110000100";
  }

  if (bitCount == "56") {
    data = "Inner Range SIFER\nFacility Code: 649731\nCard Number: 1379852\n\nFull Card Number\nCSN: 2790573397577228";
    bin = "00001001111010100000001100000000000101010000111000001100";
  }

  if (bitCount == "58") {
    data = "TWIC/CAC 58\nParity: Pass\nAgency Code: 280\nSystem Code: 1091\nCredential Code: 135036\nCredential Series: 8\nIndividual Series Issue: 0\n\nFull Card Number\nCSN: 1231745920760960";
    bin = "1000001000110000001000100001100100000111101111100100000000";
  }

  if (bitCount == "64") {
    data = "TWIC/CAC 64 BCD\nAgency Code: 4937\nSystem Code: 3600\nCredential Code: 287320\nCredential Series: 8\nIndividual Series Issue: 0\n\nTWIC/CAC 56 TSM\nAgency Code: 1234\nSystem Code: 4321\nCredential Code: 1122\nCredential Series: 5\nIndividual Series Issue: 8\nTransaction Status Messages: 0\n\n8byte iCLASS\nCSN: 1389657421976328320";
    bin = "0001001101001001000011100001000000000100011000100101100010000000";
  }

  if (bitCount == "75") {
    data = "PIV\nParity: Pass\nSite Code: 3681\nCard Number: 131071\nAgency Code: 4095\nExpiration Date: 555968\n\nPIV Class\nCard Number: 70355424247807";
    bin = "000111111111111001110011000010001111111111111111100000100001111011110000001";
  }

  if (bitCount == "80") {
    data = "10byte CSN\nCSN:12345678902";
    bin = "00000000000000000000000000000000000000000000001011011111110111000001110000110110";
  }

  if (bitCount == "83") {
    data = "TWIC/CAC 83\nParity: Pass\nAgency Code: 1234\nSystem Code: 1111\nCredential Code: 9876\nCredential Series: 12\nIndividual Series Issue: 8\nExpiration Date: 33554431";
    bin = "00001001101001000010001010111000000100110100101001100100011111111111111111111111110";
  }

  if (bitCount == "91") {
    data = "TWIC/CAC 83 TSM\nAgency Code: 1234\nSystem Code: 1111\nCredential Code: 9876\nCredential Series: 12\nIndividual Series Issue: 8\nExpiration Date: 33554431\nTransaction Status Messages: 201";
    bin = "0000100110100100001000101011100000010011010010100110010001111111111111111111111111110010010";
  }

  if (bitCount == "107") {
    data = "PIV 75 HMAC 32\nParity: Pass\nSite Code: 1234\nCard Number: 1048575\nAgency Code: 255\nExpiration Date: 16777215";
    bin = "00000001111111100010011010010111111111111111111110111111111111111111111111000000000000000000000000000000000";
  }

  if (bitCount == "128") {
    data = "PIV-I\nCSN: 12345678901";
    bin = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001011011111110111000001110000110101";
  }

  if (bitCount == "200") {
    data = "PIV\nFacility Code: 925215\nCard Number: 1073741823\n\nFASC-N Embedded HMA/EXP\nFacility Code: 1017735\nCard Number: 986895";
    bin = "00000000000000000000000000000111100001111000011111000001111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  }

  if (bitCount == "245") {
    data = "FASC-N Appended EXP\nFacility Code: 1017735\nCard Number: 986895";
    bin = "00000000000000000000000000000111100001111000011111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  }

  SendWiegand();
}

void setup() {
  pinMode(D0_PIN, OUTPUT);
  pinMode(D1_PIN, OUTPUT);
  digitalWrite(D0_PIN, 1);
  digitalWrite(D1_PIN, 1);
  Serial.begin(115200);
  while (!Serial) { delay(50); }
  delay(1000);
  SerialPrintMenu();
}

void loop() {
  while (Serial.available()) {
    String bitCount = Serial.readStringUntil('\n');
    if (bitCount == "0") {
      SerialPrintMenu();
    } else {
      Send(bitCount);
    }
  }
  delay(10);
}