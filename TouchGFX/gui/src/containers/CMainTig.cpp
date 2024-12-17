#include <gui/containers/CMainTig.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
	#include "ARMLib/Common/Data.h"
	#include <ViperUISupport.h>
};

#define CMAINMIG_MSMENUPRINCIPALE 1500

CMainTig::CMainTig()
{
	ValShowSX=0xffff;	// Tanto per forzare un primo refresh

	tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	cPC.setVisible(false);
	cPCPainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	ManagerTickEvent();	// Prima generata forzata
}

void CMainTig::initialize()
{
    CMainTigBase::initialize();
}

void CMainTig::ManagerTickEvent(void)
{
	int32_t lA;
	char tmpStr[10];
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
	}	switch(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_CENTER))
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

	lA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
	if(lA)
	{
		lA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_TIG_CORRENTE]);
		data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_TIG_CORRENTE],lA);
	}

	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_TIG_CORRENTE]);
	if(ValShowSX!=lA)
	{
		ValShowSX = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_TIG_CORRENTE,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tAValSXBuffer,tmpStr, TAVALSX_SIZE-1);
		tAValSX.invalidate();
	}


}
