#ifndef HR_SERVER_H
#define HR_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebSrv.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include "hr_settings.h"
#include "hr_util.h"
#include "hr_html.h"
#include "hr_ble.h"

namespace hrServer {

void ServerInit(void);
static void ServerTasks(void* p);

}  //namespace hrServer

#endif
