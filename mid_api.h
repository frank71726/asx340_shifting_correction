#ifndef _MID_API_H
#define	_MID_API_H

//================================================================================================================
//cmos object
#define	CmosClassMember_Init(OBJ)	void (*Sen_Init)(struct OBJ* )
#define CmosClassMember_Read		void (*Sen_Read)( INT8U const, INT8U const, INT8U * const, INT8U const)
#define CmosClassMember_Delay		void (*Delay)(INT16U const)
#define CmosClassMember_MirrorCtr	void (*MirrorCtr)(INT8U const)
//#define CmosMembers(OBJ)	//c18 need put value, gcc don't need value			

#if _HI_TECH
#define CmosClassMember_Write		void (*Sen_Write)( INT8U const *, INT8U const *, INT8U const *,  INT8U const *, INT16U )
#elif _PIC_C18
#define CmosClassMember_Write		void (*Sen_Write)( rom INT8U const *, rom INT8U const *, rom INT8U const *, rom INT8U const *, INT16U )
#else
//none
#endif

#define CmosClassMember_RamWrite	void (*Sen_RamWrite)( INT8U *, INT8U *, INT8U *, INT8U *, INT16U )

//===================================================================================
//mcu object
#define McuPeripheralNum		3//peripheral(depend on project),timer2,i2c,eusart
#define McuClassMember_Init(OBJ)	void (*Cpu_Init)(struct OBJ* ) 
#define McuClassMember_GetUartFifoFalg	volatile INT8U (*Cpu_GetUartFifoFlag)(void)  
#define McuClassMember_GetUartFifoData	INT8U (*Cpu_GetUartFifoData)(void)  
#define McuClassMember_Peripheral	void (*Cpu_PeriInit[McuPeripheralNum])(void) 

#if _HI_TECH
#define McuClassMember_UartWr		void (*Cpu_UartWr)( INT8S const * )  
#elif _PIC_C18
#define McuClassMember_UartWr		void (*Cpu_UartWr)( INT8C * )  
#else
//none
#endif

//================================================================================================================

struct	_AutoMachine;

typedef struct	_AutoMachineClass
{
	CmosClassMember_Init( _AutoMachine ); 
	CmosClassMember_Read;
	CmosClassMember_Write;	
	CmosClassMember_RamWrite;	
	McuClassMember_Init( _AutoMachine );
	McuClassMember_UartWr;
	McuClassMember_GetUartFifoFalg;
	McuClassMember_GetUartFifoData;
	McuClassMember_Peripheral;	
}AutoMachineClass;

typedef struct _AutoMachine
{
	rom AutoMachineClass *class;
	
	//CmosMembers(_AutoMachine);
	INT8U	gainval[2];
	INT8U	expoval[2];

	struct _flag{
		INT8U	mirror : 1;
		INT8U	       : 7;
	}flag;
	
}AutoMachine;

/* for gcc
AutoMachineClass automachineclass = 
{
	.Init = aaa,
	.Read = aaa
};
*/

static void SensorInit( AutoMachine *am_val);
//mcu peripheral function initialization
static void McuInit( AutoMachine *am_val);
void McuI2CInit(void);
void McuTimerInit(void);
void McuEusartWr( INT8C *);
void McuEusartInit(void);
INT8U McuGetUartFifoData(void);
volatile INT8U McuGetUartFifoFlag(void);

#pragma romdata
rom AutoMachineClass __automachineclass = 
{
	SensorInit,
	ASX340Read, //not ready
	WriteCmosConfig,
	RamWriteCmosConfig,
	McuInit,
	McuEusartWr,//peripheral->Eusart wirte
	McuGetUartFifoFlag,//peripheral->get uard fifo flag
	McuGetUartFifoData,//peripheral->get uard fifo data
	McuI2CInit,//peripheral->I2C
	McuTimerInit,//peripheral->Timer2
	McuEusartInit//peripheral->Eusart
};

#pragma romdata




#endif
