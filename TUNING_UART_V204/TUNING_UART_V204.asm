
message '***************************************************************'
message '*SUB. NAME:UART TEST CODE                                     *'
message '*INPUT    :                                                   *'
message '*OUTPUT   :                                                   *'
message '*USED REG.:                                                   *'
message '*FUNCTION :                                                   *'
message '*REMARK   :                                                   *'
message '***************************************************************'
                ;=INCLUDE REFERENCE FILE
                INCLUDE TUNING_UART_V204.INC

                PUBLIC  _TUNING_UART_V204_INITIAL
                PUBLIC  _TUNING_UART_V204



;;-UART INTERFACE DEFINE
                ;================
                ;=BAUDRATE MACRO=
                ;================
BAUDRATE_DELAY  MACRO
IFNDEF SystemClock
	#define	SystemClock 0
ENDIF
IF      SystemClock == 0                ;8MHZ
        IF      BAUDRATE_OPTION == 0
                JMP	$+1
                MOV     A,13            ;38400
	nop

        ELSEIF  BAUDRATE_OPTION == 1
                MOV     A,8             ;57600
          nop


        ELSE    ;-----
                MOV     A,2             ;115200
                nop

        ENDIF

;--
ELSEIF  SystemClock == 1                ;12MHZ
        IF      BAUDRATE_OPTION == 0
                NOP
                MOV     A,22            ;38400

        ELSEIF  BAUDRATE_OPTION == 1
        		JMP	$+1
                MOV     A,13            ;57600

        ELSE    ;---
                MOV     A,5             ;115200

        ENDIF
;--
ELSE                                    ;16MHZ
        IF      BAUDRATE_OPTION == 0
                NOP
                MOV     A,31            ;38400

        ELSEIF  BAUDRATE_OPTION == 1
                JMP	$+1
                MOV     A,19            ;57600

        ELSE    ;--
                MOV     A,8             ;115200

        ENDIF
;--
ENDIF
                SDZ     ACC
                JMP     $-1
                ENDM







                ;==============
                ;=DATA SETCTION
                ;==============
TUNING_UART_V204_DATA  .SECTION   'DATA'
TX_ORDER        DB      ?
TX_LEN        DB      ?
CHECK_SUM       DB      ?

IFNDEF _LESS_DATA_
TX_BUF       DB      12	dup(?)
ENDIF
                ;==============
                ;=CODE SETCTION
                ;==============
IFDEF _LESS_DATA_
TUNING_UART_V204_CODE_04A_03H  .SECTION AT 003H  'CODE'
	SEND_PREAMBLE:
		;-SEND PREAMBLE CODE
		MOV	A,055H
		CALL	SEND_BYTE
		MOV	A,0AAH
		CALL	SEND_BYTE
		JMP	SEND_ADDR
TUNING_UART_V204_CODE_04A_0CH  .SECTION AT 00CH  'CODE'
	GET_ENV_REF_BITMAP:
		MOV	A,_DATA_BUF[4]
		SZ	TX_ORDER.0
		JMP	_GET_ENV_VALUE
		SZ	TX_ORDER.1
		JMP	_GET_REF_VALUE
		SZ	TX_ORDER.2
		JMP	_GET_KEY_BITMAP
		RET
ENDIF
TUNING_UART_V204_CODE  .SECTION   'CODE'

;;***********************************************************
;;*SUB. NAME:                                               *
;;*INPUT    :                                               *
;;*OUTPUT   :                                               *
;;*USED REG.:                                               *
;;*FUNCTION :                                               *
;;***********************************************************
_TUNING_UART_V204_INITIAL:
IFNDEF _LESS_DATA_
		CALL	_GET_LIB_VER
		MOV	A,_DATA_BUF[0]
		MOV	TX_BUF[0],A
		MOV	A,_DATA_BUF[1]
		MOV	TX_BUF[1],A
		CALL	_GET_KEY_AMOUNT
		MOV	TX_BUF[2],A
ENDIF
		RET
IFNDEF _LESS_DATA_
IAR0_2_IAR1 PROC
		MOV	_DATA_BUF[0],A
	$1:
		MOV	A,IAR0
		MOV	IAR1,A
		INC	MP0
		INC	MP1
		SDZ	_DATA_BUF[0]
		JMP	$1
		RET
IAR0_2_IAR1 ENDP
ENDIF

