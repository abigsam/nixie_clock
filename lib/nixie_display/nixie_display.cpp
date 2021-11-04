#include "nixie_display.h"
#include <avr/pgmspace.h>

#define UPD_DELAY_MS            (10u)
#define BEGIN_TRANSACTION()     do {SPI.beginTransaction(SPISettings(SPI_CLOCK_HZ, MSBFIRST, SPI_MODE0));} while(0);
#define GET_REG_NUM(n)          ((n) / 8u)
#define GET_REG_MASK(n)         ((uint8_t)(1u << ((n) % 8u)))


const PROGMEM uint8_t MINUTES_ONES_SET[] = {
    //Register number  | Register mask
    GET_REG_NUM(MO_0),   GET_REG_MASK(MO_0),
    GET_REG_NUM(MO_1),   GET_REG_MASK(MO_1),
    GET_REG_NUM(MO_2),   GET_REG_MASK(MO_2),
    GET_REG_NUM(MO_3),   GET_REG_MASK(MO_3),
    GET_REG_NUM(MO_4),   GET_REG_MASK(MO_4),
    GET_REG_NUM(MO_5),   GET_REG_MASK(MO_5),
    GET_REG_NUM(MO_6),   GET_REG_MASK(MO_6),
    GET_REG_NUM(MO_7),   GET_REG_MASK(MO_7),
    GET_REG_NUM(MO_8),   GET_REG_MASK(MO_8),
    GET_REG_NUM(MO_9),   GET_REG_MASK(MO_9)
};

const PROGMEM uint8_t MINUTES_TENS_SET[] = {
    //Register number  | Register mask
    GET_REG_NUM(MT_0),   GET_REG_MASK(MT_0),
    GET_REG_NUM(MT_1),   GET_REG_MASK(MT_1),
    GET_REG_NUM(MT_2),   GET_REG_MASK(MT_2),
    GET_REG_NUM(MT_3),   GET_REG_MASK(MT_3),
    GET_REG_NUM(MT_4),   GET_REG_MASK(MT_4),
    GET_REG_NUM(MT_5),   GET_REG_MASK(MT_5),
    GET_REG_NUM(MT_6),   GET_REG_MASK(MT_6),
    GET_REG_NUM(MT_7),   GET_REG_MASK(MT_7),
    GET_REG_NUM(MT_8),   GET_REG_MASK(MT_8),
    GET_REG_NUM(MT_9),   GET_REG_MASK(MT_9)
};

const PROGMEM uint8_t HOURS_ONES_SET[] = {
    //Register number  | Register mask
    GET_REG_NUM(HO_0),   GET_REG_MASK(HO_0),
    GET_REG_NUM(HO_1),   GET_REG_MASK(HO_1),
    GET_REG_NUM(HO_2),   GET_REG_MASK(HO_2),
    GET_REG_NUM(HO_3),   GET_REG_MASK(HO_3),
    GET_REG_NUM(HO_4),   GET_REG_MASK(HO_4),
    GET_REG_NUM(HO_5),   GET_REG_MASK(HO_5),
    GET_REG_NUM(HO_6),   GET_REG_MASK(HO_6),
    GET_REG_NUM(HO_7),   GET_REG_MASK(HO_7),
    GET_REG_NUM(HO_8),   GET_REG_MASK(HO_8),
    GET_REG_NUM(HO_9),   GET_REG_MASK(HO_9)
};

const PROGMEM uint8_t HOURS_TENS_SET[] = {
    //Register number  | Register mask
    GET_REG_NUM(HT_0),   GET_REG_MASK(HT_0),
    GET_REG_NUM(HT_1),   GET_REG_MASK(HT_1),
    GET_REG_NUM(HT_2),   GET_REG_MASK(HT_2),
    GET_REG_NUM(HT_3),   GET_REG_MASK(HT_3),
    GET_REG_NUM(HT_4),   GET_REG_MASK(HT_4),
    GET_REG_NUM(HT_5),   GET_REG_MASK(HT_5),
    GET_REG_NUM(HT_6),   GET_REG_MASK(HT_6),
    GET_REG_NUM(HT_7),   GET_REG_MASK(HT_7),
    GET_REG_NUM(HT_8),   GET_REG_MASK(HT_8),
    GET_REG_NUM(HT_9),   GET_REG_MASK(HT_9)
};


