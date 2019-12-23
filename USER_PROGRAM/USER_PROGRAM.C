
#include "..\TKS_GLOBE_VARIES.H"
#include "..\MAIN_PROGRAM_V100\MAIN_PROGRAM_V100.H"

#define _test 1

#define uchar unsigned char
#define uint unsigned int

#define _pin_RECEIVE_D _pa3
#define _pin_RECEIVE_C _pac3
#define _pin_SEND_D _pa7
#define _pin_SEND_C _pac7

#define _pin_SEG_L _pd
#define _pin_SEG_H _pc
#define _pin_COM _pa

//#define		_pin_COM1			_pa1
//#define		_pin_COM2			_pa4
//#define		_pin_COM3			_pa3
//#define		_pin_COM4			_pa7
//#define		_pin_COM1_CT			_pac1
//#define		_pin_COM2_CT			_pac4
//#define		_pin_COM3_CT			_pac3
//#define		_pin_COM4_CT			_pac7

#define _pin_COM3 _pa1
#define _pin_COM4 _pa4
#define _pin_COM2 _pa0
#define _pin_COM1 _pa2
#define _pin_COM3_CT _pac1
#define _pin_COM4_CT _pac4
#define _pin_COM2_CT _pac0
#define _pin_COM1_CT _pac2

#define _pin_SEG1 _pd2
#define _pin_SEG2 _pd3
#define _pin_SEG3 _pc4
#define _pin_SEG4 _pc5
#define _pin_SEG5 _pc6
#define _pin_SEG6 _pc7
#define _pin_SEG7 _pd0
#define _pin_SEG8 _pd1

#define _pin_SEG1_CT _pdc2
#define _pin_SEG2_CT _pdc3
#define _pin_SEG3_CT _pcc4
#define _pin_SEG4_CT _pcc5
#define _pin_SEG5_CT _pcc6
#define _pin_SEG6_CT _pcc7
#define _pin_SEG7_CT _pdc0
#define _pin_SEG8_CT _pdc1

#define _pin_SEG_L_CT _pdc
#define _pin_SEG_H_CT _pcc
#define _pin_COM_CT _pac

#define c_2ms 8     //  8* 250uS
#define c_100ms 60  //
#define C_KEY_DEBOUNCE_TIME 1
#define c_KEY_DELAY_TIME 40  //  40* 250uS

#define C_0_L 2  /// 0:   >=C_0_L,   < C_0_H   // ʱ�� 125uS
#define C_0_H 5

#define C_1_L 6  /// 1:   >C_1_L,   <=C_1_H   // ʱ�� 125uS
#define C_1_H 10

//#define		C_pin_COM_OFF					0x8f					//test
#define C_pin_COM_OFF 0x17
#define C_pin_SEG_L_OFF 0x0f
#define C_pin_SEG_H_OFF 0xf0

#pragma rambank0
bit b_2ms, b_receive_ok, b_KEY_ACTION, b_KEY_SEND, b_pin_RECEIVE_D, b_PIN_ST;
uchar r_show_buf[4], r_show_led_buf[6], r_receive_buf[8], r_send_buf, r_send_buf1, r_Adjustment_num;
uchar i, r_SEND_BIT_TIME, r_100ms, r_KEY_DELAY_TIME;
#pragma norambank

uchar r_2ms, r_scan_num, r_temp, r_temp1, r_temp2;
uchar r_PIN_RECEIVE_LOW_TIME, r_PIN_RECEIVE_HIGH_TIME, r_RECEIVE_NUM_COUNT;
uchar r_KEY_STATUS1, r_KEY_STATUS2, r_KEY_BUF[2], r_KEY_DEBOUNCE_TIME;
r_SEND_KEY_COUNT;

/*
  PD0->C; PD1->B; PD2->E; PD3->D; PC4->G; PC5->DP; PC6->A; PC7->F;
*/
#define SEG_A 0x40
#define SEG_B 0x02
#define SEG_C 0x01
#define SEG_D 0x08
#define SEG_E 0x04
#define SEG_F 0x80
#define SEG_G 0x10
#define SEG_DP 0x20

#define C_SEG_OFF 16

