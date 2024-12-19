#include <gui/containers/CWeldingProcessMig.hpp>
#include <gui/containers/CWeldingProcessMig1.hpp>
#include <gui/containers/CWeldingProcessMig2.hpp>
#include <gui/containers/CWeldingProcessMig3.hpp>
#include <gui/containers/CWeldingProcessMig4.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
};


CWeldingProcessMig::CWeldingProcessMig():IndiceMaxEnumDiametro(0)
{
	OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Tanto per dare prima rigenerata

	viperui_Info.SelezioneProcesso.IndiceDiametroFilo = 0;
	viperui_Info.SelezioneProcesso.IndiceTipoGas = 0;
	viperui_Info.SelezioneProcesso.IndiceTipoMig = 0;

	viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo, viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas);
}

void CWeldingProcessMig::initialize()
{
    CWeldingProcessMigBase::initialize();
}

void CWeldingProcessMig::ManagerTickEvent(void)
{
	int16_t sA;
	uint8_t uCA;
	bool invalida=false;

/*
	switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		default:
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
			viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
			viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
			viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo, VIPERDEF_TIPOGAS_MAX);
		break;
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
			viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo, viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas);
		break;

	}
*/
	


	/*

	// Per cambiare il dato selezionato o si preme il joy destro o si muove il sinistro
	sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);	// ... lasciarlo qui per sA...
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
	{
		// Se siamo nell'ultima selezione cambia effettivamente il processo
		if(viperui_Info.SelezioneProcesso.AttualeSelezione==VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG)
		{
			viperui_Generatore_CambiaProcesso(viperui_Info.SelezioneProcesso.Processo,viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig,viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo,viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas,viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo);

			viperdef_Pack8GenTx_Base.Processo = viperui_Info.SelezioneProcesso.Processo;	// Solo per anticipare il refresh el quadro main al nuovo processo...
			application().VisualizzaSMain();
		}
		else if((viperui_Info.SelezioneProcesso.AttualeSelezione==VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO)&&(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo==VIPERDEF_TIPOFILO_MANUAL))
		{
			viperui_Generatore_CambiaProcesso(viperui_Info.SelezioneProcesso.Processo,VIPERDEF_TIPOMIG_MAN,VIPERDEF_TIPOFILO_MANUAL,VIPERDEF_TIPOGAS_M21_MIX8020,VIPERDEF_DIAMETROFILO_10);

			viperdef_Pack8GenTx_Base.Processo = viperui_Info.SelezioneProcesso.Processo;	// Solo per anticipare il refresh el quadro main al nuovo processo...
			application().VisualizzaSMain();
		}
		else
			sA=1;
	}
	if(sA)
	{
		if(viperui_Info.SelezioneProcesso.AttualeSelezione==VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO)
		{
			if(sA>0)
				viperui_Info.SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
		}
		else
		{
			sA+=viperui_Info.SelezioneProcesso.AttualeSelezione;
			if(sA>VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG)
				sA=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
			else if(sA<VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO)
				sA=VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;

			if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL)
			{
				if(sA>VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO)
					sA=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
			}

			viperui_Info.SelezioneProcesso.AttualeSelezione = (viperui_AttualeSelezioneProcesso_e)sA;
		}
	}
	*/

	/*
	switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		default:
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
			if(tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT)>0)	// Passa al tipo filo
				viperui_Info.SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
			if(viperui_Info.SelezioneProcesso.IndiceDiametroFilo!=0xff)
				viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = (viperdef_DiametroFilo_e)tabEnumDiametro[viperui_Info.SelezioneProcesso.IndiceDiametroFilo];
			if(viperui_Info.SelezioneProcesso.IndiceTipoGas!=0xff)
				viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = (viperdef_TipoGas_e)tabEnumTipoGas[viperui_Info.SelezioneProcesso.IndiceTipoGas];
			if(viperui_Info.SelezioneProcesso.IndiceTipoMig!=0xff)
				viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig = (viperdef_TipoMig_e)tabEnumTipoMig[viperui_Info.SelezioneProcesso.IndiceTipoMig];
		break;
	}
*/
	switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		default:
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
			sA = 0;
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT) == TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				sA = 1;
			else 
				sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
			if(sA>0)
			{	// Dove andare...
				// Parte dal TipoFilo attualmente in uso
				viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo;

				viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

				viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
			}
			else
			{ 	// Che valore mettere..
				sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
				if(sA)	// Con encoder destro si cambia il processo
				{
					sA += viperui_Info.SelezioneProcesso.Processo;
					if(sA<0)
						sA = 0;
					else if(sA>=VIPERDEF_PROCESSO_MAX)
						sA = VIPERDEF_PROCESSO_MAX-1;
					viperui_Info.SelezioneProcesso.Processo = (viperdef_Processo_e)sA;

					invalida = true;
				}
			}

			viperui_Info.SelezioneProcesso.IndiceDiametroFilo = 0xff;	// Seleziona quello simile o il primo se mancante
			viperui_Info.SelezioneProcesso.IndiceTipoGas = 0xff;		// Seleziona quello simile o il primo se mancante
			viperui_Info.SelezioneProcesso.IndiceTipoMig = 0xff;		// Seleziona quello simile o il primo se mancante
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
			sA = 0;
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT) == TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				sA = 1;
			else
			{
				sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
				if(sA>0)
				{
					if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL)
					{	// Se MIG MAN non andare oltre....
						sA = 0;	// Sta fermo
					}
					else 
						sA = 1;	// Va avanti
				}
			}
			if(sA)
			{	// Dove andare...
				if((sA>0) && (viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL))
				{	// Se MIG MAN confermo e fine dei giochi
					viperui_Generatore_CambiaProcesso(viperui_Info.SelezioneProcesso.Processo,VIPERDEF_TIPOMIG_MAN,VIPERDEF_TIPOFILO_MANUAL,VIPERDEF_TIPOGAS_M21_MIX8020,VIPERDEF_DIAMETROFILO_10);

					viperdef_Pack8GenTx_Base.Processo = viperui_Info.SelezioneProcesso.Processo;	// Solo per anticipare il refresh el quadro main al nuovo processo...
					application().VisualizzaSMain();
				}
				else
				{
					if(sA>0)
					{
						viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

						viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO;
					}
					else
						viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
				}
				
			}
			else
			{	// Che valore mettere..
				sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
				if(sA)
				{
					sA += viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;
					if(sA<0)
						sA = 0;
					else if(sA>=VIPERDEF_TIPOFILO_MAX)
						sA = VIPERDEF_TIPOFILO_MAX-1;
					if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo != (viperdef_TipoFilo_e)sA)
					{
						viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo = (viperdef_TipoFilo_e)sA;
						viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo = viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;

						viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

						viperui_Info.SelezioneProcesso.IndiceDiametroFilo = 0xff;	// Seleziona quello simile o il primo se mancante
						viperui_Info.SelezioneProcesso.IndiceTipoGas = 0xff;		// Seleziona quello simile o il primo se mancante
						viperui_Info.SelezioneProcesso.IndiceTipoMig = 0xff;		// Seleziona quello simile o il primo se mancante

						invalida = true;
					}
				}
			}
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
			sA = 0;
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT) == TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				sA = 1;
			else
				sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
			if(sA)
			{	// Dove andare...
				if(sA>0)
					viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
				else
					viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
			}
			else
			{	// Che valore mettere..
				sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
				if(sA)
				{
					sA += viperui_Info.SelezioneProcesso.IndiceDiametroFilo;
					if(sA < 0)
						sA = 0;
					else if(sA >= viperui_Info.SelezioneProcesso.IndiceMaxEnumDiametro)
						sA = viperui_Info.SelezioneProcesso.IndiceMaxEnumDiametro - 1;
					viperui_Info.SelezioneProcesso.IndiceDiametroFilo = sA;

					viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = (viperdef_DiametroFilo_e)viperui_Info.SelezioneProcesso.TabEnumDiametro[viperui_Info.SelezioneProcesso.IndiceDiametroFilo];
					viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo;

					viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

					viperui_Info.SelezioneProcesso.IndiceTipoGas = 0xff;		// Seleziona quello simile o il primo se mancante
					viperui_Info.SelezioneProcesso.IndiceTipoMig = 0xff;		// Seleziona quello simile o il primo se mancante

					invalida = true;
				}
			}
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
			sA = 0;
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT) == TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				sA = 1;
			else
				sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
			if(sA)
			{	// Dove andare...
				if(sA>0)
					viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
				else
					viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO;
			}
			else
			{	// Che valore mettere..
				sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
				if(sA)
				{
					sA += viperui_Info.SelezioneProcesso.IndiceTipoGas;
					if(sA<0)
						sA = 0;
					else if(sA>=viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoGas)
						sA = viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoGas-1;
					viperui_Info.SelezioneProcesso.IndiceTipoGas = sA;

					viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = (viperdef_TipoGas_e)viperui_Info.SelezioneProcesso.TabEnumTipoGas[viperui_Info.SelezioneProcesso.IndiceTipoGas];
					viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoGas = viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas;

					viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo, VIPERDEF_TIPOGAS_MAX);

					viperui_Info.SelezioneProcesso.IndiceTipoMig = 0xff;		// Seleziona quello simile o il primo se mancante

					invalida = true;
				}
			}
		break;

		case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
			sA = 0;
			if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_RIGHT) == TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
				sA = 1;
			else
			{
				sA = tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_LEFT);
				if(sA > 0)
					sA = 0;	// Sta fermo
			}
			if(sA)
			{	// Dove andare...
				if(sA > 0)
				{	// Confermo il processo congfigurato
					viperui_Generatore_CambiaProcesso(viperui_Info.SelezioneProcesso.Processo,viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig,viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo,viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas,viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo);

					viperdef_Pack8GenTx_Base.Processo = viperui_Info.SelezioneProcesso.Processo;	// Solo per anticipare il refresh el quadro main al nuovo processo...
					application().VisualizzaSMain();
				}
				else
					viperui_Info.SelezioneProcesso.AttualeSelezione=VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
			}
			else
			{	// Che valore mettere..
				sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
				if(sA)
				{
					sA += viperui_Info.SelezioneProcesso.IndiceTipoMig;
					if(sA<0)
						sA = 0;
					else if(sA>=viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoMig)
						sA = viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoMig-1;
					viperui_Info.SelezioneProcesso.IndiceTipoMig = sA;

					viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig = (viperdef_TipoMig_e)viperui_Info.SelezioneProcesso.TabEnumTipoMig[viperui_Info.SelezioneProcesso.IndiceTipoMig];
					viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoMig = viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig;

					viperui_RecalcBitMappedFiloDiametroGasMig(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo, viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo, viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas);

					invalida = true;
				}
			}
		break;
	}
	
	if(viperui_Info.SelezioneProcesso.IndiceDiametroFilo >= viperui_Info.SelezioneProcesso.IndiceMaxEnumDiametro)
		viperui_Info.SelezioneProcesso.IndiceDiametroFilo = 0xff;
	if(viperui_Info.SelezioneProcesso.IndiceDiametroFilo==0xff)
	{    
		viperui_Info.SelezioneProcesso.IndiceDiametroFilo = 0;
		for(uCA = 0; uCA < viperui_Info.SelezioneProcesso.IndiceMaxEnumDiametro;uCA++)
		{
			if((viperdef_DiametroFilo_e)viperui_Info.SelezioneProcesso.TabEnumDiametro[uCA] == viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo)
			{
				viperui_Info.SelezioneProcesso.IndiceDiametroFilo = uCA;
				break;
			}
		}
	}
	if(viperui_Info.SelezioneProcesso.IndiceTipoGas >= viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoGas)
		viperui_Info.SelezioneProcesso.IndiceTipoGas = 0xff;
	if(viperui_Info.SelezioneProcesso.IndiceTipoGas==0xff)
	{    
		viperui_Info.SelezioneProcesso.IndiceTipoGas = 0;
		for(uCA = 0; uCA < viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoGas; uCA++)
		{
			if((viperdef_TipoGas_e)viperui_Info.SelezioneProcesso.TabEnumTipoGas[uCA] == viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoGas)
			{
				viperui_Info.SelezioneProcesso.IndiceTipoGas = uCA;
				break;
			}
		}
	}
	if(viperui_Info.SelezioneProcesso.IndiceTipoMig >= viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoMig)
		viperui_Info.SelezioneProcesso.IndiceTipoMig = 0xff;
	if(viperui_Info.SelezioneProcesso.IndiceTipoMig==0xff)
	{    
		viperui_Info.SelezioneProcesso.IndiceTipoMig = 0;
		for(uCA = 0; uCA < viperui_Info.SelezioneProcesso.IndiceMaxEnumTipoMig;uCA++)
		{
			if((viperdef_TipoMig_e)viperui_Info.SelezioneProcesso.TabEnumTipoMig[uCA] == viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoMig)
			{
				viperui_Info.SelezioneProcesso.IndiceTipoMig = uCA;
				break;
			}
		}
	}

	viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = (viperdef_DiametroFilo_e)viperui_Info.SelezioneProcesso.TabEnumDiametro[viperui_Info.SelezioneProcesso.IndiceDiametroFilo];
	viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = (viperdef_TipoGas_e)viperui_Info.SelezioneProcesso.TabEnumTipoGas[viperui_Info.SelezioneProcesso.IndiceTipoGas];
	viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig = (viperdef_TipoMig_e)viperui_Info.SelezioneProcesso.TabEnumTipoMig[viperui_Info.SelezioneProcesso.IndiceTipoMig];

	if(invalida)
	{
		cWeldingProcessMig_1.Invalida();
		cWeldingProcessMig_2.Invalida();
		cWeldingProcessMig_3.Invalida();
		cWeldingProcessMig_4.Invalida();
		
		OldAttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MAX;	// Per rifare una generata
	}

	cWeldingProcessMig_1.ManagerTickEvent();
	cWeldingProcessMig_2.ManagerTickEvent();
	cWeldingProcessMig_3.ManagerTickEvent();
	cWeldingProcessMig_4.ManagerTickEvent();

	if(OldAttualeSelezione!=viperui_Info.SelezioneProcesso.AttualeSelezione)
	{
		OldAttualeSelezione = viperui_Info.SelezioneProcesso.AttualeSelezione;

		switch(viperui_Info.SelezioneProcesso.AttualeSelezione)
		{
			default:
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
				cGrande.setPosition(212+15*1, 213, 10, 10);
				cPiccolo1.setPosition(212+15*0, 213, 10, 10);
				cPiccolo2.setPosition(212+15*2, 213, 10, 10);
				cPiccolo3.setPosition(212+15*3, 213, 10, 10);

				cWeldingProcessMig_1.setVisible(true);
				cWeldingProcessMig_1.setX(5);
				cWeldingProcessMig_2.setVisible(true);
				cWeldingProcessMig_2.setX(165);
				cWeldingProcessMig_3.setVisible(true);
				cWeldingProcessMig_3.setX(325);
				cWeldingProcessMig_4.setVisible(false);
				cWeldingProcessMig_4.setX(325);
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
				cGrande.setPosition(212+15*1, 213, 10, 10);
				cPiccolo1.setPosition(212+15*0, 213, 10, 10);
				cPiccolo2.setPosition(212+15*2, 213, 10, 10);
				cPiccolo3.setPosition(212+15*3, 213, 10, 10);

				cWeldingProcessMig_1.setVisible(true);
				cWeldingProcessMig_1.setX(5);
				cWeldingProcessMig_2.setVisible(true);
				cWeldingProcessMig_2.setX(165);
				cWeldingProcessMig_3.setVisible(true);
				cWeldingProcessMig_3.setX(325);
				cWeldingProcessMig_4.setVisible(false);
				cWeldingProcessMig_4.setX(325);
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
				cGrande.setPosition(212+15*2, 213, 10, 10);
				cPiccolo1.setPosition(212+15*1, 213, 10, 10);
				cPiccolo2.setPosition(212+15*0, 213, 10, 10);
				cPiccolo3.setPosition(212+15*3, 213, 10, 10);

				cWeldingProcessMig_1.setVisible(true);
				cWeldingProcessMig_1.setX(5);
				cWeldingProcessMig_2.setVisible(true);
				cWeldingProcessMig_2.setX(165);
				cWeldingProcessMig_3.setVisible(true);
				cWeldingProcessMig_3.setX(325);
				cWeldingProcessMig_4.setVisible(false);
				cWeldingProcessMig_4.setX(325);
			break;

			case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:
				cGrande.setPosition(212+15*3, 213, 10, 10);
				cPiccolo1.setPosition(212+15*0, 213, 10, 10);
				cPiccolo2.setPosition(212+15*1, 213, 10, 10);
				cPiccolo3.setPosition(212+15*2, 213, 10, 10);

				cWeldingProcessMig_1.setVisible(false);
				cWeldingProcessMig_1.setX(5);
				cWeldingProcessMig_2.setVisible(true);
				cWeldingProcessMig_2.setX(5);
				cWeldingProcessMig_3.setVisible(true);
				cWeldingProcessMig_3.setX(165);
				cWeldingProcessMig_4.setVisible(true);
				cWeldingProcessMig_4.setX(325);
			break;
		}

		if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL)
		{
			cWeldingProcessMig_3.setVisible(false);
			cWeldingProcessMig_4.setVisible(false);
			cPiccolo2.setVisible(false);
			cPiccolo3.setVisible(false);
		}

		invalidate();
	}

}
