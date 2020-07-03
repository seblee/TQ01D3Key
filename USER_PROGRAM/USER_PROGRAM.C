// #define _CCV3_  // 定义C编译器版本.. --CCG 注释

#include "..\TKS_GLOBE_VARIES.H"
#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"

#include "user_data.h"

/*****key**************************/
volatile _TKS_FLAGA_type keyTrg[2];
volatile uchar k_count[2];

#define keyNUM 7
#define RESTAIN_TIMES 200  // 200 × 10ms = 2s
uchar keyBeepMask[2] = {0xff, 0xff};
#define KEYMASK keyBeepMask[0]
#define RESTAINMASK keyBeepMask[1]
uchar keyTime[keyNUM] = {0};
uchar keyData         = 0;
/*****beep**************************/
#define BEEP _pe3
uchar beepCount = 1;
/*****led**************************/
#define LED00 _pe1
#define LED01 _pa7
#define LED02 _pd6
#define LED03 _pa6
#define LED04 _pd2
#define LED05 _pc4
#define LED06 _pd1
#define LED07 _pe6
#define LED08 _pf5
#define LED09 _pf3
#define LED10 _pa5
#define LED11 _pe7
#define LED12 _pf2
#define LED13 _pa1

uchar ledFlashFast[14]    = {0};
volatile uchar flashCount = 0;

volatile _USR_FLAGA_type ledState[7];
static uint ledValue = 0;
/*************************************/
volatile _TKS_FLAGA_type bitFlag;
#define beepFlag bitFlag.bits.b0
#define beepON bitFlag.bits.b1
#define beepOFF bitFlag.bits.b2
#define flashFlag_0_5HZ bitFlag.bits.b4
#define flashFlag_1HZ bitFlag.bits.b5
#define flashFlag_2HZ bitFlag.bits.b6

/******************************************/
void ledSetState(uchar num, ledState_t state);
/******************************************/
//==============================================
//**********************************************
// INT0 Pin
//==============================================
void __attribute((interrupt(0x04))) Interrupt_INT0(void)
{
    GCC_NOP();
}
//==============================================
//**********************************************
// INT1 Pin
//==============================================
void __attribute((interrupt(0x08))) Interrupt_INT1(void)
{
    GCC_NOP();
}

//==============================================
//**********************************************
// M. Funct. 1
//==============================================
void __attribute((interrupt(0x18))) Interrupt_MF1(void)
{
    GCC_NOP();
}
//==============================================
//**********************************************
// A/D
//==============================================
void __attribute((interrupt(0x1C))) Interrupt_PTM1_AD(void)
{
    GCC_NOP();  //_nop();
}
//==============================================
//**********************************************
// Time Base 1
//==============================================
void __attribute((interrupt(0x28))) Interrupt_TB1(void)
{
    GCC_NOP();
}
//==============================================
//**********************************************
// M. Funct. 3
//==============================================
void __attribute((interrupt(0x2C))) Interrupt_MF3(void)
{
    GCC_NOP();
}
//==============================================
//**********************************************
//==============================================
void USER_PROGRAM_INITIAL()
{
    _lvdc = 0x00;

    _sledc = 0xff;
    /******Data****************/
    beepCount   = 1;
    KEYMASK     = 0xff;
    RESTAINMASK = 0xff;
}

//==============================================
//**********************************************
//==============================================

