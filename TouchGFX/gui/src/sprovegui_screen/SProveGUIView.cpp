#include <gui/sprovegui_screen/SProveGUIView.hpp>

extern "C"
{
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	#include "CORELib/Viper/UserInterface/ViperUI.h"
};

char TmpBuffer[20];
static char* FunzionePocioListBox(int32_t value)
{
	sprintf(TmpBuffer,"<%d>",(int)value);

	return TmpBuffer;
}





SProveGUIView::SProveGUIView()
{
	cEditVal1.SetPtrStructData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DVELFILO]);
	cEditVal2.SetPtrStructData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO]);
	cEditVal3.SetPtrStructData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO]);

	cEditVal1.Seleziona(true);
	cEditVal2.Seleziona(false);
	cEditVal3.Seleziona(false);

	cEditVal3.PtrFunStrDedicShow = FunzionePocioListBox;

    IndiceSelezionato=0;
    IndiceMaxSelezionabile=2;
    OldIndiceSelezionato = 0xff;

    handleTickEvent();	// Primo giro per generata prima della visualizzazione
}

void SProveGUIView::setupScreen()
{
    SProveGUIViewBase::setupScreen();
}

void SProveGUIView::tearDownScreen()
{
    SProveGUIViewBase::tearDownScreen();
}

void SProveGUIView::handleTickEvent()
{
	int32_t lA;

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMenuPrincipale();

	lA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
	if(lA)
	{
		lA+=IndiceSelezionato;
		if(lA<0)
			lA = 0;
		else if(lA>IndiceMaxSelezionabile)
			lA = IndiceMaxSelezionabile;
		IndiceSelezionato = lA;
	}

	if(OldIndiceSelezionato!=IndiceSelezionato)
	{
		OldIndiceSelezionato=IndiceSelezionato;

		cEditVal1.Seleziona(false);
		cEditVal2.Seleziona(false);
		cEditVal3.Seleziona(false);

		switch(IndiceSelezionato)
		{
			case 0:
				cEditVal1.Seleziona(true);
			break;
			case 1:
				cEditVal2.Seleziona(true);
			break;
			case 2:
				cEditVal3.Seleziona(true);
			break;
		}
	}

	cEditVal1.ManagerTickEvent();
	cEditVal2.ManagerTickEvent();
	cEditVal3.ManagerTickEvent();
}
