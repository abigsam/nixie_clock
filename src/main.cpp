#include <Arduino.h>
#include "RTClib.h"

RTC_DS3231 rtc;
DateTime curr_time;

void setup () {
    if (! rtc.begin()) {
        while(1);
    }

    if (rtc.lostPower()) {
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop () {
    curr_time = rtc.now();

    
    delay(3000);
}