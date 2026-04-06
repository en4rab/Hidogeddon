#include "hr_server.h"

namespace hrServer {

String baseURL = "";
bool showCloneAction = false;
bool autoRefresh = false;
int wiegandSelected = 26;

void ServerInit(void) {
  //Method called, Name, Stack size (bytes), param passed, priority, handle, core
  xTaskCreatePinnedToCore(ServerTasks, "ServerTasks", 8192, NULL, 1, NULL, 0);
}

static void ServerTasks(void* p) {
  //Connect to WiFi or Start AP
  bool captivePortalEnabled = hrSettings::allowCaptivePortal;
  if (hrSettings::wifiSSID == "" || hrSettings::wifiPassword == "") {
    hrSettings::oneShotConnectToAP = false;
  }

  if (hrSettings::oneShotConnectToAP) {
    captivePortalEnabled = false;
    WiFi.mode(WIFI_STA);
    WiFi.begin(hrSettings::wifiSSID, hrSettings::wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
      hrUtil::LedControl(false, true, false, false);
      hrUtil::LedControl(true, false, true, false);
    }
    hrUtil::LedControl(true, true, true, false);
    baseURL = "http://" + WiFi.localIP().toString();
    hrSettings::wifiIP = WiFi.localIP();
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hrSettings::apSSID, hrSettings::apPassword);
    IPAddress apSubnet(255, 255, 255, 0);
    WiFi.softAPConfig(hrSettings::apIP, hrSettings::apIP, apSubnet);
    delay(500);
    baseURL = "http://" + hrSettings::apIP.toString();
  }

  //Start DNS server
  DNSServer dnsServer;
  if (captivePortalEnabled) {
    dnsServer.start(53, "*", hrSettings::apIP);
  }

  //Start web server
  AsyncWebServer webServer(80);
  webServer.begin();


