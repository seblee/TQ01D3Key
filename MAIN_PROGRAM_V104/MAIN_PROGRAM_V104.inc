; ========================================================================================================================
; * Created by: Holtek Touch MCU Workshop, Version 3.5.3.0
; MAIN_PROGRAM_V104.inc
; 05:09:45 PM Monday, December 23, 2019
; ========================================================================================================================
#include "..\TKS_GLOBE_VARIES.inc" 
#include "MAIN_PROGRAM_20170905.MCU" 
#include "..\BS66F360_LIBV413\BS66F360_LIBV413.aex" 
#define                                
#define EXTEND_FUNCTION_1A_INITIAL     _BS66F360_LIBV413_INITIAL
#define EXTEND_FUNCTION_1A             _BS66F360_LIBV413
#include "..\USER_PROGRAM\USER_PROGRAM.aex" 
#define                                
#define EXTEND_FUNCTION_1B_INITIAL     _USER_PROGRAM_INITIAL
#define EXTEND_FUNCTION_1B             _USER_PROGRAM
#include "..\TUNING_UART_V204\TUNING_UART_V204.aex" 
#define                                
#define EXTEND_FUNCTION_1C_INITIAL     _TUNING_UART_V204_INITIAL
#define EXTEND_FUNCTION_1C             _TUNING_UART_V204
