#include <gui/ssplashscreen_screen/SSplashScreenView.hpp>
#include <images/BitmapDatabase.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "Features.h"
	#include "CORELib/Common/ViperDef.h"
	#include "_Board.h"
};

SSplashScreenView::SSplashScreenView()
{
	char tmpStr[50];

	OldCodiceFirmwareEsteso = 0;

	_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_INFOFIRMWARE);

	snprintf(tmpStr,sizeof(tmpStr)-1,"%d_%c%d.%d %s %s",FEATURES_INFOFIRMWARE_CODICEESTESO%100000,FEATURES_INFOFIRMWARE_STAGE,(int)FEATURES_INFOFIRMWARE_VERSION/10,(int)FEATURES_INFOFIRMWARE_VERSION%10,__DATE__,__TIME__);
	Unicode::strncpy(tAVersionBuffer,tmpStr, TAVERSION_SIZE-1);

	iTipoMacchina.setVisible(false);
}


void SSplashScreenView::handleTickEvent()
{
	SSplashScreenViewBase::handleTickEvent();

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_CENTER) == ENCODER_PRESSION_PRESSED)
	{
		application().VisualizzaSBasic();
	}

	if(OldCodiceFirmwareEsteso != viperdef_Pack8GenTx_InfoFirmware.CodiceFirmwareEsteso)
	{
		OldCodiceFirmwareEsteso = viperdef_Pack8GenTx_InfoFirmware.CodiceFirmwareEsteso;

		switch(viperdef_Pack8GenTx_InfoFirmware.CodiceFirmwareEsteso)
		{
			default:
				iTipoMacchina.setVisible(false);
			break;
			case 22755015:
				iTipoMacchina.setVisible(true);
				iTipoMacchina.setBitmap(touchgfx::Bitmap(BITMAP_IMGLOGOUNITECH304X86_ID));
				Unicode::strncpy(tAFirmwareNameBuffer,"350C", TAFIRMWARENAME_SIZE-1);
			break;
			case 22755016:
				iTipoMacchina.setVisible(true);
				iTipoMacchina.setBitmap(touchgfx::Bitmap(BITMAP_IMGLOGOUNITECH304X86_ID));
				Unicode::strncpy(tAFirmwareNameBuffer,"500", TAFIRMWARENAME_SIZE-1);
			break;
			case 22755017:
				iTipoMacchina.setVisible(true);
				iTipoMacchina.setBitmap(touchgfx::Bitmap(BITMAP_IMGLOGOTP220EVO304X70_ID));
			break;
		}

		iTipoMacchina.invalidate();
	}
}

void SSplashScreenView::setupScreen()
{
    SSplashScreenViewBase::setupScreen();
}

void SSplashScreenView::tearDownScreen()
{
    SSplashScreenViewBase::tearDownScreen();
}
