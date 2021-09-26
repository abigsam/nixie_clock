#ifndef __DS3231_H
#define __DS3231_H

#include <Arduino.h>
#include <Wire.h>
#include "time_data.h"
// #include "nixie_clock_defines.h"

#define DS3231_ADDRESS              ((uint8_t) 0x68)
#define DS3231_REG_CONTROL          ((uint8_t) 0x0e)
#define DS3231_CONTROL_DEFAULT      ((uint8_t) 0x1c)


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

    void update_rtc(time_data * new_time);
    void read_rtc(time_data * read_time);
};





#endif //__DS3231_H