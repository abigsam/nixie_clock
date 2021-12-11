#include <Arduino.h>
#include "nixie_clock_bsp.h"
#include <arduino-timer.h>

#define ENTER_MODE          (20u)

//Create objects
auto timer = timer_create_default(); // create a timer with default settings

//Global variables
DateTime curr_time;
uint8_t update_time = 0u, update_decimal = 0u;
uint16_t cnt_mode = 0u;
bool decimal_state = false;

//Functions defines
void onAlarm();
bool blinkDecimal(void *);

//Startup setup ***********************************************************************************
void setup () {
    bsp_init(); //Init all
    bsp_led(true);
    // Making it so, that the alarm will trigger an interrupt
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), onAlarm, FALLING);
    update_time = 1u;
    //Blink decimal every 500 mS
    timer.every(500, blinkDecimal);
    //For GPS
    Serial.begin(9600);
    bsp_led(false);
}

//Main loop ***************************************************************************************
void loop () {

    if (update_time > 0u)
    {
        bsp_clr_rtc_alarms();
        bsp_get_current_time(curr_time);
        bsp_display_time(&curr_time);
        update_time = 0u;
    }
    else if (update_decimal > 0u)
    {
        decimal_state = !decimal_state;
        bsp_point(decimal_state);
        update_decimal = 0u;
    }

    timer.tick(); // tick the timer

    // gpst.test();

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
 * @brief RTC alarm interrupt
 */
void onAlarm()
{
    bsp_led(true);
    update_time++;
}


/**
 * @brief Timer for decimal blinking
 */
bool blinkDecimal(void *)
{
    update_decimal++;
    return true; // repeat? true
}