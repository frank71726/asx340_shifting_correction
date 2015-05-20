#ifndef	_MID_APTINA_H_
#define _MID_APTINA_H_

void ASX340Mirror( INT8U const value);
void ASX340Read( INT8U const hiaddr , INT8U const loaddr , INT8U * const redata, INT8U const size);
#if _HI_TECH
void WriteCmosConfig( INT8U const *hiaddr, INT8U const *loaddr, INT8U const *data,  INT8U const *dnum, INT16U item_num);
#elif  _PIC_C18
void WriteCmosConfig( rom INT8U const *hiaddr, rom INT8U const *loaddr, rom INT8U const *data, rom INT8U const *dnum, INT16U item_num);
#else
	//none
#endif
void RamWriteCmosConfig( INT8U *, INT8U *, INT8U *, INT8U *, INT16U );

#endif
