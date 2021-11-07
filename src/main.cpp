#include <Arduino.h>
#include "nixie_clock_bsp.h"

DateTime curr_time;

uint8_t update_time = 0u;

void onAlarm();

void setup () {
    
    
    bsp_init();

    // Making it so, that the alarm will trigger an interrupt
    pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), onAlarm, FALLING);

    update_time = 1u;
}

void loop () {

    if (update_time > 0) {
        bsp_get_current_time(&curr_time);
        bsp_display_time(&curr_time);
        update_time = 0u;
    }
    // delay(3000);
}


void onAlarm() {
    update_time = 1u;
}