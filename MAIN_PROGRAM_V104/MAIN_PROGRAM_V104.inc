; ========================================================================================================================
; * Created by: Holtek Touch MCU Workshop, Version 3.5.3.0
; MAIN_PROGRAM_V104.inc
; 09:30:31 AM Tuesday, December 24, 2019
; ========================================================================================================================
#include "..\TKS_GLOBE_VARIES.inc" 
#include "MAIN_PROGRAM_20170905.MCU" 
#include "..\BS66F360_LIBV413\BS66F360_LIBV413.aex" 
 
#define                                
#define EXTEND_FUNCTION_1A_INITIAL     _BS66F360_LIBV413_INITIAL
#define EXTEND_FUNCTION_1A             _BS66F360_LIBV413

;--
INCLUDE "..\IIC_SLAVE_TEST_V101\IIC_SLAVE_TEST_V101.AEX"
#DEFINE EXTEND_FUNCTION_1B_INITIAL      _IIC_SLAVE_TEST_V101_INITIAL
#DEFINE EXTEND_FUNCTION_1B              _IIC_SLAVE_TEST_V101

#include "..\USER_PROGRAM\USER_PROGRAM.aex" 
#define                                
#DEFINE EXTEND_FUNCTION_1C_INITIAL      _USER_PROGRAM_INITIAL
#DEFINE EXTEND_FUNCTION_1C              _USER_PROGRAM

#DEFINE EXTEND_FUNCTION_1D_INITIAL      _USER_LED_INITIAL
#DEFINE EXTEND_FUNCTION_1D              _USER_LED

#include "..\USER_PROGRAM\USER_BLE.aex" 
#define 
#DEFINE EXTEND_FUNCTION_1E_INITIAL      _USER_UART_INITIAL
#DEFINE EXTEND_FUNCTION_1E              _USER_UART

#DEFINE EXTEND_FUNCTION_1F_INITIAL      _USER_BLE_INITIAL
#DEFINE EXTEND_FUNCTION_1F              _USER_BLE

#include "..\USER_PROGRAM\USER_I2C.aex" 
#define 
#DEFINE EXTEND_FUNCTION_1G_INITIAL      _USER_I2C_INITIAL
#DEFINE EXTEND_FUNCTION_1G              _USER_I2C