;;***********************************************************
;;*SUB. NAME:                                               *
;;*INPUT    :                                               *
;;*OUTPUT   :                                               *
;;*USED REG.:                                               *
;;*FUNCTION :                                               *
;;***********************************************************
_TUNING_UART_V204 PROC
		SNZ	_SCAN_CYCLEF
		RET

IFNDEF _LESS_DATA_
		RL	TX_ORDER
		SZ	TX_ORDER
		JMP	$+2
		SET	TX_ORDER.0

		SZ	TX_ORDER.0
		JMP	$1
		SZ	TX_ORDER.1
		JMP	$2
		SZ	TX_ORDER.2
		JMP	$3
		SZ	TX_ORDER.3
		JMP	$4
		SZ	TX_ORDER.4
		JMP	$5

		JMP	$6
	$1:
		MOV	A,OFFSET TX_BUF[4]
		MOV	MP1,A
		MOV	A,BANK TX_BUF[4]
		MOV	BP,A
	;..........................................
		MOV	A,OFFSET _KEY_IO_SEL
		MOV	MP0,A
		MOV	A,4
		CALL	IAR0_2_IAR1
		CPL	TX_BUF[4]
		CPL	TX_BUF[5]
		CPL	TX_BUF[6]
		CPL	TX_BUF[7]
	;..........................................
		MOV	A,OFFSET _KEY_DATA
		MOV	MP0,A

		MOV	A,4
		CALL	IAR0_2_IAR1
	;..........................................
		MOV	A,OFFSET TX_BUF
		MOV	MP1,A
		MOV	A,BANK TX_BUF
		MOV	BP,A
		CLR	_DATA_BUF[2]
		MOV	A,12
		JMP	START_SEND
	$2:
		MOV	A,OFFSET _GLOBE_VARIES
		MOV	MP1,A
		MOV	A,BANK _GLOBE_VARIES
		MOV	BP,A
		MOV	A,0D3H
		MOV	_DATA_BUF[2],A
		MOV	A,3
		JMP	START_SEND
	$3:
		MOV	A,OFFSET _GLOBE_VARIES[3]
		MOV	MP1,A
		MOV	A,BANK _GLOBE_VARIES[3]
		MOV	BP,A
		MOV	A,0D8H
		MOV	_DATA_BUF[2],A
		MOV	A,TX_BUF[2]
		JMP	START_SEND
	$4:
		MOV	A,0B0H
		MOV	_DATA_BUF[2],A
		MOV	A,TX_BUF[2]
		JMP	START_SEND
	$5:
		MOV	A,050H          ;REF ADDRESS
		JMP	$+2
	$6:
		MOV	A,010H          ;ENV ADDRESS
		MOV	_DATA_BUF[2],A
		MOV	A,TX_BUF[2]
		RL	ACC
	START_SEND:

		MOV	TX_LEN,A
		;-SEND PREAMBLE CODE
		MOV	A,055H
		CALL	SEND_BYTE
		MOV	A,0AAH
		CALL	SEND_BYTE

		;-SEND COMMAND ADDRESS
		MOV	A,_DATA_BUF[2]
		MOV	CHECK_SUM,A
		CALL	SEND_BYTE

		;-SEND LENGTH
		MOV	A,TX_LEN
		CALL	ADD_SEND_BYTE


		MOV	A,TX_ORDER
		AND	A,00001111B
		SZ	Z
		RR	TX_LEN

		;--
		CLR	_DATA_BUF[4]
SEND_LOOP:      ;-SEND COMMAND C LOOP
		MOV	A,TX_ORDER
		AND	A,00011111B
		MOV	A,_DATA_BUF[4]
		SZ	Z
		JMP	SEND_ENV
		SZ	TX_ORDER.4
		JMP	SEND_REF
		SZ	TX_ORDER.3
		JMP	SEND_RCC

		MOV	A,IAR1
		MOV	_DATA_BUF[1],A
		INC	MP1
		JMP	SEND_1Byte
	SEND_RCC:
		CALL	_GET_RCC_VALUE
		MOV	A,_DATA_BUF[0]
		MOV	_DATA_BUF[1],A
		JMP	SEND_1Byte
	SEND_REF:
		CALL	_GET_REF_VALUE
		JMP	SEND_2Byte
	SEND_ENV:
		CALL	_GET_ENV_VALUE
