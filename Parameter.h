#ifndef _Parameter_H
#define _Parameter_H

#define _DEBUG		0
#define _MICROCHIP	1 //IC vendor
#define _HI_TECH	0 //complier selecting
#define _PIC_C18	1 
#define _GCC		0 
#define _RENESAS	0 //IC vendor
#define _APTINA		1 //Sensor Vendor
#define _OV		0 //

//===============================================================
//ad cselecting
#define _AD28V_10BIT    0
#define _AD33V_10BIT    1
//===============================================================
//aptina sensor selecting
#define ASX340		1
//===============================================================
#if _MICROCHIP	
#if _HI_TECH
	typedef bit	       		BIT;//C18 can't accept this define
	typedef unsigned char  		INT8U;
	typedef signed   char  		INT8S;
	typedef unsigned short 		INT16USH;
	typedef signed   short 		INT16SSH;
	typedef unsigned int   		INT16U;
	typedef signed   int   		INT16S;
	typedef unsigned short long	INT24U;
	typedef signed short long	INT24S;
	typedef unsigned long	        INT32U;
	typedef signed long	        INT32S;
#elif _PIC_C18
	typedef char	  		INT8C;
	typedef signed   char  		INT8S;
	typedef unsigned char  		INT8U;
	typedef int 	  		INT16S;
	typedef unsigned int   		INT16U;
	typedef short	 		INT16SSH;
	typedef unsigned short 		INT16USH;
	typedef short long		INT24S;
	typedef unsigned short long	INT24U;
	typedef long		        INT32S;
	typedef unsigned long	        INT32U;
#else
	//none
#endif
#elif _RENESAS
	//none
#else
	//none
#endif
//===============================================================
#if _MICROCHIP

#define	OUTPUT	0x00	//io
#define	INPUT	0x01	//io
#define WrtCmd  0	//i2c
#define RdCmd   1	//i2c
#define _ON	1
#define _OFF	0
#define HI	1	
#define LO	0	

#define _AN0	0
#define _AN1     1
#define _AN2     2
#define _AN3     3
#define _AN4     4
#define _AN5     5
#define _AN6     6
#define _AN7     7

#elif _RENESAS
	//none
#else
	//none
#endif
//===============================================================
//camera sensor I2C ID
#if _APTINA		

#if ASX340
#define     ASX340ID	0x90
//#elif
	//none
#else
	//none
#endif

#elif _OV
	//none
#else
	//none
#endif

//===============================================================
#if _AD28V_10BIT    
	/*--------------
	1. pic16f1824: ADC 10bit, 2.8v/1024 = 2.734375mV
	2. using MCP9700A (each degree will add 10mV)
	   0 degree : 500mV => 500/2.734375 = 183
	   1 degree : 510mV => 510/2.734375 = 187
	  10 degree : 600mV => 600/2.734375 = 220
	  15 degree : 650mV => 650/2.734375 = 238
	  30 degree : 800mV => 800/2.734375 = 292
	  35 degree : 850mV => 850/2.734375 = 311
	---------------*/
	enum temp_mode
	{
	    temp_high = 238,//15
	    temp_low = 220,//10
	    temp_30 = 292,//30
	    temp_35 = 311,//35
	};
#elif _AD33V_10BIT    
	/*--------------
	1. pic18f25k20: ADC 10bit, 3.3v/1024 = 3.222mV
	2. 0 degree : 500mV => 500/3.222 = 155
	   1 degree : 510mV => 510/3.222 = 158
	  50 degree : 1000mV => 1000/3.222 = 310
	   each degree will add 10mV
	---------------*/
	enum temp_mode
	{
	    temp_high = 201,//15,
	    temp_low = 186,//10
	};
#else
//none
#endif
//===============================================================
//timer
#define     MS_1       (1)
#define     MS_3       (5)

#endif
