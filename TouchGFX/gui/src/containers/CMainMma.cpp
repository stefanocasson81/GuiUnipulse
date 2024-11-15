#include <gui/containers/CMainMma.hpp>

extern "C"
{
	#include "CORELib/Viper/UserInterface/ViperUI.h"
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	#include "CORELib/Viper/ViperDef.h"
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
	uSA = tgfxcustom_EncoderStatus[TGFXCUSTOM_ENCODER_CENTER].MsTempoPressione;
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
	switch(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_CENTER))
	{
		default:
		break;
		case TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO:
		break;
		case TGFXCUSTOM_PRESSIONEENCODER_PREMUTO:
		break;
		case TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO:
			application().VisualizzaSSetup();
		break;
		case TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO:
			application().VisualizzaSMenuPrincipale();
		break;
		case TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO:
		break;
	}

	//__________________________________________________________Valuta cosa fare
	sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
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
