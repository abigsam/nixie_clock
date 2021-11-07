#ifndef __NIXIE_DISPLAY_H
#define __NIXIE_DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "RTClib.h"
#include "../../include/nixie_clock_defines.h"

#define SPI_CLOCK_HZ                ((uint32_t) 100000)
#define HIDE_HOUR_TENS_IF_ZERO      (1)

typedef enum {
    POINT_OFF         = 0u,
    POINT_MINUTE_TENS = (1u << 0),
    POINT_HOUR_ONES   = (1u << 1),
    POINT_DONT_CHANGE = (1u << 2)
} decimal_pnt_t;

class nixie_display
{
private:
    void update_shift_reg();
    void write_display(uint8_t *ptr, uint8_t size);
    //
    uint8_t* get_table_ptr(uint8_t display_digit);
    uint8_t* clr_display_buffer(uint8_t digit_num);
    uint8_t* set_display_buffer(uint8_t digit_num, char value);
    //
    void decode_time(DateTime *time);
    void set_decimal_points(decimal_pnt_t point_mask);
public:
    nixie_display(/* args */);
    void clear();
    //Write time
    void display(DateTime *curr_time, uint8_t point_mask = POINT_DONT_CHANGE);
    //Write any digit
    void set_char(uint8_t digit_num, char val);
    void set_point(decimal_pnt_t point_mask);
    void display_update();
    
private:
    uint8_t arr [REGISTERS_NUM];
};




#endif //__NIXIE_DISPLAY_H