const uchar C_NUM_7SEG[17] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    SEG_B | SEG_C,  // 0,1
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,  // 2,3
    SEG_G | SEG_B | SEG_C | SEG_F,
    SEG_A | SEG_G | SEG_C | SEG_D | SEG_F,  // 4,5
    SEG_A | SEG_G | SEG_C | SEG_D | SEG_E | SEG_F,
    SEG_A | SEG_B | SEG_C,  // 6,7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G | SEG_F,  // 8,9
    SEG_A | SEG_G | SEG_B | SEG_C | SEG_E | SEG_F,
    SEG_G | SEG_C | SEG_D | SEG_E | SEG_F,  // A,B
    SEG_D | SEG_E | SEG_G,
    SEG_G | SEG_B | SEG_C | SEG_D | SEG_E,  // C,D
    SEG_G | SEG_A | SEG_D | SEG_E | SEG_F,
    SEG_A | SEG_G | SEG_E | SEG_F,
    0x00  // E,F,OFF
};

//==============================================
//**********************************************
//==============================================
//#pragma vector  Interrupt_Extemal        @ 0x04
// void Interrupt_Extemal()

void __attribute((interrupt(0x04))) Interrupt_Extemal(void)
{
    _nop();
}

//==============================================
//**********************************************
// CTM0 CCRP INTERRUPT
//==============================================
//#pragma vector  Interrupt_CTM0_CCRP        @ 0x10
// void Interrupt_CTM0_CCRP()

void __attribute((interrupt(0x10))) Interrupt_CTM0_CCRP(void)
{
    _nop();
}

//==============================================
//**********************************************
// CTM0 CCRA INTERRUPT
// 250uS---��ʱ��
//==============================================
//#pragma vector  Interrupt_CTM0_CCRA       @ 0x14
// void Interrupt_CTM0_CCRA()
void __attribute((interrupt(0x14))) Interrupt_CTM0_CCRA(void)
{
    _nop();
    _clrwdt();
    r_2ms++;
    if (r_2ms >= c_2ms)
    {
        r_2ms = 0;
        b_2ms = 1;

        if (r_KEY_DELAY_TIME)  // 40*0.25==10ms
        {
            r_KEY_DELAY_TIME--;
        }
        else
        {
            if (!b_KEY_SEND)
            {
                b_KEY_SEND       = 1;
                b_PIN_ST         = 1;
                r_SEND_KEY_COUNT = 0;
                r_SEND_BIT_TIME  = 0;
                b_KEY_ACTION     = 0;
            }
        }
    }

    r_scan_num++;
    r_temp = 0;
    if (r_scan_num > 13)
        r_scan_num = 0;
    _pin_COM_CT |= C_pin_COM_OFF;
    _pin_SEG_L_CT |= C_pin_SEG_L_OFF;
    _pin_SEG_H_CT |= C_pin_SEG_H_OFF;
    _pin_SEG_L &= ~C_pin_SEG_L_OFF;
    _pin_SEG_H &= ~C_pin_SEG_H_OFF;
    _pin_COM &= ~C_pin_COM_OFF;

    switch (r_scan_num)
    {
        case 0:
            r_temp |= r_show_buf[0];
            // r_temp|=C_NUM_7SEG[r_show_buf[0]];
            _pin_SEG_L |= r_temp;
            _pin_SEG_L_CT &= ~r_temp;
            _pin_COM1    = 0;
            _pin_COM1_CT = 0;
            break;

        case 1:
            r_temp |= r_show_buf[0];
            // r_temp|=C_NUM_7SEG[r_show_buf[0]];
            _pin_SEG_H |= r_temp;
            _pin_SEG_H_CT &= ~r_temp;
            _pin_COM1    = 0;
            _pin_COM1_CT = 0;
            break;

        case 2:
            r_temp |= r_show_buf[1];
            // r_temp|=C_NUM_7SEG[r_show_buf[1]];
            _pin_SEG_H |= r_temp;
            _pin_SEG_H_CT &= ~r_temp;
            _pin_COM2    = 0;
            _pin_COM2_CT = 0;
            break;

        case 3:
            r_temp |= r_show_buf[1];
            // r_temp|=C_NUM_7SEG[r_show_buf[1]];
            _pin_SEG_L |= r_temp;
            _pin_SEG_L_CT &= ~r_temp;
            _pin_COM2    = 0;
            _pin_COM2_CT = 0;
            break;

        case 4:
            r_temp |= r_show_buf[2];
            // r_temp|=C_NUM_7SEG[r_show_buf[2]];
            _pin_SEG_H |= r_temp;
            _pin_SEG_H_CT &= ~r_temp;
            _pin_COM3    = 0;
            _pin_COM3_CT = 0;
            break;

        case 5:
            r_temp |= r_show_buf[2];
            // r_temp|=C_NUM_7SEG[r_show_buf[2]];
            _pin_SEG_L |= r_temp;
            _pin_SEG_L_CT &= ~r_temp;
            _pin_COM3    = 0;
            _pin_COM3_CT = 0;
            break;

        case 6:
            r_temp |= r_show_buf[3];
            // r_temp|=C_NUM_7SEG[r_show_buf[3]];
            _pin_SEG_L |= r_temp;
            _pin_SEG_L_CT &= ~r_temp;
            _pin_COM4    = 0;
            _pin_COM4_CT = 0;
            break;

        case 7:
            r_temp |= r_show_buf[3];
            // r_temp|=C_NUM_7SEG[r_show_buf[3]];
            _pin_SEG_H |= r_temp;
            _pin_SEG_H_CT &= ~r_temp;
            _pin_COM4    = 0;
            _pin_COM4_CT = 0;
            break;

        case 8:
            r_temp |= r_show_led_buf[0];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;
            _pin_SEG1    = 0;
            _pin_SEG1_CT = 0;
            break;

        case 9:

            r_temp |= r_show_led_buf[1];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;

            _pin_SEG2    = 0;
            _pin_SEG2_CT = 0;
            break;

        case 10:
            r_temp |= r_show_led_buf[2];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;
            _pin_SEG3    = 0;
            _pin_SEG3_CT = 0;
            break;

        case 11:
            r_temp |= r_show_led_buf[3];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;
            _pin_SEG4    = 0;
            _pin_SEG4_CT = 0;
            break;

        case 12:
            r_temp |= r_show_led_buf[4];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;
            _pin_SEG5    = 0;
            _pin_SEG5_CT = 0;
            break;

        case 13:
            r_temp |= r_show_led_buf[5];
            _pin_COM |= r_temp;
            _pin_COM_CT &= ~r_temp;
            _pin_SEG6    = 0;
            _pin_SEG6_CT = 0;
            break;

        default:
            r_scan_num = 0;
            break;
    }
}

