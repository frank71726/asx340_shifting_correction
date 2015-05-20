#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#else
//none
#endif
////////////////////////////////////////////////////////////////////////////////////////
/* rs232 fifo                                                                         */
////////////////////////////////////////////////////////////////////////////////////////
#define FIFO_BUFFER_SIZE 50 // software buffer size (in bytes)

typedef struct _sw_fifo_typedef
{
	INT8U data_buf[FIFO_BUFFER_SIZE]; // FIFO buffer
	INT8U i_first;                    // index of oldest data byte in buffer
	INT8U i_last;                     // index of newest data byte in buffer
	INT8U num_bytes;                  // number of bytes currently in buffer

	struct _falg{
		volatile INT8U uart_rx_fifo_not_empty_flag : 1;
		volatile INT8U uart_rx_fifo_full_flag : 1;     
		volatile INT8U uart_rx_fifo_ovf_flag : 1;      
		volatile INT8U uart_tx_fifo_full_flag : 1;     
		volatile INT8U uart_tx_fifo_ovf_flag : 1;      
		volatile INT8U uart_tx_fifo_not_empty_flag : 1;
		volatile INT8U : 2; 
	}flag;

}sw_fifo_typedef;

sw_fifo_typedef rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
sw_fifo_typedef tx_fifo = { {0}, 0, 0, 0 }; // declare a transmit software buffer


static INT8U get_uart_fifo_data(void)
{
	INT8U data = 0;

	if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE) 
	{ // if the sw buffer is full
		rx_fifo.flag.uart_rx_fifo_full_flag = 0;	// clear the buffer full flag because we are about to make room
	}
	if(rx_fifo.num_bytes > 0)
	{// if data exists in the sw buffer
		data = rx_fifo.data_buf[rx_fifo.i_first]; 	// place oldest data element in the return buffer
		rx_fifo.i_first++;                        	// increment the index of the oldest element
		rx_fifo.num_bytes--;                      	// decrement the bytes counter
	}
	if(rx_fifo.i_first == FIFO_BUFFER_SIZE)
	{// if the index has reached the end of the buffer,
		rx_fifo.i_first = 0;                     	 // roll over the index counter
	}
	if(rx_fifo.num_bytes == 0)
	{// if no more data exists
		rx_fifo.flag.uart_rx_fifo_not_empty_flag = 0;    // clear flag
	}

	return data;
}

static volatile INT8U get_uart_fifo_flag(void)
{
	return rx_fifo.flag.uart_rx_fifo_not_empty_flag;
}

////////////////////////////////////////////////////////////////////////////////////////
INT8U McuGetUartFifoData(void)
{
	return get_uart_fifo_data();
}

volatile INT8U McuGetUartFifoFlag(void)
{
	return get_uart_fifo_flag();
}
////////////////////////////////////////////////////////////////////////////////////////
#if _HI_TECH

volatile static unsigned char gTimeFalg = _OFF;
volatile static unsigned int gTimerCount = 0;

void Time2Count(unsigned int num)
{
	gTimerCount = num;
	T2CONbits.TMR2ON = _ON;     // Turn on Timer
	while(gTimeFalg != _ON );
	gTimeFalg = _OFF;
}

	void interrupt
isr(void)			// Here be interrupt function - the name is unimportant.
{
	volatile static unsigned int count=0;

	if(TMR2IF)
	{
		if (count < (gTimerCount-1)) //because first times not count
		{
			count++;    
		}
		else
		{   
			T2CONbits.TMR2ON = _OFF; // Turn off Timer2	
			count = 0;
			gTimeFalg = _ON; 
			TMR2IF=0;
		}
	}
}

#elif	_PIC_C18
#include <usart.h>
//void Time2Count(unsigned int num);
volatile static INT8U gTimeFalg = _OFF;
volatile static INT16U gTimerCount = 0;

void Time2Count( INT16U num)
{
	gTimerCount = num;
	T2CONbits.TMR2ON = _ON;     // Turn on Timer
	while(gTimeFalg != _ON );
	gTimeFalg = _OFF;
}
//************************************************
//*       #pragma Interrupt Declarations         *
//*												 *
//*   Function: isr_high_direct                  *
//*   - Direct execution to the actual           *
//*     high-priority interrupt code.            *
//************************************************
#pragma code isrhighcode = 0x0008

void isr_high(void);
void isr_high_direct(void)	
{  								
	_asm 		//begin in-line assembly
		goto isr_high	//go to isr_high function
		_endasm 	//end in-line assembly
}
#pragma code

//************************************************
//*   Function: isr_high(void)                   *     
//*   High priority interrupt will               *
//*   - Received a serial data from RS-232       *
//*     Save the received data to buffer Rec_Data*                  
//************************************************
#pragma interrupt isr_high 

