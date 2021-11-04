#ifndef __NIXIE_DISPLAY_H
#define __NIXIE_DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "RTClib.h"
#include "nixie_clock_defines.h"

#define SPI_CLOCK_HZ                ((uint32_t) 100000)
#define HIDE_HOUR_TENS_IF_ZERO      (1)

typedef enum {
    POINT_DONT_CHANGE = (1u << 2),
    POINT_MINUTE_TENS = (1u << 0),
    POINT_HOUR_ONES   = (1u << 1)
} decimal_pnt_t;

class nixie_display
{
private:
    void write_display(uint8_t *ptr, uint8_t size);
    void update_shift_reg();
    void decode_time(DateTime *time);
    void decode_points(decimal_pnt_t point_mask);
    //
    void set_shift_reg(const uint8_t * PMBCD_PTR, uint8_t num);
public:
    nixie_display(/* args */);
    void clear();
    void display(DateTime *curr_time, uint8_t point_mask = POINT_DONT_CHANGE);
    void display_points(decimal_pnt_t point_mask);
private:
    uint8_t arr [REGISTERS_NUM];
};




#endif //__NIXIE_DISPLAY_H