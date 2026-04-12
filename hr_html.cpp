#include "hr_html.h"

namespace hrHTML {

String DocType(void) {
  return "<!DOCTYPE html><html onclick=\"\"><head>";
}

String AutoRefresh(String seconds, String page) {
  if (page == "") {
    return "<meta http-equiv=\"refresh\" content=\"" + seconds + "\">";
  }
  return "<meta http-equiv=\"refresh\" content=\"" + seconds + ";url=" + page + "\">";
}

String Header(void) {
  String html = "<meta charset=\"utf-8\"><title>Hidogeddon Reader</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style type=\"text/css\">";
  html += "body,hr{margin:0}";
  html += ".logo,a:link{color:#fff}";
  html += ".btn,.dropdown-content a,a:link{text-decoration:none}";
  html += ".infoPanel,.tbl,button{font-weight:700}";
  html += ".btns,.infoPanel,.tbl tbody tr{border-bottom:1px solid #404040}";
  html += "body,td,th{color:#ccc;font-family:arial,sans-serif;font-size:12px;-webkit-text-size-adjust:100%}";
  html += "body{background-color:#0d0d0d}";
  html += "hr{padding:0;border:0;height:1px;background:#404040}";
  html += "a:active,a:hover,a:visited{text-decoration:none;color:#fff}";
  html += ".logo{height:60px;font-size:32px}";
  html += ".btns,.infoPanel,button{font-size:12px}";
  html += ".headSpacer{padding-bottom:5px}";
  html += ".btns{display:flex;padding:0 0 10px}";
  html += ".btn{background-color:#1f1f1f;border:1px solid #404040;border-radius:5px;color:#ccc;padding:12px;margin:0 8px 0 0;text-align:center;cursor:pointer}";
  html += ".btnLg{background-color:#1f1f1f;border:1px solid #404040;border-radius:5px;color:#ccc;padding:12px;margin:0 8px 0 0;text-align:center;cursor:pointer;width:160px}";
  html += ".btn:hover,.dropdown-content{background-color:#2e2e2e}";
  html += ".dropdown,.tooltip{position:relative;display:inline-block}";
  html += ".dropdown-content{display:none;position:absolute;top:42px;min-width:180px;border-radius:5px;box-shadow:0 8px 16px 0 rgba(0,0,0,.2);z-index:1}";
  html += ".dropdown-content a{color:#ccc;font-weight:700;padding:16px;display:block}";
  html += ".dropdown-content a:hover{background-color:#3d3d3d}";
  html += ".dropdown:hover .dropdown-content{display:block}";
  html += ".infoPanel{padding:0 30px 10px 10px;background-color:#0d0d0d;color:#ccc;word-break:break-all}";
  html += ".tbl{border-collapse:collapse;width:100%}";
  html += ".tbl thead tr{background-color:#141414;text-align:left}";
  html += ".tbl td,.tbl th{padding:20px 10px 20px 10px;word-break:break-all;vertical-align: top;}";
  html += ".cellSpace{min-width:50px}";
  html += ".cellSpaceBin{width:550px}";
  html += ".actions{display:flex}";
  html += ".action{font-size:18px;padding:0 12px 0 0}";
  html += ".tooltip .tooltiptext{font-size:12px;visibility:hidden;width:120px;background-color:#1f1f1f;color:#fff;text-align:center;border-radius:6px;padding:5px 0;position:absolute;z-index:1;top:0;right:120%}";
  html += ".tooltip .tooltiptext::after{content:\"\";position:absolute;top:50%;left:100%;margin-top:-5px;border-width:5px;border-style:solid;border-color:transparent transparent transparent #1f1f1f}";
  html += ".tooltip:hover .tooltiptext{visibility:visible}";
  html += "form{border:none}";
  html += ".settingsHeader{color:#fff;padding:10px 0 15px;text-decoration:underline}";
  html += ".settingsName{padding:10px 0 5px}";
  html += "input[type=number],input[type=text],input[type=password]{background-color:#0d0d0d;color:#fff;border:1px solid #2a2a2a;border-radius:5px;height:24px;width:100%;max-width:400px;margin:0 0 10px}";
  html += "input[type=checkbox]{position:relative;left:10px;top:2px;margin:0 0 10px}";
  html += "select {background-color: #0D0D0D;color: #fff;border: 1px solid #2a2a2a;border-radius: 5px;height: 32px;width: 100%;max-width: 400px;margin: 0px 0px 10px 0px;";
  html += "-webkit-box-sizing: content-box;-moz-box-sizing: content-box;box-sizing: content-box;}";
  html += ".dataHeader {color:orange;}";
  html += ".testCard {color:chartreuse;}";
  html += ".parityPass {color:green;}";
  html += ".parityFail {color:red;}";
  html += "</style><script>";
  html += "var allowMessages = false;";
  html += "function EnableCompanion() { ";
  html += "var companionMenu = document.getElementById('companionMenu');";
  html += "companionMenu.removeAttribute('style','');";
  html += "allowMessages = true; }";
  html += "function PostMessage(message) {";
  html += "if (allowMessages) { window.chrome.webview.postMessage(message);";
  html += "} else { alert('Please open this page in Hidogeddon Reader Companion to enable this option.') } }";
  html += "</script></head><body><table width=\"100%\" height=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td align=\"center\" valign=\"top\">";
  return html;
}

String Logo(String link) {
  String html = "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"headSpacer\"><tr>";
  html += "<td height=\"60\" align=\"left\" valign=\"middle\" bgcolor=\"#1F272A\">";
  html += "<div class=\"logo\"><a href=\"" + link + "\">";
  html += "<img src=\"data:image/gif;base64,R0lGODdh8AA8AOMIAAAAgB8nKgCAgACA/4CAgICA/4D/gID//////////////////";
  html += "////////////////ywAAAAA8AA8AAAE/jDISau9OOvNu/9gKI5kaZ5oqq5s675wLM90bd94ru987//AoHBILBqPyKRyyWw6n9Co";
  html += "dEqtWq/YrHbL7Xq/4HARABiYz+h0mixuB8nquPzMdttzAMF8Lwfc/zVlfINofoCHL4KEiwOGiI8peoyLjpCWJJOTl5sjmYwaBKG";
  html += "hAaIEpKIAo6QSBAIYpRYECAiiFKoWrqwWjqYXsL4Tta8EZJWsvaC3q6exyBW1ziWeixqzs6HWtLLZCKTcBdEF2c7b3Kbl2Nfb4t";
  html += "eG69bK5dfB2+fwvtzarLPg48cH97wBVBeMn7x02Q5ECzGNULVxEPPJG+dO4r58tAJg3OhvoEVv/vkuWgM5KxZHVxx7WZwYMCXLe";
  html += "Zga8nmoLqI5jgdImtPIkSfGl9ZSudxob5w8k0R9rsSoc2dKjyFjypxDU5tRiRy7De259SdQqxub0sqG9Jq/lGK7fR1L9OtCD1P3";
  html += "VDVFVqeErAixdjXntu9eps/GCSinlNtdwHh7oiP5tkNcqhmujmQsMi9Cy2fhHYRmM2NnzJqzFrjgj7BRYaFnuUJ3kO+oxT4bc3g";
  html += "sB1RPypQ7210s63VACvcWk2V9T/LtCv4MjGSX0VbqjMJH+nM+ErYI2nFsh8UdHWzJo9YL8j38PahSxspLKs0LmHrNeT9FUu5O92";
  html += "Pr3wyxp9EOGDZ98MNp/lYKUCpVN1k2g0lX3HPsLeRWYZMRVk5O9EGoXlbS6IcGfztxx+Bm93g0EXvqndeSgp4tCB88HSIXn4WjW";
  html += "cgPY4TBKJJRGWpoBof+BURceQyedxAAAiITZGzdedeLaXWVNR1roxCZFDp1Qbnki1LpyF99zfX40TgiKuXHic0B95GFV7IIU3BN";
  html += "BvYedOaJhxGFanY5mXuXGfOBjmdseZSHUaUF55s3wvKSkR8JkFRsAUhZoptqBaiOb/AkCGRCcv6SpGwb8LkjTZc1VdR0aSFq31I";
  html += "tvmRmqrPkFICiMEEqH5peRtoirDvxhh8Ing6gAa6hjqiTQi6SWip507WIbJk3wDJbj3hvPUvSq/H5aKBm7iVbIjucatDrBvWUsx";
  html += "qp0pbl4HtyBgQsabFmGxg5j75bUJonOhNhuxMwF+s20GapIQehrHbLL/Qk040vS5JDADjNZKAMLr3FkssEE7/yjC0EEFswxsg8j";
  html += "LHGGFOQSo5acmKyB4rop+fJLFeQMnYrtyxzoxrGPLPML8skgM03tyzJVHX0LPQEcEwT9NBIJ6300kw37fTTUEct9dRUV2311Vhn";
  html += "rfXWXHft9ddghy322F9EAAA7\" width=\"240\" height=\"60\"/></a></div></td></tr></table>";
  return html;
}

String Menu(bool autoRefresh) {
  String html = "";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr><td align=\"left\" valign=\"top\"> ";
  html += "<div class=\"btns\"><form method=\"get\" action=\"/toggleRefresh\">";
  html += "<button type=\"submit\" class=\"btnLg\">";
  html += autoRefresh == true ? "⏳&nbsp;&nbsp;Auto Refresh On&nbsp;&nbsp;" : "📌&nbsp;&nbsp;Auto Refresh Off&nbsp;&nbsp;";
  html += "</button></form>";

  if (!autoRefresh) {
    html += "<div class=\"dropdown\">";
    html += "<button class=\"btn\">🔽&nbsp;&nbsp;Options&nbsp;&nbsp;</button>";
    html += "<div class=\"dropdown-content\">";
    html += "<a href=\"/sendCardNumber\">Send Card Number</a><hr>";
    html += "<a href=\"/exportLog?d=1\">Download Log</a>";
    //html += "<a href=\"/exportLog?d=0\">View Raw Log</a>";
    html += "<a href=\"/confirmClearLog\">Clear Log</a><hr>";
    html += "<a href=\"/confirmRemoveSpecialCards\">Remove Special Cards</a><hr>";
    html += hrSettings::bleEnabled ? "<a href=\"/triggerBleScan\">Pair FitPro via BLE</a><hr>" : "";
    html += "<a href=\"/confirmReboot\">Reboot</a><hr>";
    html += "<a href=\"/viewSettings\">Settings</a>";
    html += "</div></div>";
    html += "<div id=\"companionMenu\" style=\"visibility: hidden;\" class=\"dropdown\">";
    html += "<button class=\"btn\">🔻&nbsp;&nbsp;Companion&nbsp;&nbsp;</button>";
    html += "<div class=\"dropdown-content\">";
    html += "<a href=\"javascript:PostMessage('HID_WRITE_CARD')\">Write Card</a><hr>";
    html += "<a href=\"javascript:PostMessage('HID_SETTINGS')\">Settings</a>";
    html += "</div></div>";
  }

  html += "</div></td></tr></table>";
  return html;
}

String VersionAndBuild(void) {
  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "Version: ";
  html += hrSettings::VERSION_NUMBER;
  html += "<br>Build Date: ";
  html += hrSettings::BUILD_DATE;
  html += "</div></td></tr></table>";
  return html;
}

String BLEStatus(void) {
  if (!hrSettings::bleEnabled) { return ""; }
  bool isConnected = hrBLE::BLEisConnected();
  if (hrSettings::bleMAC == "") { isConnected = false; }
  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "FitPro BLE Status";
  html += "<br>Device: ";
  html += hrSettings::bleMAC == "" ? "None" : hrSettings::bleMAC;
  html += "<br>State: ";
  html += isConnected == true ? "Connected" : "Disconnected";
  html += "<br></div></td></tr></table>";
  return html;
}

String LastCardData(void) {
  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += hrSettings::lastCardData;
  html += "</div></td></tr></table>";
  return html;
}

String LoadingMessage(void) {
  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "Loading, please wait...";
  html += "</div></td></tr></table>";
  return html;
}

String CardData(bool autoRefresh, int requestedPage, bool showCloneAction) {
  int logCount = hrSettings::logCount;
  String cardRows[logCount + 5];
  int totalPages = 0;

  if (logCount <= 5) {
    totalPages = 1;
    requestedPage = 1;
  } else {
    totalPages = ceil((float)logCount / 5);
    if (requestedPage > totalPages) {
      requestedPage = 1;
    }
  }

  if (logCount > 0) {
    File logFile = SPIFFS.open("/card_data.txt", "r");
    if (logFile) {
      for (int i = 0; i < logCount + 5; i++) {
        cardRows[i] = "";
      }
      int i = logCount;
      while (logFile.available()) {
        String line = logFile.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
          cardRows[i] = line;
          i--;
        }
      }
      logFile.close();
    } else {
      logCount = 0;
    }
  }

  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "Total cards in log: " + String(logCount) + "<br>";
  html += logCount > 0 ? "Page " + String(requestedPage) + " of " + String(totalPages) : "";
  html += "</div></td></tr></table>";

