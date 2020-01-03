
#include "..\MAIN_PROGRAM_V104\MAIN_PROGRAM_V104.H"
#include "..\TKS_GLOBE_VARIES.H"
#include "user_type.h"
#include <string.h>
/****************************************/
#define UART_SW _pb6
#define PWR_ON _pb5
#define WKUP _pb4

/**************************************/
volatile _TKS_FLAGA_type BLEbitFlag;
#define BLEInit BLEbitFlag.bits.b0
#define uartRecOK BLEbitFlag.bits.b1
#define uartGetflag BLEbitFlag.bits.b2
#define commandMode BLEbitFlag.bits.b3
#define commandOK BLEbitFlag.bits.b4
#define bleNeedRestart BLEbitFlag.bits.b5

/**************************************/
#define RXMAX 40
uchar rxCount       = 0;
uchar txBuff[30]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0a};
uchar rxBuff[RXMAX] = {0};
uchar txCount       = 0;
uchar txCountBak    = 0;

uint delayCount      = 0;
ble_cmd_t bleCmdType = BLE_PPP;
char bleAddr[30]     = {0};
char bleName[30]     = {0};
//==============================================

const cmd_map_st AT_command[17] = {
    {"+++", "AT+", "Mode=AT Mode"},                             // BLE_PPP
    {"AT+setBR 9600\r\n", "AT+", ""},                           // BLE_SETBR
    {"AT+getName\r\n", "AT+", ""},                              // BLE_GETNAME
    {"AT+setName=", "AT+", "restart effect!"},                  // BLE_SETNAME
    {"AT+getAddr\r\n", "AT+", ""},                              // BLE_GETADDR
    {"AT+setAddr=112233445566\r\n", "AT+", "restart effect!"},  // BLE_SETADDR
    {"AT+getStatus\r\n", "AT+", ""},                            // BLE_GETSTATUS
    {"AT+setAdvInt 80 100\r\n", "AT+", "restart effect!"},      // BLE_SETADVINT
    {"AT+setConnInt 36 24 23 00\r\n", "AT+", ""},               // BLE_SETCONNINT
    {"AT+disConnect\r\n", "AT+", ""},                           // BLE_DISCONNECT
    {"AT+reStart\r\n", "AT+", "restart now"},                   // BLE_RESTART
    {"AT+getPara\r\n", "AT+", ""},                              // BLE_GETPARA
    {"AT+exit\r\n", "AT+", "Mode=Normal"},                      // BLE_EXIT
    {"AT+shutDown\r\n", "AT+", "power off now!"},               // BLE_SHUTDOWN
    {"AT+reStore\r\n", "AT+", "restart now"},                   // BLE_RESTORE
    {"AT+getInfo\r\n", "AT+", ""},                              // BLE_GETINFO
    {"AT+help\r\n", "AT+", ""},                                 // BLE_HELP
};
//==============================================
enum
{
    BLE_IDEL,
    BLE_TRANSFERING,
    BLE_STATE_IDEL,
    //    BLE_STATE_IDEL,
};
char bleState = BLE_IDEL;
/**************************************/
/**************************************/
void bleInitialization();
/**************************************/
void pushCmdSendBuff(ble_cmd_t cmd, const char *data);
/**************************************/
void rxProcess(void);
/**************************************/
/**************************************/

/**************************************/
/**************************************/
//==============================================
//**********************************************
// UART
//==============================================
void __attribute((interrupt(0x10))) Interrupt_UART(void)
{
    if (_rxif)
    {
        if (rxCount < RXMAX)
        {
            rxBuff[rxCount] = _txr_rxr;
            rxCount++;
            uartGetflag = 1;
        }
        //        receiveUart(rxBuff);
    }
}
/*********************************************/
void USER_UART_INITIAL()
{
    /********uart**************/
    _pbs04 = 1;
    _pbs05 = 0;
    _pbs06 = 1;
    _pbs07 = 0;
    _usr   = 0x00;
    _ucr1  = 0x80;
    _ucr2  = 0xe4;
    _brg   = 0x67;
    _emi   = 1;
    _ure   = 1;

    memset(rxBuff, 0, sizeof(rxBuff));
}

void USER_UART()
{
    if (TKS_63MSF)
    {
    }
    if (txCount && _txif)
    {
        _txr_rxr = txBuff[txCountBak - txCount];
        txCount--;
    }
}

/**************************************/

void USER_BLE_INITIAL()
{
    /********UART_SW *PWR_ON ********/
    _pbc &= 0b10011111;
    /********WKUP**************/
    _pbc |= 0b00010000;

    UART_SW         = 1;
    PWR_ON          = 1;
    BLEbitFlag.byte = 0;
}

