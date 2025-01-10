/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Gestore standard RTC con le tipiche primitive classiche per set e get orologio.

	USO & ABUSO:

	bool SettaOrologio=false;
	customrtc_Time_t time;
	//....
	customrtc_InitModule();
	
	if(SettaOrologio)
	{
		SettaOrologio = false;

		time.Secondi = 30;
		time.Minuti = 59;
		time.Ore = 23;
		time.GiornoDelMese = 30;
		time.GiornoDellaSettimana = CUSTOMRTC_GIORNODELLASETTIMANA_MAR;
		time.Mese = 6;
		time.Anno = 2018;
		
		customrtc_SetTime(&time);
	}
	
	customrtc_GetTime(&time);

	

------------------------------------------------------------------------------*/


//===================================================================== INCLUDES
#include "ARMLib/Common/CustomRtc.h"
#include <integer.h>

#if defined (STM32F4XX)
#elif defined(STM32H750xx) || defined(STM32H743xx)
	//#include "stm32h7xx_hal_rtc.h"
	extern RTC_HandleTypeDef hrtc;
#elif defined(_STM32H7XX_)

#else
	#error "DbgRtc.c : Microprocessore NON supportato. Implementarlo!!!"
#endif



//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
customrtc_Init_t customrtc_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t customrtc_InitModule(void)
{
#if defined (STM32F4XX)
	RTC_InitTypeDef rTCInitStruct;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;                            // enable clock for Power interface
	PWR->CR      |= PWR_CR_DBP;                                   // enable access to RTC, BDC registers

	PWR_BackupAccessCmd(ENABLE);
	RCC_LSEConfig(RCC_LSE_ON);	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	
	
	rTCInitStruct.RTC_HourFormat = RTC_HourFormat_24;
	rTCInitStruct.RTC_AsynchPrediv = 127;
	rTCInitStruct.RTC_SynchPrediv = 255;
	
	RTC_WriteProtectionCmd(DISABLE);
	RTC_Init(&rTCInitStruct);
	
	customrtc_Init.InitOk = true;
#elif defined(_STM32H7XX_)
	if(customrtc_Init.PtrHrtc)
		customrtc_Init.InitOk = true;
	else
		customrtc_Init.InitOk = false;
#endif
	
	if(customrtc_Init.InitOk)
		return _STATUS_OK_;
	else
		return _STATUS_FAIL_;
}