//==============================================
//**********************************************
// PTM1 CCRP INTERRUPT
//==============================================
#pragma vector Interrupt_PTM1_CCRP @0x18
void Interrupt_PTM1_CCRP()
{
    _nop();
}

//==============================================
//**********************************************
// PTM1 CCRA INTERRUPT
//==============================================
#pragma vector Interrupt_PTM1_CCRA @0x1C
void Interrupt_PTM1_CCRA()
{
    //

    if (b_KEY_SEND)  // ��ʼ�������
    {
        if (r_SEND_BIT_TIME)
        {
            r_SEND_BIT_TIME--;
            _pin_SEND_D = 0;
            if (b_PIN_ST)
                _pin_SEND_C = 1;
            else
                _pin_SEND_C = 0;
        }
        else
        {
            if (b_PIN_ST)
            {
                if (r_SEND_KEY_COUNT < 65)
                {
                    r_SEND_KEY_COUNT++;
                }
                else
                {
                    b_KEY_SEND = 0;
                    r_KEY_DELAY_TIME = c_KEY_DELAY_TIME;  //  ������ɺ���ʱ10ms... ���������� //---2014/7/19 12:34
                }

                b_PIN_ST = 0;
                if (r_send_buf & 0x80)
                    r_SEND_BIT_TIME = 7;  // 1: -> LOW 8*125uS
                else
                    r_SEND_BIT_TIME = 3;  // 0: -> LOW 4*125uS
            }
            else
            {
                b_PIN_ST = 1;
                if (r_send_buf & 0x80)
                    r_SEND_BIT_TIME = 3;  // 1: -> HIGH  4*125uS
                else
                    r_SEND_BIT_TIME = 7;  // 0: -> HIGH  8*125uS
// r_send_buf=r_send_buf<<1;											// BUF��������һλ, ������һBIT����
#asm
                rl _r_send_buf
#endasm
            }
        }
    }

    //-----------------
    //������������
    //

    //-----------------
    //��������----�������� �� ���ݸ��� �г�ͻ�������ӿ���λ
    _pin_RECEIVE_C = 1;
    if (_pin_RECEIVE_D)
    {
        if (b_pin_RECEIVE_D)  // �ϴ� ���սŵ�״̬
        {                     // 1->1
            r_PIN_RECEIVE_HIGH_TIME++;
            if (r_PIN_RECEIVE_HIGH_TIME > 15)  // �ߵ�ƽʱ��̫��, ��ʾ��������, ��ʼ��BIT�����Ĵ���
            {
                r_RECEIVE_NUM_COUNT     = 0;
                r_PIN_RECEIVE_HIGH_TIME = 0;
            }
        }
        else
        {  // 0->1
            b_pin_RECEIVE_D         = 1;
            r_PIN_RECEIVE_HIGH_TIME = 0;  // ��¼�ߵ�ƽ�Ĵ�����ʼ��
            // if(r_RECEIVE_NUM_COUNT<64)
            if (r_RECEIVE_NUM_COUNT < 16)
            {
                if (b_receive_ok == 0)  //  �������ǰ �ƶ�BITλ, ������յ�����
                {
                    _status = _status & 0xfe;
#asm
                    rlc _r_receive_buf[0]      // CHECKSUM
                        rlc _r_receive_buf[4]  //---r_show_buf[3]	    //-----dspbuf3
                        rlc _r_receive_buf[3]  //---r_show_buf[2]	    //-----dspbuf2
                        rlc _r_receive_buf[2]  //---r_show_buf[1]	    //-----dspbuf1
                        rlc _r_receive_buf[1]  //---r_show_buf[0]	    //-----dspbuf0
                        rlc _r_receive_buf[7]  //---r_show_led_buf[2] //-----ledbuf3
                        rlc _r_receive_buf[6]  //---r_show_led_buf[1] //-----ledbuf2
                        rlc _r_receive_buf[5]  //---r_show_led_buf[0] //-----ledbuf1
#endasm

                        if (r_PIN_RECEIVE_LOW_TIME <= C_0_H &&
                            r_PIN_RECEIVE_LOW_TIME >= C_0_L)  // �͵�ƽ�Ĵ�����Χ�� 250uS-500uS֮��, ��ʾ��BITΪ 0
                    {
                        r_receive_buf[0] |= 0x00;
                    }
                    else
                    {
                        if (r_PIN_RECEIVE_LOW_TIME >= C_1_L &&
                            r_PIN_RECEIVE_LOW_TIME <= C_1_H)  // �͵�ƽ�Ĵ�����Χ�� 750uS-1000uS֮��, ��ʾ��BITΪ 1
                        {
                            r_receive_buf[0] |= 0x01;
                        }
                        else
                        {
                            r_RECEIVE_NUM_COUNT = 0;
                        }
                    }

                    r_RECEIVE_NUM_COUNT++;
                }
                else  //  ��� b_receive_ok ��Ϊ 0, ˵����һ�����ݵ���δ���, �������µ���������
                {
                    r_RECEIVE_NUM_COUNT    = 0;
                    r_PIN_RECEIVE_LOW_TIME = 0;
                }
            }
            else
            {
                b_receive_ok        = 1;  //  ���ݽ������ ����ȷ... �˱�־���ڶ���ʾ����ĸ���
                r_Adjustment_num    = 6;
                r_RECEIVE_NUM_COUNT = 0;
            }
        }
    }
    else
    {
        if (b_pin_RECEIVE_D)  // �ϴ� ���սŵ�״̬
        {                     // 1->0
            b_pin_RECEIVE_D        = 0;
            r_PIN_RECEIVE_LOW_TIME = 0;
        }
        else
        {  // 0->0
            if (r_PIN_RECEIVE_LOW_TIME < 15)
                r_PIN_RECEIVE_LOW_TIME++;
        }
    }

    if (b_receive_ok)  // ������ȷ�󣬽����յ����ݵ�������ʾBUF��
    {
        if (r_Adjustment_num > 0)
        {
            r_Adjustment_num--;
            if (r_Adjustment_num < 4)  //��������---��������
            {
                r_show_buf[r_Adjustment_num] = 0;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x02)
                    r_show_buf[r_Adjustment_num] |= SEG_A;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x080)
                    r_show_buf[r_Adjustment_num] |= SEG_B;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x020)
                    r_show_buf[r_Adjustment_num] |= SEG_C;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x08)
                    r_show_buf[r_Adjustment_num] |= SEG_D;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x04)
                    r_show_buf[r_Adjustment_num] |= SEG_E;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x01)
                    r_show_buf[r_Adjustment_num] |= SEG_F;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x040)
                    r_show_buf[r_Adjustment_num] |= SEG_G;
                if (r_receive_buf[r_Adjustment_num + 1] & 0x010)
                    r_show_buf[r_Adjustment_num] |= SEG_DP;  // SEG_H;
            }

            r_temp = r_receive_buf[(r_Adjustment_num / 2) + 5];
            if (r_Adjustment_num % 2)
                r_temp = r_temp >> 4;
            r_temp1                          = (r_temp << 1) & 0x10;
            r_temp                           = r_temp & 0x07;
            r_show_led_buf[r_Adjustment_num] = r_temp1 | r_temp;
        }
        else
        {
            b_receive_ok     = 0;
            r_Adjustment_num = 6;
        }
    }
}

