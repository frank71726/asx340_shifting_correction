#include    "Parameter.h"
#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#else
//none
#endif

#include    "mid_aptina.h"
#include    "i2c.h"

//extern void McuDelayCtr(INT16U nop_num);
//extern void I2CDelay(void);
//extern void I2CDlyClr(void);
//

#if _APTINA

#if _HI_TECH
static void write_cmos_value( INT8U const *hiaddr, INT8U const *loaddr, INT8U const *data,  INT8U const *dnum, INT16U item_num)
#elif  _PIC_C18
static void write_cmos_value( rom INT8U const *hiaddr, rom INT8U const *loaddr, rom INT8U const *data, rom INT8U const *dnum, INT16U item_num)
#else
#endif
{
	INT8S Runt;
	INT16U index;
	INT8U buffer[5];
	INT16U num=0; 

	for(index=0 ; index < item_num ; index++)
	{
		buffer[4] = dnum[index];
		buffer[0] = hiaddr[index];
		buffer[1] = loaddr[index];

		if( buffer[4] == 0x02 )
		{
			buffer[2] = data[num++];
			buffer[3] = data[num++];
		} else
		{
			buffer[2] = data[num++];
		}

		IdleI2C();
		StartI2C();
		I2C_Done();
		
		Runt = WriteI2C( ASX340ID | WrtCmd);//ID	  
		I2C_Done();					

		Runt = WriteI2C(buffer[0]);//slave address hi
		I2C_Done();	

		Runt = WriteI2C(buffer[1]);//slave address lo
		I2C_Done();						

		if( buffer[4] == 0x02 )//send data 2 or 1 byte
		{
			Runt = WriteI2C(buffer[2]);			
			I2C_Done();
			Runt = WriteI2C(buffer[3]);			
			I2C_Done();
		} 
		else
		{
			Runt = WriteI2C(buffer[2]);			
			I2C_Done();
		}
		
		StopI2C();
		I2C_Done();
		
		if( Runt < 0 )
		{
#if _HI_TECH
			RESET();
#elif	_PIC_C18
			Reset();
#else
#endif
		}
		//McuDelayCtr(50);
	}
}
//===========================
//ram i2c
static void ram_write_cmos_value( INT8U *hiaddr, INT8U *loaddr, INT8U *data, INT8U *dnum, INT16U item_num)
{
	INT8S Runt;
	INT16U index;
	INT8U buffer[5];
	INT16U num=0; 

	for(index=0 ; index < item_num ; index++)
	{
		buffer[4] = dnum[index];
		buffer[0] = hiaddr[index];
		buffer[1] = loaddr[index];

		if( buffer[4] == 0x02 )
		{
			buffer[2] = data[num++];
			buffer[3] = data[num++];
		} else
		{
			buffer[2] = data[num++];
		}

		IdleI2C();
		StartI2C();
		I2C_Done();
		
		Runt = WriteI2C( ASX340ID | WrtCmd);//ID	  
		I2C_Done();					

		Runt = WriteI2C(buffer[0]);//slave address hi
		I2C_Done();	

		Runt = WriteI2C(buffer[1]);//slave address lo
		I2C_Done();						

		if( buffer[4] == 0x02 )//send data 2 or 1 byte
		{
			Runt = WriteI2C(buffer[2]);			
			I2C_Done();
			Runt = WriteI2C(buffer[3]);			
			I2C_Done();
		} 
		else
		{
			Runt = WriteI2C(buffer[2]);			
			I2C_Done();
		}
		
		StopI2C();
		I2C_Done();
		
		if( Runt < 0 )
		{
#if _HI_TECH
			RESET();
#elif	_PIC_C18
			Reset();
#else
#endif
		}
		//McuDelayCtr(50);
	}
}
//////////////////////////////////////////////////////////////
//extern function
#if _HI_TECH
void WriteCmosConfig( INT8U const *hiaddr, INT8U const *loaddr, INT8U const *data,  INT8U const *dnum, INT16U item_num)
#elif  _PIC_C18
void WriteCmosConfig( rom INT8U const *hiaddr, rom INT8U const *loaddr, rom INT8U const *data, rom INT8U const *dnum, INT16U item_num)
#else
#endif
{
	write_cmos_value( hiaddr, loaddr, data, dnum, item_num);
}

void RamWriteCmosConfig( INT8U *hiaddr, INT8U *loaddr, INT8U *data, INT8U *dnum, INT16U item_num)
{
	ram_write_cmos_value( hiaddr, loaddr, data, dnum, item_num);
}
#elif  _OV
	// waiting for me porting	
#endif
//==========================================================================//
	/*  
static INT8S getsI2C( INT8U *rdptr, INT8U length )
{
	while ( length-- )  
	{
		*rdptr++ = ReadI2C();
		while ( SSP1CON2bits.RCEN )//;
		{
			I2CDelay();
		}
		I2CDlyClr();

		if ( PIR2bits.BCL1IF )     
		{
			return ( -1 );          
		}

		if( ((SSP1CON1&0x0F)==0x08) || ((SSP1CON1&0x0F)==0x0B) )
		{	
			if ( length )            
			{
				SSP1CON2bits.ACKDT = 0; 
				SSP1CON2bits.ACKEN = 1;
				while ( SSP1CON2bits.ACKEN )//;
				{
					I2CDelay();
				}
				I2CDlyClr();
			} 
		} 
	}
	return ( 0 );      
}

	*/
//==========================================================================//
	/*
static void ReadASX340Value( INT8U const hiaddr, INT8U const loaddr, INT8U *redata, INT8U size)
{
	INT8U Runt;

	IdleI2C();
	StartI2C();
	I2C_Done();

	Runt = WriteI2C( ASX340ID | WrtCmd);	
	I2C_Done();

	Runt = WriteI2C(hiaddr);				        
	I2C_Done();	

	Runt = WriteI2C(loaddr);				        
	I2C_Done();

	RestartI2C();
	I2C_Done();

	Runt = WriteI2C( ASX340ID | RdCmd);	  
	I2C_Done();

	getsI2C( redata, size );
	I2C_Done();

	NotAckI2C();
	I2C_Done();	  

	StopI2C();     
	I2C_Done();	
}
	*/
//==========================================================================
	/*
void ASX340Mirror( INT8U const value)
{
	if( value == HI )//mirror
	{
		Write_ASX340_Config( ASX340_mirror_hi, ASX340_mirror_lo, ASX340_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}
	else//no mirror
	{
		Write_ASX340_Config( ASX340_mirror_hi, ASX340_mirror_lo, ASX340_no_mirror_data,\
				ASX340_mirror_num, sizeof(ASX340_mirror_hi)/sizeof(ASX340_mirror_hi[0]) );
	}
}
	*/
//==========================================================================
void ASX340Read( INT8U const hiaddr , INT8U const loaddr , INT8U * const redata, INT8U const size)
{
	/*
	ReadASX340Value(hiaddr, loaddr, redata, size);
	*/
}



