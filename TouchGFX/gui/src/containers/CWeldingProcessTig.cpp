#include <gui/containers/CWeldingProcessTig.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
};

CWeldingProcessTig::CWeldingProcessTig()
{
	bProcesso.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
}

void CWeldingProcessTig::initialize()
{
    CWeldingProcessTigBase::initialize();
}

void CWeldingProcessTig::ManagerTickEvent(void)
{
	int16_t sA;

	switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		default:
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
/*			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				viperui_Info.SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
				*/
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
			{
				viperui_Generatore_CambiaProcesso(viperui_Info.SelezioneProcesso.Processo,VIPERDEF_TIPOMIG_MAX,VIPERDEF_TIPOFILO_MAX,VIPERDEF_TIPOGAS_MAX,VIPERDEF_DIAMETROFILO_MAX);
				viperdef_Pack8GenTx_Base.Processo = viperui_Info.SelezioneProcesso.Processo;	// Solo per anticipare il refresh el quadro main al nuovo processo...
				application().VisualizzaSMain();
			}

			sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
			if(sA)
			{
				sA += viperui_Info.SelezioneProcesso.Processo;
				if(sA<0)
					sA = 0;
				else if(sA>=VIPERDEF_PROCESSO_MAX)
					sA = VIPERDEF_PROCESSO_MAX-1;
				viperui_Info.SelezioneProcesso.Processo = (viperdef_Processo_e)sA;
			}

		break;
	}
}