  if (!autoRefresh && logCount > 5) {
    html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
    html += "<td align=\"left\" valign=\"top\"><div class=\"btns\">";
    html += requestedPage > 1 ? "<a href=\"/home?page=" + String(requestedPage - 1) + "\" class=\"btn\">◀&nbsp;&nbsp;Previous&nbsp;</a>" : "";
    html += requestedPage < totalPages ? "<a href=\"/home?page=" + String(requestedPage + 1) + "\" class=\"btn\">&nbsp;Next&nbsp;&nbsp;▶</a>" : "";
    html += "</div></td></tr></table>";
  }

  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"> <table class=\"tbl\"><thead><tr>";
  html += "<th class=\"cellSpace\">Data</th>";
  html += "<th class=\"cellSpace\">Actions</th>";
  html += "</tr></thead><tbody>";

  if (logCount > 0) {
    int start = 1;
    int end = requestedPage * 5;
    if (requestedPage > 1) {
      start = ((requestedPage - 1) * 5) + 1;
    }

    for (int i = start; i <= end; i++) {
      if (cardRows[i] == "") {
        break;
      }

      String bits = hrUtil::SplitString(cardRows[i], ',', 0);
      String bin = hrUtil::SplitString(cardRows[i], ',', 1);
      String auth = hrUtil::SplitString(cardRows[i], ',', 2);

      String cardRow = "<tr>";
      cardRow += "<td>";
      if (bin == hrSettings::TEST_CARD) {
        cardRow += "<span class=\"testCard\">TEST CARD</span><br>";
      } else {
        cardRow += "<span class=\"dataHeader\">Raw</span><br>";
      }
      cardRow += "Bits: " + bits + "<br>";
      cardRow += "Bin: " + bin + "<br>";
      if (auth != "") {
        cardRow += "<br>";
        int ad0 = 0;
        while (true) {
          String authData = hrUtil::SplitString(auth, ';', ad0);
          if (authData == "") { break; }
          if (ad0 != 0) { cardRow += "<br>"; }
          ad0++;
          int ad1 = 0;
          while (true) {
            String authItem = hrUtil::SplitString(authData, '|', ad1);
            if (authItem == "") { break; }
            if (ad1 == 0) {
              cardRow += "<span class=\"dataHeader\">" + authItem + "</span><br>";
            } else {
              if (authItem == "Parity: Pass") {
                cardRow += "Parity: <span class=\"parityPass\">Pass</span><br>";
              } else if (authItem == "Parity: Fail") {
                cardRow += "Parity: <span class=\"parityFail\">Fail</span><br>";
              } else {
                cardRow += authItem + "<br>";
              }
            }
            ad1++;
          }
        }
      }
      cardRow += "</td>";

      if (!autoRefresh) {
        cardRow += "<td><div class=\"actions\">";
        cardRow += "<a class=\"action tooltip\" href=\"/replayCard?page=" + String(requestedPage) + "&b=" + bin + "\">";
        cardRow += "🔑<span class=\"tooltiptext\">Replay Card</span></a>";
        if (showCloneAction) {
          cardRow += "<a class=\"action tooltip\" href=\"javascript:PostMessage('CLONE_CARD_RAW_BIN," + String(bits) + "," + bin + "')\">";
          cardRow += "🔐<span class=\"tooltiptext\">Clone Card</span></a>";
        } else {
          cardRow += "<a class=\"action tooltip\" href=\"/selectSpecialCard?page=" + String(requestedPage) + "&b=" + bin + "\">";
          cardRow += "💾<span class=\"tooltiptext\">Special Cards</span></a>";
        }
        cardRow += "</div></td>";
      } else {
        cardRow += "<td>&nbsp;</td>";
      }

      cardRow += "</tr>";
      html += cardRow;
    }
  } else {
    html += "<tr><td>Log is empty</td><td>&nbsp;</td></tr>";
  }

