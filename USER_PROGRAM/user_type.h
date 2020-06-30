#ifndef _USER_TYPE_H_
#define _USER_TYPE_H_

#define Version "03.00.01"
#define uchar unsigned char
#define uint unsigned int
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

typedef struct
{
    unsigned char *cmd;
    unsigned char *bkHead;
    unsigned char *bkKey;
} cmd_map_st;
typedef enum
{
    BLE_PPP,
    BLE_SETBR,
    BLE_GETNAME,
    BLE_SETNAME,
    BLE_GETADDR,
    BLE_SETADDR,
    BLE_GETSTATUS,
    BLE_SETADVINT,
    BLE_SETCONNINT,
    BLE_DISCONNECT,
    BLE_RESTART,
    BLE_GETPARA,
    BLE_EXIT,
    BLE_SHUTDOWN,
    BLE_RESTORE,
    BLE_GETINFO,
    BLE_HELP,
    BLE_TRANSMISSION,
} ble_cmd_t;
typedef enum
{
    LEDON   = 0,
    LED_ON  = 0,
    LED_OFF = 1,
    LEDOFF  = !LEDON,
} ledState_t;
enum
{
    LEDNUM00 = 0,
    LEDNUM01 = 1,
    LEDNUM02 = 2,
    LEDNUM03 = 3,
    LEDNUM04 = 4,
    LEDNUM05 = 5,
    LEDNUM06 = 6,
    LEDNUM07 = 7,
    LEDNUM08 = 8,
    LEDNUM09 = 9,
    LEDNUM10 = 10,
    LEDNUM11 = 11,
    LEDNUM12 = 12,
    LEDNUM13 = 13,
};
enum
{
    STATE_LED_OFF,
    STATE_LED_ON,
    STATE_LED_FLASH_2HZ,
    STATE_LED_FLASH_1HZ,
    STATE_LED_FLASH_0_5HZ,
    STATE_LED_FLASH_1_T,  //闪烁一下
    STATE_LED_FLASH_2_T,  //闪烁两下
    STATE_LED_FLASH_3_T,  //闪烁三下
};

enum
{
    STATE_BEEP_OFF,
    STATE_BEEP_ON,
    STATE_BEEP_FLASH,
};

enum
{
    UART_IDEL,
    UART_TRANSFERING,
    UART_,
    UART_STATE_IDEL,
    //    UART_STATE_IDEL,
};

enum
{
    CMD_IDEL     = 0,
    CMD_KEY      = 1,
    CMD_LED      = 2,
    CMD_REG_UP   = 3,
    CMD_REG_DOWN = 4,
    CMD_AUX_DIAI = 5,
    CMD_AUX_DO   = 6,
    CMD_PARA     = 7,
};

#endif
