#include "gps_time.h"
#include <SoftwareSerial.h>
#include "../../include/nixie_clock_defines.h"

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial sserial(GPS_RX, GPS_TX);

gps_time::gps_time(/* args */)
{
    
}


void gps_time::display_nfo()
{
    Serial.print(F("Location: ")); 
    if (gps.location.isValid())
    {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}


void gps_time::begin()
{
    digitalWrite(EN_GPS_PIN, LOW);
    pinMode(EN_GPS_PIN, OUTPUT);
    digitalWrite(EN_GPS_PIN, HIGH);
    delay(400);
    sserial.begin(GPS_UART_SPEED);
}


void gps_time::test()
{
    while (sserial.available() > 0)
    if (gps.encode(sserial.read()))
        display_nfo();
}
