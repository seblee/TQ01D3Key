#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_data.h"
#include <string.h>
#include <stdlib.h>
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
#define SPEED_N 1

//==============================================
volatile _TKS_FLAGA_type SegmentFlag;
#define CTM0FLAG0 SegmentFlag.bits.b0
#define CTM0FLAG1 SegmentFlag.bits.b1
static uchar segmentBuff[2]      = {0xff, 0xff};
static uchar sort                = 0;
const uchar segmentNumberLib[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

uint segmentValue[3] = {0};

//==============================================
void Write_74HC595(unsigned char ChipNum, unsigned char *DataBuf);

//==============================================
//**********************************************
// M. Funct. 0
//==============================================
void __attribute((interrupt(0x14))) Interrupt_MF0(void)
{
    uchar segmentData = 0;
    _ctm0af           = 0;
    sort++;
    if (sort > 7)
    {
        sort = 0;
    }
    segmentBuff[0] = ~(_rotl(1, sort));
    switch (sort)
    {
        case 0:
            segmentData = segmentValue[0] / 100;
            break;
        case 1:
            segmentData = segmentValue[0] / 10 % 10;
            break;
        case 2:
            segmentData = segmentValue[0] % 10;
            break;
        case 3:
            segmentData = segmentValue[1] / 100;
            break;
        case 4:
            segmentData = segmentValue[1] / 10 % 10;
            break;
        case 5:
            segmentData = segmentValue[1] % 10;
            break;
        case 6:
            segmentData = segmentValue[2] / 10;
            break;
        case 7:
            segmentData = segmentValue[2] % 10;
            break;
        default:
            sort        = 0;
            segmentData = segmentValue[0] / 100;
            break;
    }
    segmentBuff[1] = segmentNumberLib[segmentData];

    Write_74HC595(NUM_74HC595, segmentBuff);
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
    _ctm0al = 446 & 0xff;
    _ctm0ah = 446 >> 8;
    _ct0on  = 1;
    _ctm0ae = 1;
    _mf0e   = 1;
    _emi    = 1;
    /******************************/
}
/****************************************/
void USER_SEGMENT(void)
{
}
/****************************************/
//锁存74HC595
void Latch_74HC595(void)
{
    SPI_CS_L;  //设置RCK为低电平，上升沿数据锁存
    // GCC_NOP();
    SPI_CS_H;
    // GCC_NOP();
    // SPI_CS_L;
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
            // GCC_NOP();
            SPI_SCK_H;  // CLK高，上升沿数据移位
            // GCC_NOP();

            DataBufTmp <<= 1;
        }
        DataBuf++;
    }
    Latch_74HC595();
}