//==============================================
//**********************************************
//==============================================
void USER_PROGRAM_INITIAL()
{
    _papu = 0x88;
    _pcpu = 0;
    _pdpu = 0;

    _pac = 0xff;
    _pcc = 0xff;
    _pdc = 0xff;
    _pc  = 0xff;
    _pd  = 0xff;
    _pa  = 0xff;

    _lvdc = 0x07;

    _slcdc0 = 0x00;
    _slcdc1 = 0x00;
    _slcdc2 = 0x00;
    _slcdc3 = 0x00;
    _tmpc   = 0x00;  // PTM ������ܿ��ƼĴ���

    _usr     = 0x00;
    _ucr1    = 0x00;
    _ucr2    = 0x00;
    _txr_rxr = 0x00;
    _brg     = 0x00;

    _sledc0 = 0xff;  // IO���������趨�����
    _sledc1 = 0xff;  // IO���������趨�����

    _pscr = 0x00;  // ʱ��-ʱ��Դ���üĴ���
    _ctrl = 0x10;  // 0x00: 8MHz, 0x10:16MHz , 0x02: 12MHz, 0x11:8MHz

    _iicc0 = 0x00;
    _iicc1 = 0x00;
    _iicd  = 0x00;
    _iica  = 0x00;

    //;---------��ʱ��TM0����

    _ctm0al = 1000 & 0xff;  // // ���ֽڵĲ���, ��������Ӧ�ĸ��ֽڲ���ִ��ʱ���� //
    _ctm0ah = 1000 / 256;   //  CCRA���ڣ� (256*_tm0ah + _tm0al)��TM0ʱ������

    _ctm0c0 = 0x00;   // CTM0ʱ�ӣ� Fsys/4
    _ctm0c1 = 0x0c1;  //   ��ʱ����ģʽ;  CCRAƥ��;  ��ʱʱ�� 1000*4/16MHz = 250uS

    _ct0on = 1;

    //;--------

    _ptm0al = 500 & 0xff;  // // ���ֽڵĲ���, ��������Ӧ�ĸ��ֽڲ���ִ��ʱ���� //
    _ptm0ah = 500 / 256;   //  CCRA���ڣ� (256*_tm0ah + _tm0al)��TM0ʱ������

    _ptm0c0 = 0x00;   // CTM0ʱ�ӣ� Fsys/4
    _ptm0c1 = 0x0c1;  //   ��ʱ����ģʽ;  CCRAƥ��;  ��ʱʱ�� 500*4/16MHz = 125uS

    _pt0on = 1;
    //;----------INTCG
    _ints1 = 1;
    _ints0 = 1;  // _integ[1:0]:INT0������, 11:˫��
                 //;----------INTC0
    // INTC0[6:4]: �ж������־�� _int0f, _ocpf, _ovpf
    _inte = 0;  // INTC0.1: INT0 �жϿ���λ
    _emi  = 1;  // INTC0.0: ���ж�
                //;----------MFI1
    //  INTC1[5:4] : �ж������־:
    //  INTC1[1:0] : �жϿ��Ʊ�־:
    _ctma0e = 1;
    _ptma1e = 1;

    r_receive_buf[0] = 0x00;
    r_receive_buf[1] = 0xff;
    r_receive_buf[2] = 0xff;
    r_receive_buf[3] = 0xff;
    r_receive_buf[4] = 0xff;
    r_receive_buf[5] = 0xff;
    r_receive_buf[6] = 0xff;
    r_receive_buf[7] = 0xff;
    b_receive_ok     = 1;
    r_Adjustment_num = 6;
}

