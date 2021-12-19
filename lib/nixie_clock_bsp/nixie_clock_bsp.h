#ifndef __NIXIE_CLOCK_BSP_H
#define __NIXIE_CLOCK_BSP_H


#include "nixie_display.h"
#include "RTClib.h"

#define HIDE_HOUR_TENS_IF_ZERO      (1)
#define EXIT_FROM_MODE_100MS        (100u)
#define SWITCH_MODE_100MS           (400u)
#define DIGIT_BLINK_100MS           (400u)
#define GPS_UPDATE_ALARM            ("2021-12-19T13:52:00")
#define CONFIG_VALID_VALUE          ((uint8_t) 0xAB)

typedef enum {
    BTN_MINUS = BTN_MINUS_PIN,
    BTN_PLUS = BTN_PLUS_PIN,
    BTN_MODE = BTN_MODE_PIN
} bsp_btn_t;

typedef enum {
    RTC_ALARM_GPS_UPDATE = (1u << 0),
    RTC_ALARM_TIME_UPDATE = (1u << 1)
} rtc_alarm_t;

typedef struct {
    uint8_t valid;              //Should be equal to CONFIG_VALID_VALUE
    uint8_t gps_upd_en;         //Enable or disable GPS update
    int8_t utc_offset;          //UTC offset for GPS update
} clock_config_t;

//Public functiones
void bsp_init();
//Work with display
void bsp_check_display();
void bsp_display_time(DateTime *curr_time);
void bsp_display_digit(uint16_t value);
void bsp_display_clear();
//Work with RTC
void bsp_get_current_time(DateTime &current_time);
void bsp_set_new_time(const DateTime &current_time);
void bsp_clr_rtc_alarms();
uint8_t bsp_check_alarm();
//Control periphery
void bsp_led(bool en);
void bsp_gps_power(bool en);
bool bsp_read_btn(bsp_btn_t btn);
bool bsp_mode_set(DateTime &current_time);
void bsp_point(bool enable);
//Work with GPS
bool bsp_gps_check_ready();
DateTime bsp_gps_get_time(int8_t utc_offset = 0);
void bsp_gps_test();
//EEPROM
bool bsp_read_config(clock_config_t &config);
void bsp_update_coinfig(const clock_config_t *config);
void bsp_print_config(const clock_config_t &config);

#endif //__NIXIE_CLOCK_BSP_H