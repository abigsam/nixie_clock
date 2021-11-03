#include <Arduino.h>
#include "RTClib.h"
#include "nixie_clock_defines.h"
#include "nixie_display.h"

RTC_DS3231 rtc;
DateTime curr_time;
nixie_display dspl;

uint8_t update_time = 0u;

void onAlarm();

void setup () {
    if (! rtc.begin()) {
        while(1);
    }

    rtc.disable32K();

    if (rtc.lostPower()) {
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Making it so, that the alarm will trigger an interrupt
    pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), onAlarm, FALLING);
    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);
    // turn off alarm 1
    rtc.disableAlarm(1);
    //Alarm once per minute (whenever seconds are 00)
    rtc.setAlarm2(DateTime(0u,0u,0u), DS3231_A2_PerMinute);
    
    update_time = 1u;
}

void loop () {

    if (update_time > 0) {
        curr_time = rtc.now();
        dspl.display(&curr_time);
        update_time = 0u;
    }
    // delay(3000);
}


void onAlarm() {
    update_time = 1u;
}