  html += "</tbody></table></td></tr></table>";
  return html;
}

String ConfirmMessage(String action, String message) {
  String html = "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"btns\">";
  html += "<a href=\"/" + action + "\" class=\"btn\">✅&nbsp;&nbsp;OK&nbsp;&nbsp;</a>";
  html += "<a href=\"/home\" class=\"btn\">❌&nbsp;&nbsp;Cancel&nbsp;&nbsp;</a>";
  html += "</div></td></tr></table>";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<p><strong>" + message + "</strong></p>";
  html += "</div></td></tr></table>";
  return html;
}

String SendCardNumber(int wiegandSelected) {
  String html = "<form id=\"esp32\" name=\"esp32\" method=\"get\" enctype=\"multipart/form-data\" action=\"/sendWiegand\">";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"btns\">";
  html += "<button type=\"submit\" class=\"btn\">✅&nbsp;&nbsp;Send&nbsp;&nbsp;</button>";
  html += "<a href=\"/home\" class=\"btn\">❌&nbsp;&nbsp;Cancel&nbsp;&nbsp;</a>";
  html += "</div></td></tr></table>";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\">";
  html += "<div class=\"infoPanel\"><div class=\"settingsName\">Wiegand Output</div>";
  html += "<select name=\"w\" id=\"w\">";
  html += "<option value=\"26\" ";
  html += wiegandSelected == 26 ? "selected" : "";
  html += ">26 bit (24 bit number, 2 bit parity)</option>";
  html += "<option value=\"32\" ";
  html += wiegandSelected == 32 ? "selected" : "";
  html += ">32 bit (32 bit number, no parity)</option>";
  html += "<option value=\"34\" ";
  html += wiegandSelected == 34 ? "selected" : "";
  html += ">34 bit (32 bit number, 2 bit parity)</option>";
  html += "</select></div>";
  html += "<div class=\"infoPanel\">";
  html += "<div class=\"settingsName\">CSN Card Number</div><input type=\"number\" id=\"n\" name=\"n\">";
  html += "</div></td></tr></table></form>";
  return html;
}

