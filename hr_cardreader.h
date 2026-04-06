#ifndef HR_CARDREADER_H
#define HR_CARDREADER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "hr_settings.h"
#include "hr_util.h"

namespace hrCardReader {

void CardReaderInit(void);
void ParseCard(void);
static void OnCardD0Learn(void);
static void OnCardD1Learn(void);
static void OnCardD0(void);
static void OnCardD1(void);
static void ClearAuthData(void);
static void ClearParity(void);
static void WiegandLearn(void);
static bool IsFunctionCard(void);
static void SaveCardData();
static void AddAuthHeader(String header);
static void AddAuthItem(String key, String value, String separator);
static uint64_t BitOp(int i, uint64_t n);
static String GetAuth(uint64_t n);
static void ParseXbits(void);
static void Parse26bit(void);
static void Parse27bit(void);
static void Parse28bit(void);
static void Parse29bit(void);
static void Parse30bit(void);
static void Parse31bit(void);
static void Parse32bit(void);
static void Parse33bit(void);
static void Parse34bit(void);
static void Parse35bit(void);
static void Parse36bit(void);
static void Parse37bit(void);
static void Parse38bit(void);
static void Parse39bit(void);
static void Parse40bit(void);
static void Parse42bit(void);
static void Parse46bit(void);
static void Parse48bit(void);
static void Parse56bit(void);
static void Parse58bit(void);
static void Parse64bit(void);
static void Parse75bit(void);
static void Parse80bit(void);
static void Parse83bit(void);
static void Parse91bit(void);
static void Parse107bit(void);
static void Parse128bit(void);
static void Parse200bit(void);
static void Parse245bit(void);

}  //namespace hrCardReader

#endif
