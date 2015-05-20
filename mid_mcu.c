#include "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include <p18cxxx.h>
#else
//none
#endif

//#include    "i2c.h"
//#include    "adc.h"
//#include    "mid_mcu.h"
//#include    "timer2.h"

/*
#define	Led_TRIS 		TRISAbits.TRISA5
#define Led 			LATAbits.LATA5
#define	Heater_TRIS 		TRISAbits.TRISA2
#define Heater 			LATAbits.LATA2
#define Asx340Reset_TRIS 	TRISAbits.TRISA4
#define Asx340Reset 		LATAbits.LATA4
#define	SW12PIN_TRIS 		TRISCbits.TRISC3
#define	SW12PIN_WPUC 		WPUCbits.WPUC3                
#define	SW12PIN			PORTCbits.RC3 
#define	MIRRORPIN_TRIS		TRISCbits.TRISC5
#define	MIRRORPIN		PORTCbits.RC5 

static INT8U MirrorPinStatus(void)
{
	return MIRRORPIN;
}
static INT8U SW12PinStatus(void)
{
	return SW12PIN;
}
static void LedStatus(INT8U value)
{
	Led = value;
}
static void HeaterStatus(INT8U value)
{
	Heater = value;
}
static void Delay_nop(INT16U nop_num)
{
	volatile INT16U	Delay_cnt = 0;

	while(Delay_cnt < nop_num)	
	{
		Delay_cnt++;
	}
	Delay_cnt = 0;
}

static void mcu_init(void)
{
	INT8U i;
	
	OSCCONbits.IRCF=0x0f;//internal oscillator 16MHz
	OSCCONbits.SPLLEN=0x00;              
	OSCCONbits.SCS1=0x01;//using internal clock

	//GOPIO init
	ANSELA = 0x00;  //disable ADC
	ANSELC = 0x00;  //disable ADC, microchip initialize I/O to ADC, than have to change to digital I/O
	ANSELCbits.ANSC2=0x01;//enable RC2 to AD 

	Led_TRIS = OUTPUT;
	Heater_TRIS  = OUTPUT;

	SW12PIN_TRIS = INPUT;
//	SW12PIN_WPUC = _OFF; 		

	MIRRORPIN_TRIS = INPUT;
	
//	Asx340Reset_TRIS = OUTPUT;//initial asx340 reset pin
//	Asx340Reset = LO;
//	Delay_nop(100);
//	Asx340Reset = HI;
//	Delay_nop(100);

	//I2C init
	Initialize_I2C_Master();

	//AD init
	Init_Adc();

	//timer2 init
	TimerInit();

	LedStatus(_OFF);
	HeaterStatus(_OFF);
}

static void McuToResetSensor(void)
{
	Asx340Reset = LO;
	Delay_nop(500);
	Asx340Reset = HI;
	Delay_nop(300);
}

//=============
void McuInitCtr(void)
{
	mcu_init();
}
void McuDelayCtr(INT16U nop_num)
{
	Delay_nop(nop_num);
}

void McuLedSCtr(INT8U value)
{
	LedStatus(value);
}

void McuHeaterCtr(INT8U value)
{
	HeaterStatus(value);
}

INT8U McuSw12PinCtr(void)
{
	return 	SW12PinStatus();
}

INT8U McuMirrorPinCtr(void)
{
	return 	MirrorPinStatus();
}

void McuResetSensor(void)
{
	McuToResetSensor();
}
*/
