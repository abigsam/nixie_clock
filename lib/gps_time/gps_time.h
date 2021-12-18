#ifndef __GPS_TIME_T
#define __GPS_TIME_T

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define GPS_UART_SPEED          ((uint32_t) 9600)
#define GPS_POLL_DEFAULT        (5000u)

class gps_time
{
private: /* data */
    TinyGPSPlus *_gps;
    SoftwareSerial *_sserial;
public: /* methods */
    void display_info();
public:
    gps_time(uint8_t rx, uint8_t tx);
    void begin();
    void test();
    //
    bool read_gps(uint16_t poll_time = GPS_POLL_DEFAULT);
    bool location_valid();
    bool time_valid();
    //
    double get_lat();
    double get_lng();
    //
    uint8_t get_month();
    uint8_t get_day();
    uint16_t get_year();
    uint8_t get_hour();
    uint8_t get_minute();
    uint8_t get_second();
    uint8_t get_centisecond();
};


#endif //__GPS_TIME_T