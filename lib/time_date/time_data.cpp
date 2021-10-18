
#include "time_data.h"

time_data::time_data(const char* date, const char* time)
{
    //Clear all
    seconds.clear();
    minutes.clear();
    hours.clear();
    date.clear();
    month.clear();
    year.clear();
    // sample input: date = "Dec 06 2009", time = "12:34:56"
    year.tens = StringToUint8(date + 7);
    year.ones = StringToUint8(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0])
    {
    case 'J':
        if ( date[1] == 'a' )
            month.ones = 1u;
        else if ( date[2] == 'n' )
            month.ones = 6u;
        else
            month.ones = 7u;
        break;
    case 'F':
        month.ones = 2u;
        break;
    case 'A':
        month.ones = (date[1] == 'p') ? 4u : 8u;
        break;
    case 'M':
        month.ones = (date[2] == 'r') ? 3u : 5u;
        break;
    case 'S':
        month.ones = 9u;
        break;
    case 'O':
        month.ones = 0u;
        month.tens = 1u;
        break;
    case 'N':
        month.ones = 1u;
        month.tens = 1u;
        break;
    case 'D':
        month.ones = 2;
        month.tens = 1;
        break;
    }
    date.init(StringToUint8(date + 4));
    hours.init(StringToUint8(time));
    minutes.init(StringToUint8(time + 3));
    seconds.init(StringToUint8(time + 6));
}

time_data::~time_data()
{
}


uint8_t time_data::StringToUint8(const char* pString)
{
    uint8_t value = 0;

    // skip leading 0 and spaces
    while ('0' == *pString || *pString == ' ')
    {
        pString++;
    }

    // calculate number until we hit non-numeral char
    while ('0' <= *pString && *pString <= '9')
    {
        value *= 10;
        value += *pString - '0';
        pString++;
    }
    return value;
}


