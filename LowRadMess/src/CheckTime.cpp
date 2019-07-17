#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
int Day = 0;
DateTime now;
void startRTC() {
if (! rtc.begin()) {
    Serial.println("RTC connection fail. Check wiring");
    while (1);
  }
  DateTime now = rtc.now();
}
void updateTime() {
  Day = now.day();
}
int checkDay() {
  return Day;
}
