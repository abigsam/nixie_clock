#include "nixie_clock_bsp.h"
#include "../../include/nixie_clock_defines.h"


nixie_display ndisplay;
RTC_DS3231 rtc;

//Privat functiones
static void decode_time(DateTime *time);
static void rtc_init();


/**
 * @brief Init all clock periphery
 * 
 */
void bsp_init()
{
    ndisplay.clear();
    rtc_init();
    //Init pins
    pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
    pinMode(BOARD_LED_PIN, OUTPUT);
    pinMode(EN_GPS_PIN, OUTPUT);
    pinMode(BTN_MINUS_PIN, INPUT);
    pinMode(BTN_PLUS_PIN, INPUT);
    pinMode(BTN_MODE_PIN, INPUT);
    //Default state
    bsp_led(false);
    bsp_gps_power(false);
}

/**
 * @brief Check nixie display, enabling all digits in turn
 * 
 */
void bsp_check_display()
{
    ndisplay.clear();
    ndisplay.set_point((POINT_MINUTE_TENS | POINT_MINUTE_TENS));
    for (auto i = 0u; i < 10u; i++) {
        for (auto n = 0u; n < 4u; n++)
            ndisplay.set_char(n, (char)('0' + i));
        ndisplay.display_update();
        delay(600u);
    }
    ndisplay.clear();
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
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);
    // turn off alarm 1
    rtc.disableAlarm(1);
    //Alarm once per minute (whenever seconds are 00)
    rtc.setAlarm2(DateTime(0u,0u,0u), DS3231_A2_PerMinute);
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
void bsp_get_current_time(DateTime *current_time)
{
    *current_time = rtc.now();
}

void bsp_led(bool en)
{
    digitalWrite(BOARD_LED_PIN, (en ? HIGH : LOW));
}

void bsp_gps_power(bool en)
{
    digitalWrite(EN_GPS_PIN, (en ? HIGH : LOW));
}

bool bsp_read_btn(bsp_btn_t btn)
{
    return (digitalRead((uint8_t)btn) == 0 ? true : false);
}