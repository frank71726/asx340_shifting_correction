#include    "Parameter.h"

#if _HI_TECH
#include    <HTC.h>
#elif	_PIC_C18
#include    <p18cxxx.h>
#else
//none
#endif

#include    "mid_api.h"
#include    "mid_aptina.h"



void AutoMachineApiInit(AutoMachineClass *am_class,  AutoMachine *am_val )
{
	//class definition
	am_class->Init = SensorInit;
        am_class->Read = ASX340Read;
	am_class->Write = WriteCmosConfig;

	//link class
	am_val->class = am_class;

	//set mirror pin value
	am_val->MirrorVal = 0;
}
/*
void McuApiInit(McuApiStru *dstru)
{
	dstru->Init = McuInitCtr;
	dstru->Delay = McuDelayCtr;
	dstru->IRLed = McuLedSCtr;
	dstru->Heater = McuHeaterCtr;
	dstru->AdRead = McuReadA2DCtr;
	dstru->Sw12Pin = McuSw12PinCtr;
	dstru->MirrorPin = McuMirrorPinCtr;
	dstru->ResetSensor = McuResetSensor;
}
*/

