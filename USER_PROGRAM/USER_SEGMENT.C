#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_data.h"
#include <string.h>
/****************************************/
#define SPI_SCK _pe4
#define SPI_CS _pa3
#define SPI_MOSI _pa4

#define SPI_SCK_L SPI_SCK = 0
#define SPI_CS_L SPI_CS = 0
#define SPI_MOSI_L SPI_MOSI = 0

#define SPI_SCK_H SPI_SCK = 1
#define SPI_CS_H SPI_CS = 1
#define SPI_MOSI_H SPI_MOSI = 1

#define NUM_74HC595 2
#define SPEED_N 5

//==============================================
volatile _TKS_FLAGA_type SegmentFlag;
#define CTM0FLAG SegmentFlag.bits.b0
//==============================================
//**********************************************
// M. Funct. 0
//==============================================
void __attribute((interrupt(0x14))) Interrupt_MF0(void)
{
    SPI_SCK = !SPI_SCK;
    // SPI_CS   = !SPI_CS;
    // SPI_MOSI = !SPI_MOSI;
    _ctm0af  = 0;
    CTM0FLAG = 1;
    _pa7     = !_pa7;
}
/****************************************/
void USER_SEGMENT_INITIAL(void)
{
    /******** SPI_SCK *********/
    _pec &= 0b11101111;
    /******** SPI_CS SPI_MOSI *********/
    _pac &= 0b11100111;

    SPI_SCK_L;
    SPI_CS_L;
    SPI_MOSI_L;

    /*******CTM0********************/
    _ctm0c0 = 0x30;  // FH/64
    _ctm0c1 = 0xc1;  // CCRA
    _ctm0al = 500 & 0xff;
    _ctm0ah = 500 >> 8;
    _ct0on  = 1;
    _ctm0ae = 1;
    _mf0e   = 1;
    _emi    = 1;
    _pa7    = 1;
}
/****************************************/
void USER_SEGMENT(void)
{
    if (TKS_63MSF)
    {
        //  SPI_SCK  = !SPI_SCK;
        SPI_CS   = !SPI_CS;
        SPI_MOSI = !SPI_MOSI;
    }
}
/****************************************/
//锁存74HC595
void Latch_74HC595(void)
{
    SPI_CS_L;  //设置RCK为低电平，上升沿数据锁存
    GCC_DELAY(SPEED_N);
    SPI_CS_H;
    GCC_DELAY(SPEED_N);
    SPI_CS_L;
    return;
}

void Write_74HC595(unsigned char ChipNum, unsigned char *DataBuf)
{
    unsigned char i          = 0;
    unsigned char DataBufTmp = 0;

    for (; ChipNum > 0; ChipNum--)
    {
        DataBufTmp = *DataBuf;
        for (i = 0; i < 8; i++)
        {
            SPI_SCK_L;  // CLK低
            if (DataBufTmp & 0x80)
            {
                SPI_MOSI_H;  //输出1
            }
            else
            {
                SPI_MOSI_L;  //输出0
            }
            GCC_DELAY(SPEED_N);
            SPI_SCK_H;  // CLK高，上升沿数据移位
            GCC_DELAY(SPEED_N);

            DataBufTmp <<= 1;
        }
        DataBuf++;
    }
    Latch_74HC595();
    return;
}
