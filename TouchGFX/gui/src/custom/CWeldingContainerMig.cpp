#include <gui/custom/CWeldingContainerMig.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <texts/TextKeysAndLanguages.hpp>
extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "Features.h"
};


CWeldingContainerMig::CWeldingContainerMig():indexMenu(0),indexSubMenu(0)
{
	CWeldingGenContParm[0].setXY(5,0);
	CWeldingGenContParm[1].setXY(165,0);
	CWeldingGenContParm[2].setXY(325,0);


	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		add(CWeldingGenContParm[i]);
	}

}


void CWeldingContainerMig::initialize()
{
   CWeldingGenericContainer::initialize();
   CWeldingGenContParm[0].set_TA_Process_1(touchgfx::TypedText(T_PROC_MIG));
   CWeldingGenContParm[0].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
   CWeldingGenContParm[0].set_TA_Title_2(touchgfx::TypedText(T_PROC_MANUAL));
   CWeldingGenContParm[1].set_TA_Title_1(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[1].set_TA_Title_2(touchgfx::TypedText(T_PROC_DIAMETER));
   CWeldingGenContParm[2].set_TA_Title_1(touchgfx::TypedText(T_PROC_TYPE));
   CWeldingGenContParm[2].set_TA_Title_2(touchgfx::TypedText(T_PROC_GAS));
	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		CWeldingGenContParm[i].initialize();
	}
}


void CWeldingContainerMig::confirmMenu(S8 p)
{

}
void CWeldingContainerMig::seletcMenu(S8 menu,S8 submenu)
{
	indexMenu += menu;
	indexSubMenu += submenu;
	for (int i = 0; i < CWeldingGenContParm.size(); i++)
	{
		CWeldingGenContParm[i].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
		CWeldingGenContParm[i].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
	}

	switch (indexMenu)
	{
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
         CWeldingGenContParm[0].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         seletcSubMenu_TipoMig(indexSubMenu);
      break;

      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
         CWeldingGenContParm[1].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         seletcSubMenu_TipoFilo(indexSubMenu);
      break;
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
         CWeldingGenContParm[1].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         seletcSubMenu_TipoDiametro(indexSubMenu);

      break;
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
         CWeldingGenContParm[2].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
         seletcSubMenu_TipoMGas(indexSubMenu);
      break;

      default:
         if ( indexMenu < VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO )
            indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
         else if ( indexMenu > VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG )
            indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
   }
   if ( MenuSelected_Callback && MenuSelected_Callback->isValid() )
   {
      MenuSelected_Callback->execute(indexMenu,Model::MODEL_PROCESSO_MMA);
   }
}

void CWeldingContainerMig::seletcSubMenu_TipoMig(S8 submenu)
{
   switch (submenu)
   {
      default:
         CWeldingGenContParm[1].set_TA_Desc_2("XxX");
      break;

      case VIPERDEF_TIPOMIG_MAN:
         CWeldingGenContParm[1].set_TA_Desc_2("MANUAL");
      break;
      case VIPERDEF_TIPOMIG_SYN:
         CWeldingGenContParm[1].set_TA_Desc_2("SYNERGIC");
      break;
      case VIPERDEF_TIPOMIG_PULSE:
         CWeldingGenContParm[1].set_TA_Desc_2("PULSED");
      break;
   }
}

void CWeldingContainerMig::seletcSubMenu_TipoDiametro(S8 submenu)
{
   switch(submenu)
         {
            default:
               CWeldingGenContParm[1].set_TA_Desc_2("XxX mm");
            break;
            case VIPERDEF_DIAMETROFILO_06:
               CWeldingGenContParm[1].set_TA_Desc_2("0.6 mm");
            break;
            case VIPERDEF_DIAMETROFILO_08:
               CWeldingGenContParm[1].set_TA_Desc_2("0.8 mm");
            break;
            case VIPERDEF_DIAMETROFILO_09:
               CWeldingGenContParm[1].set_TA_Desc_2("0.9 mm");
            break;
            case VIPERDEF_DIAMETROFILO_10:
               CWeldingGenContParm[1].set_TA_Desc_2("1.0 mm");
            break;
            case VIPERDEF_DIAMETROFILO_12:
               CWeldingGenContParm[1].set_TA_Desc_2("1.2 mm");
            break;
            case VIPERDEF_DIAMETROFILO_14:
               CWeldingGenContParm[1].set_TA_Desc_2("1.4 mm");
            break;
            case VIPERDEF_DIAMETROFILO_16:
               CWeldingGenContParm[1].set_TA_Desc_2("1.6 mm");
            break;
         }
}

void CWeldingContainerMig::seletcSubMenu_TipoFilo(S8 submenu)
{

}
void CWeldingContainerMig::seletcSubMenu_TipoMGas(S8 submenu)
{

}

//void CWeldingContainerMig::setViewCallback(GenericCallback<U8>& callback)
//{
//	viewCallback = &callback;
//}
