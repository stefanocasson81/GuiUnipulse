#ifndef EEDATASTORAGE_H_INCLUDED
#define EEDATASTORAGE_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
typedef struct
{     
	void *PtrData;
	uint32_t OffsetData;
	uint32_t SizeData;
	bool FlgCrc16ETagCheck;		// Se a true analizza crc16 (primi 2 byte) e tag finale Tagaa556699 (4 byte fissi a H55aa9966
	bool FlgAutoLoadInit;   	// Auto caricamento da init modulo
}eedatastorage_MappaturaEEProm_t;

typedef struct
{
	uint32_t EETag;
	
	const eedatastorage_MappaturaEEProm_t *PtrArrayMappaturaEEProm;
	uint8_t NumMappature;
	
	bool (*PtrFunCheckValidataParametri)(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm);
	void (*PtrFunSetDefaultParametri)(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm);
	
	int32_t (*PtrFunReadData)(void* ptrDest,void* ptrOrg,uint32_t size);
	int32_t (*PtrFunWriteData)(void* ptrDest,void* ptrOrg,uint32_t size);
	
	bool InitOk;		// a true sse modulo correttamente inizializzato
}eedatastorage_Init_t;

//============================================================= GLOBAL VARIABLES
extern eedatastorage_Init_t eedatastorage_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t eedatastorage_InitModule(void);
bool eedatastorage_Write(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm);
int32_t eedatastorage_Read(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm);

#endif /* EEDATASTORAGE_H_INCLUDED */