nixie_display::nixie_display(/* args */)
{
    SPI.begin();
    memset(arr, (uint8_t)0u, REGISTERS_NUM);
    pinMode(SHIFT_REG_LOAD_PIN, OUTPUT);
    digitalWrite(SHIFT_REG_LOAD_PIN, LOW);
}

void nixie_display::write_display(uint8_t *ptr, uint8_t size)
{
    SPI.beginTransaction(SPISettings(SPI_CLOCK_HZ, MSBFIRST, SPI_MODE0));
    SPI.transfer(ptr, size);
    SPI.endTransaction();
    update_shift_reg();
}

void nixie_display::update_shift_reg()
{
    digitalWrite(SHIFT_REG_LOAD_PIN, HIGH);
    delay(UPD_DELAY_MS);
    digitalWrite(SHIFT_REG_LOAD_PIN, LOW);
}

void nixie_display::set_shift_reg(const uint8_t * PMBCD_PTR, uint8_t bcd_num)
{
    uint8_t reg_num = 0u, mask = 0u;
    reg_num = pgm_read_byte_near(PMBCD_PTR + bcd_num*2u);
    mask = pgm_read_byte_near(PMBCD_PTR + bcd_num*2u + 1u);
    arr[reg_num] = mask;
}

void nixie_display::decode_time(DateTime *time)
{
    uint8_t ones = 0u, tens = 0u;
    //Clear buffer
    memset(arr, (uint8_t)0u, REGISTERS_NUM);
    //Minutes
    ones = time->minute() % 10u;
    tens = time->minute() / 10u;
    set_shift_reg(MINUTES_ONES_SET, ones);
    set_shift_reg(MINUTES_TENS_SET, tens);
    //Hours
    ones = time->hour() % 10u;
    tens = time->hour() / 10u;
    set_shift_reg(HOURS_ONES_SET, ones);
    if (tens > 0u && (uint8_t)HIDE_HOUR_TENS_IF_ZERO > 0u)
        set_shift_reg(HOURS_TENS_SET, tens);
}

void nixie_display::decode_points(decimal_pnt_t point_mask)
{
    uint8_t reg_num = 0u, mask = 0u;

    if (POINT_DONT_CHANGE & point_mask)
        return; // Do nothing
    
    reg_num = GET_REG_NUM(MT_POINT);
    mask = GET_REG_MASK(MT_POINT);
    if (POINT_MINUTE_TENS & point_mask) {
        arr[reg_num] |= mask;
    } else {
        arr[reg_num] &= ~mask;
    }
    
    reg_num = GET_REG_NUM(HO_POINT);
    mask = GET_REG_MASK(HO_POINT);
    if (POINT_HOUR_ONES & point_mask) {
       arr[reg_num] |= mask;
    } else {
        arr[reg_num] &= ~mask;
    }
}

void nixie_display::display_points(decimal_pnt_t point_mask)
{
    decode_points(point_mask);
    write_display(arr, REGISTERS_NUM);
}

void nixie_display::clear()
{
    BEGIN_TRANSACTION();
    for(auto i = 0u; i < REGISTERS_NUM; i++) {
        SPI.transfer(0u);
    }
    SPI.endTransaction();
    update_shift_reg();
}

void nixie_display::display(DateTime *curr_time, uint8_t point_mask)
{
    decode_time(curr_time);
    if (!(POINT_DONT_CHANGE & point_mask))
        decode_points((decimal_pnt_t)point_mask);
    write_display(arr, REGISTERS_NUM);
}