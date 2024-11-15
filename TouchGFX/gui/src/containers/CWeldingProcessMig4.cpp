#include <gui/containers/CWeldingProcessMig4.hpp>
#include <touchgfx/Color.hpp>

CWeldingProcessMig4::CWeldingProcessMig4()
{
	Invalida();

	ManagerTickEvent();
}

void CWeldingProcessMig4::initialize()
{
    CWeldingProcessMig4Base::initialize();
}

void CWeldingProcessMig4::Invalida(void)
{
	OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Tanto per dare prima rigenerata
	OldTipoMig = VIPERDEF_TIPOMIG_MAX;

}

void CWeldingProcessMig4::ManagerTickEvent(void)
{
	if(OldAttualeSelezione!=viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		OldAttualeSelezione=viperui_Info.SelezioneProcesso.AttualeSelezione;

		bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

		bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));

		tATipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tATipoDescr.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bTipo.setColor(colortype(touchgfx::Color::getColorFromRGB(0, 0, 0)));
		iTipo.setAlpha(90);

		switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
		{
			default:
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

				tATipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				tATipoDescr.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				bTipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				iTipo.setAlpha(255);
			break;
		}

		invalidate();
	}

	if(OldTipoMig!=viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig)
	{
		OldTipoMig=viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig;

		switch(viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig)
		{
			default:
				Unicode::strncpy(tATipoDescrBuffer,"XxX", TATIPODESCR_SIZE-1);
			break;

			case VIPERDEF_TIPOMIG_MAN:
				Unicode::strncpy(tATipoDescrBuffer,"MANUAL", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOMIG_SYN:
				Unicode::strncpy(tATipoDescrBuffer,"SYNERGIC", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOMIG_PULSE:
				Unicode::strncpy(tATipoDescrBuffer,"PULSED", TATIPODESCR_SIZE-1);
			break;
		}

		tATipoDescr.invalidate();
	}
}
