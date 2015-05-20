#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#else
//none
#endif

#include    "i2c.h"

//#define SCL PORTCbits.RC0
//#define SDA PORTCbits.RC1
//#define SCL_TRIS TRISCbits.TRISC0
//#define SDA_TRIS TRISCbits.TRISC1

#if _HI_TECH

extern void Time2Count(INT16U num);
static unsigned char i2c_dly_num=0;

void I2CDelay(void)
{
	Time2Count(MS_1);
	i2c_dly_num++;
	if(i2c_dly_num >= MS_3 )
	{
		NOP();
		NOP();
		NOP();
		RESET();
	}
}

void I2CDlyClr(void)
{
	i2c_dly_num = 0;	
}
//********************************************************************
// As below are I2C function
// These functions were modified from C18 I2C Libyaries' Source Code
//********************************************************************
//**** Test and wait until I2C module is idle.
void IdleI2C( void ) 				
{
	while ( ( SSP1CON2 & 0x1F ) || ( SSP1STATbits.R_nW ) )
	{
		I2CDelay();
	}
	I2CDlyClr();
}

//Send I2C bus start condition.
void StartI2C( void )
{
	SSP1CON2bits.SEN = 1;            // initiate bus start condition

	while(SSP1CON2bits.SEN == 1)//;20140725
	{
		NOP();
		I2CDelay();
	}
	I2CDlyClr();
}

//This routine writes a single byte to the I2C bus.
unsigned char WriteI2C( unsigned char data_out )	
{
	SSPBUF = data_out;           	// write single byte to SSPBUF

	if (SSP1CONbits.WCOL)      	// test if write collision occurred
		return ( -1 );          // if WCOL bit is set return negative #
	else
	{
		while(SSP1STATbits.BF)//;	// wait until write cycle is complete         
		{
			I2CDelay();
		}
		I2CDlyClr();

		IdleI2C();                 	// ensure module is idle
		if ( SSP1CON2bits.ACKSTAT ) 	// test for ACK condition received
		{
			return ( -2 );
		}
		else
		{
			return ( 0 );          // if WCOL bit is not set return non-negative #
		}
	}
}

unsigned char ReadI2C( void )
{
	SSP1CON2bits.RCEN = 1;           // enable master for 1 byte reception
	while ( !SSP1STATbits.BF )//;      // wait until byte received  
	{
		I2CDelay();
	}
	I2CDlyClr();

	return ( SSPBUF );              // return with read byte 
}

void AckI2C( void )
{
	SSP1CON2bits.ACKDT = 0;           // set acknowledge bit state for ACK
	SSP1CON2bits.ACKEN = 1;           // initiate bus acknowledge sequence
}

void NotAckI2C( void )
{
	SSP1CON2bits.ACKDT = 1;          // set acknowledge bit for not ACK
	SSP1CON2bits.ACKEN = 1;          // initiate bus acknowledge sequence
}

void RestartI2C( void )
{
	SSP1CON2bits.RSEN = 1;           // initiate bus restart condition
	while(SSP1CON2bits.RSEN == 1)
	{
		I2CDelay();
	}
	I2CDlyClr();

}
void StopI2C( void )
{
	SSP1CON2bits.PEN = 1;            // initiate bus stop condition
	while(SSP1CON2bits.PEN == 1)
	{
		I2CDelay();
	}
	I2CDlyClr();
}

void I2C_Done(void)
{
	while(!SSP1IF)//;	// Completed the action when the SSPIF is Hi.
	{
		I2CDelay();
	}
	I2CDlyClr();

	SSP1IF=0;   		// Clear SSPIF
}

//************************************************
//* Initial I2C Master Mode with 7 bits Address  *
//* Clock Speed : 400KHz @16MHz			 *
//************************************************
void Initialize_I2C_Master(void)
{
	SCL=1;			// Initial PortC,bit 3 & 4 as Input with Open Drain
	SDA=1;			// RC0 = SCL  ,  RC1 = SDA
	SCL_TRIS =1;			 
	SDA_TRIS =1;

	SSPCON = 0x28;		// select serial mode 
	//SSPSTAT = 0x80; 	// 1 = Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
	SSPSTAT = 0x00; 	// 0 = Slew rate control enabled for high speed mode (400 kHz)
	SSP1ADD= 9;		// (16MHz/4) / (9+1)= 400KHz 
	SSP1MSK= 0xFF;
}

#elif	_PIC_C18

/* SSPCON1 REGISTER */
#define   SSPENB    			0b00100000  	/* Enable serial port and configures SCK, SDO, SDI*/
#define   SLAVE_7   			0b00000110     	/* I2C Slave mode, 7-bit address*/
#define   SLAVE_10  			0b00000111    	/* I2C Slave mode, 10-bit address*/
#define   MASTER    			0b00001000     	/* I2C Master mode */
#define   MASTER_FIRMW			0b00001011	//I2C Firmware Controlled Master mode (slave Idle)
#define   SLAVE_7_STSP_INT 		0b00001110	//I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
#define   SLAVE_10_STSP_INT 		0b00001111	//I2C Slave mode, 10-bit address with Start and Stop bit interrupts enabled

/* SSPSTAT REGISTER */
#define   SLEW_OFF  			0b10000000  	/* Slew rate disabled for 100kHz mode */
#define   SLEW_ON   			0b00000000  	/* Slew rate enabled for 400kHz mode  */

extern void Time2Count(unsigned int num);
static unsigned char i2c_dly_num = 0;
static unsigned char i2c_dly_num1 = 0;

static void I2CDelay(void)
{
	Time2Count(MS_1);
	i2c_dly_num++;
	if(i2c_dly_num >= MS_3)
	{
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Reset();
	}

}
static void I2CDlyClr(void)
{
	i2c_dly_num = 0;	
}

