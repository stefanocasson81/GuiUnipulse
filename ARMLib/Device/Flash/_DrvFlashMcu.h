#ifndef __DRVFLASHMCU_H_INCLUDED
#define __DRVFLASHMCU_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Device/Flash/_DrvFlashMcu.h"
#include "ARMLib/Device/Flash/DrvFlash.h"

//====================================================================== DEFINES
typedef struct
{
	bool InitOk;					// a true sse modulo corrattemente inizializzato
	uint32_t AddressMapping;		// Address mapping della flash
}_drvflashmcu_Init_t;

//============================================================= GLOBAL VARIABLES
extern _drvflashmcu_Init_t _drvflashmcu_Init;
extern drvflash_Driver_t _drvflashmcu_Driver;


//============================================================ GLOBAL PROTOTYPES
int32_t _drvflashmcu_InitModule(void);
int32_t _drvflashmcu_GetSizeSectorFromAddress(uint32_t address);
int32_t _drvflashmcu_EraseSector(uint32_t address, uint32_t dimData);
int32_t _drvflashmcu_WriteData(uint32_t address, void *ptrData, uint32_t dimData);

#endif /* __DRVFLASHMCU_H_INCLUDED */

