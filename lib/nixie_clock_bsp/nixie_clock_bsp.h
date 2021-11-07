#ifndef __NIXIE_CLOCK_BSP_H
#define __NIXIE_CLOCK_BSP_H


#include "nixie_display.h"
#include "RTClib.h"

#define HIDE_HOUR_TENS_IF_ZERO      (1)


//Public functiones
void bsp_init();
void bsp_check_display();
void bsp_display_time(DateTime *curr_time);
void bsp_display_clear();
void bsp_get_current_time(DateTime *current_time);

//Privat functiones
static void decode_time(DateTime *time);
static void rtc_init();


#endif //__NIXIE_CLOCK_BSP_H