#include <gui/custom/CWeldingContainerMma.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <texts/TextKeysAndLanguages.hpp>
//extern "C"
//{
//	#include "CORELib/UserInterface/ViperUI.h"
//	#include "ARMLib/TGFX/TGFXCustom.h"
//	#include "CORELib/Common/ViperDef.h"
//	#include "Features.h"
//};


CWeldingContainerMma::CWeldingContainerMma():indexMenu(0)
{
	CWeldingGenContParm[0].setXY(5,0);
//	CWeldingGenContParm[1].setXY(325,1);
//	CWeldingGenContParm[2].setXY(485,1);
	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		add(CWeldingGenContParm[i]);
	}
}



void CWeldingContainerMma::initialize()
{
   CWeldingGenericContainer::initialize();
   CWeldingGenContParm[0].set_TA_Process_1(touchgfx::TypedText(T_PROC_MMA));
   CWeldingGenContParm[0].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		CWeldingGenContParm[i].initialize();
	}
}


//void CWeldingContainerMma::setViewCallback(GenericCallback<U8>& callback)
//{
//	viewCallback = &callback;
//}

void CWeldingContainerMma::confirmMenu(S8 p)
{

}

void CWeldingContainerMma::seletcMenu(S8 menu,S8 submenu)
{
	indexMenu += menu;

	for (int i = 0; i < CWeldingGenContParm.size(); i++)
	{
		CWeldingGenContParm[i].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
		CWeldingGenContParm[i].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_NERO]));
	}

	switch (indexMenu)
	{
      case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
         CWeldingGenContParm[0].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
      break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
			CWeldingGenContParm[1].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
			CWeldingGenContParm[1].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
			CWeldingGenContParm[1].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;

		default:
			if ( indexMenu < VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO)
				indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
			else if ( indexMenu > VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG )
				indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
	}
}
