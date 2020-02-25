#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_type.h"

#define I2C_ADDRESS 0x7e

uchar I2cDataIn[10]  = {0};
uchar I2cDataOut[10] = {0};
static uchar inIndex = 0, outIndex = 0;

volatile _TKS_FLAGA_type I2CbitFlag;
#define I2CInFlag I2CbitFlag.bits.b0
#define I2COutFlag I2CbitFlag.bits.b0

/******************************************/
extern _USR_FLAGA_type ledState[5];
extern uchar beepCount;
/******************************************/
//==============================================
//**********************************************
// M. Funct. 2
//==============================================

void __attribute((interrupt(0x20))) Interrupt_MF2(void)
{
    uchar temp;
    _simf = 0;
    if (_simtof)  // time out
    {
        _simtoen = 1;
        _simtof  = 0;
    }
    else if (_haas)  // address match
    {
        if (_srw)  // slave send
        {
            outIndex = 0;
            _htx     = 1;
            _simd    = I2cDataOut[outIndex];
            outIndex++;
            I2COutFlag = 1;
        }
        else  // slave receive
        {
            inIndex   = 0;
            _htx      = 0;
            _txak     = 0;
            temp      = _simd;
            I2CInFlag = 1;
        }
    }
    else  // 8 bit trans ok
    {
        if (_htx)  // slave send
        {
            if (_rxak)
            {
                _htx  = 0;
                _txak = 0;
                temp  = _simd;
            }
            else
            {
                _simd = I2cDataOut[outIndex];
                outIndex++;
            }
        }
        else  // slave receive
        {
            I2cDataIn[inIndex] = _simd;
            inIndex++;
        }
    }
}
void USER_I2C_INITIAL()
{
    _pbpu0 = 1;
    _pbpu1 = 1;
    _simc0 = 0b11000010;
    _sima  = I2C_ADDRESS << 1;

    _htx   = 0;
    _txak  = 0;
    _iamwu = 1;
    _simf  = 0;

    _simtoc = 0b10111111;
    _sime   = 1;

    _mf2e = 1;

    _pbs00 = 1;
    _pbs01 = 0;

    _pbs02 = 1;
    _pbs03 = 0;
}

void USER_I2C()
{
    if ((_hbb == 0) & I2CInFlag)
    {
        I2CInFlag        = 0;
        ledState[0].byte = I2cDataIn[0];
        ledState[1].byte = I2cDataIn[1];
        ledState[2].byte = I2cDataIn[2];
        ledState[3].byte = I2cDataIn[3];
        ledState[4].byte = I2cDataIn[4];
        beepCount += I2cDataIn[5];
    }
    // if (_hbb & I2COutFlag)
    // {
    // }
}