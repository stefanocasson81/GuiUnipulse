#ifndef SWTIMER_H_INCLUDED
#define SWTIMER_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
typedef struct
{
	bool InitOk;                      	// a true sse modulo corrattemente inizializzato

	uint16_t NumTimerDaGestire;       	// Numero dei timer da gestire
}swtimer_Init_t;

typedef uint32_t swtimer_Time_t;
typedef int32_t swtimer_TimerHandler_t;


//============================================================= GLOBAL VARIABLES
extern volatile uint32_t swtimer_TickCounter;
extern swtimer_Init_t swtimer_Init;


//============================================================ GLOBAL PROTOTYPES
int32_t swtimer_InitModule(void);

void swtimer_Manager1MS(void);
swtimer_TimerHandler_t swtimer_Create(void);
void swtimer_Destroy(swtimer_TimerHandler_t *timerHandler);
void swtimer_Set(swtimer_TimerHandler_t timerHandler,uint32_t msTimer);
uint32_t swtimer_Test(swtimer_TimerHandler_t timerHandler);

uint32_t swtimer_GetMsActivity(void);

#endif /* SWTIMER_H_INCLUDED */
