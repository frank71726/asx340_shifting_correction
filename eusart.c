#include "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include <p18cxxx.h>
#else
//none
#endif
#include <usart.h>

#if _HI_TECH
#elif	_PIC_C18

//***********************************************
//*   Send a ASCII string to Terminal           *
//***********************************************
static void Send_CR_LF(void)
{
	while (BusyUSART());
	WriteUSART('\n');
	while (BusyUSART());
	WriteUSART('\r');
}
//void Display_Terminal(const rom far char *data)	
static void display_terminal( INT8C *data )
{	
	Send_CR_LF();
	putsUSART(data);
//	Delay10KTCYx(10);
}

//************************************************
//* Function: Initialize USART                   *
//*   - Initialize the RS-232 communication      *
//*   - Disable Interrupr for TxD                *
//*   - Enable Interrupt fro Rxd                 *
//*   - Format: 9600, N, 8, 1                    *
//************************************************
static void initialize_usart(void)
{
	OpenUSART( USART_TX_INT_OFF 	// Set TXSTA Reg. =0b00100100 
			& USART_RX_INT_ON	// Set RCSTA Reg. =0b10010000 
			& USART_ASYNCH_MODE
			& USART_EIGHT_BIT
			& USART_CONT_RX
			& USART_BRGH_HIGH
			,103);			// Set SPBRG=103, Baud Rate = 9600

	RCSTAbits.SPEN=1;
	TRISCbits.TRISC7=1;		// Set input for RXD
	TRISCbits.TRISC6=0;		// Set output for TXD
	PIE1bits.TXIE=0;
	PIR1bits.RCIF=0;
	IPR1bits.RCIP=1;       		// Set Receive of USART are High priority
	PIE1bits.RCIE=1;		// Enable RxD Interrupt
}
//=================================================================================================
void McuEusartWr( INT8C *data )
{
	display_terminal(data);
}
#else
//none
#endif


void McuEusartInit(void)
{
	initialize_usart();
}
//=================================================================================================
