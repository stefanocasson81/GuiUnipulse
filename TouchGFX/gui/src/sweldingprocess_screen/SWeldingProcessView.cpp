#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/custom/CWeldingContainerMig.hpp>
#include <gui/custom/CWeldingContainerMma.hpp>
#include <gui/custom/CWeldingContainerTig.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>
extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
};


SWeldingProcessView::SWeldingProcessView():ActualSelection(VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO),
      setMenuSelectedContainer_Callback(this,&SWeldingProcessView::MenucSelectedeContainer)
{
	PtrQuadro = _NULL_;
//	CWeldingProcessMig_.setPtr(this);

//	Process = presenter->getProcess();
//	viperui_Info.SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
//	viperui_Info.SelezioneProcesso.Processo = viperdef_Pack8GenTx_Base.Processo;

	if(viperui_Info.SelezioneProcesso.Processo==VIPERDEF_PROCESSO_MIG)
	{
//		viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig = viperdef_Pack8GenTx_Base.TipoMig;
//
//		if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
//			viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = VIPERDEF_TIPOFILO_MANUAL;
//		else
//			viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = viperdef_Pack8GenTx_Base.TipoFilo;

//		viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = viperdef_Pack8GenTx_Base.TipoGas;
//		viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = viperdef_Pack8GenTx_Base.DiametroFilo;
//
//		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoMig 		= viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig;
//		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo 	= viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;
//		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoGas 		= viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas;
//		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo;
	}

//	pCWeldingSetParamContainer[0] = &cWeldingSetParamContainer1;
//	pCWeldingSetParamContainer[1] = &cWeldingSetParamContainer2;
//	pCWeldingSetParamContainer[2] = &cWeldingSetParamContainer3;
//	pCWeldingSetParamContainer[3] = &cWeldingSetParamContainer4;

	cInfoBar.SetTitolo(T_TITLE_WELDING_PROCESS);
//	cInfoBar.SetIco(0xffffffff);

//	handleTickEvent();	// Prima rinfrescata per generare bene il quadro
//   processChanged(Process);// Prima rinfrescata per generare bene il quadro
//	PtrQuadro = new CWeldingContainerMig();
//	PtrQuadro->setPtr(this);
//	CGeneric.setPtr(this);

}

SWeldingProcessView::~SWeldingProcessView()
{
	if(PtrQuadro)
		_DELETE_(PtrQuadro);

}

void SWeldingProcessView::setupScreen()
{
   SWeldingProcessViewBase::setupScreen();
	Process = presenter->getProcess();
	switch (Process)
	{
		case Model::VIPERDEF_PROCESSO_MMA:
//			PtrQuadro = new CWeldingContainerMma();
		break;
		case Model::VIPERDEF_PROCESSO_TIG:
//			PtrQuadro = new CWeldingContainerTig();
		break;
		case Model::VIPERDEF_PROCESSO_MIG:
			PtrQuadro = new CWeldingContainerMig();
			PtrQuadro->initialize();
//			PtrQuadro->setPtr(this);
		break;
		default:
		break;
	}
}

void SWeldingProcessView::tearDownScreen()
{
    SWeldingProcessViewBase::tearDownScreen();
}

void SWeldingProcessView::GoBack()
{
   application().VisualizzaSMenuPrincipale();
}


void SWeldingProcessView::encRconfirm()
{

}

void SWeldingProcessView::encLup()
{
//	menuLis++;
//	if(menuLis == VIPERUI_ATUALESELEZIONEPROCESSO_MAX)
//	{
//		menuLis = 0;
//	}
//	switch (menuLis)
//	{
//		case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
//
//		break;
//		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
//
//		break;
//		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
//
//		break;
//		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
//
//		break;
//		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
//
//		break;
//
//	}
}
void SWeldingProcessView::encLdown()
{
//	menuLis--;
}

#if 0
void SWeldingProcessView::encRup()
{
	if ( PtrQuadro )
		_DELETE_(PtrQuadro);
	Process++;
	switch (Process)
	{
		case Model::VIPERDEF_PROCESSO_MMA:
			PtrQuadro = new CWeldingProcessMma();
		break;
		case Model::VIPERDEF_PROCESSO_TIG:
			PtrQuadro = new CWeldingProcessTig();
		break;
		case Model::VIPERDEF_PROCESSO_MIG:
			PtrQuadro = new CWeldingProcessMig_();
		break;
		case Model::VIPERDEF_PROCESSO_MAX:
		default:

		break;
	}
}
void SWeldingProcessView::encRdown()
{
	if ( PtrQuadro )
		_DELETE_(PtrQuadro);
	Process--;
	switch (Process)
	{
		case Model::VIPERDEF_PROCESSO_MMA:
			PtrQuadro = new CWeldingProcessMma();
		break;
		case Model::VIPERDEF_PROCESSO_TIG:
			PtrQuadro = new CWeldingProcessTig();
		break;
		case Model::VIPERDEF_PROCESSO_MIG:
			PtrQuadro = new CWeldingProcessMig_();
		break;
		case Model::VIPERDEF_PROCESSO_MAX:
		default:

		break;
	}
}
#endif
void SWeldingProcessView::encRX_incDec(S8 toward)
{
	if ( PtrQuadro )
	{
		remove(*PtrQuadro);
		_DELETE_(PtrQuadro);
	}
	Process += toward;
	switch (Process)
	{
		case Model::VIPERDEF_PROCESSO_MMA:
//		   cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(T_PROC_MMA));
//		   cWeldingSetProcess
			PtrQuadro = new CWeldingContainerMma();
			add(*PtrQuadro);
			PtrQuadro->initialize();
			PtrQuadro->invalidate();
			PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
		break;
		case Model::VIPERDEF_PROCESSO_TIG:
//		   cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_TIG));
			PtrQuadro = new CWeldingContainerTig();
			add(*PtrQuadro);
			PtrQuadro->initialize();
			PtrQuadro->invalidate();
			PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
		break;
		case Model::VIPERDEF_PROCESSO_MIG:
//		   cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_MIG));
			PtrQuadro = new CWeldingContainerMig();
			add(*PtrQuadro);
			PtrQuadro->initialize();
			PtrQuadro->invalidate();
			PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
		break;

		case Model::VIPERDEF_PROCESSO_MAX:
		default:
			if ( Process >= VIPERDEF_PROCESSO_MAX )
				Process = VIPERDEF_PROCESSO_MIG;
			else if ( Process < VIPERDEF_PROCESSO_MMA )
				Process = VIPERDEF_PROCESSO_MMA;
		break;
	}
}