String ViewSelectSpecialCard(String bin, int requestedPage) {
  int selected = 0;
  if (hrSettings::FC_DEFAULT == bin) { selected = 1; }
  if (hrSettings::FC_CONNECT_TO_AP == bin) { selected = 2; }
  if (hrSettings::FC_SHOW_AP_INFO == bin) { selected = 3; }
  String html = "<form id=\"esp32\" name=\"esp32\" method=\"get\" enctype=\"multipart/form-data\" action=\"/saveSpecialCard\">";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"btns\">";
  html += "<button type=\"submit\" class=\"btn\">✅&nbsp;&nbsp;Save&nbsp;&nbsp;</button>";
  html += "<a href=\"/home?page=" + String(requestedPage) + "\" class=\"btn\">❌&nbsp;&nbsp;Cancel&nbsp;&nbsp;</a>";
  html += "</div></td></tr></table>";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\">";
  html += "<div class=\"infoPanel\"><div class=\"settingsName\">Card Assignment</div>";
  html += "<select name=\"t\" id=\"t\">";
  html += "<option value=\"TEST_CARD\" ";
  html += selected == 0 ? "selected" : "";
  html += ">Test Card</option>";
  html += "<option value=\"FC_DEFAULT\" ";
  html += selected == 1 ? "selected" : "";
  html += ">Function: Default settings, retain log</option>";
  html += "<option value=\"FC_CONNECT_TO_AP\" ";
  html += selected == 2 ? "selected" : "";
  html += ">Function: Connect to AP</option>";
  html += "<option value=\"FC_SHOW_AP_INFO\" ";
  html += selected == 3 ? "selected" : "";
  html += ">Function: Show AP info on OLED</option>";
  html += "<hr>";
  html += "<option value=\"REMOVE_CARD\">Remove card</option>";
  html += "</select></div>";
  html += "<div class=\"infoPanel\">";
  html += "<div class=\"settingsName\">Bits: " + String(bin.length()) + "</div>";
  html += "<div class=\"settingsName\">Bin: " + bin + "</div></div>";
  html += "<input type=\"hidden\" id=\"b\" name=\"b\" value=\"" + bin + "\">";
  html += "<input type=\"hidden\" id=\"page\" name=\"page\" value=\"" + String(requestedPage) + "\">";
  html += "</td></tr></table></form>";
  return html;
}

