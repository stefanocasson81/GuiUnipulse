#ifndef DRVFLASH_H_INCLUDED
#define DRVFLASH_H_INCLUDED

/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	26/06/20
 Descrizione: Layer driver per scrittura in FLASH
------------------------------------------------------------------------------*/

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES

typedef union
{
	bool Busy:1;
	bool Error:1;
}dvrflash_Status_u;

typedef struct
{
	uint32_t NumeroSettori;
	uint32_t DimensioneTotale;
}drvflash_Info_t;

typedef struct
{
	dvrflash_Status_u (*GetStatus)(void);
	drvflash_Info_t* (*GetInfo)(void);

	int32_t (*GetSizeSector)(uint32_t numSector);
	int32_t (*GetNumSectorFromAddress)(uint32_t address);
	int32_t (*ReadData)(uint32_t address, void *ptrData, uint32_t dimData);
	int32_t (*WriteData)(uint32_t address, void *ptrData, uint32_t dimData);
	int32_t (*EraseSector)(uint32_t numSector);
	int32_t (*EraseMultipleSector)(uint32_t address, uint32_t dimData);
	bool 	(*IsAddressStartSector)(uint32_t address);
	int32_t (*LockChip)(bool status);
	int32_t	(*EraseChip)(void);
}drvflash_Driver_t;



//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES

#endif /* DRVFLASH_H_INCLUDED */

