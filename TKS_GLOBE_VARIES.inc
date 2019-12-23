; ========================================================================================================================
; * Created by: Holtek Touch MCU Workshop, Version 3.5.3.0
; TKS_GLOBE_VARIES.inc
; 05:09:45 PM Monday, December 23, 2019
; ========================================================================================================================
#include "BS66F360.inc"
#define _BS66F360_
#define _V413_
#define SystemClock     0
#define IO              0
#define KEY             1
; Keys' attribute & settings ...
#define KEY28           IO
#define Key28Threshold  16
#define KEY27           IO
#define Key27Threshold  16
#define KEY26           IO
#define Key26Threshold  16
#define KEY25           IO
#define Key25Threshold  16
#define KEY24           IO
#define Key24Threshold  16
#define KEY23           IO
#define Key23Threshold  16
#define KEY22           IO
#define Key22Threshold  16
#define KEY21           IO
#define Key21Threshold  16
#define KEY20           KEY
#define Key20Threshold  16
#define KEY19           KEY
#define Key19Threshold  16
#define KEY18           KEY
#define Key18Threshold  16
#define KEY17           KEY
#define Key17Threshold  16
#define KEY16           IO
#define Key16Threshold  16
#define KEY15           IO
#define Key15Threshold  16
#define KEY14           IO
#define Key14Threshold  16
#define KEY12           IO
#define Key12Threshold  16
#define KEY11           IO
#define Key11Threshold  16
#define KEY10           IO
#define Key10Threshold  16
#define KEY9            IO
#define Key9Threshold   16
#define KEY8            IO
#define Key8Threshold   16
#define KEY7            IO
#define Key7Threshold   16
#define KEY4            IO
#define Key4Threshold   16
#define KEY3            IO
#define Key3Threshold   16
#define KEY2            IO
#define Key2Threshold   16
#define KEY1            IO
#define Key1Threshold   16
#define KEY5            IO
#define Key5Threshold   16
#define KEY6            IO
#define Key6Threshold   16
#define KEY13           IO
#define Key13Threshold  16
#define IO_TOUCH_ATTR   00000000000011110000000000000000b
#define IO_TOUCH_ATTR2  0000b
; Components' settings ...
;	'TUNING_UART_V204' 
#define _UART_TX        PA1
#define _UART_TX_C      PAC1
#define BAUDRATE_OPTION 0 	; 38400bps
; Global options' settings ...
#define DebounceTimes   5 	; 80mS
#define AutoCalibrationPeriod 7 	; x 8 (640mS)
#define HighSensitive   0 	; uncheck
#define MaximumKeyHoldTime 0 	; Disable
#define FastResponse    0 	; uncheck
#define AutoFrequencyHopping 0 	; uncheck
#define OneKeyActive    0 	; uncheck
#define PowerSave       0 	; uncheck
#define NoiseProtect    0 	; uncheck
#define MovingCalibration 0 	; uncheck
#define GlobeOptionA    ((AutoCalibrationPeriod << 4) | DebounceTimes)
#define GlobeOptionB    ((MaximumKeyHoldTime << 4) | HighSensitive)
#define GlobeOptionC    ((FastResponse << 2) | (PowerSave << 6) | (OneKeyActive << 5) | (NoiseProtect << 4) | (MovingCalibration << 7) | (AutoFrequencyHopping << 3))
