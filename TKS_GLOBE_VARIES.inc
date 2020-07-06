; ========================================================================================================================
; * Created by: Holtek Touch MCU Workshop, Version 3.5.3.0
; TKS_GLOBE_VARIES.inc
; 04:47:45 PM Monday, June 29, 2020
; ========================================================================================================================
#include "BS66F360.inc"
#define _BS66F360_
#define _V413_
#define SystemClock     2
#define IO              0
#define KEY             1
; Keys' attribute & settings ...
#define KEY28           KEY
#define Key28Threshold  16
#define KEY27           KEY
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
#define KEY19           IO
#define Key19Threshold  16
#define KEY18           KEY
#define Key18Threshold  16
#define KEY17           KEY
#define Key17Threshold  16
#define KEY16           IO
#define Key16Threshold  16
#define KEY15           IO
#define Key15Threshold  16
#define KEY14           KEY
#define Key14Threshold  16
#define KEY12           KEY
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
#define IO_TOUCH_ATTR   00001100000010110010100000000000b
#define IO_TOUCH_ATTR2  0000b
; Components' settings ...
; Global options' settings ...
#define DebounceTimes   2 	; 50mS
#define AutoCalibrationPeriod 7 	; x 8 (640mS)
#define HighSensitive   0 	; uncheck
#define MaximumKeyHoldTime 15 	; 60 Sec
#define FastResponse    0 	; uncheck
#define AutoFrequencyHopping 1 	; check
#define OneKeyActive    0 	; uncheck
#define PowerSave       0 	; uncheck
#define NoiseProtect    0 	; uncheck
#define MovingCalibration 1 	; check
#define GlobeOptionA    (DebounceTimes | (AutoCalibrationPeriod << 4))
#define GlobeOptionB    (HighSensitive | (MaximumKeyHoldTime << 4))
#define GlobeOptionC    ((AutoFrequencyHopping << 3) | (FastResponse << 2) | (MovingCalibration << 7) | (NoiseProtect << 4) | (OneKeyActive << 5) | (PowerSave << 6))
