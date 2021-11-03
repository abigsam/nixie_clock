#include "nixie_display.h"


nixie_display::nixie_display(/* args */)
{
    SPI.begin();
}

nixie_display::~nixie_display()
{
}

// void nixie_display::init()
// {

// }

void nixie_display::clear()
{
    
}

void nixie_display::display(DateTime *curr_time, uint8_t point_mask)
{

}