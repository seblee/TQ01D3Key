// #define _CCV3_  // 定义C编译器版本.. --CCG 注释

#include "..\TKS_GLOBE_VARIES.H"
#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"

#include "user_data.h"

/*****key**************************/
volatile _TKS_FLAGA_type keyTrg[2];
volatile uchar k_count[2];

#define keyNUM 4
#define RESTAIN_TIMES 200  // 200 × 10ms = 2s
#define KEYMASK 0x07
#define RESTAINMASK 0x08
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

uchar ledFlashFast[8]     = {0};
volatile uchar flashCount = 0;

volatile _USR_FLAGA_type ledState[4];

/*************************************/
volatile _TKS_FLAGA_type bitFlag;
#define beepFlag bitFlag.bits.b0
#define beepON bitFlag.bits.b1
#define beepOFF bitFlag.bits.b2
#define flashFlag_0_5HZ bitFlag.bits.b4
#define flashFlag_1HZ bitFlag.bits.b5
#define flashFlag_2HZ bitFlag.bits.b6

/******************************************/

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
// M. Funct. 0
//==============================================
void __attribute((interrupt(0x14))) Interrupt_MF0(void)
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
    beepCount = 1;
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
        keyData = ((DATA_BUF[1] & 0x0c) >> 2) | ((DATA_BUF[1] & 0x80) >> 5) | ((DATA_BUF[2] & 0x02) << 3) |
                  ((DATA_BUF[2] & 0x08) << 2) | ((DATA_BUF[3] & 0x0c) << 4);  //& 0x0f;

        keyTrg[0].byte = keyData & (keyData ^ k_count[0]);
        k_count[0]     = keyData;

        if (keyTrg[0].byte)
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
        if (keyTrg[1].byte)
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
    _pdc &= 0b10111101;
    /********LED***BEEP***********/
    _pec &= 0b00110110;
    /********LED**************/
    _pec &= 0b11010011;

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
    /*******led0****************/
    if (ledFlashFast[0] > 0)
    {
        if (led00State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[0] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[0]--;
            LED00 = !LED00;
        }
    }
    else
    {
        switch (led00State)
        {
            case STATE_LED_OFF:
                LED00 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED00 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED00 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED00 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED00 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[0] += 4;
                led00State = 0;
                break;
        }
    }
    /*******led01****************/
    if (ledFlashFast[1] > 0)
    {
        if (led01State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[1] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[1]--;
            LED01 = !LED01;
        }
    }
    else
    {
        switch (led01State)
        {
            case STATE_LED_OFF:
                LED01 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED01 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED01 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED01 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED01 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[1] += 4;
                led01State = 0;
                break;
        }
    }
    /*******LED02****************/
    if (ledFlashFast[2] > 0)
    {
        if (led02State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[2] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[2]--;
            LED02 = !LED02;
        }
    }
    else
    {
        switch (led02State)
        {
            case STATE_LED_OFF:
                LED02 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED02 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED02 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED02 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED02 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[2] += 4;
                led02State = 0;
                break;
        }
    }
    /*******LED03****************/
    if (ledFlashFast[3] > 0)
    {
        if (led03State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[3] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[3]--;
            LED03 = !LED03;
        }
    }
    else
    {
        switch (led03State)
        {
            case STATE_LED_OFF:
                LED03 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED03 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED03 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED03 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED03 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[3] += 4;
                led03State = 0;
                break;
        }
    }

    /*******LED04****************/
    if (ledFlashFast[4] > 0)
    {
        if (led04State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[4] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[4]--;
            LED04 = !LED04;
        }
    }
    else
    {
        switch (led04State)
        {
            case STATE_LED_OFF:
                LED04 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED04 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED04 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED04 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED04 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[4] += 4;
                led04State = 0;
                break;
        }
    }
    /*******LED05****************/
    if (ledFlashFast[5] > 0)
    {
        if (led05State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[5] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[5]--;
            LED05 = !LED05;
        }
    }
    else
    {
        switch (led05State)
        {
            case STATE_LED_OFF:
                LED05 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED05 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED05 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED05 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED05 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[5] += 4;
                led05State = 0;
                break;
        }
    }
    /*******LED06****************/
    if (ledFlashFast[6] > 0)
    {
        if (led06State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[6] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[6]--;
            LED06 = !LED06;
        }
    }
    else
    {
        switch (led06State)
        {
            case STATE_LED_OFF:
                LED06 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED06 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED06 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED06 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED06 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[6] += 4;
                led06State = 0;
                break;
        }
    }
    /*******LED07****************/
    if (ledFlashFast[7] > 0)
    {
        if (led07State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[7] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[7]--;
            LED07 = !LED07;
        }
    }
    else
    {
        switch (led07State)
        {
            case STATE_LED_OFF:
                LED07 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED07 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED07 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED07 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED07 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[7] += 4;
                led07State = 0;
                break;
        }
    }

    // {
    //     if (led00State == STATE_LED_OFF)
    //     {
    //         LED00 = LED_OFF;
    //     }
    //     if (led01State == STATE_LED_OFF)
    //     {
    //         LED01 = LED_OFF;
    //     }
    //     if (led02State == STATE_LED_OFF)
    //     {
    //         LED02 = LED_OFF;
    //     }
    //     if (led03State == STATE_LED_OFF)
    //     {
    //         LED03 = LED_OFF;
    //     }

    //     if (led04State == STATE_LED_OFF)
    //     {
    //         LED04 = LED_OFF;
    //     }

    //     if (led05State == STATE_LED_OFF)
    //     {
    //         LED05 = LED_OFF;
    //     }
    //     if (led06State == STATE_LED_OFF)
    //     {
    //         LED06 = LED_OFF;
    //     }
    //     if (led07State == STATE_LED_OFF)
    //     {
    //         LED07 = LED_OFF;
    //     }

    //     if (led00State == STATE_LED_ON)
    //     {
    //         LED00 = LED_ON;
    //     }
    //     if (led01State == STATE_LED_ON)
    //     {
    //         LED01 = LED_ON;
    //     }
    //     if (led02State == STATE_LED_ON)
    //     {
    //         LED02 = LED_ON;
    //     }
    //     if (led03State == STATE_LED_ON)
    //     {
    //         LED03 = LED_ON;
    //     }

    //     if (led04State == STATE_LED_ON)
    //     {
    //         LED04 = LED_ON;
    //     }

    //     if (led05State == STATE_LED_ON)
    //     {
    //         LED05 = LED_ON;
    //     }
    //     if (led06State == STATE_LED_ON)
    //     {
    //         LED06 = LED_ON;
    //     }
    //     if (led07State == STATE_LED_ON)
    //     {
    //         LED07 = LED_ON;
    //     }
    // }

    // if (TKS_250MSF)
    // {
    //     if (flashFlag_2HZ)
    //     {
    //         flashFlag_2HZ = 0;
    //     }
    //     else
    //     {
    //         flashFlag_2HZ = 1;
    //     }
    //     if (led00State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED00 = flashFlag_2HZ;
    //     }
    //     if (led01State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED01 = flashFlag_2HZ;
    //     }
    //     if (led02State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED02 = flashFlag_2HZ;
    //     }
    //     if (led03State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED03 = flashFlag_2HZ;
    //     }

    //     if (led04State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED04 = flashFlag_2HZ;
    //     }

    //     if (led05State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED05 = flashFlag_2HZ;
    //     }
    //     if (led06State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED06 = flashFlag_2HZ;
    //     }
    //     if (led07State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED07 = flashFlag_2HZ;
    //     }
    // }

    // if (TKS_500MSF)
    // {
    //     if (flashFlag_1HZ)
    //     {
    //         flashFlag_1HZ = 0;
    //     }
    //     else
    //     {
    //         flashFlag_1HZ = 1;
    //     }
    //     if (led00State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED00 = flashFlag_1HZ;
    //     }
    //     if (led01State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED01 = flashFlag_1HZ;
    //     }
    //     if (led02State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED02 = flashFlag_1HZ;
    //     }
    //     if (led03State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED03 = flashFlag_1HZ;
    //     }
    //     if (led04State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED04 = flashFlag_1HZ;
    //     }
    //     if (led05State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED05 = flashFlag_1HZ;
    //     }
    //     if (led06State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED06 = flashFlag_1HZ;
    //     }
    //     if (led07State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED07 = flashFlag_1HZ;
    //     }

    //     if (flashFlag_1HZ)
    //     {
    //         if (flashFlag_0_5HZ)
    //         {
    //             flashFlag_0_5HZ = 0;
    //         }
    //         else
    //         {
    //             flashFlag_0_5HZ = 1;
    //         }
    //         if (led00State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED00 = flashFlag_0_5HZ;
    //         }
    //         if (led01State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED01 = flashFlag_0_5HZ;
    //         }
    //         if (led02State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED02 = flashFlag_0_5HZ;
    //         }
    //         if (led03State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED03 = flashFlag_0_5HZ;
    //         }
    //         if (led04State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED04 = flashFlag_0_5HZ;
    //         }
    //         if (led05State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED05 = flashFlag_0_5HZ;
    //         }
    //         if (led06State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED06 = flashFlag_0_5HZ;
    //         }
    //         if (led07State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED07 = flashFlag_0_5HZ;
    //         }
    //     }
    // }
}