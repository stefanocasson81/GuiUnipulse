#include <gui/containers/CWeldingProcessMig1.hpp>
#include <touchgfx/Color.hpp>

CWeldingProcessMig1::CWeldingProcessMig1()
{
	Invalida();

	ManagerTickEvent();
}

void CWeldingProcessMig1::initialize()
{
    CWeldingProcessMig1Base::initialize();
}

void CWeldingProcessMig1::Invalida(void)
{
	OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Tanto per dare prima rigenerata
}

void CWeldingProcessMig1::ManagerTickEvent(void)
{
	if(OldAttualeSelezione!=viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		OldAttualeSelezione=viperui_Info.SelezioneProcesso.AttualeSelezione;

		switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
		{
			default:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				bProcesso.setColor(colortype(touchgfx::Color::getColorFromRGB(0, 0, 0)));
				tProcesso.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

				iProcesso.setAlpha(90);

				iD.setAlpha(90);
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				bProcesso.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				tProcesso.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

				iProcesso.setAlpha(255);

				iD.setAlpha(255);
			break;
		}
		invalidate();
	}
}
