/*******************************************************************************
 *@Project: Auto machine
 *@MCU:Pic18f25K20
 *@Complier:v3.43
 *@Author:Frank.Chiang
 *@Comment: 1. Oscillator:16MHz
 *          2.   {                -------
 *                    I2CDelay();       | using this routine to avoid it mcu
 *               }                      | cann't get ACK signal from other slave IC.
 *                    I2CDlyClr();-------
 *	    3. RS232 : 9600, 8, N, 1, X,
 *	    4.乘10：（x << 3） + （x << 1）   == x*8 + x*2 ==  x*(8+2)
 *	      除10 : (x >> 3) +   (x >> 1)    == x/8 + x/2 ==  x/(8+2)
 *v1.0:20140811 by frank - draft
 *******************************************************************************/
#include "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include <p18cxxx.h>
#else
//none
#endif

#include "mid_aptina.h"
#include "mid_api.h"
#include "automachine_asx340_para.h"

#if _HI_TECH
#elif	_PIC_C18

#pragma config	IESO = OFF
#pragma config	FCMEN = OFF
#pragma config	FOSC = HS
#pragma config	XINST = OFF
#pragma config	WDTEN = OFF
//#pragma config	WDTPS = 4096
#pragma config	PWRT  = OFF
#pragma config	BOREN = OFF
#pragma config	STVREN = OFF
#pragma config	LVP = OFF
#else
//none
#endif

INT8C MSG_init[]="hello world!";
//INT8C MSG_left[]="a - LEFT";
//INT8C MSG_leftest[]="The leftest";
//INT8C MSG_rightest[]="The rightest";
//INT8C MSG_right[]="d - RIGHT";
//INT8C MSG_other[]="Please input 'a'(LEFT) or 'd'(RIGHT)";

//for Horizontal
INT8U ASX340_offset_hi[4]={ 0x09, 0xC8, 0xFC, 0x00 };
INT8U ASX340_offset_lo[4]={ 0x8E, 0x5C, 0x00, 0x40 };
INT8U ASX340_offset_data[7]={ 0xC8, 0x5C, 0x00, 0x28, 0x00, 0x81, 0x00 };
INT8U ASX340_offset_num[4]={ 0x02, 0x01, 0x02, 0x02 };
//==========================================================================
static void SensorInit( AutoMachine *am_val)
{
	am_val->class->Sen_Write( ASX340_hi, ASX340_lo, ASX340_data, ASX340_num, sizeof(ASX340_hi)/sizeof(ASX340_hi[0]) );

	if(am_val->flag.mirror == 1 )//mirror
	{
		am_val->class->Sen_Write(ASX340_mirror_hi, ASX340_mirror_lo, ASX340_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}else//no mirror
	{
		am_val->class->Sen_Write( ASX340_mirror_hi, ASX340_mirror_lo, ASX340_no_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}
}
//==========================================================================
static void McuInit( AutoMachine *am_val)
{
	INT8U count;

	//peripheral initialization
	for( count=0 ; count < McuPeripheralNum	; count++ )
	{
		am_val->class->Cpu_PeriInit[count]();
	}

	//falg setting
	am_val->flag.mirror = 0;//no mirror
}

//==========================================================================
void main(void)
{
	AutoMachineClass automachineclass;
	AutoMachine	 am;
	INT8U	dir_udrl, rs232_temp, rec_count=0, rs232_rx_fifo_not_empty_flag ;

	am.class = &__automachineclass;

	am.class->Cpu_Init(&am);
	am.class->Sen_Init(&am);

	am.class->Cpu_UartWr(MSG_init);
	while(1)
	{
		rs232_rx_fifo_not_empty_flag = am.class->Cpu_GetUartFifoFlag();
		if( rs232_rx_fifo_not_empty_flag == 1 )
		{
			rs232_temp = am.class->Cpu_GetUartFifoData();

			switch ( rec_count )
			{
				case 0 :
					if((rs232_temp == 'H')||(rs232_temp == 'V'))
					{
						dir_udrl = rs232_temp;
						rec_count++;
					}
					break;
				case 1 :
					switch ( dir_udrl )
					{
						case 'V' :
							if(((rs232_temp>= 0x00)&&(rs232_temp<= 0x24)) ||\
								((rs232_temp<= 0xFF)&&(rs232_temp>= 0xDC)))
							{ //sending i2c to sensor optical_offset_Vertical
								ASX340_offset_data[1] = 0x5d;//for verticalo
								ASX340_offset_lo[1]=0x5d;
								ASX340_offset_data[2] = rs232_temp;
							}
							else
								rec_count = 0;
							break;
						case 'H' :
							if(((rs232_temp>= 0x00)&&(rs232_temp<= 0x29)) ||\
								((rs232_temp<= 0xFF)&&(rs232_temp>= 0xD7)))
							{ //sending i2c to sensor optical_offset_Herizontal
								ASX340_offset_data[1] = 0x5c;//for herizontal
								ASX340_offset_lo[1]=0x5c;
								ASX340_offset_data[2] = rs232_temp;
							}
							else
								rec_count = 0;
							break;
						default :
							rec_count = 0;
							break;

					}
					if( rec_count != 0 )
						am.class->Sen_RamWrite( ASX340_offset_hi, ASX340_offset_lo, ASX340_offset_data,\
							ASX340_offset_num, sizeof(ASX340_offset_hi)/sizeof(ASX340_offset_hi[0]) );

					rec_count = 0;
					break;
				default :
					break;
			}
		}
	}
}

/*
//==========================================================================
void puthexUart(INT8U HEX_Val, AutoMachine *am_val )
{
	INT8C Temp_HEX[]={"0x00"};

	Temp_HEX[2] = (HEX_Val >> 4) & 0x0f ;

	if ( Temp_HEX[2] > 9 )
		Temp_HEX[2] += 0x37 ;
	else
		Temp_HEX[2] += 0x30 ;

	Temp_HEX[3] = HEX_Val  & 0x0f ;
	if ( Temp_HEX[3] > 9 )
		Temp_HEX[3] += 0x37 ;
	else
		Temp_HEX[3] += 0x30 ;

	am_val->class->Cpu_UartWr(Temp_HEX);
}