void USER_PROGRAM()
{
    uchar i;
    uchar keyRestain = 0;
    if (TKS_63MSF)
    {
        if (beepON)
        {
            beepFlag  = 1;
            BEEP      = 1;
            beepCount = 0;
        }
        else
        {
            if (beepFlag)
            {
                beepFlag = 0;
                BEEP     = 0;
            }
            else if (beepCount)
            {
                beepCount--;
                beepFlag = 1;
                BEEP     = 1;
            }
        }
    }

    if (SCAN_CYCLEF)
    {
        GET_KEY_BITMAP();
        keyData = ((DATA_BUF[1] & 0x0c) >> 2) | ((DATA_BUF[1] & 0x80) >> 5) | ((DATA_BUF[2] & 0x02) << 2) |
                  ((DATA_BUF[2] & 0x08) << 1) | ((DATA_BUF[3] & 0x0c) << 3);  //& 0x0f;

        keyTrg[0].byte = keyData & (keyData ^ k_count[0]);
        k_count[0]     = keyData;
        if (keyTrg[0].byte & KEYMASK)
        {
            beepCount++;
        }

        for (i = 0; i < keyNUM; i++)
        {
            if (k_count[0] & (1 << i))
            {
                if (keyTime[i] < RESTAIN_TIMES)
                    keyTime[i]++;
                else
                    keyRestain |= (1 << i);
            }
            else
            {
                // if (keyTime[i] > 0)  // short press
                //     Key_Up_Trg |= (1 << i);
                // else
                //     Key_Up_Trg &= (~(1 << i));

                keyTime[i] = 0;
                keyRestain &= (~(1 << i));
            }
        }
        keyTrg[1].byte = keyRestain & (keyRestain ^ k_count[1]);
        k_count[1]     = keyRestain;
        if (keyTrg[1].byte & RESTAINMASK)
        {
            beepCount++;
        }
    }
}

void USER_LED_INITIAL()
{
    /********LED**************/
    _pac &= 0b00011101;
    /********LED**************/
    _pcc &= 0b11101111;
    /********LED**************/
    _pdc &= 0b10111001;
    /********LED***BEEP***********/
    _pec &= 0b00110101;
    /********LED**************/
    _pfc &= 0b11010011;

    LED00 = LED_OFF;
    LED01 = LED_OFF;
    LED02 = LED_OFF;
    LED03 = LED_OFF;
    LED04 = LED_OFF;
    LED05 = LED_OFF;
    LED06 = LED_OFF;
    LED07 = LED_OFF;
    LED08 = LED_OFF;
    LED09 = LED_OFF;
    LED10 = LED_OFF;
    LED11 = LED_OFF;
    LED12 = LED_OFF;
    LED13 = LED_OFF;
}

void USER_LED()
{
    if (TKS_250MSF)
    {
        if (flashFlag_2HZ)
        {
            flashFlag_2HZ = 0;
        }
        else
        {
            flashFlag_2HZ = 1;
        }
    }
    if (TKS_500MSF)
    {
        if (flashFlag_1HZ)
        {
            flashFlag_1HZ = 0;
        }
        else
        {
            flashFlag_1HZ = 1;
        }
        if (flashFlag_1HZ)
        {
            if (flashFlag_0_5HZ)
            {
                flashFlag_0_5HZ = 0;
            }
            else
            {
                flashFlag_0_5HZ = 1;
            }
        }
    }

    /***************************/
    if (TKS_63MSF || TKS_250MSF || TKS_500MSF)
    {
        uchar i, value, State;
        uint ledValueTemp = ledValue;
        for (i = 0; i < LEDNUM; i++)
        {
            value = ((ledValueTemp >> i) & 1);
            if (i % 2 == 0)
            {
                State = ledState[i / 2].byte & 0x0f;
            }
            else
            {
                State = (ledState[i / 2].byte >> 4);
            }

            if (ledFlashFast[i] > 0)
            {
                if (State >= STATE_LED_FLASH_1_T)
                {
                    ledFlashFast[i] += 2 * (State - STATE_LED_FLASH_0_5HZ);
                    if (i % 2 == 0)
                    {
                        ledState[i / 2].byte &= 0xf0;
                    }
                    else
                    {
                        ledState[i / 2].byte &= 0x0f;
                    }
                }
                if (TKS_63MSF)
                {
                    ledFlashFast[i]--;
                    value = !((ledValueTemp >> i) & 1);
                }
            }
            else
            {
                switch (State)
                {
                    case STATE_LED_OFF:
                        value = LED_OFF;
                        break;
                    case STATE_LED_ON:
                        value = LED_ON;
                        break;
                    case STATE_LED_FLASH_2HZ:
                        if (TKS_250MSF)
                            value = flashFlag_2HZ;
                        break;
                    case STATE_LED_FLASH_1HZ:
                        if (TKS_500MSF)
                            value = flashFlag_1HZ;
                        break;
                    case STATE_LED_FLASH_0_5HZ:
                        if (TKS_500MSF)
                            value = flashFlag_0_5HZ;
                        break;
                    case STATE_LED_FLASH_1_T:  //闪烁一下
                    case STATE_LED_FLASH_2_T:  //闪烁两下
                    case STATE_LED_FLASH_3_T:  //闪烁三下
                        ledFlashFast[i] += 2 * (State - STATE_LED_FLASH_0_5HZ);
                        if (i % 2 == 0)
                        {
                            ledState[i / 2].byte &= 0xf0;
                        }
                        else
                        {
                            ledState[i / 2].byte &= 0x0f;
                        }
                        break;
                }
            }
            if (value)
            {
                ledValueTemp |= 1 << i;
            }
            else
            {
                ledValueTemp &= ~(1 << i);
            }
            if (ledValueTemp ^ ledValue)
            {
                if (ledValueTemp & (1 << i))
                {
                    ledSetState(i, LED_OFF);
                }
                else
                {
                    ledSetState(i, LED_ON);
                }
            }
            ledValue = ledValueTemp;
        }
    }
}

