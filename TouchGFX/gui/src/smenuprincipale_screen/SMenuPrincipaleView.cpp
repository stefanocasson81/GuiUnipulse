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

SMenuPrincipaleView::SMenuPrincipaleView():indexSelected(0)
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
//	cInfoBar.SetIco(0xffffffff);

	cInfoBar.SetTitolo(T_TITLE_MAIN);
//	add(*TGFXMenu.CreateTile((char*)"WELDING\nPROCESS",BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS));
//	add(*TGFXMenu.CreateTile((char*)"WELDING\nPROCESS",BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS));
//	add(*TGFXMenu.CreateTile(T_MAIN_MENU,BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS));

	TGFXMenu.CreateTile(&cMenuList[0],T_ICN_MM_WELDING_PROCESS,BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS);
	add(cMenuList[0]);
	cMenuList[0].Abilita(true);

   TGFXMenu.CreateTile(&cMenuList[1],T_ICN_MM_WELDING_MODE,BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS);
   add(cMenuList[1]);

   TGFXMenu.CreateTile(&cMenuList[2],T_ICN_MM_JOB_MENU,BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS);
   add(cMenuList[2]);

   TGFXMenu.CreateTile(&cMenuList[3],T_ICN_MM_WELDING_SETUP,BITMAP_ICOWELDPROCESS42X51_ID,true,SMENUPRINCIPALEVIEW_TAG_WELDINGPROCESS);
   add(cMenuList[3]);

   for(U8 i=0;i<cMenuList.size();i++)cMenuList[i].Seleziona(false);
   cMenuList[indexSelected].Seleziona(true);

//	if(applicazione_Info.ShowDebug)
//   add(*TGFXMenu.CreateTile((char*)"Test\nHardware",BITMAP_ICOTESTHARWARE58X50_ID,true,SMENUPRINCIPALEVIEW_TAG_TESTHARDWARE));
//	add(*TGFXMenu.CreateTile((char*)"Test\nTraino",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_TESTHARDWARETRAINO));
//	add(*TGFXMenu.CreateTile((char*)"BASIC",BITMAP_ICOBASIC62X50_ID,false,SMENUPRINCIPALEVIEW_TAG_BASIC));
//	add(*TGFXMenu.CreateTile((char*)"RAPID\nMENU",BITMAP_ICORAPIDMENU60X38_ID,false,SMENUPRINCIPALEVIEW_TAG_RAPIDMENU));
//	add(*TGFXMenu.CreateTile((char*)"SETTINGS",BITMAP_ICOSETTINGS46X46_ID,false,SMENUPRINCIPALEVIEW_TAG_SETTINGS));
//	add(*TGFXMenu.CreateTile((char*)"LANGUAGE",BITMAP_ICOLANGUAGE46X46_ID,false,SMENUPRINCIPALEVIEW_TAG_LANGUAGE));
//	add(*TGFXMenu.CreateTile((char*)"ADVANCED\nMENU",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_MENUADVANCED));
//	add(*TGFXMenu.CreateTile((char*)"INFO",BITMAP_ICOINFO46X46_ID,true,SMENUPRINCIPALEVIEW_TAG_INFO));
//	add(*TGFXMenu.CreateTile((char*)"CLOCK",0xffffffff,true,SMENUPRINCIPALEVIEW_TAG_CLOCK));

	SMenuPrincipaleViewBase::setupScreen();
}

void SMenuPrincipaleView::tearDownScreen()
{
    SMenuPrincipaleViewBase::tearDownScreen();
}


void SMenuPrincipaleView::GoBack()
{
   application().VisualizzaSMain();  // Torna indietro
}


void SMenuPrincipaleView::encRup()
{
   cMenuList[indexSelected].Seleziona(false);
   indexSelected++;
   if(indexSelected > (cMenuList.size()-1))
   {
      indexSelected = 0;
   }
   cMenuList[indexSelected].Seleziona(true);
}

void SMenuPrincipaleView::encRdown()
{
   cMenuList[indexSelected].Seleziona(false);
   indexSelected--;
   if(indexSelected < 0)
   {
      indexSelected = cMenuList.size()-1;
   }
   cMenuList[indexSelected].Seleziona(true);
}


void SMenuPrincipaleView::setMenu()
{
	switch(cMenuList[indexSelected].Tag)
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
