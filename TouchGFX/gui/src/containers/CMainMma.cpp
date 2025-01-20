#include <gui/containers/CMainMma.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
};

#define CMAINMIG_MSMENUPRINCIPALE 1500

CMainMma::CMainMma()
{
	ValShowSX=0xffff;	// Tanto per forzare un primo refresh

	tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	cPC.setVisible(false);
	cPCPainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	ManagerTickEvent();	// Prima generata forzata
}

CMainMma::~CMainMma()
{
}


void CMainMma::initialize()
{
    CMainMmaBase::initialize();
}

void CMainMma::ManagerTickEvent(void)
{
	int16_t sA;
	int32_t lA;
	char tmpStr[20];
	uint16_t uSA;
	uint32_t uLA;

	//________________________________________________________Valuta dove andare
	uSA = tgfxcustom_EncoderStatus[ENCODER_CENTER].MsTempoPressione;
	if(uSA>100)
	{
		if(!cPC.isVisible())
			cPC.setVisible(true);

		uLA = uSA*100;
		uLA /= CMAINMIG_MSMENUPRINCIPALE;
		cPC.setValue((int16_t)uLA);
	}
	else
	{
		if(cPC.isVisible())
		{
			cPC.setVisible(false);
			cPC.invalidate();
		}
	}
	switch(tgfxcustom_GetPressioneEncoder(ENCODER_CENTER))
	{
		default:
		break;
		case ENCODER_PRESSION_NOT_PRESSED:
		break;
		case ENCODER_PRESSION_PRESSED:
		break;
		case ENCODER_PRESSION_RELEASED:
			application().VisualizzaSSetup();
		break;
		case ENCODER_PRESSION_LONG_PRESSED:
			application().VisualizzaSMenuPrincipale();
		break;
		case ENCODER_PRESSION_VERY_LONG_PRESSED:
		break;
	}

	//__________________________________________________________Valuta cosa fare
	sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
	if(sA)
	{
		sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MMA_CORRENTE]);
		data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MMA_CORRENTE],sA);
	}

	//___________________________________________________________Visualizzazione
	lA = data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MMA_CORRENTE]);
	if(ValShowSX!=lA)
	{
		ValShowSX = lA;
		customstring_FormatFix(tmpStr, ValShowSX, viperui_StrutturaDati[VIPERUI_DATA_MMA_CORRENTE].NumInteri, viperui_StrutturaDati[VIPERUI_DATA_MMA_CORRENTE].NumDecimali, ' ',',', false);
		Unicode::strncpy(tAValSXBuffer,tmpStr, TAVALSX_SIZE-1);
		tAValSX.invalidate();
	}
}
