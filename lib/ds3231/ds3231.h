#ifndef __DS3231_H
#define __DS3231_H

#include <Arduino.h>
#include <Wire.h>
#include "time_data.h"
// #include "nixie_clock_defines.h"

#define DS3231_ADDRESS              ((uint8_t) 0x68)
#define DS3231_REG_CONTROL          ((uint8_t) 0x0e)
#define DS3231_CONTROL_DEFAULT      ((uint8_t) 0x1c)
//
#define DS3231_SECONDS_REG          ((uint8_t) 0x00)
#define DS3231_MINUTES_REG          ((uint8_t) 0x01)
#define DS3231_HOURS_REG            ((uint8_t) 0x02)
#define DS3231_HOURS_12H_MODE       ((uint8_t) 0x40)
#define DS3231_HOURS_10H_MASK       ((uint8_t) 0x10)
#define DS3231_HOURS_AMPM_MASK      ((uint8_t) 0x20)
#define DS3231_DAY_REG              ((uint8_t) 0x03)
#define DS3231_DATE_REG             ((uint8_t) 0x04)
#define DS3231_MONTH_REG            ((uint8_t) 0x05)
#define DS3231_MONTH_CENTURY_MASK   ((uint8_t) 0x80)
#define DS3231_YEAR_REG             ((uint8_t) 0x06)


class ds3231
{
private:
    uint8_t _last_error;

private:
    uint8_t read_reg(uint8_t reg_addr);
    void write_reg(uint8_t reg_addr, uint8_t reg_data);

    void read_arr(uint8_t start_addr, uint8_t regs_num, uint8_t *out_arr);
    void write_arr(uint8_t start_addr, uint8_t regs_num, uint8_t *in_arr);

public:
    ds3231(/* args */);
    ~ds3231();

    void update_rtc(const time_data * new_time);
    void read_rtc(time_data * read_time);
};





#endif //__DS3231_H