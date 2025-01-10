#ifndef APPLICAZIONE_H_INCLUDED
#define APPLICAZIONE_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES



typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}applicazione_Init_t;

typedef struct
{
	uint32_t CodiceUnivocoScheda;	// Codice univoco scheda legato anche all'Id Micro
	uint32_t CodiceScheda;			// Codice della scheda esteso
	uint16_t CodiceSchedaRidotto;	// Codice scheda ridotto (Le famose 5 cifre di codifica interna Helvi)
	uint32_t Crc32Firmware;			// Crc32 del firmware

	bool ExecuteScreenShoot;	// Se a true esegui uno screenshot su USB
	bool ShowDebug;
}applicazione_Info_t;

//============================================================= GLOBAL VARIABLES
extern applicazione_Init_t applicazione_Init;
extern applicazione_Info_t applicazione_Info;

//============================================================ GLOBAL PROTOTYPES
int32_t applicazione_InitModule(void);
void applicazione_Manager1MS(void);

#endif /* APPLICAZIONE_H_INCLUDED */

