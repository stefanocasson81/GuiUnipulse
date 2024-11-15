#include <gui/smenuadvance_screen/SMenuAdvanceView.hpp>

extern "C"
{
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
};

typedef enum
{
	SMENUPRINCIPALEVIEW_TAG_VOID,	// Necessario vuoto

	SMENUPRINCIPALEVIEW_TAG_FIRMWAREUPGRADE,

}smenuadvance_Tag_e;

SMenuAdvanceView::SMenuAdvanceView()
{
}

void SMenuAdvanceView::setupScreen()
{
	cInfoBar.SetIco(0xffffffff);
	cInfoBar.SetTitolo((char*)"MENU ADVANCED");

	add(*TGFXMenu.CreateTile((char*)"UPGRADE\nFIRMWARE",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_FIRMWAREUPGRADE));

    SMenuAdvanceViewBase::setupScreen();
}

void SMenuAdvanceView::tearDownScreen()
{
    SMenuAdvanceViewBase::tearDownScreen();
}


void SMenuAdvanceView::handleTickEvent()
{
	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMenuPrincipale();	// Torna indietro

	switch(TGFXMenu.ManagerTickEvent())
	{
		case SMENUPRINCIPALEVIEW_TAG_FIRMWAREUPGRADE:
			application().VisualizzaSUpgradeFirmware();
		break;
	}

}
