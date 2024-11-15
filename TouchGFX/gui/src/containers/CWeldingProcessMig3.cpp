#include <gui/containers/CWeldingProcessMig3.hpp>
#include <touchgfx/Color.hpp>

CWeldingProcessMig3::CWeldingProcessMig3()
{
	Invalida();

	ManagerTickEvent();
}

void CWeldingProcessMig3::initialize()
{
    CWeldingProcessMig3Base::initialize();
}

void CWeldingProcessMig3::Invalida(void)
{
	OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Tanto per dare prima rigenerata
	OldTipoGas = VIPERDEF_TIPOGAS_MAX;
}

void CWeldingProcessMig3::ManagerTickEvent(void)
{
	if(OldAttualeSelezione!=viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		OldAttualeSelezione=viperui_Info.SelezioneProcesso.AttualeSelezione;

		bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

		bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));

		tATipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tATipoDescr1.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tATipoDescr2.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bTipo.setColor(colortype(touchgfx::Color::getColorFromRGB(0, 0, 0)));
		iTipo.setAlpha(90);

		iD.setAlpha(90);

		switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
		{
			default:
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

				tATipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				tATipoDescr1.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				tATipoDescr2.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				bTipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				iTipo.setAlpha(255);

				iD.setAlpha(255);
			break;
		}

		invalidate();
	}

	if(OldTipoGas!=viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas)
	{
		OldTipoGas=viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas;

		switch(viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas)
		{
			default:
				Unicode::strncpy(tATipoDescr1Buffer,"XxX", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"XxX", TATIPODESCR1_SIZE-1);
			break;

			case VIPERDEF_TIPOGAS_NOGAS:
				Unicode::strncpy(tATipoDescr1Buffer," ", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"No GAS", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_I1_ARGON:
				Unicode::strncpy(tATipoDescr1Buffer,"I1", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"Argon", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_M12_MIX9802:
				Unicode::strncpy(tATipoDescr1Buffer,"M12", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"Mix9802", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_M20_MIX9208:
				Unicode::strncpy(tATipoDescr1Buffer,"M20", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"Mix9208", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_M21_MIX8020:
				Unicode::strncpy(tATipoDescr1Buffer,"M21", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"Mix8020", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_C1_CO2:
				Unicode::strncpy(tATipoDescr1Buffer,"C1", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"CO2", TATIPODESCR2_SIZE-1);
			break;
			case VIPERDEF_TIPOGAS_C2_MIX9802:
				Unicode::strncpy(tATipoDescr1Buffer,"C2", TATIPODESCR1_SIZE-1);
				Unicode::strncpy(tATipoDescr2Buffer,"Mix9802", TATIPODESCR2_SIZE-1);
			break;
		}

		tATipoDescr1.invalidate();
		tATipoDescr2.invalidate();
	}
}