//______________________________________________________________________________
// Desc:  Setta l'ora nell'RTC
// Arg: - ptrTime: Ptr alla struttura customrtc_Time_t
// Ret: - _SATUS_OK se impostazione ok altrimenti _STATUS_FAIL_
//______________________________________________________________________________
int32_t customrtc_SetTime(customrtc_Time_t *ptrTime)
{
#if defined (STM32F4XX)
	RTC_TimeTypeDef rTCTimeStruct;
	RTC_DateTypeDef rTCDateStruct;
#endif

	int32_t valRet = _STATUS_FAIL_;
	
	if(ptrTime && customrtc_Init.InitOk)
	{
		// Un minimo di controllo
		if(ptrTime->Secondi > 59)
			ptrTime->Secondi = 59;

		if(ptrTime->Minuti > 59)
			ptrTime->Minuti = 59;

		if(ptrTime->Ore > 23)
			ptrTime->Ore = 23;

		if(ptrTime->GiornoDelMese)
		{
			if(ptrTime->GiornoDelMese > 31)
				ptrTime->GiornoDelMese = 31;
		}
		else
			ptrTime->GiornoDelMese = 1;

		if(ptrTime->GiornoDellaSettimana >= CUSTOMRTC_GIORNODELLASETTIMANA_MAX)
			ptrTime->GiornoDellaSettimana = CUSTOMRTC_GIORNODELLASETTIMANA_DOM ;

		if(ptrTime->Mese)
		{
			if(ptrTime->Mese >= CUSTOMRTC_MESE_MAX)
				ptrTime->Mese = CUSTOMRTC_MESE_GEN;
		}
		else
			ptrTime->Mese = CUSTOMRTC_MESE_GEN;

		if(ptrTime->Anno > 4095)
			ptrTime->Anno = 4095; 
#if defined (STM32F4XX)
		rTCTimeStruct.RTC_Seconds 	= ptrTime->Secondi;
		rTCTimeStruct.RTC_Minutes	= ptrTime->Minuti;
		rTCTimeStruct.RTC_Hours		= ptrTime->Ore;
		rTCDateStruct.RTC_Date		= ptrTime->GiornoDelMese;
		rTCDateStruct.RTC_WeekDay	= 1+ptrTime->GiornoDellaSettimana;
		rTCDateStruct.RTC_Month		= 1+ptrTime->Mese;
		rTCDateStruct.RTC_Year		= ptrTime->Anno%100;
		
		RTC_SetTime(RTC_Format_BIN,&rTCTimeStruct);
		RTC_SetDate(RTC_Format_BIN,&rTCDateStruct);
		
		valRet = _STATUS_OK_;
#elif defined(_STM32H7XX_)

		RTC_TimeTypeDef sTime;
		RTC_DateTypeDef sDate;
		
		sTime.Seconds = ptrTime->Secondi;
		sTime.Minutes = ptrTime->Minuti;
		sTime.Hours 	= ptrTime->Ore;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_SET;

		sDate.WeekDay = 1 + ptrTime->GiornoDellaSettimana;
		sDate.Month = 1 + ptrTime->Mese;
		sDate.Date = ptrTime->GiornoDelMese;
		sDate.Year = ptrTime->Anno%100;

		if (HAL_RTC_SetTime(customrtc_Init.PtrHrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
		{
			if (HAL_RTC_SetDate(customrtc_Init.PtrHrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
			{
				valRet = _STATUS_OK_;

				if(customrtc_Init.PtrFunSetCalendarioAttendibile)
					customrtc_Init.PtrFunSetCalendarioAttendibile();
			}
		}
#endif
	}
	
	return valRet;
}

//______________________________________________________________________________
// Desc:  Popola la struttura customrtc_Time_t passata con l'ora attuale
// Arg: - ptrTime: Ptr alla struttura customrtc_Time_t da popolare
// Ret: - _STATUS_OK_ se lettura valida, altrimenti _STATUS_FAIL_
//______________________________________________________________________________
int32_t customrtc_GetTime(customrtc_Time_t *ptrTime)
{
	int32_t valRet = _STATUS_FAIL_;
	uint8_t uca; 
	
	if(ptrTime && customrtc_Init.InitOk)
	{
#if defined (STM32F4XX)
		RTC_TimeTypeDef rTCTimeStruct;
		RTC_DateTypeDef rTCDateStruct;
		RTC_GetTime(RTC_Format_BIN,&rTCTimeStruct);
		RTC_GetDate(RTC_Format_BIN,&rTCDateStruct);
		
		ptrTime->Secondi	= rTCTimeStruct.RTC_Seconds;
		ptrTime->Minuti     = rTCTimeStruct.RTC_Minutes;
		ptrTime->Ore        = rTCTimeStruct.RTC_Hours;
		ptrTime->GiornoDelMese        = rTCDateStruct.RTC_Date;
		ptrTime->GiornoDellaSettimana = (customrtc_GiornoDellaSettimana_e)(rTCDateStruct.RTC_WeekDay-1);
		ptrTime->Mese                 = (customrtc_Mese_e)(rTCDateStruct.RTC_Month-1);
		ptrTime->Anno                 = 2000+rTCDateStruct.RTC_Year;
		
	
		valRet = _STATUS_OK_;
#elif defined(_STM32H7XX_)
		RTC_TimeTypeDef sTime;
		RTC_DateTypeDef sDate;

		if(HAL_RTC_GetTime(customrtc_Init.PtrHrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
		{
			if(HAL_RTC_GetDate(customrtc_Init.PtrHrtc, &sDate, RTC_FORMAT_BIN) == HAL_OK)
			{
				ptrTime->Secondi							= sTime.Seconds;
				ptrTime->Minuti               = sTime.Minutes;
				ptrTime->Ore                  = sTime.Hours;
				ptrTime->GiornoDelMese        = sDate.Date;
				uca = sDate.WeekDay;
				if(uca)
					uca--;
				ptrTime->GiornoDellaSettimana = (customrtc_GiornoDellaSettimana_e)uca;
				uca = sDate.Month;
				if(uca)
					uca--;
				ptrTime->Mese                 = (customrtc_Mese_e)uca;
				ptrTime->Anno                 = sDate.Year + 2000;
				valRet = _STATUS_OK_;

				if(customrtc_Init.PtrFunGetCalendarioAttendibile)
				{
					if(!customrtc_Init.PtrFunGetCalendarioAttendibile())
						valRet = _STATUS_FAIL_;
				}
			}
		}
#endif		
	}
	
	return valRet;
}


//============================================================ PRIVATE FUNCTIONS
