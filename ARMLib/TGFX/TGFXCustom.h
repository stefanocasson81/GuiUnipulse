#ifndef TGFXCUSTOM_H_INCLUDED
#define TGFXCUSTOM_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
//#include "ARMLib/FileSystem/DrvFileSystem.h"
//====================================================================== DEFINES

#define TGFXCUSTOM_MSHELPME							2000



typedef enum
{
	TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO			,
	TGFXCUSTOM_PRESSIONEENCODER_PREMUTO				,
	TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO		,	// <800ms
	TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO		,   // 2000ms
	TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO	,   // 4000ms

	TGFXCUSTOM_PRESSIONEENCODER_MAX
}tgfxcustom_PressioneEncoder_e;

typedef enum
{
	TGFXCUSTOM_ENCODER_LEFT		,
	TGFXCUSTOM_ENCODER_CENTER	,
	TGFXCUSTOM_ENCODER_RIGHT	,

	TGFXCUSTOM_ENCODER_MAX
}tgfxcustom_Encoder_e;

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

	void (*PtrFunBeep)(uint16_t mSBeepON);
	void (*PtrFunGetStatusEncoder)(tgfxcustom_Encoder_e numEncoder,bool *ptrPremuto,int16_t *ptrCounter);
}tgfxcustom_Init_t;

typedef struct
{
	int16_t OffsetEncoder;					// Offset rotazione
	uint32_t MsTempoPressione;				// Se zero pressione NON in atto
	uint32_t MsTempoPressionePrecedente;	// Il tempo che era stato premuto il tasto precedentemente

	uint32_t MsAttivityStartPress;			// In quale ms è stato premuto

	bool Premuto;							// Se è premuto
	bool PressioneConfermata;				// Se filtro di tasto premuto passato
}tgfxcustom_EncoderStatus_t;

//============================================================= GLOBAL VARIABLES
extern tgfxcustom_Init_t tgfxcustom_Init;
extern tgfxcustom_EncoderStatus_t tgfxcustom_EncoderStatus[TGFXCUSTOM_ENCODER_MAX];


//============================================================ GLOBAL PROTOTYPES
int32_t tgfxcustom_InitModule(void);
void tgfxcustom_Manager1MS(void);
void tgfxcustom_ResetEvent(tgfxcustom_Encoder_e numEncoder);
bool tgfxcustom_GetIfRefresh(uint16_t msRefresh);
int16_t tgfxcustom_GetOffsetEncoder(tgfxcustom_Encoder_e encoder);
tgfxcustom_PressioneEncoder_e tgfxcustom_GetPressioneEncoder(tgfxcustom_Encoder_e encoder);
//int32_t tgfxcustom_SaveFrameBuffer(void* ptrFrameBuffer,uint16_t dimPixelX,uint16_t dimPixelY,drvfilesystem_Driver_t *ptrCustomFsDriver,char* ptrStrNomeFile);

#endif /* TGFXCUSTOM_H_INCLUDED */

