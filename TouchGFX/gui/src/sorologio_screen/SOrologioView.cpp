#include <gui/sorologio_screen/SOrologioView.hpp>

extern "C"
{
	#include "ARMLib/Common/CustomRtc.h"
	#include "ARMLib/Common/SwTimer.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/UserInterface/ViperUI.h"
};


SOrologioView::SOrologioView()
{
	OldTickCounter = 0;
	IndiceSelezione = 0;
	OldIndiceSelezione = 0xff;

	customrtc_GetTime(&OldRtcTime);
}

void SOrologioView::setupScreen()
{
    SOrologioViewBase::setupScreen();
}

void SOrologioView::tearDownScreen()
{
    SOrologioViewBase::tearDownScreen();
}

void SOrologioView::handleTickEvent()
{
#if 0
	customrtc_Time_t localRtcTime;
	int sA;
	char tmpStr[20];
	bool invalida = false;

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
		application().VisualizzaSMenuPrincipale();	// Torna indietro

	//___________________________________________________________Cosa modificare
	sA = tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
	if(sA)
	{
		sA += IndiceSelezione;
		_LIMIT_(sA,0,6);

		IndiceSelezione = (uint8_t)sA;
	}

	if(OldIndiceSelezione!=IndiceSelezione)
	{
		OldIndiceSelezione = IndiceSelezione;

		tAGiorno.setColor(colortype(viperui_ListStdColor[Model::STDCOLOR_BIANCO]));
		tAMese.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		tAAnno.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		tATGiorno.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		tAOra.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		tAMinuti.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		tASecondi.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
		switch(IndiceSelezione)
		{
			case 0:
				tAGiorno.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 1:
				tAMese.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 2:
				tAAnno.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 3:
				tATGiorno.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 4:
				tAOra.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 5:
				tAMinuti.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
			case 6:
				tASecondi.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			break;
		}

		invalida = true;
	}

	//_____________________________________________________________Cambio valore
	customrtc_GetTime(&localRtcTime);

	sA = tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
	if(sA)
	{
		switch(IndiceSelezione)
		{
			case 0:
				sA+=localRtcTime.GiornoDelMese;
				_LIMIT_(sA,1,31);
				localRtcTime.GiornoDelMese=(uint8_t)sA;
			break;
			case 1:
				sA+=localRtcTime.Mese;
				_LIMIT_(sA,0,CUSTOMRTC_MESE_MAX-1);
				localRtcTime.Mese=(customrtc_Mese_e)sA;
			break;
			case 2:
				sA+=localRtcTime.Anno;
				_LIMIT_(sA,2000,2099);
				localRtcTime.Anno=(uint16_t)sA;
			break;
			case 3:
				sA+=localRtcTime.GiornoDellaSettimana;
				_LIMIT_(sA,0,CUSTOMRTC_GIORNODELLASETTIMANA_MAX-1);
				localRtcTime.GiornoDellaSettimana=(customrtc_GiornoDellaSettimana_e)sA;
			break;
			case 4:
				sA+=localRtcTime.Ore;
				_LIMIT_(sA,0,23);
				localRtcTime.Ore=(uint8_t)sA;
			break;
			case 5:
				sA+=localRtcTime.Minuti;
				_LIMIT_(sA,0,59);
				localRtcTime.Minuti=(uint8_t)sA;
			break;
			case 6:
				sA+=localRtcTime.Secondi;
				_LIMIT_(sA,0,59);
				localRtcTime.Secondi=(uint8_t)sA;
			break;
		}

		customrtc_SetTime(&localRtcTime);
	}

	if(memcmp(&localRtcTime,&OldRtcTime,sizeof(localRtcTime)))
	{
		memcpy(&OldRtcTime,&localRtcTime,sizeof(localRtcTime));

		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.GiornoDelMese);
		Unicode::strncpy(tAGiornoBuffer,tmpStr, TAGIORNO_SIZE-1);
		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.Mese);
		Unicode::strncpy(tAMeseBuffer,tmpStr, TAMESE_SIZE-1);
		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.Anno);
		Unicode::strncpy(tAAnnoBuffer,tmpStr, TAANNO_SIZE-1);
		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.GiornoDellaSettimana);
		Unicode::strncpy(tATGiornoBuffer,tmpStr, TATGIORNO_SIZE-1);

		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.Ore);
		Unicode::strncpy(tAOraBuffer,tmpStr, TAORA_SIZE-1);
		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.Minuti);
		Unicode::strncpy(tAMinutiBuffer,tmpStr, TAMINUTI_SIZE-1);
		snprintf(tmpStr,sizeof(tmpStr)-1,"%02d",(int)OldRtcTime.Secondi);
		Unicode::strncpy(tASecondiBuffer,tmpStr, TASECONDI_SIZE-1);

		invalida = true;
	}

	if(invalida)
	{
		tAGiorno.invalidate();
		tAMese.invalidate();
		tAAnno.invalidate();
		tATGiorno.invalidate();
		tAOra.invalidate();
		tAMinuti.invalidate();
		tASecondi.invalidate();
	}

	/*
	if(Mese)
		CustomRtcTime.Mese = (customrtc_Mese_e)(Mese-1);
	if(memcmp(&CustomRtcTime,&OldCustomRtcTime,sizeof(customrtc_Time_t)))
	{	// Se nuovo orario impostato, lo setto
		CustomRtcTime.Secondi = 0;
		customrtc_SetTime(&CustomRtcTime);
		OldCustomRtcTime = CustomRtcTime;
	}
*/
/*
	if((swtimer_TickCounter-OldTickCounter)>2000)
	{
		OldTickCounter = swtimer_TickCounter;
		// Aggiorno l'immagine dell'orologio

		customrtc_GetTime(&localRtcTime);
		analogClock.setTime24Hour(localRtcTime.Ore,localRtcTime.Minuti,localRtcTime.Secondi);
	}
	*/
#endif
}