void ledSetState(uchar num, ledState_t state)
{
    switch (num)
    {
        case LEDNUM00:
            if (state == LEDON)
            {
                LED00 = LED_ON;
            }
            else
            {
                LED00 = LED_OFF;
            }
            break;
        case LEDNUM01:
            if (state == LEDON)
            {
                LED01 = LED_ON;
            }
            else
            {
                LED01 = LED_OFF;
            }
            break;
        case LEDNUM02:
            if (state == LEDON)
            {
                LED02 = LED_ON;
            }
            else
            {
                LED02 = LED_OFF;
            }
            break;
        case LEDNUM03:
            if (state == LEDON)
            {
                LED03 = LED_ON;
            }
            else
            {
                LED03 = LED_OFF;
            }
            break;
        case LEDNUM04:
            if (state == LEDON)
            {
                LED04 = LED_ON;
            }
            else
            {
                LED04 = LED_OFF;
            }
            break;
        case LEDNUM05:
            if (state == LEDON)
            {
                LED05 = LED_ON;
            }
            else
            {
                LED05 = LED_OFF;
            }
            break;
        case LEDNUM06:
            if (state == LEDON)
            {
                LED06 = LED_ON;
            }
            else
            {
                LED06 = LED_OFF;
            }
            break;
        case LEDNUM07:
            if (state == LEDON)
            {
                LED07 = LED_ON;
            }
            else
            {
                LED07 = LED_OFF;
            }
            break;
        case LEDNUM08:
            if (state == LEDON)
            {
                LED08 = LED_ON;
            }
            else
            {
                LED08 = LED_OFF;
            }
            break;
        case LEDNUM09:
            if (state == LEDON)
            {
                LED09 = LED_ON;
            }
            else
            {
                LED09 = LED_OFF;
            }
            break;
        case LEDNUM10:
            if (state == LEDON)
            {
                LED10 = LED_ON;
            }
            else
            {
                LED10 = LED_OFF;
            }
            break;
        case LEDNUM11:
            if (state == LEDON)
            {
                LED11 = LED_ON;
            }
            else
            {
                LED11 = LED_OFF;
            }
            break;
        case LEDNUM12:
            if (state == LEDON)
            {
                LED12 = LED_ON;
            }
            else
            {
                LED12 = LED_OFF;
            }
            break;
        case LEDNUM13:
            if (state == LEDON)
            {
                LED13 = LED_ON;
            }
            else
            {
                LED13 = LED_OFF;
            }
            break;
        default:
            break;
    }
}
