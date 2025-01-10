#ifndef DATA8PACK_H_INCLUDED
#define DATA8PACK_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
#define DATA8PACK_DIMDATA 	8
				
#define DATA8PACK_NOAUTOTX	0xffff	// Abbinato al var MsToSend indica la non trasmissione automatica
#define DATA8PACK_FREETIME	0xfffe	// Abbinato al var MsToSend indica di trasmettere quando libero e con calma

//s#define DATA8PACK_ALLID		0xffff	// Inteso come tutt gli id della struttura

typedef struct
{
	uint16_t Id;
	uint16_t MsToSend;			// Ogni quanti ms ritrasmettere
	int8_t (*PtrFunExeceute)(void *ptrStructData);		// Eventuale funzione da eseguire post SET o pre GET
	struct
	{
		void* PtrData;
		uint8_t Size;
	}DataEsploso[DATA8PACK_DIMDATA];
}data8pack_Data_t;

typedef struct
{
	const data8pack_Data_t *PtrStructDataTX;	// Ptr alla struttura data da spedire
	const data8pack_Data_t *PtrStructDataRX;	// Ptr alla struttura data da ricevere
	
	uint16_t MsFreeTimeSend;	// Cadenza di trasmissione in free time

	bool InitOk;		// a true sse modulo correttamente inizializzato
}data8pack_Init_t;

//============================================================= GLOBAL VARIABLES
extern data8pack_Init_t data8pack_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t data8pack_InitModule(void);
void data8pack_Manager1MS(void);
const data8pack_Data_t* data8pack_PopData(void);
bool data8pack_ForceTX(uint16_t id);
void data8pack_ForceImmediateTX(uint16_t id);
void data8pack_ForceImmediateTXFastIndex(uint16_t index);
bool data8pack_IsIdImmediateTXFreeToSend(uint16_t id);
int16_t data8pack_GetIndexFromId(uint16_t id);
bool data8pack_IsIdFreeToSend(uint16_t id);
uint8_t data8pack_ZipData(uint8_t *ptrData, data8pack_Data_t *ptrData8PackData);
uint8_t data8pack_UnzipData(data8pack_Data_t *ptrData8PackData, uint8_t *ptrData);

#endif /* DATA8PACK_H_INCLUDED */

