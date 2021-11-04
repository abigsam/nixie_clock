#ifndef __NIXIE_CLOCK_DEFINES_H
#define __NIXIE_CLOCK_DEFINES_H

//Shift register parameters
#define REGISTERS_NUM       (6u)
#define REG_BITS            (REGISTERS_NUM * 8u)
#define REG5                (5u * 8u)
#define REG4                (4u * 8u)
#define REG3                (3u * 8u)
#define REG2                (2u * 8u)
#define REG1                (1u * 8u)
#define REG0                (0u * 8u)

//Minutes: ones
#define MO_1                (REG4 + 5u)
#define MO_2                (REG4 + 7u)
#define MO_3                (REG4 + 6u)
#define MO_4                (REG5 + 1u)
#define MO_5                (REG5 + 2u)
#define MO_6                (REG5 + 3u)
#define MO_7                (REG5 + 4u)
#define MO_8                (REG5 + 5u)
#define MO_9                (REG5 + 6u)
#define MO_0                (REG5 + 7u)
//Minutes: tens
#define MT_1                (REG3 + 6u)
#define MT_2                (REG3 + 7u)
#define MT_3                (REG3 + 4u)
#define MT_4                (REG3 + 3u)
#define MT_5                (REG3 + 2u)
#define MT_6                (REG3 + 1u)
#define MT_7                (REG4 + 3u)
#define MT_8                (REG4 + 2u)
#define MT_9                (REG4 + 1u)
#define MT_0                (REG4 + 4u)
#define MT_POINT            (REG3 + 5u)
//Hours: ones
#define HO_1                (REG1 + 5u)
#define HO_2                (REG1 + 7u)
#define HO_3                (REG1 + 6u)
#define HO_4                (REG2 + 3u)
#define HO_5                (REG2 + 4u)
#define HO_6                (REG2 + 5u)
#define HO_7                (REG2 + 6u)
#define HO_8                (REG2 + 7u)
#define HO_9                (REG2 + 1u)
#define HO_0                (REG2 + 2u)
#define HO_POINT            (REG1 + 4u)
//Hours: tens
#define HT_1                (REG0 + 1u)
#define HT_2                (REG0 + 2u)
#define HT_3                (REG0 + 3u)
#define HT_4                (REG0 + 4u)
#define HT_5                (REG0 + 5u)
#define HT_6                (REG0 + 6u)
#define HT_7                (REG0 + 7u)
#define HT_8                (REG1 + 3u)
#define HT_9                (REG1 + 2u)
#define HT_0                (REG1 + 1u)

//
#define RTC_INTERRUPT_PIN       2
#define SHIFT_REG_LOAD_PIN      (8)


#endif //__NIXIE_CLOCK_DEFINES_H