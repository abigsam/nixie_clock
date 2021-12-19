#include "nixie_clock_bsp.h"
#include "../../include/nixie_clock_defines.h"
#include "gps_time.h"
#include <EEPROM.h>

#define CONFIG_EEPROM_START_ADDR        ((int) 0)
#define SECONDS_IN_HOUR                 ((int32_t)3600)


//Create private objects **************************************************************************
nixie_display ndisplay;
RTC_DS3231 rtc;
gps_time gpst(GPS_RX, GPS_TX);

typedef enum {
    SET_BCD_MINUTES = 0u,
    SET_BCD_HOURS
} display_bcd_t;


//Privat functions ********************************************************************************
static void decode_time(DateTime *time);
static void rtc_init();
static void display_bcd(display_bcd_t dspl, uint8_t value);
static void display_bcd_clr(display_bcd_t dspl);


/**
 * @brief Init all clock periphery
 * 
 */
void bsp_init()
{
    ndisplay.clear();
    rtc_init();
    //Init pins: inputs
    pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
    pinMode(BTN_MINUS_PIN, INPUT);
    pinMode(BTN_PLUS_PIN, INPUT);
    pinMode(BTN_MODE_PIN, INPUT);
    //Init pins: outputs
    digitalWrite(BOARD_LED_PIN, LOW);
    pinMode(BOARD_LED_PIN, OUTPUT);
    // digitalWrite(EN_GPS_PIN, LOW);
    pinMode(EN_GPS_PIN, OUTPUT);
    //Default state
    bsp_led(false);
    bsp_gps_power(true);
    //GPS
    delay(200);
    gpst.begin();
}

/**
 * @brief Check nixie display, enabling all digits in turn
 * 
 */
void bsp_check_display()
{
    bsp_led(true);
    ndisplay.clear();
    for (auto i = 0u; i < 10u; i++) {
        for (auto n = 0u; n < 4u; n++)
            ndisplay.set_char(n, (char)('0' + i));
        ndisplay.set_decimal_points(POINT_MINUTE_TENS, true);
        ndisplay.display_update();
        delay(600u);
    }
    ndisplay.clear();
    bsp_led(false);
}

/**
 * @brief Decode DateTime class value and update display buffer
 * 
 * @param time  DateTime class with time value
 */
static void decode_time(DateTime *time)
{
    uint8_t ones = 0u, tens = 0u;
    //Minutes
    ones = time->minute() % 10u;
    tens = time->minute() / 10u;
    ndisplay.set_char(MINUTES_ONES_DIGIT, (char)('0' + ones));
    ndisplay.set_char(MINUTES_TENS_DIGIT, (char)('0' + tens));
    //Hours
    ones = time->hour() % 10u;
    tens = time->hour() / 10u;
    ndisplay.set_char(HOURS_ONES_DIGIT, (char)('0' + ones));
    if (0u == tens && (uint8_t)HIDE_HOUR_TENS_IF_ZERO > 0u)
        ndisplay.set_char(HOURS_TENS_DIGIT, ' ');
    else
        ndisplay.set_char(HOURS_TENS_DIGIT, (char)('0' + tens));
}


/**
 * @brief Decode integer value to the decimals
 * 
 * @param value     Integer value in the range 0...9999
 */
static void decode_digit(uint16_t value)
{
    ndisplay.set_char(MINUTES_ONES_DIGIT,  (char)('0' + value%10u) );
    value /= 10u;
    ndisplay.set_char(MINUTES_TENS_DIGIT,  (char)('0' + value%10u) );
    value /= 10u;
    ndisplay.set_char(HOURS_ONES_DIGIT, (char)('0' + value%10u) );
    value /= 10u;
    ndisplay.set_char(HOURS_TENS_DIGIT, (char)('0' + value%10u) );
}


/**
 * @brief Initilize external RTC
 * 
 */
static void rtc_init()
{
    if (! rtc.begin()) {
        while(1);
    }

    rtc.disable32K();

    if (rtc.lostPower()) {
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    bsp_clr_rtc_alarms();
    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);
    //Alarm every GPS_UPDATE_ALARM for run GPS update
    // rtc.disableAlarm(1);
    rtc.setAlarm1(DateTime(GPS_UPDATE_ALARM), DS3231_A1_Hour);
    //Alarm once per minute (whenever seconds are 00)
    rtc.setAlarm2(DateTime(0u,0u,0u), DS3231_A2_PerMinute);
}

