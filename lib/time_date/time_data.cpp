
#include "time_data.h"

time_data::time_data(const char* date, const char* time)
{
    //Clear all
    _seconds.clear();
    _minutes.clear();
    _hours.clear();
    _date.clear();
    _month.clear();
    _year.clear();
    // sample input: date = "Dec 06 2009", time = "12:34:56"
    _year.tens = StringToUint8(date + 7);
    _year.ones = StringToUint8(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0])
    {
    case 'J':
        if ( date[1] == 'a' )
            _month.ones = 1u;
        else if ( date[2] == 'n' )
            _month.ones = 6u;
        else
            _month.ones = 7u;
        break;
    case 'F':
        _month.ones = 2u;
        break;
    case 'A':
        _month.ones = (date[1] == 'p') ? 4u : 8u;
        break;
    case 'M':
        _month.ones = (date[2] == 'r') ? 3u : 5u;
        break;
    case 'S':
        _month.ones = 9u;
        break;
    case 'O':
        _month.ones = 0u;
        _month.tens = 1u;
        break;
    case 'N':
        _month.ones = 1u;
        _month.tens = 1u;
        break;
    case 'D':
        _month.ones = 2;
        _month.tens = 1;
        break;
    }
    _date.init(StringToUint8(date + 4));
    _hours.init(StringToUint8(time));
    _minutes.init(StringToUint8(time + 3));
    _seconds.init(StringToUint8(time + 6));
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


