#ifndef HR_HTML_H
#define HR_HTML_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "hr_settings.h"
#include "hr_util.h"
#include "hr_ble.h"

namespace hrHTML {

String DocType(void);
String AutoRefresh(String seconds, String page);
String Header(void);
String Logo(String link);
String Menu(bool autoRefresh);
String VersionAndBuild(void);
String BLEStatus(void);
String LastCardData(void);
String LoadingMessage(void);
String CardData(bool autoRefresh, int requestedPage, bool showCloneAction);
String ConfirmMessage(String action, String message);
String SendCardNumber(int wiegandSelected);
String ViewSelectSpecialCard(String bin, int requestedPage);
String ViewSettings(String message);
String Footer(void);

}  //namespace hrHTML

#endif
