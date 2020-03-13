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

uchar ledFlashFast[8]     = {0};
volatile uchar flashCount = 0;

volatile _USR_FLAGA_type ledState[4];
#define led1State ledState[0].s4bits.s0
#define led2State ledState[0].s4bits.s1
#define led3State ledState[1].s4bits.s0
#define led4State ledState[1].s4bits.s1
#define led5State ledState[2].s4bits.s0
#define led6State ledState[2].s4bits.s1
#define led7State ledState[3].s4bits.s0
#define led8State ledState[3].s4bits.s1
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
        keyData = DATA_BUF[2];  //& 0x0f;

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
    /*******led1****************/
    if (ledFlashFast[0] > 0)
    {
        if (led1State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[0] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[0]--;
            LED1 = !LED1;
        }
    }
    else
    {
        switch (led1State)
        {
            case STATE_LED_OFF:
                LED1 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED1 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED1 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED1 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED1 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[0] += 4;
                led1State = 0;
                break;
        }
    }
    /*******led2****************/
    if (ledFlashFast[1] > 0)
    {
        if (led2State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[1] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[1]--;
            LED2 = !LED2;
        }
    }
    else
    {
        switch (led2State)
        {
            case STATE_LED_OFF:
                LED2 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED2 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED2 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED2 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED2 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[1] += 4;
                led2State = 0;
                break;
        }
    }
    /*******led3****************/
    if (ledFlashFast[2] > 0)
    {
        if (led3State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[2] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[2]--;
            LED3 = !LED3;
        }
    }
    else
    {
        switch (led3State)
        {
            case STATE_LED_OFF:
                LED3 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED3 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED3 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED3 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED3 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[2] += 4;
                led3State = 0;
                break;
        }
    }
    /*******led4****************/
    if (ledFlashFast[3] > 0)
    {
        if (led4State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[3] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[3]--;
            LED4 = !LED4;
        }
    }
    else
    {
        switch (led4State)
        {
            case STATE_LED_OFF:
                LED4 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED4 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED4 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED4 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED4 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[3] += 4;
                led4State = 0;
                break;
        }
    }

    /*******led5****************/
    if (ledFlashFast[4] > 0)
    {
        if (led5State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[4] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[4]--;
            LED5 = !LED5;
        }
    }
    else
    {
        switch (led5State)
        {
            case STATE_LED_OFF:
                LED5 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED5 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED5 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED5 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED5 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[4] += 4;
                led5State = 0;
                break;
        }
    }
    /*******led6****************/
    if (ledFlashFast[5] > 0)
    {
        if (led6State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[5] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[5]--;
            LED6 = !LED6;
        }
    }
    else
    {
        switch (led6State)
        {
            case STATE_LED_OFF:
                LED6 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED6 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED6 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED6 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED6 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[5] += 4;
                led6State = 0;
                break;
        }
    }
    /*******led7****************/
    if (ledFlashFast[6] > 0)
    {
        if (led7State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[6] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[6]--;
            LED7 = !LED7;
        }
    }
    else
    {
        switch (led7State)
        {
            case STATE_LED_OFF:
                LED7 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED7 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED7 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED7 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED7 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[6] += 4;
                led7State = 0;
                break;
        }
    }
    /*******led8****************/
    if (ledFlashFast[7] > 0)
    {
        if (led8State == STATE_LED_FLASH_2_T)
        {
            ledFlashFast[7] += 4;
        }
        if (TKS_63MSF)
        {
            ledFlashFast[7]--;
            LED8 = !LED8;
        }
    }
    else
    {
        switch (led8State)
        {
            case STATE_LED_OFF:
                LED8 = LED_OFF;
                break;
            case STATE_LED_ON:
                LED8 = LED_ON;
                break;
            case STATE_LED_FLASH_2HZ:
                if (TKS_250MSF)
                    LED8 = flashFlag_2HZ;
                break;
            case STATE_LED_FLASH_1HZ:
                if (TKS_500MSF)
                    LED8 = flashFlag_1HZ;
                break;
            case STATE_LED_FLASH_0_5HZ:
                if (TKS_500MSF)
                    LED8 = flashFlag_0_5HZ;
                break;
            case STATE_LED_FLASH_2_T:  //闪烁两下
                ledFlashFast[7] += 4;
                led8State = 0;
                break;
        }
    }

    // {
    //     if (led1State == STATE_LED_OFF)
    //     {
    //         LED1 = LED_OFF;
    //     }
    //     if (led2State == STATE_LED_OFF)
    //     {
    //         LED2 = LED_OFF;
    //     }
    //     if (led3State == STATE_LED_OFF)
    //     {
    //         LED3 = LED_OFF;
    //     }
    //     if (led4State == STATE_LED_OFF)
    //     {
    //         LED4 = LED_OFF;
    //     }

    //     if (led5State == STATE_LED_OFF)
    //     {
    //         LED5 = LED_OFF;
    //     }

    //     if (led6State == STATE_LED_OFF)
    //     {
    //         LED6 = LED_OFF;
    //     }
    //     if (led7State == STATE_LED_OFF)
    //     {
    //         LED7 = LED_OFF;
    //     }
    //     if (led8State == STATE_LED_OFF)
    //     {
    //         LED8 = LED_OFF;
    //     }

    //     if (led1State == STATE_LED_ON)
    //     {
    //         LED1 = LED_ON;
    //     }
    //     if (led2State == STATE_LED_ON)
    //     {
    //         LED2 = LED_ON;
    //     }
    //     if (led3State == STATE_LED_ON)
    //     {
    //         LED3 = LED_ON;
    //     }
    //     if (led4State == STATE_LED_ON)
    //     {
    //         LED4 = LED_ON;
    //     }

    //     if (led5State == STATE_LED_ON)
    //     {
    //         LED5 = LED_ON;
    //     }

    //     if (led6State == STATE_LED_ON)
    //     {
    //         LED6 = LED_ON;
    //     }
    //     if (led7State == STATE_LED_ON)
    //     {
    //         LED7 = LED_ON;
    //     }
    //     if (led8State == STATE_LED_ON)
    //     {
    //         LED8 = LED_ON;
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
    //     if (led1State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED1 = flashFlag_2HZ;
    //     }
    //     if (led2State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED2 = flashFlag_2HZ;
    //     }
    //     if (led3State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED3 = flashFlag_2HZ;
    //     }
    //     if (led4State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED4 = flashFlag_2HZ;
    //     }

    //     if (led5State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED5 = flashFlag_2HZ;
    //     }

    //     if (led6State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED6 = flashFlag_2HZ;
    //     }
    //     if (led7State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED7 = flashFlag_2HZ;
    //     }
    //     if (led8State == STATE_LED_FLASH_2HZ)
    //     {
    //         LED8 = flashFlag_2HZ;
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
    //     if (led1State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED1 = flashFlag_1HZ;
    //     }
    //     if (led2State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED2 = flashFlag_1HZ;
    //     }
    //     if (led3State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED3 = flashFlag_1HZ;
    //     }
    //     if (led4State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED4 = flashFlag_1HZ;
    //     }
    //     if (led5State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED5 = flashFlag_1HZ;
    //     }
    //     if (led6State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED6 = flashFlag_1HZ;
    //     }
    //     if (led7State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED7 = flashFlag_1HZ;
    //     }
    //     if (led8State == STATE_LED_FLASH_1HZ)
    //     {
    //         LED8 = flashFlag_1HZ;
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
    //         if (led1State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED1 = flashFlag_0_5HZ;
    //         }
    //         if (led2State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED2 = flashFlag_0_5HZ;
    //         }
    //         if (led3State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED3 = flashFlag_0_5HZ;
    //         }
    //         if (led4State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED4 = flashFlag_0_5HZ;
    //         }
    //         if (led5State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED5 = flashFlag_0_5HZ;
    //         }
    //         if (led6State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED6 = flashFlag_0_5HZ;
    //         }
    //         if (led7State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED7 = flashFlag_0_5HZ;
    //         }
    //         if (led8State == STATE_LED_FLASH_0_5HZ)
    //         {
    //             LED8 = flashFlag_0_5HZ;
    //         }
    //     }
    // }
}