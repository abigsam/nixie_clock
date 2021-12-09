#include "nixie_display.h"
#include <avr/pgmspace.h>

#define UPD_DELAY_MS            (10u)
#define BEGIN_TRANSACTION()     do {SPI.beginTransaction(SPISettings(SPI_CLOCK_HZ, MSBFIRST, SPI_MODE0));} while(0);
#define GET_REG_NUM(n)          (REGISTERS_NUM - 1u - ((n) / 8u))
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


/**
 * @brief Nixie display class constructor
 * 
 */
nixie_display::nixie_display(/* args */)
{
    SPI.begin();
    clr_buffer();
    digitalWrite(SHIFT_REG_LOAD_PIN, LOW);
    pinMode(SHIFT_REG_LOAD_PIN, OUTPUT);
}

/**
 * @brief Send update strobe to the external shift register
 * 
 */
void nixie_display::update_shift_reg()
{
    digitalWrite(SHIFT_REG_LOAD_PIN, HIGH);
    delay(UPD_DELAY_MS);
    digitalWrite(SHIFT_REG_LOAD_PIN, LOW);
}

/**
 * @brief Write display buffer to the external shift register
 * 
 * @param ptr   Pointer to the buffer
 * @param size  Number of digits
 */
void nixie_display::write_display(uint8_t *ptr, uint8_t size)
{
    SPI.beginTransaction(SPISettings(SPI_CLOCK_HZ, MSBFIRST, SPI_MODE0));
    SPI.transfer(ptr, size);
    SPI.endTransaction();
    update_shift_reg();
}

/**
 * @brief Get convert table pointer in FLASH for specified display digit number
 * 
 * @param display_digit     Display digit number, should be in the range 0...3
 * @return uint8_t*         Pointer to the convert table in the FLASH
 */
uint8_t* nixie_display::get_table_ptr(uint8_t display_digit)
{
    if (3u == display_digit)
        return (uint8_t *)HOURS_TENS_SET;
    else if (2u == display_digit)
        return (uint8_t *)HOURS_ONES_SET;
    else if (1u == display_digit)
        return (uint8_t *)MINUTES_TENS_SET;
    else
        return (uint8_t *)MINUTES_ONES_SET;
}

/**
 * @brief Clear display digit
 * 
 * @param digit_num     Display digit number that need to clear
 * @return uint8_t*     Pointer to the convert table in the FLASH
 */
uint8_t* nixie_display::clr_display_buffer(uint8_t digit_num)
{
    uint8_t *ptr = get_table_ptr(digit_num);
    uint8_t reg_num = 0u, mask = 0u;
    for(auto i = 0u; i < 10u; i++) {
        reg_num = pgm_read_byte_near(ptr + i*2u);
        mask = pgm_read_byte_near(ptr + i*2u + 1u);
        arr[reg_num] &= ~mask;
    }
    return ptr;
}

/**
 * @brief Set display buffer with specified number
 * 
 * @param digit_num     Display digit number
 * @param value         Char need to be displayed; can be in range '0'...'9' or ' ' for clear
 * @return uint8_t*     Pointer to the convert table in the FLASH 
 */
uint8_t* nixie_display::set_display_buffer(uint8_t digit_num, char value)
{
    uint8_t reg_num = 0u, mask = 0u;
    uint8_t *ptr = clr_display_buffer(digit_num);
    if (value >= '0' || value <= '9') {
        reg_num = pgm_read_byte_near(ptr + (value - '0')*2u);
        mask = pgm_read_byte_near(ptr + (value - '0')*2u + 1u);
        arr[reg_num] |= mask;
    }
    return ptr;
}



/**
 * @brief Control decimal points and update display buffer
 * 
 * @param point_mask    Decimap points mask; can be one/ORed value POINT_OFF, POINT_MINUTE_TENS, POINT_HOUR_ONES, POINT_DONT_CHANGE
 */
void nixie_display::set_decimal_points(decimal_pnt_t point_mask)
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


void nixie_display::clr_buffer()
{
    memset(arr, (uint8_t)0u, REGISTERS_NUM);
}

/**
 * @brief Clear external shift register. Display buffer will not cleared
 * 
 */
void nixie_display::clear()
{
    BEGIN_TRANSACTION();
    for(auto i = 0u; i < REGISTERS_NUM; i++) {
        SPI.transfer(0u);
    }
    SPI.endTransaction();
    update_shift_reg();
    clr_buffer();
}

/**
 * @brief Set char to display. Updates only display beffer
 * 
 * @param val           Char to display; can be in range '0'...'9' or ' ' for clear sigit
 * @param digit_num     Display digit number, can be in range 0...3
 */
void nixie_display::set_char(uint8_t digit_num, char val)
{   
    set_display_buffer(digit_num, val);
}

/**
 * @brief Write buffer to the external shift register
 * 
 */
void nixie_display::display_update()
{
    write_display(arr, REGISTERS_NUM);
}
