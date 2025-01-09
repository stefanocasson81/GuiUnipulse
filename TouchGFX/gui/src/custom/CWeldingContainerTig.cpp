#include <gui/custom/CWeldingContainerTig.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <texts/TextKeysAndLanguages.hpp>
extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "Features.h"
};


CWeldingContainerTig::CWeldingContainerTig():indexMenu(0)
{
	CWeldingGenContParm[0].setXY(5,0);
	CWeldingGenContParm[1].setXY(165,0);
//	CWeldingGenContParm[2].setXY(485,1);
	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		add(CWeldingGenContParm[i]);
	}

}


void CWeldingContainerTig::initialize()
{
	CWeldingGenericContainer::initialize();
   CWeldingGenContParm[0].set_TA_Process_1(touchgfx::TypedText(T_PROC_TIG));
   CWeldingGenContParm[0].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	for(int i=0;i<CWeldingGenContParm.size();i++)
	{
		CWeldingGenContParm[i].initialize();
	}
}

//void CWeldingContainerTig::setViewCallback(GenericCallback<U8>& callback)
//{
//	viewCallback = &callback;
//}

void CWeldingContainerTig::confirmMenu(S8 p)
{

}


void CWeldingContainerTig::seletcMenu(S8 menu)
{
	indexMenu += menu;

	for (int i = 0; i < CWeldingGenContParm.size(); i++)
	{
		CWeldingGenContParm[i].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		CWeldingGenContParm[i].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
	}

	switch (indexMenu)
	{
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
			CWeldingGenContParm[0].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
			CWeldingGenContParm[0].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
			CWeldingGenContParm[1].set_Color_BASE_1(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
			CWeldingGenContParm[1].set_Color_BASE_2(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
		break;
		default:
			if ( indexMenu < VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO)
				indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
			else if ( indexMenu > VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG )
				indexMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
	}
}
