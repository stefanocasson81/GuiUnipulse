#include <gui/smenuprincipale_screen/SMenuPrincipaleView.hpp>
#include <gui/containers/CTileMenu.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "Applicazione.h"
};

SMenuPrincipaleView::SMenuPrincipaleView()
{
}

typedef enum
{
	SMENUPRINCIPALEVIEW_TAG_VOID,	// Necessario vuoto

	SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS,
	SMENUPRINCIPALEVIEW_TAG_TESTGUI,
	SMENUPRINCIPALEVIEW_TAG_TESTHARDWARE,
	SMENUPRINCIPALEVIEW_TAG_TESTVAR,
	SMENUPRINCIPALEVIEW_TAG_BASIC,
	SMENUPRINCIPALEVIEW_TAG_RAPIDMENU,
	SMENUPRINCIPALEVIEW_TAG_SETTINGS,
	SMENUPRINCIPALEVIEW_TAG_LANGUAGE,
	SMENUPRINCIPALEVIEW_TAG_MENUADVANCED,
	SMENUPRINCIPALEVIEW_TAG_CLOCK,
	SMENUPRINCIPALEVIEW_TAG_INFO,
	SMENUPRINCIPALEVIEW_TAG_TESTHARDWARETRAINO,

}smenuprincipaleview_Tag_e;

void SMenuPrincipaleView::setupScreen()
{
	cInfoBar.SetIco(0xffffffff);
	cInfoBar.SetTitolo((char*)"MENU SETUP");

	add(*TGFXMenu.CreateTile((char*)"WELDING\nPROCESS",BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS));
//	if(applicazione_Info.ShowDebug)
		add(*TGFXMenu.CreateTile((char*)"Test\nHardware",BITMAP_ICOTESTHARWARE58X50_ID,true,SMENUPRINCIPALEVIEW_TAG_TESTHARDWARE));
	add(*TGFXMenu.CreateTile((char*)"Test\nTraino",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_TESTHARDWARETRAINO));
	add(*TGFXMenu.CreateTile((char*)"BASIC",BITMAP_ICOBASIC62X50_ID,false,SMENUPRINCIPALEVIEW_TAG_BASIC));
	add(*TGFXMenu.CreateTile((char*)"RAPID\nMENU",BITMAP_ICORAPIDMENU60X38_ID,false,SMENUPRINCIPALEVIEW_TAG_RAPIDMENU));
//	add(*TGFXMenu.CreateTile((char*)"SETTINGS",BITMAP_ICOSETTINGS46X46_ID,false,SMENUPRINCIPALEVIEW_TAG_SETTINGS));
//	add(*TGFXMenu.CreateTile((char*)"LANGUAGE",BITMAP_ICOLANGUAGE46X46_ID,false,SMENUPRINCIPALEVIEW_TAG_LANGUAGE));
	add(*TGFXMenu.CreateTile((char*)"ADVANCED\nMENU",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_MENUADVANCED));
	add(*TGFXMenu.CreateTile((char*)"INFO",BITMAP_ICOINFO46X46_ID,true,SMENUPRINCIPALEVIEW_TAG_INFO));
	add(*TGFXMenu.CreateTile((char*)"CLOCK",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_CLOCK));

	SMenuPrincipaleViewBase::setupScreen();
}

void SMenuPrincipaleView::tearDownScreen()
{
    SMenuPrincipaleViewBase::tearDownScreen();
}

void SMenuPrincipaleView::handleTickEvent()
{
	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMain();	// Torna indietro

	switch(TGFXMenu.ManagerTickEvent())
	{
		case SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS:
			application().VisualizzaSWeldingProcess();
		break;
		case SMENUPRINCIPALEVIEW_TAG_TESTGUI:
			application().VisualizzaSProveGUI();
		break;
		case SMENUPRINCIPALEVIEW_TAG_TESTHARDWARE:
			application().VisualizzaSHwCheck();
		break;
		case SMENUPRINCIPALEVIEW_TAG_TESTVAR:
			application().VisualizzaSDebugVar();
		break;
		case SMENUPRINCIPALEVIEW_TAG_MENUADVANCED:
			application().VisualizzaSMenuAdvance();
		break;
		case SMENUPRINCIPALEVIEW_TAG_CLOCK:
			application().VisualizzaSOrologio();
		break;
		case SMENUPRINCIPALEVIEW_TAG_INFO:
			application().VisualizzaSInfo();
		break;
		case SMENUPRINCIPALEVIEW_TAG_TESTHARDWARETRAINO:
			application().VisualizzaSHwCheckTraino();
		break;
	}
}