ELSE
		RL	TX_ORDER
		SZ	TX_ORDER.5
		;SZ	TX_ORDER.4
		;SZ	TX_ORDER.3
		;SZ	TX_ORDER.2
		CLR	TX_ORDER
		SZ	TX_ORDER
		JMP	$+2
		SET	TX_ORDER.0


		;;;;;;;-SEND PREAMBLE CODE
		;;;;;MOV	A,055H
		;;;;;CALL	SEND_BYTE
		;;;;;MOV	A,0AAH
		;;;;;CALL	SEND_BYTE
	JMP	SEND_PREAMBLE
	SEND_ADDR:
		;-SEND COMMAND ADDRESS
	SZ	TX_ORDER.0
	MOV	A,010H		;ENV
	SZ	TX_ORDER.1
	MOV	A,050H		;REF
	SZ	TX_ORDER.2
	MOV	A,008H		;KEY_STATUS0
	;SZ	TX_ORDER.3
	;MOV	A,0B0H		;RCC
	SZ	TX_ORDER.4
	MOV	A,002H		;KEY AMOUNT

		MOV	CHECK_SUM,A
		CALL	SEND_BYTE

		;-SEND LENGTH
		CALL	_GET_KEY_AMOUNT
	SNZ	TX_ORDER.4
	SZ	TX_ORDER.2
	MOV	A,1
		MOV	TX_LEN,A
	;SNZ	TX_ORDER.3
		RL	ACC
		CALL	ADD_SEND_BYTE
                	CLR	_DATA_BUF[4]

	SEND_LOOP:   	;-SEND COMMAND C LOOP
	CALL	GET_ENV_REF_BITMAP
	;MOV	A,_DATA_BUF[4]
	;SZ	TX_ORDER.0
	;CALL	_GET_ENV_VALUE
	;SZ	TX_ORDER.1
	;CALL	_GET_REF_VALUE
	;SZ	TX_ORDER.2
	;CALL	_GET_KEY_BITMAP

	SNZ	TX_ORDER.4
	JMP	$+3
	CALL	_GET_KEY_AMOUNT
	MOV	_DATA_BUF[0],A
	SNZ	TX_ORDER.3
	JMP	SEND_2Byte
	CALL	_GET_RCC_VALUE
	MOV	A,_DATA_BUF[0]
	MOV	_DATA_BUF[1],A
	JMP	SEND_1Byte
ENDIF



          SEND_2Byte:
		;-SEND LOW BYTE
		MOV	A,_DATA_BUF[0]
		CALL	ADD_SEND_BYTE
	SEND_1Byte:
		;-SEND HIGH BYTE
		MOV	A,_DATA_BUF[1]
		CALL	ADD_SEND_BYTE

		;-CHECK LENGTH
		INC	_DATA_BUF[4]
		MOV	A,_DATA_BUF[4]
		SUB	A,TX_LEN
		SNZ	C
		JMP	SEND_LOOP

		;-SEND CHECK SUM
		MOV	A,CHECK_SUM
		CALL	SEND_BYTE
		RET
_TUNING_UART_V204 ENDP

;;***********************************************************
;;*SUB. NAME:                                               *
;;*INPUT    :                                               *
;;*OUTPUT   :                                               *
;;*USED REG.:                                               *
;;*FUNCTION :                                               *
;;***********************************************************
ADD_SEND_BYTE PROC
	ADDM	A,CHECK_SUM
SEND_BYTE:
                MOV     _DATA_BUF[5],A
                SET     _DATA_BUF[6]
                ;--
                MOV     A,9+3           ;3 STOP BIT
                MOV     _DATA_BUF[7],A
                ;----------------
                ;-SEND START BIT-
                ;----------------
                CLR     EMI
                _set     _UART_TX
                _clr     _UART_TX_C
                CLR     C               ;START BIT
SEND_BIT_LOOP:  ;-SEND BIT DATA LOOP
                SZ      C
                JMP     TX_HIGH
                ;-TX LOW
                NOP                     ;DELAY 1 INST.
                _clr     _UART_TX
                JMP     TX_BIT
TX_HIGH:        ;-TX HIGH
                _set     _UART_TX
                JMP     $+1             ;DELAY 2 INST.
TX_BIT:         ;-TX ONE BIT
                RRC     _DATA_BUF[6]
                RRC     _DATA_BUF[5]
                ;--
                BAUDRATE_DELAY
                ;--
                SDZ     _DATA_BUF[7]
                JMP     SEND_BIT_LOOP
                ;--
                SET     EMI
                RET
ADD_SEND_BYTE ENDP






