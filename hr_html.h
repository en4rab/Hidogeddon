#ifndef HR_HTML_H
#define HR_HTML_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "hr_settings.h"
#include "hr_util.h"

namespace hrHTML {

String Header(bool autoRefresh, int requestedPage);
String Menu(bool autoRefresh);
String VersionAndBuild(void);
String LastCardData(void);
int TotalCardsInLog(void);
String CardData(bool autoRefresh, int requestedPage, bool showCloneAction);
String ConfirmMessage(String action, String message);
String SendCardNumber(int wiegandSelected);
String ViewSelectSpecialCard(String bin, int requestedPage);
String ViewSettings(String message);
String Footer(void);

}  //namespace hrHTML

#endif
