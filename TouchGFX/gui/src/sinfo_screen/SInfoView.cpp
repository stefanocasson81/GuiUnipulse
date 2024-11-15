#include <gui/sinfo_screen/SInfoView.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	#include "Features.h"
	#include "CORELib/Viper/ViperDef.h"
};


SInfoView::SInfoView()
{
	char tmpStr[200];

	snprintf(tmpStr,sizeof(tmpStr)-1,"%d_%c%d.%d %s %s",(int)(FEATURES_INFOFIRMWARE_CODICEESTESO%100000),FEATURES_INFOFIRMWARE_STAGE,(int)FEATURES_INFOFIRMWARE_VERSION/10,(int)FEATURES_INFOFIRMWARE_VERSION%10,__DATE__,__TIME__);
	Unicode::strncpy(tAInfoUserInterfaceBuffer,tmpStr, TAINFOUSERINTERFACE_SIZE);


	snprintf(tmpStr,sizeof(tmpStr)-1,"%d_%c%d.%d",(int)(viperdef_Pack8GenTx_InfoFirmware.CodiceFirmwareEsteso%100000),
												  (char)(viperdef_Pack8GenTx_InfoFirmware.Version>>8),
												  (viperdef_Pack8GenTx_InfoFirmware.Version&0xff)/10,
												  (viperdef_Pack8GenTx_InfoFirmware.Version&0xff)%10);
	Unicode::strncpy(tAInfoCntInvBuffer,tmpStr, TAINFOCNTINV_SIZE);

}

void SInfoView::setupScreen()
{

	SInfoViewBase::setupScreen();
}

void SInfoView::tearDownScreen()
{
    SInfoViewBase::tearDownScreen();
}

void SInfoView::handleTickEvent()
{
	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMenuPrincipale();	// Torna indietro
}
