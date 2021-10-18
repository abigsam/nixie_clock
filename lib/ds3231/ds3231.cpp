#include "ds3231.h"

#define GET_ONES(d)             ((d) & (uint8_t)0x0f )
#define GET_TENS(d)             (((d) >> 4) & (uint8_t)0x0f )


ds3231::ds3231(/* args */)
{
    _last_error = 0u;
    Wire.begin();
}

ds3231::~ds3231()
{
}



uint8_t ds3231::read_reg(uint8_t reg_addr)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(reg_addr);
    _last_error = Wire.endTransmission();
    if (_last_error != 0)
        return 0u;
    
    if (1u != Wire.requestFrom(DS3231_ADDRESS, 1u)) {
        _last_error = 4;
        return 0;
    }

    return Wire.read();
}

void ds3231::write_reg(uint8_t reg_addr, uint8_t reg_data)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(reg_addr);
    Wire.write(reg_data);
    _last_error = Wire.endTransmission();
}

void ds3231::read_arr(uint8_t start_addr, uint8_t regs_num, uint8_t *out_arr)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(start_addr);
    _last_error = Wire.endTransmission();
    if (_last_error != 0)
        return;
    
    if (regs_num != Wire.requestFrom(DS3231_ADDRESS, regs_num)) {
        _last_error = 4;
        return;
    }

    for(uint8_t i = 0u; i < regs_num; i++) {
        out_arr[i] = Wire.read();
    }
}


void ds3231::write_arr(uint8_t start_addr, uint8_t regs_num, uint8_t *in_arr)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(start_addr);

    Wire.write(in_arr, regs_num);
    _last_error = Wire.endTransmission();
}


void ds3231::update_rtc(const time_data * new_time)
{
    uint8_t warr[(DS3231_YEAR_REG - DS3231_SECONDS_REG)] = {0u};


    //combine

    // warr[DS3231_SECONDS_REG - DS3231_SECONDS_REG] = new_time->seconds.combine();

    uint8_t t = new_time->seconds.


    write_arr(DS3231_SECONDS_REG, (DS3231_YEAR_REG - DS3231_SECONDS_REG), warr);
}


void ds3231::read_rtc(time_data *read_time)
{
    uint8_t rarr[(DS3231_YEAR_REG - DS3231_SECONDS_REG)] = {0u};
    read_arr(DS3231_SECONDS_REG, (DS3231_YEAR_REG - DS3231_SECONDS_REG), rarr);
    if (_last_error) {
        //Error occured
        return;
    }
    //Seconds & minutes
    read_time->seconds.ones = GET_ONES( rarr[DS3231_SECONDS_REG - DS3231_SECONDS_REG] );
    read_time->seconds.tens = GET_TENS( rarr[DS3231_SECONDS_REG - DS3231_SECONDS_REG] );
    read_time->minutes.ones = GET_ONES( rarr[DS3231_MINUTES_REG - DS3231_SECONDS_REG] );
    read_time->minutes.tens = GET_TENS( rarr[DS3231_MINUTES_REG - DS3231_SECONDS_REG] );
    //Hours
    if (DS3231_HOURS_12H_MODE & rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG]) {
        //12h mode
        uint8_t h = GET_ONES( rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG] );
        if (DS3231_HOURS_10H_MASK & rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG]) {
            h += 10u;
        }
        if (DS3231_HOURS_AMPM_MASK & rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG]) {
            h += 12u;
        }
        read_time->hours.init(h);
    } else {
        //24h mode
        read_time->hours.ones = GET_ONES( rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG] );
        read_time->hours.tens = GET_TENS( rarr[DS3231_HOURS_REG - DS3231_SECONDS_REG] );
    }
    //Day (skip)
    //Date
    read_time->date.ones = GET_ONES( rarr[DS3231_DATE_REG - DS3231_SECONDS_REG] );
    read_time->date.tens = GET_TENS( rarr[DS3231_DATE_REG - DS3231_SECONDS_REG] );
    //Month (eleminate century bits)
    read_time->month.ones = GET_ONES( rarr[DS3231_MONTH_REG - DS3231_SECONDS_REG] );
    read_time->month.tens = ~(DS3231_MONTH_CENTURY_MASK >> 4) & GET_TENS( rarr[DS3231_MONTH_REG - DS3231_SECONDS_REG] );
    //Year
    read_time->year.ones = GET_ONES( rarr[DS3231_YEAR_REG - DS3231_SECONDS_REG] );
    read_time->year.tens = GET_TENS( rarr[DS3231_YEAR_REG - DS3231_SECONDS_REG] );

}
