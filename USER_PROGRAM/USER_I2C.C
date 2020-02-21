#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_type.h"
//==============================================
//**********************************************
// M. Funct. 2
//==============================================
void __attribute((interrupt(0x20))) Interrupt_MF2(void)
{
    uchar temp = 5;
    _simf      = 0;
    if (_simtof)  // time out
    {
        _simtoen = 1;
        _simtof  = 0;
    }
    else if (_haas)  // address match
    {
        if (_srw)  // slave send
        {
            _htx  = 1;
            _simd = 1;
        }
        else  // slave receive
        {
            _htx  = 0;
            _txak = 0;
            temp  = _simd;
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
                _simd = 0;
            }
        }
        else  // slave receive
        {
            temp = _simd;
        }
    }
}
void USER_I2C_INITIAL()
{
    _simc0 = 0b11000010;
    //  _sim0 = 0;
    // _sim1    = 1;
    // _sim2    = 1;
    // _simdeb0 = 0;
    // _simdeb1 = 1;
    // _simen   = 1;

    _sima = 0xae;
    _htx  = 0;
    _txak = 0;
    _simf = 0;

    _simtoc = 0b10111111;

    _sime = 1;
    _mf2e = 1;
    _emi  = 1;
}

void USER_I2C()
{
    uchar temp = 5;
    temp++;
    _sim0 = 0;
}