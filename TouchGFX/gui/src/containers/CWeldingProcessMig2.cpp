#include <gui/containers/CWeldingProcessMig2.hpp>
#include <touchgfx/Color.hpp>

extern "C"
{
	//#include "CORELib/Viper/UserInterface/ViperUI.h"
	//#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	//#include "CORELib/Viper/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
};


CWeldingProcessMig2::CWeldingProcessMig2()
{
	Invalida();

	ManagerTickEvent();
}

void CWeldingProcessMig2::initialize()
{
    CWeldingProcessMig2Base::initialize();
}

void CWeldingProcessMig2::Invalida(void)
{
	OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Tanto per dare prima rigenerata
	OldTipoFilo = VIPERDEF_TIPOFILO_MAX;
	OldDiametroFilo = VIPERDEF_DIAMETROFILO_MAX;
}

void CWeldingProcessMig2::ManagerTickEvent(void)
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

		tADiametro.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		tADiametroDescr.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bDiametro.setColor(colortype(touchgfx::Color::getColorFromRGB(0, 0, 0)));
		iDiametro.setAlpha(90);

		iD.setAlpha(90);

		switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
		{
			default:
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

				tATipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				tATipoDescr.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				bTipo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				iTipo.setAlpha(255);

				iD.setAlpha(255);
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
				bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

				bTitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOCHIARO]));
				tATitolo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

				tADiametro.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				tADiametroDescr.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
				bDiametro.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				iDiametro.setAlpha(255);

				iD.setAlpha(255);
			break;
		}

		invalidate();
	}

	if(OldTipoFilo!=viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo)
	{
		OldTipoFilo=viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;

		switch(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo)
		{
			default:
				Unicode::strncpy(tATipoDescrBuffer,"XxX", TATIPODESCR_SIZE-1);
			break;

			case VIPERDEF_TIPOFILO_MANUAL:
				Unicode::strncpy(tATipoDescrBuffer,"MANUAL", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_FE:
				Unicode::strncpy(tATipoDescrBuffer,"Fe", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_FEMETAL:
				Unicode::strncpy(tATipoDescrBuffer,"FeMetal", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_FERUTIL:
				Unicode::strncpy(tATipoDescrBuffer,"FeRutil", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CRNI308:
				Unicode::strncpy(tATipoDescrBuffer,"CrNi-308", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CRNI316:
				Unicode::strncpy(tATipoDescrBuffer,"CrNi-316", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CRNI2209:
				Unicode::strncpy(tATipoDescrBuffer,"CrNi-2209", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL4043:
				Unicode::strncpy(tATipoDescrBuffer,"Al-4043", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL4047:
				Unicode::strncpy(tATipoDescrBuffer,"Al-4047", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL4145:
				Unicode::strncpy(tATipoDescrBuffer,"Al-4145", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL4557:
				Unicode::strncpy(tATipoDescrBuffer,"Al-4557", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL5356:
				Unicode::strncpy(tATipoDescrBuffer,"Al-5356", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_AL5554:
				Unicode::strncpy(tATipoDescrBuffer,"Al-5356", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CUSI3CRNI:
				Unicode::strncpy(tATipoDescrBuffer,"CuSi3CrNi", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CUSI3ZN:
				Unicode::strncpy(tATipoDescrBuffer,"CuSi3Zn", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CUAL8CRNI:
				Unicode::strncpy(tATipoDescrBuffer,"CuAl8CrNi", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_CUAL8ZN:
				Unicode::strncpy(tATipoDescrBuffer,"CuAl8Zn", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_ER80:
				Unicode::strncpy(tATipoDescrBuffer,"Er80", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_ER120:
				Unicode::strncpy(tATipoDescrBuffer,"Er120", TATIPODESCR_SIZE-1);
			break;
			case VIPERDEF_TIPOFILO_NICR3:
				Unicode::strncpy(tATipoDescrBuffer,"NiCr3", TATIPODESCR_SIZE-1);
			break;
		}

		if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL)
		{
			tADiametro.setVisible(false);
			tADiametroDescr.setVisible(false);
			iDiametro.setVisible(false);

			tADiametro.invalidate();
			tADiametroDescr.invalidate();
			iDiametro.invalidate();
		}
		else
		{
			tADiametro.setVisible(true);
			tADiametroDescr.setVisible(true);
			iDiametro.setVisible(true);

			tADiametro.invalidate();
			tADiametroDescr.invalidate();
			iDiametro.invalidate();
		}

		tATipoDescr.invalidate();
	}

	if(OldDiametroFilo!=viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo)
	{
		OldDiametroFilo=viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo;

		switch(viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo)
		{
			default:
				Unicode::strncpy(tADiametroDescrBuffer,"XxX mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_06:
				Unicode::strncpy(tADiametroDescrBuffer,"0.6 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_08:
				Unicode::strncpy(tADiametroDescrBuffer,"0.8 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_09:
				Unicode::strncpy(tADiametroDescrBuffer,"0.9 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_10:
				Unicode::strncpy(tADiametroDescrBuffer,"1.0 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_12:
				Unicode::strncpy(tADiametroDescrBuffer,"1.2 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_14:
				Unicode::strncpy(tADiametroDescrBuffer,"1.4 mm", TADIAMETRODESCR_SIZE-1);
			break;
			case VIPERDEF_DIAMETROFILO_16:
				Unicode::strncpy(tADiametroDescrBuffer,"1.6 mm", TADIAMETRODESCR_SIZE-1);
			break;
		}

		tADiametroDescr.invalidate();
	}
}
