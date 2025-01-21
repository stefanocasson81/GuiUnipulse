#include <gui/custom/CWeldingContainerMma.hpp>
#include <texts/TextKeysAndLanguages.hpp>


CWeldingContainerMma::CWeldingContainerMma():indexMenu(0)
{
   setContainerType(VIPERDEF_PROCESSO_MMA);
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

}
