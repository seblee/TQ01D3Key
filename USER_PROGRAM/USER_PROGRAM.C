// #define _CCV3_  // 定义C编译器版本.. --CCG 注释

#include "..\TKS_GLOBE_VARIES.H"
#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"

#include "user_type.h"
#define uchar unsigned char
#define uint unsigned int
/*****key**************************/
volatile _TKS_FLAGA_type keyTrg[2];
volatile uchar k_count[2];

#define keyNUM 4
#define RESTAIN_TIMES 200  // 200 × 10ms = 2s
uchar keyTime[keyNUM] = {0};
uchar keyData         = 0;
/*****beep**************************/
#define BEEP _pb7
uchar beepCount = 1;
/*****led**************************/
#define LED1 _pa1
#define LED2 _pa5
#define LED3 _pa6
#define LED4 _pa7
#define LED5 _pc4
#define LED6 _pc5
#define LED7 _pc6
#define LED8 _pc7
/*************************************/
volatile _TKS_FLAGA_type bitFlag;
#define beepFlag bitFlag.bits.b0
#define beepON bitFlag.bits.b1
#define beepOFF bitFlag.bits.b2
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
        keyData = DATA_BUF[2];  //& 0x0f;

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
    /********BEEP**************/
    _pbc &= 0b01111111;
    /********LED**************/
    _pcc &= 0b00001111;

    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    LED4 = LED_OFF;
    LED5 = LED_OFF;
    LED6 = LED_OFF;
    LED7 = LED_OFF;
    LED8 = LED_OFF;
}

void USER_LED()
{
}