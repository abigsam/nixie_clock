#include <Arduino.h>
#include "nixie_clock_bsp.h"
#include <arduino-timer.h>

#define ENTER_MODE          (20u)

DateTime curr_time;
auto timer = timer_create_default(); // create a timer with default settings

uint8_t update_time = 0u;
uint16_t cnt_mode = 0u;

void onAlarm();
bool blinkDecimal(void *);

void setup () {
    bsp_init();
    bsp_led(true);
    bsp_check_display();
    // Making it so, that the alarm will trigger an interrupt
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), onAlarm, FALLING);
    update_time = 1u;
    bsp_point(true);
    delay(200);
    bsp_led(false);
    //Blink decimal every 500 mS
    timer.every(5000, blinkDecimal);
}

void loop () {

    if (update_time > 0) {
        bsp_get_current_time(curr_time);
        bsp_display_time(&curr_time);
        update_time = 0u;
        bsp_led(false);
    }

    timer.tick(); // tick the timer

    // bsp_point(true);
    // delay(500u);
    // bsp_point(false);
    // delay(500u);

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

/**
 * @brief 
 * 
 */
void onAlarm()
{
    update_time++;
    bsp_led(true);
}


/**
 * @brief 
 * 
 */
bool blinkDecimal(void *)
{
    static bool decimal_state = false;
    bsp_point(decimal_state);
    decimal_state = !decimal_state;
    return true; // repeat? true
}