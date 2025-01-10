#ifndef __DRVFLASHOSPI_H_INCLUDED
#define __DRVFLASHOSPI_H_INCLUDED

//===================================================================== INCLUDES
#include <ARMLib/Common/ComDef.h>
#include <ARMLib/Device/Flash/DrvFlash.h>

//====================================================================== DEFINES
typedef struct
{
	uint32_t SectorSize;		// Size di ogni settore (tipicamente 64KB o  128KB) NOTA: E' la somma dei due chip
	uint32_t NumSectors;		// Quanti settori presenti NOTA: E' la somma dei due chip
	uint32_t PageSizeWrite;		// Dimensione della pagone di scrittura. NOTA: E' la somma dei due chip

	uint32_t AddressMapping;	// Address mapping della QSPI (tipicamente QSPI_BASE)

#if defined(_STM32H7XX_)
	OSPI_HandleTypeDef *PtrOspiHandleTypeDef;
#else
	#error "_DrvFlashOSPI.h : Microprocessore NON supportato. Implementarlo!!!"
#endif

	bool InitOk;					// a true sse modulo corrattemente inizializzato
}_drvflashospi_Init_t;
 
//============================================================= GLOBAL VARIABLES
extern _drvflashospi_Init_t _drvflashospi_Init;
extern drvflash_Driver_t _drvflashospi_Driver;

//============================================================ GLOBAL PROTOTYPES
int32_t _drvflashospi_InitModule(void);
int32_t _drvflashospi_GoMemoryMapping(void);
int32_t _drvflashospi_ChipResetMemory(void);

#endif /* __DRVFLASHOSPI_H_INCLUDED */

