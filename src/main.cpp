#include <Arduino.h>
#include "nixie_clock_bsp.h"
#include <arduino-timer.h>

#define ENTER_MODE                  (20u)
#define CLOCK_DEFAULT_CONFIG        (clock_config_t{CONFIG_VALID_VALUE, (uint8_t)1u, (int8_t)+2})


//Create objects **********************************************************************************
auto timer = timer_create_default(); // create a timer with default settings


//Global variables ********************************************************************************
DateTime curr_time, gps_time;
uint8_t alarm_type = 0u;
uint16_t cnt_mode = 0u;
bool decimal_state = false, blink_decimal_en = false, irc_alarm_on = false, time_update = false;
uint32_t loop_cnt = 0u;
clock_config_t clock_config;


//Functions defines *******************************************************************************
void IRQ_rtc_alarm();
bool IRQ_blink_timer(void *);


//Startup setup ***********************************************************************************
void setup () {
    bsp_init(); //Init all
    bsp_led(true); //Configuration started
    //For debug
    Serial.begin(9600);
    //Add IRQ for alarm
    attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), IRQ_rtc_alarm, FALLING);
    //Configure time to blink decimal every 500 mS
    timer.every(500, IRQ_blink_timer);
    //Read clock configuration froom EEPROM
    if (!bsp_read_config(clock_config)) {
        clock_config = CLOCK_DEFAULT_CONFIG;
        bsp_update_coinfig(&clock_config);
    }
    // bsp_print_config(clock_config); //For debug
    //Configure internal variables
    irc_alarm_on = false;
    alarm_type = 0u;
    time_update = true; //Update time after startup
    blink_decimal_en = true;
    loop_cnt = 0u;
    //Configuration ended
    bsp_led(false);
}

//Main loop ***************************************************************************************
void loop () {

    //Timer for decimal point blinking
    timer.tick();

    //Read RTC to figure out what alarm is goes
    if (irc_alarm_on) {
        alarm_type |= bsp_check_alarm();
        irc_alarm_on = false;
    }

    //Update time on display
    if (time_update || (alarm_type & RTC_ALARM_TIME_UPDATE))
    {
        //Disable all flags
        time_update = false;
        alarm_type &= ~RTC_ALARM_TIME_UPDATE;
        //Read time from RTC and display it
        bsp_get_current_time(curr_time);
        bsp_display_time(&curr_time);
        //For debug
        Serial.print("Display updated ");
        Serial.print(curr_time.hour()); Serial.print(":");
        Serial.println(curr_time.minute());
    }

    //Update time from GPS
    if ((clock_config.gps_upd_en) && (0u == loop_cnt % 10000u) && (alarm_type & RTC_ALARM_GPS_UPDATE)) {
        bsp_led(true); //For debug
        if (bsp_gps_check_ready()) {
            alarm_type &= ~RTC_ALARM_GPS_UPDATE;
            gps_time = bsp_gps_get_time(clock_config.utc_offset);
            // Serial.println("GPS time is valid");
            // Serial.print("Data ");
            // Serial.print(gps_time.year(), DEC);   Serial.print("-");
            // Serial.print(gps_time.month(), DEC);  Serial.print("-");
            // Serial.print(gps_time.day(), DEC);    Serial.println(" ");
            // Serial.print("Time ");
            // Serial.print(gps_time.hour(), DEC);   Serial.print(":");
            // Serial.print(gps_time.minute(), DEC); Serial.print(":");
            // Serial.print(gps_time.second(), DEC); Serial.print("\n");
            bsp_set_new_time(gps_time);
            bsp_led(true); //For debug
        } else {
            // Serial.println("GPS is not ready");
            bsp_led(false); //For debug
        }
    }

    // if (0u == loop_cnt % 1000u) {
    //     bsp_gps_test();
    // }

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
    loop_cnt++;
}

/**
 * @brief RTC alarm interrupt
 */
void IRQ_rtc_alarm()
{
    irc_alarm_on = true;
}


/**
 * @brief Timer for decimal blinking
 */
bool IRQ_blink_timer(void *)
{
    if (blink_decimal_en) {
        decimal_state = !decimal_state;
        bsp_point(decimal_state);
    }
    return true; // repeat? true
}