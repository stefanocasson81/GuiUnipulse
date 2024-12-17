#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/containers/CWeldingProcessMig.hpp>
#include <gui/containers/CWeldingProcessTig.hpp>
#include <gui/containers/CWeldingProcessMma.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
};


SWeldingProcessView::SWeldingProcessView()
{
	PtrQuadro = _NULL_;

	ProcessoAllocato = VIPERDEF_PROCESSO_MAX;
	viperui_Info.SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
	viperui_Info.SelezioneProcesso.Processo = viperdef_Pack8GenTx_Base.Processo;

	if(viperui_Info.SelezioneProcesso.Processo==VIPERDEF_PROCESSO_MIG)
	{
		viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig = viperdef_Pack8GenTx_Base.TipoMig;

		if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
			viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = VIPERDEF_TIPOFILO_MANUAL;
		else
			viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = viperdef_Pack8GenTx_Base.TipoFilo;

		viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = viperdef_Pack8GenTx_Base.TipoGas;
		viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = viperdef_Pack8GenTx_Base.DiametroFilo;

		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoMig 		= viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig;
		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo 	= viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;
		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoGas 		= viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas;
		viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo;
	}

	cInfoBar.SetTitolo((char*)"WELDING PROCESS");
	cInfoBar.SetIco(0xffffffff);

	handleTickEvent();	// Prima rinfrescata per generare bene il quadro
}

SWeldingProcessView::~SWeldingProcessView()
{
	if(PtrQuadro)
		_DELETE_(PtrQuadro);
}

void SWeldingProcessView::setupScreen()
{
    SWeldingProcessViewBase::setupScreen();
}

void SWeldingProcessView::tearDownScreen()
{
    SWeldingProcessViewBase::tearDownScreen();
}

void SWeldingProcessView::handleTickEvent()
{
	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMenuPrincipale();

	if(viperui_Info.SelezioneProcesso.Processo!=ProcessoAllocato)
	{	// Cambiato processo, visualizzo il quadro giusto
		if(PtrQuadro)
		{
			remove(*PtrQuadro);
			delete PtrQuadro;
		}

		switch(viperui_Info.SelezioneProcesso.Processo)
		{
			default:
			break;
			case VIPERDEF_PROCESSO_MMA:
				PtrQuadro = new CWeldingProcessMma();
				if(PtrQuadro)
				{
					((CWeldingProcessMma*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
					add(*PtrQuadro);
					((CWeldingProcessMma*)PtrQuadro)->invalidate();
				}
			break;
			case VIPERDEF_PROCESSO_TIG:
				PtrQuadro = new CWeldingProcessTig();
				if(PtrQuadro)
				{
					((CWeldingProcessTig*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
					add(*PtrQuadro);
					((CWeldingProcessTig*)PtrQuadro)->invalidate();
				}
			break;
			case VIPERDEF_PROCESSO_MIG:
				PtrQuadro = new CWeldingProcessMig();
				if(PtrQuadro)
				{
					((CWeldingProcessMig*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
					add(*PtrQuadro);
					((CWeldingProcessMig*)PtrQuadro)->invalidate();
				}
			break;
		}

		ProcessoAllocato = viperui_Info.SelezioneProcesso.Processo;
	}

	switch(ProcessoAllocato)
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
