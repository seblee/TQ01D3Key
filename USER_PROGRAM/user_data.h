#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include "user_type.h"

#define BLE_ON _pc2

extern _USR_FLAGA_type ledState[5];
extern uchar beepCount;
extern volatile uchar k_count[2];
extern uchar I2cDataOut[20];
extern uchar I2cDataIn[20];

extern volatile _TKS_FLAGA_type BLEbitFlag;
#define BLEInit BLEbitFlag.bits.b0
#define uartRecOK BLEbitFlag.bits.b1
#define uartGetflag BLEbitFlag.bits.b2
#define commandMode BLEbitFlag.bits.b3
#define commandOK BLEbitFlag.bits.b4
#define timeToTrans BLEbitFlag.bits.b5
#define regDataOk BLEbitFlag.bits.b6

#define RXMAX 40
extern uchar rxBuff[RXMAX];
extern uchar rxStep;
extern uchar rxCount;

uchar getCheckSum(uchar* data);

#endif