void isr_high(void)
{
	volatile static INT16U count=0;
	INT8U Rec_Data;

	if(PIR1bits.TMR2IF && PIE1bits.TMR2IE)	// TMR2 Interrupt
	{
		PIR1bits.TMR2IF=0;		// Clear Timer2 interrupt Flag

		if (count < (gTimerCount-1)) 	//because first times not count
			count++;    
		else
		{   
			T2CONbits.TMR2ON = _OFF; // Turn off Timer2
			count = 0;
			gTimeFalg = _ON; 
		}
	}

	if( PIR1bits.RCIF && PIE1bits.RCIE)	// RS232 Interrupt
	{
		PIR1bits.RCIF=0;

		if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE)
		{       // if the sw buffer is full
			rx_fifo.flag.uart_rx_fifo_ovf_flag = 1;  	// set the overflow flag
		}
		else if(rx_fifo.num_bytes < FIFO_BUFFER_SIZE)
		{ 	// if there's room in the sw buffer
			rx_fifo.data_buf[rx_fifo.i_last] = ReadUSART();	// Get RS-232 data; clear RCIF
			rx_fifo.i_last++;                              	// increment the index of the most recently added element
			rx_fifo.num_bytes++;                           	// increment the bytes counter
		}
		if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE) 
		{      // if sw buffer just filled up
			rx_fifo.flag.uart_rx_fifo_full_flag = 1;        // set the RX FIFO full flag
		}
		if(rx_fifo.i_last == FIFO_BUFFER_SIZE) 
		{       // if the index has reached the end of the buffer,
			rx_fifo.i_last = 0;                             // roll over the index counter
		}

		rx_fifo.flag.uart_rx_fifo_not_empty_flag = 1;	        // set received-data flag
	}
}								
#pragma code

#else
//none
#endif

#if 0
/***************************************************************************************************************/
// UART transmit interrupt sub-routine
//  - interrupts when the tx hardware buffer is empty
//  - checks if data exists in the tx software buffer
//  - if data exists, it places the oldest element of the sw buffer into the tx hardware buffer
//  - if the sw buffer is emptied, it disables the "hw buffer empty" interrupt
//  - automatically handles "uart_tx_buffer_full_flag"
//////////////////////////////////////////////
/* enter name of UART TX IRQ Handler here */ {
	//////////////////////////////////////////////

	/* Explicitly clear the source of interrupt if necessary */

	if(tx_fifo.num_bytes == FIFO_BUFFER_SIZE) { // if the sw buffer is full
		uart_tx_fifo_full_flag = 0;               // clear the buffer full flag because we are about to make room
	}
	if(tx_fifo.num_bytes > 0) {                 // if data exists in the sw buffer

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* enter pointer to UART tx hardware buffer here */ = tx_fifo.data_buf[tx_fifo.i_first]; // place oldest data element in the TX hardware buffer
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		tx_fifo.i_first++;                        // increment the index of the oldest element
		tx_fifo.num_bytes--;                      // decrement the bytes counter
	}
	if(tx_fifo.i_first == FIFO_BUFFER_SIZE) {   // if the index has reached the end of the buffer,
		tx_fifo.i_first = 0;                      // roll over the index counter
	}
	if(tx_fifo.num_bytes == 0) {                // if no more data exists

		uart_tx_fifo_not_empty_flag = 0;          // clear flag

		//////////////////////////////////////////////////////////////////////////
		/* disable UART "TX hw buffer empty" interrupt here                     */
		/* if using shared RX/TX hardware buffer, enable RX data interrupt here */
		//////////////////////////////////////////////////////////////////////////

	}
}// end UART TX IRQ handler
/***************************************************************************************************************/


/***************************************************************************************************************/
// UART data transmit function
//  - checks if there's room in the transmit sw buffer
//  - if there's room, it transfers data byte to sw buffer
//  - automatically handles "uart_tx_buffer_full_flag"
//  - sets the overflow flag upon software buffer overflow (doesn't overwrite existing data)
//  - if this is the first data byte in the buffer, it enables the "hw buffer empty" interrupt
void uart_send_byte(uint8_t byte) {

	///////////////////////////////////////////////////////////
	/* disable interrupts while manipulating buffer pointers */
	///////////////////////////////////////////////////////////

	if(tx_fifo.num_bytes == FIFO_BUFFER_SIZE) {      // no room in the sw buffer
		uart_tx_fifo_ovf_flag = 1;                     // set the overflow flag
	}else if(tx_fifo.num_bytes < FIFO_BUFFER_SIZE) { // if there's room in the sw buffer
		tx_fifo.data_buf[tx_fifo.i_last] = byte;       // transfer data byte to sw buffer
		tx_fifo.i_last++;                              // increment the index of the most recently added element
		tx_fifo.num_bytes++;                           // increment the bytes counter
	}
	if(tx_fifo.num_bytes == FIFO_BUFFER_SIZE) {      // if sw buffer is full
		uart_tx_fifo_full_flag = 1;                    // set the TX FIFO full flag
	}
	if(tx_fifo.i_last == FIFO_BUFFER_SIZE) {         // if the "new data" index has reached the end of the buffer,
		tx_fifo.i_last = 0;                            // roll over the index counter
	}

	///////////////////////
	/* enable interrupts */
	///////////////////////

	if(tx_fifo.num_bytes > 0) {                      // if there is data in the buffer

		uart_tx_fifo_not_empty_flag = 1;               // set flag

		///////////////////////////////////////////////////////////////////////////
		/* if using shared RX/TX hardware buffer, disable RX data interrupt here */
		/* enable UART "TX hw buffer empty" interrupt here                       */
		///////////////////////////////////////////////////////////////////////////

	}
}
#endif
