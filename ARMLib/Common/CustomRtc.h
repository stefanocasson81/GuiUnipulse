#ifndef CUSTOMRTC_H_INCLUDED
#define CUSTOMRTC_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
typedef struct
{
#if defined(_STM32H7XX_)
	RTC_HandleTypeDef *PtrHrtc;
#endif

	void (*PtrFunSetCalendarioAttendibile)(void);
	bool (*PtrFunGetCalendarioAttendibile)(void);

	bool InitOk;		// a true sse modulo correttamente inizializzato
}customrtc_Init_t;


typedef enum
{
	CUSTOMRTC_GIORNODELLASETTIMANA_LUN ,
	CUSTOMRTC_GIORNODELLASETTIMANA_MAR ,
	CUSTOMRTC_GIORNODELLASETTIMANA_MER ,
	CUSTOMRTC_GIORNODELLASETTIMANA_GIO ,
	CUSTOMRTC_GIORNODELLASETTIMANA_VEN ,
	CUSTOMRTC_GIORNODELLASETTIMANA_SAB ,
	CUSTOMRTC_GIORNODELLASETTIMANA_DOM ,
				 
	CUSTOMRTC_GIORNODELLASETTIMANA_MAX
}customrtc_GiornoDellaSettimana_e;

typedef enum
{
	CUSTOMRTC_MESE_GEN,
	CUSTOMRTC_MESE_FEB,
	CUSTOMRTC_MESE_MAR,
	CUSTOMRTC_MESE_APR,
	CUSTOMRTC_MESE_MAG,
	CUSTOMRTC_MESE_GIU,
	CUSTOMRTC_MESE_LUG,
	CUSTOMRTC_MESE_AGO,
	CUSTOMRTC_MESE_SET,
	CUSTOMRTC_MESE_OTT,
	CUSTOMRTC_MESE_NOV,
	CUSTOMRTC_MESE_DIC,

	CUSTOMRTC_MESE_MAX,
}customrtc_Mese_e;


typedef struct 
{
	uint8_t Secondi;			// 0..59
	uint8_t Minuti; 			// 0..59
	uint8_t Ore;				// 0..23
	uint8_t GiornoDelMese;		// 1..31
	customrtc_GiornoDellaSettimana_e GiornoDellaSettimana;
	customrtc_Mese_e Mese;
	uint16_t Anno;				// 0..4095
}customrtc_Time_t;

//============================================================= GLOBAL VARIABLES
extern customrtc_Init_t customrtc_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t customrtc_InitModule(void);

int32_t customrtc_SetTime(customrtc_Time_t *ptrTime);
int32_t customrtc_GetTime(customrtc_Time_t *ptrTime);

#endif /* CUSTOMRTC_H_INCLUDED */

