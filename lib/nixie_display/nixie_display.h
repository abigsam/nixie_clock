#ifndef __NIXIE_DISPLAY_H
#define __NIXIE_DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "time_data.h"
// #include "nixie_clock_defines.h"

class nixie_display
{
private:
    /* data */
public:
    nixie_display(/* args */);
    ~nixie_display();
    // void init();
    void clear();
    void display(time_data *curr_time, uint8_t point_mask);
};




#endif //__NIXIE_DISPLAY_H