void SWeldingProcessView::MenucSelectedeContainer(U8 Menu,U8 Process)
{


}

void SWeldingProcessView::encSX_incDec(S8 toward)
{
	Process += toward;
	if(PtrQuadro)
	{
		PtrQuadro->seletcMenu(toward);
	}
}

void SWeldingProcessView::handleTickEvent()
{

//	switch(Process)
//	{
//		default:
//		break;
//		case VIPERDEF_PROCESSO_MMA:
//			((CWeldingProcessMma*)PtrQuadro)->ManagerTickEvent();
//		break;
//		case VIPERDEF_PROCESSO_TIG:
//			((CWeldingProcessTig*)PtrQuadro)->ManagerTickEvent();
//		break;
//		case VIPERDEF_PROCESSO_MIG:
////			((CWeldingProcessMig*)PtrQuadro)->ManagerTickEvent();
//		break;
//	}
}

//void SWeldingProcessView::processChanged(uint8_t prc)
//{
//   Process = prc;
//
//   if ( PtrQuadro )
//		{
//			remove(*PtrQuadro);
//			delete PtrQuadro;
//		}
//
//   switch (Process)
//		{
//			default:
//			break;
//			case VIPERDEF_PROCESSO_MMA:
//				PtrQuadro = new CWeldingProcessMma();
//
//         if ( PtrQuadro )
//         {
//            ((CWeldingProcessMma*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessMma*) PtrQuadro)->invalidate();
//         }
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//         PtrQuadro = new CWeldingProcessTig();
//         if ( PtrQuadro )
//         {
//            ((CWeldingProcessTig*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessTig*) PtrQuadro)->invalidate();
//         }
//      break;
//      case VIPERDEF_PROCESSO_MIG:
////         PtrQuadro = new CWeldingProcessMig();
//         if ( PtrQuadro )
//         {
////            ((CWeldingProcessMig*) PtrQuadro)
//            ((CWeldingProcessMig_*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessMig_*) PtrQuadro)->invalidate();
//				}
//			break;
//		}
//}


void SWeldingProcessView::gasChanged(uint8_t type)
{
   /******** SCA dynami cast not permitted **********/
//   if(typeid(*PtrQuadro) == typeid(CWeldingProcessMig_))
//   {
//      ((CWeldingProcessMig_*)PtrQuadro)->setGas(type);
//   }
//   if(CWeldingProcessMig_* Ptr = dynamic_cast<CWeldingProcessMig_*>(PtrQuadro))
//   {
//      Ptr->setGas(type);
//   }
//   switch (Process)
//   {
//      default:
//      break;
//      case VIPERDEF_PROCESSO_MMA:
//
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//
//      break;
//      case VIPERDEF_PROCESSO_MIG:
////         ((CWeldingProcessMig*) PtrQuadro)->setGas(type);
//      break;
//   }
}


#if 0
void SWeldingProcessView::setMigConfiguration()
{

	pCWeldingSetParamContainer[0]->setVisible_TA_MainTitle(true);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_1(false);
   pCWeldingSetParamContainer[0]->set_Color_BASE_1(color.setBackground_Focus());

   pCWeldingSetParamContainer[0]->setVisible_TA_Title_2(false);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_2(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iD_2(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iTypo_2(false);

   pCWeldingSetParamContainer[0]->setVisible_TA_Title_3(false);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_3(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iD_3(true);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iTypo_2(false);
   pCWeldingSetParamContainer[0]->set_IMG_iD_3(BITMAP_ICOTIG33X35_ID);

   pCWeldingSetParamContainer[0]->setActveBase(0, true);
   pCWeldingSetParamContainer[0]->setActveBase(1, false);
   pCWeldingSetParamContainer[0]->setActveBase(2, false);
   pCWeldingSetParamContainer[0]->set_TA_MainTitle(T_PROC_MIG);

}

void SWeldingProcessView::setTigConfiguration()
{

}
void SWeldingProcessView::setWireCB()
{

}
#endif
