#ifndef _DRVFLASHEEPROM_H_INCLUDED
#define _DRVFLASHEEPROM_H_INCLUDED

//===================================================================== INCLUDES
#include <ARMLib/Common/ComDef.h>
#include <ARMLib/Device/Flash/DrvFlash.h>


//====================================================================== DEFINES
typedef enum
{
	_DRVFLASHEEPROM_CHIPNAME_VOID,
	_DRVFLASHEEPROM_CHIPNAME_M95512,	// 512KBit >> 64KByte >> 0x10000
	_DRVFLASHEEPROM_CHIPNAME_M95M01  	// 1MBit >> 128KByte >> 0x20000
}_drvflasheeprom_ChipName_e;

typedef struct
{
#if defined(_STM32H7XX_) || defined(_STM32G4XX_)
	SPI_HandleTypeDef *PtrSpiHandleTypeDef;
#endif

	_drvflasheeprom_ChipName_e ChipName;

	void (*PtrFunSetCS)(bool status);	// Ptr a fun necessaria sse il CS deve essere pilotato manualmente

	bool InitOk;			// a true sse modulo corrattemente inizializzato
}_drvflasheeprom_Init_t;

//============================================================= GLOBAL VARIABLES
extern _drvflasheeprom_Init_t _drvflasheeprom_Init;
extern const drvflash_Driver_t _drvflasheeprom_Driver;

//============================================================ GLOBAL PROTOTYPES
int32_t _drvflasheeprom_InitModule(void);

#endif /* _DRVFLASHEEPROM_H_INCLUDED */