/**
 * @brief Display BCDs on display
 * 
 * @param dspl      Digitals, can be one of the SET_BCD_MINUTES, SET_BCD_HOURS
 * @param value     Integer value in range 0..99
 */
static void display_bcd(display_bcd_t dspl, uint8_t value)
{
    if (SET_BCD_MINUTES == dspl) {
        ndisplay.set_char(0u, (char)('0' + value%10u));
        ndisplay.set_char(1u, (char)('0' + value/10u));
    } else if (SET_BCD_HOURS == dspl) {
        ndisplay.set_char(2u, (char)('0' + value%10u));
        ndisplay.set_char(3u, (char)('0' + value/10u));
    }
    ndisplay.display_update();
}

/**
 * @brief Clear display
 * 
 * @param dspl      Digitals, can be one of the SET_BCD_MINUTES, SET_BCD_HOURS
 */
static void display_bcd_clr(display_bcd_t dspl)
{
    if (SET_BCD_MINUTES == dspl) {
        ndisplay.set_char(0u, (char)(' '));
        ndisplay.set_char(1u, (char)(' '));
    } else if (SET_BCD_HOURS == dspl) {
        ndisplay.set_char(2u, (char)(' '));
        ndisplay.set_char(3u, (char)(' '));
    }
    ndisplay.display_update();
}

/**
 * @brief Display time
 * 
 * @param curr_time     Time to display
 */
void bsp_display_time(DateTime *curr_time)
{
    decode_time(curr_time);
    ndisplay.display_update();
}

/**
 * @brief Display "raw" digit in the range 0...9999
 * 
 * @param value     Digit value in the range 0...9999
 */
void bsp_display_digit(uint16_t value)
{
    decode_digit(value);
    ndisplay.display_update();
}


/**
 * @brief Clear display
 * 
 */
void bsp_display_clear()
{
    ndisplay.clear();
}

/**
 * @brief Read RTC for current time
 * 
 * @param current_time  Pointer where current time value should be stored
 */
void bsp_get_current_time(DateTime &current_time)
{
    current_time = rtc.now();
}

/**
 * @brief Write to RTC new value
 * 
 * @param current_time  Pointer where current time value should be stored
 */
void bsp_set_new_time(const DateTime &current_time)
{
    rtc.adjust(current_time);
}


/**
 * @brief Control BCD LED
 * 
 * @param en    True for enable on-board LED
 */
void bsp_led(bool en)
{
    digitalWrite(BOARD_LED_PIN, (en ? HIGH : LOW));
}


/**
 * @brief Control GPS power
 * 
 * @param en    True for enable GPS power
 */
void bsp_gps_power(bool en)
{
    digitalWrite(EN_GPS_PIN, (en ? HIGH : LOW));
}


/**
 * @brief Read on-board button state
 * 
 * @param btn       Button type, can be one of the BTN_MINUS, BTN_PLUS, BTN_MODE
 * @return true     Choosen button is pressed
 * @return false    Choosen button is unpressed
 */
bool bsp_read_btn(bsp_btn_t btn)
{
    return (digitalRead((uint8_t)btn) == 0 ? true : false);
}


/**
 * @brief Change set mode for clock
 * 
 * @param current_time      Return changed time
 * @return true             Time has been changed by user
 * @return false            Time has not been changed by user
 */
