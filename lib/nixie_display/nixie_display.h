#ifndef __NIXIE_DISPLAY_H
#define __NIXIE_DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "../../include/nixie_clock_defines.h"

#define SPI_CLOCK_HZ                ((uint32_t) 100000)
#define MINUTES_ONES_DIGIT          (0u)
#define MINUTES_TENS_DIGIT          (1u)
#define HOURS_ONES_DIGIT            (2u)
#define HOURS_TENS_DIGIT            (3u)

typedef enum {
    POINT_MINUTE_TENS = 0u,
    POINT_HOUR_ONES,
    POINT_ALL
} decimal_pnt_t;

class nixie_display
{
private:
    void update_shift_reg();
    //
    uint8_t* get_table_ptr(uint8_t display_digit);
    uint8_t* clr_display_buffer(uint8_t digit_num);
    uint8_t* set_display_buffer(uint8_t digit_num, char value);
    //
    void clr_buffer();
    void clr_shift_reg();
public:
    nixie_display(/* args */);
    void clear();
    //Write any digit
    void set_char(uint8_t digit_num, char val);
    void set_decimal_points(decimal_pnt_t displ_point, bool state);
    void display_update();
    
private:
    uint8_t arr [REGISTERS_NUM];
};




#endif //__NIXIE_DISPLAY_H