//==============================================
//**********************************************
//==============================================
void USER_PROGRAM()
{
    if (b_2ms)
    {
        r_2ms = 0;
        BS82C16A_LIBV413();
    }

    if (SCAN_CYCLEF)
    {
        GET_KEY_BITMAP();

        if (r_KEY_STATUS1 != DATA_BUF[0] || r_KEY_STATUS2 != DATA_BUF[1])
        {
            r_KEY_STATUS1       = DATA_BUF[0];
            r_KEY_STATUS2       = DATA_BUF[1];
            r_KEY_DEBOUNCE_TIME = 0;
        }
        else
        {
            if (r_KEY_DEBOUNCE_TIME < C_KEY_DEBOUNCE_TIME)
            {
                r_KEY_DEBOUNCE_TIME++;
            }
            else
            {
                if (r_KEY_BUF[0] != r_KEY_STATUS1 || r_KEY_BUF[1] != r_KEY_STATUS2)
                {
                    // b_KEY_SEND=0;														// ���°������ݹ�����,
                    // ������
                    r_KEY_BUF[0] = r_KEY_STATUS1;
                    r_KEY_BUF[1] = r_KEY_STATUS2;
                    b_KEY_ACTION = 1;
                    // b_KEY_SEND=1;														// ���°���������ɺ�, ����
                }
            }
        }
    }

    if (r_KEY_BUF[0] == 0x01 && r_KEY_BUF[1] == 0)
    {
        r_temp2           = 0xA1;
        r_show_led_buf[1] = 0x01;
    }
    if (r_KEY_BUF[0] == 0x02 && r_KEY_BUF[1] == 0)
    {
        r_temp2           = 0xA2;
        r_show_led_buf[2] = 0x10;
    }
    if (r_KEY_BUF[0] == 0x04 && r_KEY_BUF[1] == 0)
    {
        r_temp2           = 0xA3;
        r_show_led_buf[3] = 0x10;
    }
    if (r_KEY_BUF[0] == 0x08 && r_KEY_BUF[1] == 0)
        r_temp2 = 0xA4;
    if (r_KEY_BUF[0] == 0x010 && r_KEY_BUF[1] == 0)
        r_temp2 = 0xA5;
    if (r_KEY_BUF[0] == 0x020 && r_KEY_BUF[1] == 0)
        r_temp2 = 0xA6;
    if (r_KEY_BUF[0] == 0x040 && r_KEY_BUF[1] == 0)
        r_temp2 = 0xA7;
    if (r_KEY_BUF[0] == 0x080 && r_KEY_BUF[1] == 0)
        r_temp2 = 0xA8;
    if (r_KEY_BUF[1] == 0x01 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xA9;
    if (r_KEY_BUF[1] == 0x02 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAA;
    if (r_KEY_BUF[1] == 0x04 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAB;
    if (r_KEY_BUF[1] == 0x08 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAC;
    if (r_KEY_BUF[1] == 0x010 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAD;
    if (r_KEY_BUF[1] == 0x020 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAE;
    if (r_KEY_BUF[1] == 0x040 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xAF;
    if (r_KEY_BUF[1] == 0x080 && r_KEY_BUF[0] == 0)
        r_temp2 = 0xA0;
    if (r_KEY_BUF[1] == 0 && r_KEY_BUF[0] == 0)
    {
        r_temp2           = 0x00;
        r_show_led_buf[1] = 0x00;
        r_show_led_buf[2] = 0x00;
        r_show_led_buf[3] = 0x00;
    }

    if (b_KEY_SEND == 0)
        r_send_buf = r_temp2;  // �ڲ������ʱ��, ���°���ֵ
}

/*
����  ����---->��������-->����----->��������
      ����---->��������-->����----->��������
*/