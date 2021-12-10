#ifndef __NIXIE_CLOCK_BSP_H
#define __NIXIE_CLOCK_BSP_H


#include "nixie_display.h"
#include "RTClib.h"

#define HIDE_HOUR_TENS_IF_ZERO      (1)
#define EXIT_FROM_MODE_100MS        (100u)
#define SWITCH_MODE_100MS           (400u)
#define DIGIT_BLINK_100MS           (400u)

typedef enum {
    BTN_MINUS = BTN_MINUS_PIN,
    BTN_PLUS = BTN_PLUS_PIN,
    BTN_MODE = BTN_MODE_PIN
} bsp_btn_t;

//Public functiones
void bsp_init();
void bsp_check_display();
void bsp_display_time(DateTime *curr_time);
void bsp_display_digit(uint16_t value);
void bsp_display_clear();
void bsp_get_current_time(DateTime &current_time);
void bsp_set_new_time(const DateTime &current_time);
void bsp_led(bool en);
void bsp_gps_power(bool en);
bool bsp_read_btn(bsp_btn_t btn);
bool bsp_mode_set(DateTime &current_time);
void bsp_point(bool enable);
void bsp_clr_rtc_alarms();

#endif //__NIXIE_CLOCK_BSP_H