void USER_BLE()
{
    rxProcess();
    if (delayCount)
    {
        if (TKS_63MSF)
            delayCount--;
        return;
    }
    if (!BLEInit)
    {
        bleInitialization();
        return;
    }
    else
    {
        // BLEInit
    }
}
void bleInitialization()
{
    static char step = 0;

    if (step == 0)
    {
        delayCount = 100;
        step++;
    }
    else if (step == 1)
    {
        if ((!commandMode) && (txCount == 0))
        {
            commandMode = 1;
            commandOK   = 0;
            pushCmdSendBuff(BLE_PPP, 0);
            step = 2;
        }
    }
    else if (step == 2)
    {
        if ((commandOK) && (txCount == 0))
        {
            commandOK = 0;
            pushCmdSendBuff(BLE_GETADDR, 0);
            step = 3;
        }
    }
    else if (step == 3)
    {
        if ((commandOK) && (txCount == 0))
        {
            commandOK = 0;
            pushCmdSendBuff(BLE_GETNAME, 0);
            step = 4;
        }
    }
    else if (step == 4)
    {
        static char count = 5;
        if ((commandOK) && (txCount == 0))
        {
            if (strncmp(bleName, bleAddr, 17))
            {
                commandOK = 0;
                pushCmdSendBuff(BLE_SETNAME, bleAddr);
                step = 5;
            }
            else
            {
                step = 7;
            }
        }
    }
    else if (step == 5)
    {
        if ((commandOK) && (txCount == 0))
        {
            commandOK = 0;
            pushCmdSendBuff(BLE_RESTART, 0);
            step = 6;
        }
    }
    else if (step == 6)
    {
        if ((commandOK) && (txCount == 0))
        {
            step = 0;
        }
    }
    else if (step == 7)
    {
        if ((commandOK) && (txCount == 0))
        {
            commandOK = 0;
            pushCmdSendBuff(BLE_EXIT, 0);
            step = 8;
        }
    }
    else if (step == 8)
    {
        if (commandOK)
        {
            BLEInit = 1;
        }
    }
}

void pushCmdSendBuff(ble_cmd_t cmd, const char *data)
{
    bleCmdType = cmd;
    memset(txBuff, 0, sizeof(rxBuff));
    strcpy(txBuff, AT_command[bleCmdType].cmd);
    if (data)
    {
        strcat(txBuff, data);
        strcat(txBuff, "\r\n");
    }
    txCount    = strlen(txBuff);
    txCountBak = txCount;
}
void rxProcess(void)
{
    if (uartGetflag)
    {
        uartGetflag = 0;
        if (commandMode)
        {
            if (strstr(rxBuff, "\r\n"))
            {
                uartRecOK = 1;
            }
        }
    }
    if (uartRecOK)
    {
        char *atBak = strstr(rxBuff, "AT+ok");
        uartRecOK   = 0;
        if (atBak)
        {
            switch (bleCmdType)
            {
                case BLE_PPP:
                    if (strstr(rxBuff, "AT+"))
                    {
                        commandOK = 1;
                    }
                    break;
                // case BLE_SETBR:                    break;
                case BLE_GETNAME:
                    commandOK = 1;
                    strncpy(bleName, atBak + strlen("AT+ok "), 20);
                    break;
                case BLE_SETNAME:
                    commandOK = 1;
                    if (strstr(rxBuff, AT_command[bleCmdType].bkKey))
                    {
                    }
                    break;
                case BLE_GETADDR:
                    commandOK = 1;
                    strncpy(bleAddr, atBak + strlen("AT+ok "), 20);
                    break;
                    // case BLE_GETSTATUS:                    break;
                    // case BLE_SETADVINT:                    break;
                    // case BLE_SETCONNINT:                    break;
                    // case BLE_DISCONNECT:                    break;
                case BLE_RESTART:
                    commandOK = 1;
                    if (strstr(rxBuff, AT_command[bleCmdType].bkKey))
                    {
                        commandMode = 0;
                    }
                    break;
                    // case BLE_GETPARA:                    break;
                case BLE_EXIT:
                    commandOK = 1;
                    break;
                // case BLE_SHUTDOWN:                    break;
                // case BLE_RESTORE:                    break;
                // case BLE_GETINFO:                    break;
                // case BLE_HELP:                    break;
                default:
                    commandOK = 1;
                    break;
            }
        }
        else if (strstr(rxBuff, "AT+err"))
        {
            switch (bleCmdType)
            {
                case BLE_PPP:
                    commandOK = 1;
                    break;

                default:
                    break;
            }
        }

        memset(rxBuff, 0, sizeof(rxBuff));
        rxCount = 0;
    }
}