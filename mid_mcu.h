#ifndef	_MID_VP2231_H_
#define	_MID_VP2231_H_

void McuInitCtr(void);
void McuDelayCtr(INT16U nop_num);
void McuLedSCtr(INT8U value);
void McuHeaterCtr(INT8U value);
INT8U McuSw12PinCtr(void);
INT8U McuMirrorPinCtr(void);
void McuResetSensor(void);
#endif