void NotAckI2C( void )
{
	SSPCON2bits.ACKDT = 1;          // set acknowledge bit for not ACK
	SSPCON2bits.ACKEN = 1;          // initiate bus acknowledge sequence

	while(SSPCON2bits.ACKEN == 1)
	{
		I2CDelay();
	}
	I2CDlyClr();
}

void StopI2C( void )
{
	SSPCON2bits.PEN = 1;            // initiate bus stop condition

	while(SSPCON2bits.PEN == 1)
	{
		I2CDelay();
	}
	I2CDlyClr();
}

void StartI2C( void )
{
	SSPCON2bits.SEN = 1;            // initiate bus start condition

	while(SSPCON2bits.SEN == 1)
	{
		I2CDelay();
	}
	I2CDlyClr();
		Nop();
		Nop();
		Nop();
}

void IdleI2C( void )
{
	while ( ( SSPCON2 & 0x1F ) || ( SSPSTATbits.R_W ) )
	{
		I2CDelay();
	}
	I2CDlyClr();
		Nop();
		Nop();
		Nop();
}

INT8U ReadI2C( void )
{
	if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
		SSPCON2bits.RCEN = 1;           // enable master for 1 byte reception
	while ( !SSPSTATbits.BF )//;      // wait until byte received  
	{
		I2CDelay();
	}
	I2CDlyClr();

	return ( SSPBUF );              // return with read byte 
}


INT8U getsI2C( INT8U *rdptr, INT8U length )
{
	while ( length-- )           // perform getcI2C() for 'length' number of bytes
	{
		*rdptr++ = ReadI2C();   // save byte received
		while ( SSPCON2bits.RCEN )//; // check that receive sequence is over
		{
			I2CDelay();
		}
		I2CDlyClr();

		if ( PIR2bits.BCLIF )       // test for bus collision
		{
			return ( -1 );            // return with Bus Collision error 
		}

		if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
		{	
			if ( length )               // test if 'length' bytes have been read
			{
				SSPCON2bits.ACKDT = 0;    // set acknowledge bit state for ACK        
				SSPCON2bits.ACKEN = 1;    // initiate bus acknowledge sequence
				while ( SSPCON2bits.ACKEN )//; // wait until ACK sequence is over 
				{
					I2CDelay();
				}
				I2CDlyClr();
			} 
		} 

	}
	return ( 0 );                 // last byte received so don't send ACK      
}


INT8S WriteI2C( INT8U data_out )
{
	SSPBUF = data_out;           // write single byte to SSPBUF
	if ( SSPCON1bits.WCOL )      // test if write collision occurred
		return ( -1 );              // if WCOL bit is set return negative #
	else
	{
		if( ((SSPCON1&0x0F)!=0x08) && ((SSPCON1&0x0F)!=0x0B) )	//Slave mode only
		{
			SSPCON1bits.CKP = 1;        // release clock line 
			while ( !PIR1bits.SSPIF )//;  // wait until ninth clock pulse received
			{
				I2CDelay();
			}
			I2CDlyClr();

			if ( ( !SSPSTATbits.R_W ) && ( !SSPSTATbits.BF ) )// if R/W=0 and BF=0, NOT ACK was received
			{
				return ( -2 );           //return NACK
			}
			else
			{
				return ( 0 );				//return ACK
			}	
		}
		else if( ((SSPCON1&0x0F)==0x08) || ((SSPCON1&0x0F)==0x0B) )	//master mode only
		{ 
			while( SSPSTATbits.BF )//;   // wait until write cycle is complete   
			{
				I2CDelay();
			}
			I2CDlyClr();

			IdleI2C();                 // ensure module is idle
			if ( SSPCON2bits.ACKSTAT ) // test for ACK condition received
				return ( -2 );			// return NACK
			else return ( 0 );              //return ACK
		}
	}
}
//***********************************************
//*   Check I2C action that is completed        *
//***********************************************
void I2C_Done(void)
{
	while (!PIR1bits.SSPIF)		// Completed the action when the SSPIF is Hi.
	{
		Nop();
		Nop();
		Nop();
		I2CDelay();
	}
	I2CDlyClr();
		Nop();
		Nop();
		Nop();
	PIR1bits.SSPIF=0;   		// Clear SSPIF
}

#if defined (I2C_V1)

void OpenI2C( INT8U sync_mode, INT8U slew )
{
	SSPSTAT &= 0x3F;                // power on state 
	SSPCON1 = 0x00;                 // power on state
	SSPCON2 = 0x00;                 // power on state
	SSPCON1 |= sync_mode;           // select serial mode 
	SSPSTAT |= slew;                // slew rate on/off 

	I2C_SCL = 1;
	I2C_SDA = 1;
	SSPCON1 |= SSPENB;              // enable synchronous serial port 
}
#endif

#if defined (I2C_V4)

void OpenI2C( INT8U sync_mode, INT8U slew )
{
	SSPSTAT &= 0x3F;                // power on state 
	SSPCON = 0x00;                 // power on state
	SSPCON |= sync_mode;           // select serial mode 
	SSPSTAT |= slew;                // slew rate on/off 

	I2C_SCL = 1;
	I2C_SDA = 1;
	SSPCON |= SSPENB;              // enable synchronous serial port 

}
#endif
//************************************************
//* Initial I2C Master Mode with 7 bits Address  *
//* Clock Speed : 100KHz @12MHz			 *
//************************************************
static void Initialize_I2C_Master(void)
{
	OpenI2C(MASTER,SLEW_OFF);
	//	SSPADD=29;//16 MHz
	SSPADD=39;
}

//for outside connect
void McuI2CInit(void)
{
	Initialize_I2C_Master();
}

#else
//none
#endif

