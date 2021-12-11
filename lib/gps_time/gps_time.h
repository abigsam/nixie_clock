#ifndef __GPS_TIME_T
#define __GPS_TIME_T

#include <TinyGPS++.h>

#define GPS_UART_SPEED          ((uint32_t) 9600)

class gps_time
{
private:
    /* data */
    void display_nfo();
public:
    gps_time(/* args */);
    void begin();
    void test();
};


#endif //__GPS_TIME_T