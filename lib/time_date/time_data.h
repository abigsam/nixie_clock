#ifndef __TIME_DATA_H
#define __TIME_DATA_H

#include <Arduino.h>

typedef struct bcd_t
{
    uint8_t tens;
    uint8_t ones;
    void init(uint8_t init_val) {
        this->tens = init_val / 10u;
        this->ones = init_val % 10u;
    }
    void clear() {
        this->tens = 0u;
        this->ones = 0u;
    }
    uint8_t combine() {
        return ((this->ones & 0x0fu) | ((this->tens << 4) & 0xf0u));
    }
};


class time_data
{
public:
    bcd_t seconds;
    bcd_t minutes;
    bcd_t hours;
    bcd_t date;
    bcd_t month;
    bcd_t year;

private:
    uint8_t StringToUint8(const char* pString);

public:
    time_data(const char* date, const char* time);
    ~time_data();
    void add_minutes(uint8_t m);
    void add_hours(uint8_t m);
    void sub_minutes(uint8_t m);
    void sub_hours(uint8_t m);
};






#endif //__TIME_DATA_H