#include "gps_time.h"

#include "../../include/nixie_clock_defines.h"

// The TinyGPS++ object
// TinyGPSPlus gps;

// The serial connection to the GPS device
// SoftwareSerial sserial(GPS_RX, GPS_TX);

/**
 * @brief gps_time class constructor
 * 
 */
gps_time::gps_time(/* args */)
{
    _gps = new TinyGPSPlus;
    _sserial = new SoftwareSerial(GPS_RX, GPS_TX);
}


/**
 * @brief Decode and display received data from GPS 
 * 
 */
void gps_time::display_info()
{
    Serial.print(F("Location: ")); 
    if (_gps->location.isValid())
    {
        Serial.print(_gps->location.lat(), 6);
        Serial.print(F(","));
        Serial.print(_gps->location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (_gps->date.isValid())
    {
        Serial.print(_gps->date.month());
        Serial.print(F("/"));
        Serial.print(_gps->date.day());
        Serial.print(F("/"));
        Serial.print(_gps->date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (_gps->time.isValid())
    {
        if (_gps->time.hour() < 10) Serial.print(F("0"));
        Serial.print(_gps->time.hour());
        Serial.print(F(":"));
        if (_gps->time.minute() < 10) Serial.print(F("0"));
        Serial.print(_gps->time.minute());
        Serial.print(F(":"));
        if (_gps->time.second() < 10) Serial.print(F("0"));
        Serial.print(_gps->time.second());
        Serial.print(F("."));
        if (_gps->time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(_gps->time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}


/**
 * @brief Makes necessary actions to run
 * 
 */
void gps_time::begin()
{
    power(false);
    pinMode(EN_GPS_PIN, OUTPUT);
    power(true);
    delay(400);
    _sserial->begin(GPS_UART_SPEED);
}


/**
 * @brief Run test GPS
 * 
 */
void gps_time::test()
{
    while (_sserial->available() > 0)
    if (_gps->encode(_sserial->read())) {
        display_info();
    }
}


/**
 * @brief Control GPS power
 * 
 * @param state     True for enable, False for disabling power
 */
void gps_time::power(bool state)
{
    digitalWrite(EN_GPS_PIN, (state) ? HIGH : LOW);
}


/**
 * @brief 
 * 
 */
void gps_time::read_gps()
{
    if (_sserial->available() > 0) {
        _gps->encode(_sserial->read());
    }
}


/**
 * @brief Check if GPS location is avaliable
 * 
 * @return true     GPS location is avaliable/valid
 * @return false    GPS location is not avaliable/not valid
 */
bool gps_time::location_valid()
{
    return _gps->location.isValid();
}


/**
 * @brief Check if GPS time is avaliable
 * 
 * @return true     GPS time is avaliable/valid
 * @return false    GPS time is not avaliable/not valid
 */
bool gps_time::time_valid()
{
    return (_gps->date.isValid() && _gps->time.isValid());
}


/**
 * @brief Return latitude value
 * 
 * @return double   Latitude value
 */
double gps_time::get_lat()
{
    return (_gps->location.lat());
}


/**
 * @brief Return longitude value
 * 
 * @return double   Longitude value
 */
double gps_time::get_lng()
{
    return (_gps->location.lng());
}


/**
 * @brief Return GPS date month
 * 
 * @return uint8_t  Date month
 */
uint8_t gps_time::get_month()
{
    return (_gps->date.month());
}


/**
 * @brief Return GPS date day
 * 
 * @return uint8_t  Date day
 */
uint8_t gps_time::get_day()
{
    return (_gps->date.day());
}


/**
 * @brief Return GPS date year
 * 
 * @return uint16_t     Date year
 */
uint16_t gps_time::get_year()
{
    return (_gps->date.year());
}


/**
 * @brief Return GPS time hour
 * 
 * @return uint8_t  Time hour
 */
uint8_t gps_time::get_hour()
{
    return (_gps->time.hour());
}


/**
 * @brief Return GPS time minute
 * 
 * @return uint8_t  Time minute
 */
uint8_t gps_time::get_minute()
{
    return (_gps->time.minute());
}


/**
 * @brief Return GPS time second
 * 
 * @return uint8_t  Time second
 */
uint8_t gps_time::get_second()
{
    return (_gps->time.second());
}


/**
 * @brief Return GPS time centisecond
 * 
 * @return uint8_t  Time centisecond
 */
uint8_t gps_time::get_centisecond()
{
    return (_gps->time.centisecond());
}
