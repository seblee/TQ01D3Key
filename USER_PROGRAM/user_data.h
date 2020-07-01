#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include "user_type.h"

#define BLE_ON _pb7

#define LEDNUM 14

extern volatile _USR_FLAGA_type ledState[7];
#define led00State ledState[0].s4bits.s0
#define led01State ledState[0].s4bits.s1
#define led02State ledState[1].s4bits.s0
#define led03State ledState[1].s4bits.s1
#define led04State ledState[2].s4bits.s0
#define led05State ledState[2].s4bits.s1
#define led06State ledState[3].s4bits.s0
#define led07State ledState[3].s4bits.s1
#define led08State ledState[4].s4bits.s0
#define led09State ledState[4].s4bits.s1
#define led10State ledState[5].s4bits.s0
#define led11State ledState[5].s4bits.s1
#define led12State ledState[6].s4bits.s0
#define led13State ledState[6].s4bits.s1

extern uchar keyBeepMask[2];

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