String ViewSettings(String message) {
  String allowCaptivePortal = hrSettings::allowCaptivePortal == true ? "checked" : "";
  String oledConnected = hrSettings::oledConnected == true ? "checked" : "";
  String wiegandLearnMode = hrSettings::wiegandLearnMode == true ? "checked" : "";
  String captureUnknownBitLengths = hrSettings::captureUnknownBitLengths == true ? "checked" : "";
  String bleEnabled = hrSettings::bleEnabled == true ? "checked" : "";

  String html = "<form id=\"esp32Settings\" name=\"esp32Settings\" method=\"post\" enctype=\"multipart/form-data\" action=\"/saveSettings\">";
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"btns\">";
  html += "<button type=\"submit\" class=\"btn\">✅&nbsp;&nbsp;Save&nbsp;&nbsp;</button>";
  html += "<a href=\"/home\" class=\"btn\">❌&nbsp;&nbsp;Cancel&nbsp;&nbsp;</a>";
  html += "</div></td></tr></table>";

  if (message != "") {
    html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
    html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
    html += "<p>" + message + "</p>";
    html += "</div></td></tr></table>";
  }

  //Create Access Point
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<div class=\"settingsHeader\">When creating an Access Point</div>";
  html += "<div class=\"settingsName\">Access Point Name</div><input type=\"text\" id=\"apname\" name=\"apname\" value=\"" + hrSettings::apSSID + "\">";
  html += "<div class=\"settingsName\">Access Point Password</div><input type=\"text\" id=\"appassword\" name=\"appassword\" value=\"" + hrSettings::apPassword + "\">";
  html += "<div class=\"settingsName\">Access Point IP Address</div><input type=\"text\" id=\"apip\" name=\"apip\" value=\"" + hrSettings::apIP.toString() + "\">";
  html += "</div></td></tr></table>";

  //Connect to Access Point
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<div class=\"settingsHeader\">When connecting to an Access Point</div>";
  html += "<div class=\"settingsName\">SSID</div><input type=\"text\" id=\"ssid\" name=\"ssid\" value=\"" + hrSettings::wifiSSID + "\">";
  html += "<div class=\"settingsName\">Password</div><input type=\"password\" id=\"ssidpassword\" name=\"ssidpassword\" value=\"" + hrSettings::wifiPassword + "\">";
  html += "</div></td></tr></table>";

  //Wiegnad Send
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<div class=\"settingsHeader\">Wiegand Send (Replay and Manual)</div>";
  html += "<div class=\"settingsName\">Wiegand Pulse Width (microseconds)</div><input type=\"number\" min=\"10\" max=\"1000\" id=\"wiegandpulsewidth\" name=\"wiegandpulsewidth\" value=\"" + String(hrSettings::wiegandPulseWidth) + "\">";
  html += "<div class=\"settingsName\">Wiegand Interleave (microseconds)</div><input type=\"number\" min=\"100\" max=\"20000\" id=\"wiegandinterleave\" name=\"wiegandinterleave\" value=\"" + String(hrSettings::wiegandInterleave) + "\">";
  html += "<div class=\"settingsName\">Auto Pulse Width/Interleave:<input type=\"checkbox\" id=\"wiegandlearnmode\" name=\"wiegandlearnmode\" " + wiegandLearnMode + "></div>";
  html += "</div></td></tr></table>";

  //Wiegand Receive
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<div class=\"settingsHeader\">Wiegand Receive</div>";
  html += "<div class=\"settingsName\">Capture Unknown Bit Lengths:<input type=\"checkbox\" id=\"captureunknownbitlengths\" name=\"captureunknownbitlengths\" " + captureUnknownBitLengths + "></div>";
  html += "</div></td></tr></table>";

  //General Settings
  html += "<table width=\"100%\" border=\"0\" cellspacing=\"5\" cellpadding=\"0\"><tr>";
  html += "<td align=\"left\" valign=\"top\"><div class=\"infoPanel\">";
  html += "<div class=\"settingsHeader\">General Settings</div>";
  html += "<div class=\"settingsName\">HTML Auto Refresh (seconds)</div><input type=\"number\" min=\"1\" max=\"60\" id=\"refreshseconds\" name=\"refreshseconds\" value=\"" + hrSettings::webPollSeconds + "\">";
  html += "<div class=\"settingsName\">Allow Captive Portal:<input type=\"checkbox\" id=\"allowcaptiveportal\" name=\"allowcaptiveportal\" " + allowCaptivePortal + "></div>";
  html += "<div class=\"settingsName\">OLED Connected:<input type=\"checkbox\" id=\"oledconnected\" name=\"oledconnected\" " + oledConnected + "></div>";
  html += "<div class=\"settingsName\">BLE Enabled:<input type=\"checkbox\" id=\"bleenabled\" name=\"bleenabled\" " + bleEnabled + "></div>";
  html += "</div></td></tr></table></form>";
  return html;
}

String Footer(void) {
  return "<p>&nbsp;</p></td></tr></table></body></html>";
}


}  //namespace hrHTML
