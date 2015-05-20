#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#include    <timers.h>
#else
//none
#endif

#include    "timer2.h"

#if _HI_TECH

#define T2_PS_1_16     0b00000010  //Timer2 Prescale 1:16
#define T2_POST_1_10   0b01001000  //Timer2 Postscaler 1:10

void TimerInit(void)
{
	PIE1bits.TMR2IE = 1;              
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	/***********************************/
	/*        Interrupt Time	   */ 		
	/*(1/(16Mhz/4)) (16*10*(24+1)) = 1mS */
	/***********************************/
	T2CON = (0xfb & (T2_PS_1_16 | T2_POST_1_10));  // Set all configuration values, but

	PR2 = 24;  //1ms by cyh

	T2CONbits.TMR2ON = _OFF; // Turn off Timer2
}

#elif	_PIC_C18

static void TimerInit(void)
{
	//  T1CONbits.TMR1ON = _OFF;
	RCONbits.IPEN=1;			// Enable Interrupt Priority bit
	IPR1bits.TMR2IP=1;			// Set Timer2 for High Priority
	INTCONbits.GIEH=1;			// Enable High Priority Interrupt

	/***********************************/
	/*        Interrupt Time	   */ 		
	/*(1/(16Mhz/4)) (16*10*(24+1)) = 1mS */
	/***********************************/
	OpenTimer2 ( TIMER_INT_ON		// Turn On the Timer2 with Interrupt
			& T2_PS_1_16
			& T2_POST_1_10);

	PR2 = 24;//1ms by cyh

	T2CONbits.TMR2ON = _OFF; // Turn off Timer2
}

//connect from outside
void McuTimerInit(void)
{
	TimerInit();
}
#else
//none
#endif

