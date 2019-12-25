#ifndef _USER_TYPE_H_
#define _USER_TYPE_H_

#define Version "03.00.01"

typedef struct
{
    unsigned char s0 : 2;
    unsigned char s1 : 2;
    unsigned char s2 : 2;
    unsigned char s3 : 2;
} _STATE_2bits;

typedef struct
{
    unsigned char s0 : 4;
    unsigned char s1 : 4;
} _STATE_4bits;

typedef union
{
    _STATE_2bits s2bits;
    _STATE_4bits s4bits;
    unsigned char byte;
} _USR_FLAGA_type;

enum
{
    LED_ON,
    LED_OFF,
};
enum
{
    STATE_LED_OFF,
    STATE_LED_ON,
    STATE_LED_FLASH_2HZ,
    STATE_LED_FLASH_1HZ,
    STATE_LED_FLASH_0_5HZ,
};

enum
{
    STATE_BEEP_OFF,
    STATE_BEEP_ON,
    STATE_BEEP_FLASH,
};

#endif