bool bsp_mode_set(DateTime &current_time)
{
    bool active_loop = true, time_changed = false, digit_en = true;
    uint8_t state = 0u, time_cnt = 0u, mode_time_cnt = 0u, blink_cnt = 0u;
    uint8_t minutes = 0u, hours = 0u;
    minutes = current_time.minute();
    hours = current_time.hour();
    // DateTime new_time(current_time);
    do {
        if (bsp_read_btn(BTN_PLUS) || bsp_read_btn(BTN_MINUS)) {
            time_changed = true;
            time_cnt = 0u;
            mode_time_cnt = 0u;
        }
        
        blink_cnt++;
        if (blink_cnt > DIGIT_BLINK_100MS) {
            blink_cnt = 0u;
            digit_en = !digit_en;
        }

        if (0u == state) {
            //*************************************************************************************
            if (bsp_read_btn(BTN_PLUS)) {
                if (59u == minutes)
                    minutes = 0u;
                else
                    minutes++;
            } else if (bsp_read_btn(BTN_MINUS)) {
                if (0u == minutes)
                    minutes = 59u;
                else
                    minutes--;
            } else if (bsp_read_btn(BTN_MODE)) {
                mode_time_cnt++;
            }
            if (mode_time_cnt > SWITCH_MODE_100MS) {
                state = 1u;
                mode_time_cnt = 0u;
            }
            if (digit_en)
                display_bcd(SET_BCD_MINUTES, minutes);
            else
                display_bcd_clr(SET_BCD_MINUTES);
        } else if (1u == state) {
            //*************************************************************************************
            if (bsp_read_btn(BTN_PLUS)) {
                if (23u == hours)
                    hours = 0u;
                else
                    hours++;
            } else if (bsp_read_btn(BTN_MINUS)) {
                if (0u == hours)
                    hours = 23u;
                else
                    hours--;
            } else if (bsp_read_btn(BTN_MODE)) {
                mode_time_cnt++;
            }
            if (mode_time_cnt > SWITCH_MODE_100MS) {
                state = 2u;
                mode_time_cnt = 0u;
            }
            if (digit_en)
                display_bcd(SET_BCD_HOURS, hours);
            else
                display_bcd_clr(SET_BCD_HOURS);
        } else if (2u == state) {
            //*************************************************************************************
            active_loop = false;
        }
        delay(100u);
        time_cnt++;
        if (time_cnt > EXIT_FROM_MODE_100MS) {
            active_loop = false;
        }
    } while(active_loop);

    if (time_changed) {
        current_time = DateTime(current_time.year(), current_time.month(), current_time.day(), hours, minutes, 0u);
    }

    return time_changed;
}


/**
 * @brief Control display point
 * 
 * @param enable    True for enable, False for disable
 */
void bsp_point(bool enable)
{
    ndisplay.set_decimal_points(POINT_MINUTE_TENS, enable);
    ndisplay.display_update();
}


/**
 * @brief Clear RTC alarms
 * 
 */
void bsp_clr_rtc_alarms()
{
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
}


/**
 * @brief Check what alarm is goes
 * 
 * @return rtc_alarm_t      Alarm type, on of the or ORed value RTC_ALARM_GPS_UPDATE, RTC_ALARM_TIME_UPDATE
 */
uint8_t bsp_check_alarm()
{
    uint8_t result = 0u;
    if (rtc.alarmFired(1)) {
        result |= RTC_ALARM_GPS_UPDATE;
        rtc.clearAlarm(1);
    }
    if (rtc.alarmFired(2)) {
        result |= RTC_ALARM_TIME_UPDATE;
        rtc.clearAlarm(2);
    }
    return result;
}


/**
 * @brief Check if GPS has valid date/time date
 * 
 * @return true     If GPS has valid date/time
 * @return false    If GPS does not have valid date/time
 */
bool bsp_gps_check_ready()
{
    if (gpst.read_gps()) {
        return (gpst.time_valid());
    } else {
        return false;
    }
}


/**
 * @brief Return GPS date/time as DataTime object
 * 
 * @return DateTime     GPS date and time
 */
DateTime bsp_gps_get_time(int8_t utc_offset)
{
    DateTime gps_time = DateTime(gpst.get_year(),
                                 gpst.get_month(),
                                 gpst.get_day(),
                                 gpst.get_hour(),
                                 gpst.get_minute(),
                                 gpst.get_second());
    if (utc_offset != 0) {
        gps_time = gps_time + TimeSpan((int32_t)utc_offset * SECONDS_IN_HOUR);
    }
    return gps_time;
}


/**
 * @brief For testing GPS
 * 
 */
void bsp_gps_test()
{
    gpst.test();
}


/**
 * @brief Read clock configuration from EEPROM
 * 
 * @param config    Reference to external RAM variable
 * @return true     c
 * @return false    c
 */
bool bsp_read_config(clock_config_t &config)
{
    EEPROM.get(CONFIG_EEPROM_START_ADDR, config);
    return (CONFIG_VALID_VALUE == config.valid);
}


/**
 * @brief Update clock configuration to the EEPROM
 * 
 * @param config    New configuration value
 */
void bsp_update_coinfig(const clock_config_t *config)
{
    uint8_t *ptr = (uint8_t *) config;
    for (auto i = 0ULL; i < sizeof(clock_config_t); i++) {
        EEPROM.update((CONFIG_EEPROM_START_ADDR + 1), *(ptr + i));
    }
}


void bsp_print_config(const clock_config_t &config)
{
    Serial.println("Configuration:");
    Serial.print(" valid = 0x");
    Serial.println(config.valid, HEX);
    Serial.print(" gps_upd_en = ");
    Serial.println(config.gps_upd_en, DEC);
    Serial.print(" utc_offset = ");
    Serial.println(config.utc_offset, DEC);
}