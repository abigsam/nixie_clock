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
};


class time_data
{
private:
    bcd_t _seconds;
    bcd_t _minutes;
    bcd_t _hours;
    bcd_t _date;
    bcd_t _month;
    bcd_t _year;

private:
    uint8_t StringToUint8(const char* pString);

public:
    time_data(const char* date, const char* time);
    ~time_data();
};






#endif //__TIME_DATA_H