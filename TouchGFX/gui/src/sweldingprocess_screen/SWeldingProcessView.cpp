#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/containers/CWeldingProcessMig.hpp>
#include <gui/containers/CWeldingProcessTig.hpp>
#include <gui/containers/CWeldingProcessMma.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
};


SWeldingProcessView::SWeldingProcessView():ActualSelection(VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO)
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

	cInfoBar.SetTitolo(T_TITLE_WELDING_PROCESS);
	cInfoBar.SetIco(0xffffffff);

//	handleTickEvent();	// Prima rinfrescata per generare bene il quadro
   processChanged(Process);// Prima rinfrescata per generare bene il quadro
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
}

void SWeldingProcessView::tearDownScreen()
{
    SWeldingProcessViewBase::tearDownScreen();
}

void SWeldingProcessView::setMenuPrincipale()
{
   application().VisualizzaSMenuPrincipale();
}

void SWeldingProcessView::handleTickEvent()
{

	switch(Process)
	{
		default:
		break;
		case VIPERDEF_PROCESSO_MMA:
			((CWeldingProcessMma*)PtrQuadro)->ManagerTickEvent();
		break;
		case VIPERDEF_PROCESSO_TIG:
			((CWeldingProcessTig*)PtrQuadro)->ManagerTickEvent();
		break;
		case VIPERDEF_PROCESSO_MIG:
			((CWeldingProcessMig*)PtrQuadro)->ManagerTickEvent();
		break;
	}
}

void SWeldingProcessView::processChanged(uint8_t prc)
{
   Process = prc;

   if ( PtrQuadro )
		{
			remove(*PtrQuadro);
			delete PtrQuadro;
		}

   switch (Process)
		{
			default:
			break;
			case VIPERDEF_PROCESSO_MMA:
				PtrQuadro = new CWeldingProcessMma();

         if ( PtrQuadro )
         {
            ((CWeldingProcessMma*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
            add(*PtrQuadro);
            ((CWeldingProcessMma*) PtrQuadro)->invalidate();
         }
      break;
      case VIPERDEF_PROCESSO_TIG:
         PtrQuadro = new CWeldingProcessTig();
         if ( PtrQuadro )
         {
            ((CWeldingProcessTig*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
            add(*PtrQuadro);
            ((CWeldingProcessTig*) PtrQuadro)->invalidate();
         }
      break;
      case VIPERDEF_PROCESSO_MIG:
         PtrQuadro = new CWeldingProcessMig();
         if ( PtrQuadro )
         {
//            ((CWeldingProcessMig*) PtrQuadro)
            ((CWeldingProcessMig*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
            add(*PtrQuadro);
            ((CWeldingProcessMig*) PtrQuadro)->invalidate();
				}
			break;
		}

}


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
   switch (Process)
   {
      default:
      break;
      case VIPERDEF_PROCESSO_MMA:

      break;
      case VIPERDEF_PROCESSO_TIG:

      break;
      case VIPERDEF_PROCESSO_MIG:
//         ((CWeldingProcessMig*) PtrQuadro)->setGas(type);
      break;
   }
}


void SWeldingProcessView::setWireCB()
{

}
