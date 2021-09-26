#include "ds3231.h"

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


void ds3231::update_rtc(time_data * new_time)
{
    
}


void ds3231::read_rtc(time_data * read_time)
{

}
