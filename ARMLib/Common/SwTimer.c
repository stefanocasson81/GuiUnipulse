/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Gestione timer software con allocazione dinamica anche in thread safe purchè
	da init vengano inizializzate le funzioni di semaforica.
	La base tempi di questo modulo è di 1 ms e quindi ogni riferimento sui tempi
	passati è tale.
 
	Uso e abuso:
 
	* INIT:
		swtimer_Init.NumTimerDaGestire = 10;
		swtimer_InitModule();

	* ESEMPIO USO:
 
		static swtimer_TimerHandler_t HTimerDummy;

		HTimerDummy=swtimer_Create();
		if(HTimerDummy<0)
			errore = true;
		else
		{
			swtimer_Set(HTimerDummy,1000);

			if(!swtimer_Test(!HTimerDummy))
			{
				.... // Timer scaduto
			}
		}

 
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "SwTimer.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES
static uint32_t TimerCreati=0;							// Per ottimizzazione interna...
static uint32_t *PtrAllocTimer=_NULL_; 					// Mallocata dei timer utilizzabili

//============================================================= GLOBAL VARIABLES
volatile uint32_t swtimer_TickCounter = 0;	 			// TickCounter da attivazione micro

swtimer_Init_t swtimer_Init=
{
	.NumTimerDaGestire = 10
};

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Descrizione della funzione.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t swtimer_InitModule(void)
{
	bool errore = false;
	uint32_t sizeMalloc;

	TimerCreati = 0;

	sizeMalloc = sizeof(uint32_t) * swtimer_Init.NumTimerDaGestire;
	PtrAllocTimer = _MALLOC_(sizeMalloc);
	if(PtrAllocTimer)
		memset(PtrAllocTimer, 0, sizeMalloc);
	else // Se mallocata non riuscita notifico l'errore
		errore = true;

	if(errore)
		return _STATUS_FAIL_;
	else
	{
		swtimer_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

/*______________________________________________________________________________ 
 Desc:  Crea un nuovo timer software
 Arg: - ptrFunExecute: eventuale funzione da eseguire a timer scaduto
 Ret: - >=0 se timer allocato. Se -1 errore
______________________________________________________________________________*/
swtimer_TimerHandler_t swtimer_Create()
{
	int32_t valRet = - 1;

	if(swtimer_Init.InitOk)
	{
		if(TimerCreati<swtimer_Init.NumTimerDaGestire)
		{
			PtrAllocTimer[TimerCreati] = 0;
			valRet = TimerCreati;
			TimerCreati++;
		}
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Setta i ms del timer passato
 Arg: - timerHandler: L'handle del timer da settare
			- msTimer: I ms del timer
 Ret: - Nulla
______________________________________________________________________________*/
void swtimer_Set(swtimer_TimerHandler_t timerHandler,uint32_t msTimer)
{
	if(timerHandler<TimerCreati)
		PtrAllocTimer[timerHandler] = msTimer;
}

/*______________________________________________________________________________ 
 Desc:  Ritorna i ms del timer in corso mancanti
 Arg: - timerHandler: L'handle del timer da verificare
 Ret: - Nulla
______________________________________________________________________________*/
uint32_t swtimer_Test(swtimer_TimerHandler_t timerHandler)
{
	return PtrAllocTimer[timerHandler];
}

//============================================================ PRIVATE FUNCTIONS

/*******************************************************************************
 Desc:  Da eseguire su timer base tempi 1 ms
*******************************************************************************/
void swtimer_Manager1MS(void)
{
	uint32_t indice;
	
	swtimer_TickCounter++;

	for(indice = 0; indice < TimerCreati;indice++)
	{
		if(PtrAllocTimer[indice])
			PtrAllocTimer[indice]--;
	}
}

