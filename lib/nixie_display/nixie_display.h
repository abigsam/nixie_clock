#ifndef __NIXIE_DISPLAY_H
#define __NIXIE_DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "RTClib.h"

class nixie_display
{
private:
    /* data */
public:
    nixie_display(/* args */);
    ~nixie_display();
    // void init();
    void clear();
    void display(DateTime *curr_time, uint8_t point_mask = 0u);
};




#endif //__NIXIE_DISPLAY_H