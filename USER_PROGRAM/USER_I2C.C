#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_type.h"
//==============================================
//**********************************************
// M. Funct. 2
//==============================================
void __attribute((interrupt(0x20))) Interrupt_MF2(void)
{
    if (_simtof)  // time out
    {
    }
    else if (_simf)  // address match
    {
    }
    else  // 8 bit trans ok
    {
        /* code */
    }
}
void USER_I2C_INITIAL()
{
    _sim0       = 0;
    _sim1       = 1;
    _sim2       = 1;
    __sima.byte = 0X02;
    _sime       = 1;
    _mf2e       = 1;
    _simen      = 1;
}

void USER_I2C()
{
}