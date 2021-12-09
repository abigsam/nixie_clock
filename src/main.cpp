#include <Arduino.h>
#include "nixie_clock_bsp.h"

#define ENTER_MODE          (20u)

DateTime curr_time;

uint8_t update_time = 0u;
uint16_t cnt_mode = 0u;

void onAlarm();

void setup () {
    bsp_init();
    bsp_led(true);
    // bsp_check_display();
    // Making it so, that the alarm will trigger an interrupt
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), onAlarm, FALLING);
    update_time = 1u;
    delay(200);
    bsp_led(false);
}

void loop () {

    if (update_time > 0) {
        bsp_get_current_time(curr_time);
        bsp_display_time(&curr_time);
        update_time = 0u;
        bsp_led(false);
    }

    // bsp_point(true);
    delay(500u);
    // bsp_point(false);
    delay(500u);

    // if (bsp_read_btn(BTN_MODE)) {
    //     cnt_mode++;
    // }

    // if (cnt_mode > ENTER_MODE) {
    //     cnt_mode = 0u;
    //     bool is_changed = bsp_mode_set(curr_time);
    //     if (is_changed) {
    //         bsp_set_new_time(curr_time);
    //     } else {
    //         bsp_get_current_time(curr_time);
    //     }
    // }
}


void onAlarm() {
    update_time++;
    bsp_led(true);
}