  //----- Captive Portal ----------------------------------------------------------------------------------------------------
  if (captivePortalEnabled) {
    //Android captive portal.
    webServer.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->redirect(baseURL);
    });

    //iOS captive portal.
    webServer.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->redirect(baseURL);
    });

    //Windows captive portal
    webServer.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(200, "text/html", "ok");
    });

    //Windows captive portal
    webServer.on("/redirect", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->redirect(baseURL);
    });
  }
  //-------------------------------------------------------------------------------------------------------------------------


  //----- Hidogeddon Reader ------------------------------------------------------------------------------------------------
  //Shows home page
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    int requestedPage = 1;
    if (!autoRefresh && request->params() == 1 && request->hasParam("page")) {
      AsyncWebParameter* p1 = request->getParam("page");
      String page = p1->value();
      if (hrUtil::IsNumeric(page)) {
        requestedPage = page.toInt();
      }
    }

    String html = hrHTML::Header(autoRefresh, requestedPage);
    html += hrHTML::Menu(autoRefresh);
    html += hrHTML::VersionAndBuild();
    html += hrHTML::LastCardData();
    html += hrHTML::CardData(autoRefresh, requestedPage, showCloneAction);
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Companion detect
  // webServer.on("/companionDetect", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   request->send(200, "text/html", "hidogeddon-reader");
  // });

  //Get special cards (test and function)
  // webServer.on("/getSpecialCards", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   String data = hrSettings::TEST_CARD + ",";
  //   data += hrSettings::FC_DEFAULT + ",";
  //   data += hrSettings::FC_CONNECT_TO_AP + ",";
  //   data += hrSettings::FC_SHOW_AP_INFO;
  //   request->send(200, "text/html", data);
  // });

  //Companion clone action button
  // webServer.on("/enableCloneAction", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   showCloneAction = true;
  //   request->send(200, "text/html", "ok");
  // });

  // webServer.on("/disableCloneAction", HTTP_GET, [](AsyncWebServerRequest* request) {
  //   showCloneAction = false;
  //   request->send(200, "text/html", "ok");
  // });

  //Replays binary data via Wiegand
  webServer.on("/replayCard", HTTP_GET, [](AsyncWebServerRequest* request) {
    int requestedPage = 1;

    if (request->params() == 2) {
      String binaryData = "";
      int numOfBits = 0;

      if (request->hasParam("page")) {
        AsyncWebParameter* p1 = request->getParam("page");
        String page = p1->value();
        if (hrUtil::IsNumeric(page)) {
          requestedPage = page.toInt();
        }
      }

      if (request->hasParam("b")) {
        AsyncWebParameter* p2 = request->getParam("b");
        binaryData = p2->value();
        numOfBits = binaryData.length();
      }

      if (numOfBits > 0) {
        hrSettings::replayBin = binaryData;
        hrSettings::lastCardData = "Replay Wiegand<br>";
        hrSettings::lastCardData += "Bits: " + String(numOfBits) + "<br>";
        hrSettings::lastCardData += "Bin: " + binaryData + "<br>";
        hrUtil::OLEDShowMessage("Replay Wiegand", "", "Bits: " + String(numOfBits), "", "Bin: " + binaryData, "");
      }
    }

    request->redirect(baseURL + "/?page=" + String(requestedPage));
  });

  //Toggle auto refresh on/off
  webServer.on("/toggleRefresh", HTTP_GET, [](AsyncWebServerRequest* request) {
    autoRefresh = !autoRefresh;
    request->redirect(baseURL);
  });

  //Shows send card number page
  webServer.on("/sendCardNumber", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = hrHTML::Header(false, 1);
    html += hrHTML::SendCardNumber(wiegandSelected);
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Send Wiegand (data recevied from send card number page)
  webServer.on("/sendWiegand", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->params() == 2) {
      String cardNo = "";
      int numOfDigits = 0;
      int numOfBits = 26;

      if (request->hasParam("n")) {
        AsyncWebParameter* p1 = request->getParam("n");
        cardNo = p1->value();
        numOfDigits = cardNo.length();
      }

      if (request->hasParam("w")) {
        AsyncWebParameter* p2 = request->getParam("w");
        if (hrUtil::IsNumeric(p2->value())) {
          numOfBits = p2->value().toInt();
          wiegandSelected = numOfBits;
        }
      }

      if (numOfDigits > 0 && numOfDigits < 9 && hrUtil::IsNumeric(cardNo)) {
        hrSettings::replayBin = hrUtil::CalculateBinary(cardNo, numOfBits);
        hrSettings::lastCardData = "Send Wiegand<br>";
        hrSettings::lastCardData += "Number: " + cardNo + "<br>";
        hrSettings::lastCardData += "Bits: " + String(numOfBits) + "<br>";
        hrSettings::lastCardData += "Bin: " + hrSettings::replayBin + "<br>";
        hrUtil::OLEDShowMessage("Send Wiegand", "", ":Number:", cardNo, ":Bits:", String(numOfBits));
      }
    }
    request->redirect(baseURL);
  });

  //Download/view raw log
  webServer.on("/exportLog", HTTP_GET, [](AsyncWebServerRequest* request) {
    bool logAvailable = false;
    File logFile = SPIFFS.open("/card_data.txt", "r");
    if (logFile) {
      if (logFile.available()) {
        logAvailable = true;
      }
      logFile.close();
    }

    if (logAvailable && request->params() == 1 && request->hasParam("d")) {
      AsyncWebParameter* p1 = request->getParam("d");
      bool download = true;
      if (p1->value() == "0") {
        download = false;
      }
      request->send(SPIFFS, "/card_data.txt", String(), download);
      return;
    }

    hrSettings::lastCardData = "Logfile not available";
    request->redirect(baseURL);
  });

  //Shows confirm clear log page
  webServer.on("/confirmClearLog", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = hrHTML::Header(false, 1);
    html += hrHTML::ConfirmMessage("clearLog", "Are you sure you want to clear the log?");
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Clear log
  webServer.on("/clearLog", HTTP_GET, [](AsyncWebServerRequest* request) {
    SPIFFS.remove("/card_data.txt");
    hrSettings::lastCardData = "Logfile cleared";
    request->redirect(baseURL);
  });

  //Trigger BLE Scan (via VibrateBLE firmware on another ESP32)
  webServer.on("/triggerBleScan", HTTP_GET, [](AsyncWebServerRequest* request) {
    hrSettings::lastCardData = "Trigger BLE Scan command sent";
    hrUtil::RequestTriggerBLEScan();
    request->redirect(baseURL);
  });

  //Shows confirm reboot ESP32 page
  webServer.on("/confirmReboot", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = hrHTML::Header(false, 1);
    html += hrHTML::ConfirmMessage("rebootESP32", "Are you sure you want to reboot the ESP32?");
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Reboot ESP32
  webServer.on("/rebootESP32", HTTP_GET, [](AsyncWebServerRequest* request) {
    autoRefresh = false;
    if (showCloneAction) {
      hrSettings::lastCardData = "Reboot command sent<br>Device will reconnect to AP";
      hrUtil::RequestRebootAndConnectToAP(5000);
    } else {
      hrSettings::lastCardData = "Reboot command sent";
      hrUtil::RequestReboot(5000);
    }
    request->redirect(baseURL);
  });

  //Shows confirm remove special cards
  webServer.on("/confirmRemoveSpecialCards", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = hrHTML::Header(false, 1);
    html += hrHTML::ConfirmMessage("RemoveSpecialCards", "Are you sure you want to remove all test and function cards?");
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Remove special cards
  webServer.on("/RemoveSpecialCards", HTTP_GET, [](AsyncWebServerRequest* request) {
    autoRefresh = false;
    hrUtil::ClearSpecialCards();
    hrSettings::lastCardData = "Speical cards have been removed";
    request->redirect(baseURL);
  });

  //Shows settings page
  webServer.on("/viewSettings", HTTP_GET, [](AsyncWebServerRequest* request) {
    String html = hrHTML::Header(false, 1);
    html += hrHTML::ViewSettings("");
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });

  //Shows select special card page
  webServer.on("/selectSpecialCard", HTTP_GET, [](AsyncWebServerRequest* request) {
    int requestedPage = 1;

    if (request->params() == 2) {
      String binaryData = "";
      int numOfBits = 0;

      if (request->hasParam("page")) {
        AsyncWebParameter* p1 = request->getParam("page");
        String page = p1->value();
        if (hrUtil::IsNumeric(page)) {
          requestedPage = page.toInt();
        }
      }

      if (request->hasParam("b")) {
        AsyncWebParameter* p2 = request->getParam("b");
        binaryData = p2->value();
        numOfBits = binaryData.length();
      }

      if (numOfBits >= 12 && numOfBits <= 255) {
        String html = hrHTML::Header(false, 1);
        html += hrHTML::ViewSelectSpecialCard(binaryData, requestedPage);
        html += hrHTML::Footer();
        request->send(200, "text/html", html);
        return;
      }
    }
    request->redirect(baseURL + "/?page=" + String(requestedPage));
  });

  //Save special cards
  webServer.on("/saveSpecialCard", HTTP_GET, [](AsyncWebServerRequest* request) {
    int requestedPage = 1;

    if (request->params() == 3) {
      String binaryData = "";
      int numOfBits = 0;
      String sType = "TEST_CARD";

      if (request->hasParam("page")) {
        AsyncWebParameter* p1 = request->getParam("page");
        String page = p1->value();
        if (hrUtil::IsNumeric(page)) {
          requestedPage = page.toInt();
        }
      }

      if (request->hasParam("b")) {
        AsyncWebParameter* p2 = request->getParam("b");
        binaryData = p2->value();
        numOfBits = binaryData.length();
      }

      if (request->hasParam("t")) {
        AsyncWebParameter* p3 = request->getParam("t");
        sType = p3->value();
      }

      if (numOfBits >= 12 && numOfBits <= 255) {
        if (hrSettings::TEST_CARD == binaryData) { hrSettings::TEST_CARD = "n"; }
        if (hrSettings::FC_DEFAULT == binaryData) { hrSettings::FC_DEFAULT = "n"; }
        if (hrSettings::FC_CONNECT_TO_AP == binaryData) { hrSettings::FC_CONNECT_TO_AP = "n"; }
        if (hrSettings::FC_SHOW_AP_INFO == binaryData) { hrSettings::FC_SHOW_AP_INFO = "n"; }
        if (sType == "TEST_CARD") { hrSettings::TEST_CARD = binaryData; }
        if (sType == "FC_DEFAULT") { hrSettings::FC_DEFAULT = binaryData; }
        if (sType == "FC_CONNECT_TO_AP") { hrSettings::FC_CONNECT_TO_AP = binaryData; }
        if (sType == "FC_SHOW_AP_INFO") { hrSettings::FC_SHOW_AP_INFO = binaryData; }
        hrUtil::SaveSpecialCards();
        hrSettings::lastCardData = "Special Card Assignment<br>";
        hrSettings::lastCardData += "Type: " + sType + "<br>";
        hrSettings::lastCardData += "Bits: " + String(numOfBits) + "<br>";
        hrSettings::lastCardData += "Bin: " + binaryData + "<br>";
      }
    }
    request->redirect(baseURL + "/?page=" + String(requestedPage));
  });

  //Save settings
  webServer.on("/saveSettings", HTTP_POST, [](AsyncWebServerRequest* request) {
    bool hasAllParams = true;
    int wpw = 50;
    int wil = 2000;
    String message = "";

    if (request->params() > 0) {
      if (!request->hasParam("apname", true)) { hasAllParams = false; }
      if (!request->hasParam("appassword", true)) { hasAllParams = false; }
      if (!request->hasParam("apip", true)) { hasAllParams = false; }
      if (!request->hasParam("ssid", true)) { hasAllParams = false; }
      if (!request->hasParam("ssidpassword", true)) { hasAllParams = false; }
      if (!request->hasParam("refreshseconds", true)) { hasAllParams = false; }
      if (!request->hasParam("wiegandpulsewidth", true)) { hasAllParams = false; }
      if (!request->hasParam("wiegandinterleave", true)) { hasAllParams = false; }

      if (hasAllParams) {
        AsyncWebParameter* p1 = request->getParam("apname", true);
        String p1_apname = p1->value();
        AsyncWebParameter* p2 = request->getParam("appassword", true);
        String p2_appassword = p2->value();
        AsyncWebParameter* p3 = request->getParam("apip", true);
        String p3_apip = p3->value();
        AsyncWebParameter* p4 = request->getParam("ssid", true);
        String p4_ssid = p4->value();
        AsyncWebParameter* p5 = request->getParam("ssidpassword", true);
        String p5_ssidpassword = p5->value();
        AsyncWebParameter* p6 = request->getParam("refreshseconds", true);
        String p6_refreshseconds = p6->value();
        AsyncWebParameter* p7 = request->getParam("wiegandpulsewidth", true);
        String p7_wiegandpulsewidth = p7->value();
        AsyncWebParameter* p8 = request->getParam("wiegandinterleave", true);
        String p8_wiegandinterleave = p8->value();

        p1_apname.trim();
        if (p1_apname.length() < 8) {
          hasAllParams = false;
          message += "AP name must be at least 8 characters";
        }

        p2_appassword.trim();
        if (p2_appassword.length() < 8) {
          hasAllParams = false;
          if (message != "") { message += "<br>"; }
          message += "AP password must be at least 8 characters";
        }

        p3_apip.trim();
        if (hrUtil::IsIpAddress(p3_apip) == false) {
          hasAllParams = false;
          if (message != "") { message += "<br>"; }
          message += "Please enter a valid AP IP address";
        }

        p6_refreshseconds.trim();
        if (p6_refreshseconds == "") {
          hasAllParams = false;
          if (message != "") { message += "<br>"; }
          message += "HTML auto refresh seconds cannot be empty";
        } else {
          if (hrUtil::IsNumeric(p6_refreshseconds) == false) {
            p6_refreshseconds = "5";
          }
          int rs = p6_refreshseconds.toInt();
          if (rs < 1 || rs > 60) {
            p6_refreshseconds = "5";
          }
        }

        p7_wiegandpulsewidth.trim();
        if (p7_wiegandpulsewidth == "") {
          hasAllParams = false;
          if (message != "") { message += "<br>"; }
          message += "Wiegand Pulse Width cannot be empty";
        } else {
          if (hrUtil::IsNumeric(p7_wiegandpulsewidth) == false) {
            p7_wiegandpulsewidth = "50";
          }
          wpw = p7_wiegandpulsewidth.toInt();
          if (wpw < 10 || wpw > 1000) {
            wpw = 50;
          }
        }

        p8_wiegandinterleave.trim();
        if (p8_wiegandinterleave == "") {
          hasAllParams = false;
          if (message != "") { message += "<br>"; }
          message += "Wiegand Interleave cannot be empty";
        } else {
          if (hrUtil::IsNumeric(p8_wiegandinterleave) == false) {
            p8_wiegandinterleave = "2000";
          }
          wil = p8_wiegandinterleave.toInt();
          if (wil < 100 || wil > 20000) {
            wil = 2000;
          }
        }

        if (hasAllParams) {
          hrSettings::apSSID = p1_apname;
          hrSettings::apPassword = p2_appassword;
          hrSettings::apIP.fromString(p3_apip);
          hrSettings::wifiSSID = p4_ssid;
          hrSettings::wifiPassword = p5_ssidpassword;
          hrSettings::webPollSeconds = p6_refreshseconds;
          hrSettings::allowCaptivePortal = false;
          hrSettings::oledConnected = false;
          hrSettings::wiegandPulseWidth = wpw;
          hrSettings::wiegandInterleave = wil;
          hrSettings::wiegandLearnMode = false;
          hrSettings::captureUnknownBitLengths = false;

          if (request->hasParam("allowcaptiveportal", true)) {
            AsyncWebParameter* p9 = request->getParam("allowcaptiveportal", true);
            if (p9->value() == "on") {
              hrSettings::allowCaptivePortal = true;
            }
          }

          if (request->hasParam("oledconnected", true)) {
            AsyncWebParameter* p10 = request->getParam("oledconnected", true);
            if (p10->value() == "on") {
              hrSettings::oledConnected = true;
            }
          }

          if (request->hasParam("wiegandlearnmode", true)) {
            AsyncWebParameter* p11 = request->getParam("wiegandlearnmode", true);
            if (p11->value() == "on") {
              hrSettings::wiegandLearnMode = true;
            }
          }

          if (request->hasParam("captureunknownbitlengths", true)) {
            AsyncWebParameter* p12 = request->getParam("captureunknownbitlengths", true);
            if (p12->value() == "on") {
              hrSettings::captureUnknownBitLengths = true;
            }
          }

          hrUtil::SaveSettings();
          if (showCloneAction) {
            message = "Settings have been saved<br>Reboot command sent<br>Device will reconnect to AP";
            hrUtil::RequestRebootAndConnectToAP(5000);
          } else {
            message = "Settings have been saved<br>Reboot command sent";
            hrUtil::RequestReboot(5000);
          }
        }
      }
    }

    String html = hrHTML::Header(false, 1);
    html += hrHTML::ViewSettings(message);
    html += hrHTML::Footer();
    request->send(200, "text/html", html);
  });
  //-------------------------------------------------------------------------------------------------------------------------


  //----- No found route, send 404 ------------------------------------------------------------------------------------------
  webServer.onNotFound([](AsyncWebServerRequest* request) {
    request->send(404);
  });
  //-------------------------------------------------------------------------------------------------------------------------

  //Keep task alive
  while (true) {
    vTaskDelay(portMAX_DELAY);
  }
}

}  //namespace hrServer
