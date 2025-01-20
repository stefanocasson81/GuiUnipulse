#include <gui/containers/CMainMig.hpp>
#include <touchgfx/Unicode.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
	#include "ARMLib/Common/Data.h"
	#include <ViperUISupport.h>
	#include "_Board.h"
};

#define CMAINMIG_MSWAITIMPOSTAZIONERAPIDA 666
#define CMAINMIG_MSWAITVIS CMAINMIG_MSWAITIMPOSTAZIONERAPIDA*2
#define CMAINMIG_MSMENUPRINCIPALE 1500

CMainMig::CMainMig()
{
	Selezionato=CMAINMIG_SELEZIONATO_VOID;
	OldSelezionato=CMAINMIG_SELEZIONATO_MAX;	// Tanto per forzare un primo refresh

	ValShowSX=0xffff;	// Tanto per forzare un primo refresh
	ValShowDX=0xffff;	// Tanto per forzare un primo refresh

	ValShowSX1Val=0xffff;	// Tanto per forzare un primo refresh
	ValShowSX2Val=0xffff;	// Tanto per forzare un primo refresh
	ValShowSX3Val=0xffff;	// Tanto per forzare un primo refresh
	ValShowDX1Val=0xffff;	// Tanto per forzare un primo refresh
	ValShowDX3Val=0xffff;	// Tanto per forzare un primo refresh

	MigSynSXVisShow = VIPERDEF_MIGSYNSXVIS_MAX;
	MigSynDXVisShow = VIPERDEF_MIGSYNDXVIS_MAX;

	FlgRichiestoImpostazioneRapida = false;

	PosYWhatDataOriginal = tASX1.getY();
	PosYValDataOriginal = tASX1Val.getY();

	cPSX.setVisible(false);
	cPSXPainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	cPC.setVisible(false);
	cPCPainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	cPDX.setVisible(false);
	cPDXPainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	switch(viperdef_Pack8GenTx_Base.TipoMig)
	{
		default:
		break;

		case VIPERDEF_TIPOMIG_MAN:
			tASX1.setVisible(false);
			tASX1Val.setVisible(false);
			bSX1.setVisible(false);
			tASX3.setVisible(false);
			tASX3Val.setVisible(false);
			bSX3.setVisible(false);

			tADX3.setVisible(false);
			tADX3Val.setVisible(false);
			bDX3.setVisible(false);

			cImpostazioneRapida1.setVisible(true);
			cImpostazioneRapida1.SetInfo((data_Data_t*)(&viperui_StrutturaDati[VIPERUI_DATA_MIGMAN_INDUTTANZA]),T_DINAMICA);
			cImpostazioneRapida1.SetIco(BITMAP_ICOINDUTTANZAWHITE54X18_ID);
			cImpostazioneRapida1.Seleziona(false);

			cImpostazioneRapida2.setVisible(true);
			cImpostazioneRapida2.SetInfo((data_Data_t*)(&viperui_StrutturaDati[VIPERUI_DATA_MIG_BBT]),T_BBT);
			cImpostazioneRapida2.SetIco(BITMAP_ICOBBT20X20_ID);
			cImpostazioneRapida2.Seleziona(false);

			cImpostazioneRapidaSinergia.setVisible(false);
		break;
		case VIPERDEF_TIPOMIG_SYN:
		case VIPERDEF_TIPOMIG_PULSE:
			cImpostazioneRapida1.setVisible(true);
			cImpostazioneRapida1.SetInfo((data_Data_t*)(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO]),T_DINAMICA);
			cImpostazioneRapida1.SetIco(BITMAP_ICOINDUTTANZAWHITE54X18_ID);
			cImpostazioneRapida1.Seleziona(false);

			cImpostazioneRapida2.setVisible(true);
			cImpostazioneRapida2.SetInfo((data_Data_t*)(&viperui_StrutturaDati[VIPERUI_DATA_MIG_BBT]),T_BBT);
			cImpostazioneRapida2.SetIco(BITMAP_ICOBBT20X20_ID);
			cImpostazioneRapida2.Seleziona(false);

			cImpostazioneRapidaSinergia.setVisible(true);
		break;
	}

	ManagerTickEvent();	// Prima generata forzata
}

void CMainMig::initialize()
{
    CMainMigBase::initialize();
}


void CMainMig::ManagerTickEvent(void)
{
	int16_t sA;
	uint16_t uSA;
	int32_t lA;
	uint32_t uLA;
	char tmpStr[20];
	viperui_Data_e enumData=VIPERUI_DATA_MIGSYN_MAX;

	//________________________________________________________Valuta dove andare
	uSA = tgfxcustom_EncoderStatus[ENCODER_CENTER].MsTempoPressione;
	if(uSA>100)
	{
		if(!cPC.isVisible())
			cPC.setVisible(true);

		uLA = uSA*100;
		uLA /= CMAINMIG_MSMENUPRINCIPALE;
		cPC.setValue((int16_t)uLA);
	}
	else
	{
		if(cPC.isVisible())
		{
			cPC.setVisible(false);
			cPC.invalidate();
		}
	}
	switch(tgfxcustom_GetPressioneEncoder(ENCODER_CENTER))
	{
		default:
		break;
		case ENCODER_PRESSION_NOT_PRESSED:
		break;
		case ENCODER_PRESSION_PRESSED:
		break;
		case ENCODER_PRESSION_RELEASED:
			application().VisualizzaSSetup();
		break;
		case ENCODER_PRESSION_LONG_PRESSED:
			application().VisualizzaSMenuPrincipale();
		break;
		case ENCODER_PRESSION_VERY_LONG_PRESSED:
		break;
	}

	//___________________________________________________Valuta cosa selezionare
	switch(Selezionato)
	{
		default:
		break;
		case CMAINMIG_SELEZIONATO_VOID:
			switch(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT))
			{
				default:
				break;
				case ENCODER_PRESSION_NOT_PRESSED:
					BeepSegnalato = 0;

					if(cPSX.isVisible())
					{
						cPSX.setVisible(false);
						cPSX.invalidate();
					}
					sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
					if(sA)
					{
						if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
						{
							sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGMAN_DVELFILO]);
							data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGMAN_DVELFILO],sA);
						}
						else
						{
							sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DVELFILO]);
							data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DVELFILO],sA);
						}
					}

					if(FlgRichiestoImpostazioneRapida)
					{
						FlgRichiestoImpostazioneRapida = false;
						Selezionato = CMAINMIG_SELEZIONATO_IMPOSTAZIONERAPIDA;
						IndiceImpostazioneRapida = 0;
						OldIndiceImpostazioneRapida = 0xff;
					}
				break;
				case ENCODER_PRESSION_PRESSED:
					uSA = tgfxcustom_EncoderStatus[ENCODER_LEFT].MsTempoPressione;
					if(uSA>100)
					{
						if(!cPSX.isVisible())
							cPSX.setVisible(true);

						uLA = uSA*100;
						uLA /= CMAINMIG_MSWAITVIS;
						cPSX.setValue((int16_t)uLA);
					}
					else
					{
						if(cPSX.isVisible())
						{
							cPSX.setVisible(false);
							cPSX.invalidate();
						}
					}

					if(tgfxcustom_EncoderStatus[ENCODER_LEFT].MsTempoPressione>=CMAINMIG_MSWAITVIS)
					{
						cPSXPainter.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
						cPSX.invalidate();

						FlgRichiestoImpostazioneRapida = false;
						if(viperdef_Pack8GenTx_Base.TipoMig!=VIPERDEF_TIPOMIG_MAN)
						{
							Selezionato = CMAINMIG_SELEZIONATO_WHATVISSX;
							_board_Beep_Set(400);
						}
					}
					else if(tgfxcustom_EncoderStatus[ENCODER_LEFT].MsTempoPressione>=CMAINMIG_MSWAITIMPOSTAZIONERAPIDA)
					{
						cPSXPainter.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],0.5)));
						cPSX.invalidate();

						FlgRichiestoImpostazioneRapida = true;

						if(!BeepSegnalato)
						{
							BeepSegnalato = 1;
							_board_Beep_Set(200);
						}
					}
					else if(tgfxcustom_EncoderStatus[ENCODER_LEFT].MsTempoPressione<CMAINMIG_MSWAITVIS)
					{
						cPSXPainter.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],0.2)));
						cPSX.invalidate();
					}
				break;
				case ENCODER_PRESSION_RELEASED:
					if(Selezionato == CMAINMIG_SELEZIONATO_IMPOSTAZIONERAPIDA)
						Selezionato = CMAINMIG_SELEZIONATO_VOID;
				break;
				case ENCODER_PRESSION_LONG_PRESSED:
				break;
				case ENCODER_PRESSION_VERY_LONG_PRESSED:
				break;
			}

			switch(tgfxcustom_GetPressioneEncoder(ENCODER_RIGHT))
			{
				default:
				break;
				case ENCODER_PRESSION_NOT_PRESSED:
					if(cPDX.isVisible())
					{
						cPDX.setVisible(false);
						cPDX.invalidate();
					}
					sA=tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
					if(sA)
					{
						if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
						{
							sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGMAN_DTENSIONE]);
							data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGMAN_DTENSIONE],sA);
						}
						else
						{
							sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO]);
							data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO],sA);
						}
					}
				break;
				case ENCODER_PRESSION_PRESSED:
					uSA = tgfxcustom_EncoderStatus[ENCODER_RIGHT].MsTempoPressione;
					if(uSA>100)
					{
						if(!cPDX.isVisible())
							cPDX.setVisible(true);

						uLA = uSA*100;
						uLA /= CMAINMIG_MSWAITVIS;
						cPDX.setValue((int16_t)uLA);
					}
					else
					{
						if(cPDX.isVisible())
						{
							cPDX.setVisible(false);
							cPDX.invalidate();
						}
					}
					if(tgfxcustom_EncoderStatus[ENCODER_RIGHT].MsTempoPressione>=CMAINMIG_MSWAITVIS)
					{
						if(viperdef_Pack8GenTx_Base.TipoMig!=VIPERDEF_TIPOMIG_MAN)
							Selezionato = CMAINMIG_SELEZIONATO_WHATVISDX;
					}
				break;
				case ENCODER_PRESSION_RELEASED:
				break;
				case ENCODER_PRESSION_LONG_PRESSED:
				break;
				case ENCODER_PRESSION_VERY_LONG_PRESSED:
				break;
			}
		break;
		case CMAINMIG_SELEZIONATO_WHATVISSX:
			if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
				Selezionato = CMAINMIG_SELEZIONATO_VOID;
			else
			{
				sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
				if(sA)
				{
					ValShowSX=0xffff;
					sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UISXVIS]);
					data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UISXVIS],sA);
				}

				OldSelezionato = CMAINMIG_SELEZIONATO_MAX;	// Tanto per rigenerare la pagina
			}
		break;
		case CMAINMIG_SELEZIONATO_WHATVISDX:
			if(tgfxcustom_GetPressioneEncoder(ENCODER_RIGHT)==ENCODER_PRESSION_RELEASED)
				Selezionato = CMAINMIG_SELEZIONATO_VOID;
			else
			{
				sA=tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
				if(sA)
				{
					ValShowDX=0xffff;
					sA += data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UIDXVIS]);
					data_SetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UIDXVIS],sA);
				}

				OldSelezionato = CMAINMIG_SELEZIONATO_MAX;	// Tanto per rigenerare la pagina
			}
		break;
		case CMAINMIG_SELEZIONATO_IMPOSTAZIONERAPIDA:
			if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
			{	// Esce dalla impostazione rapida
				cImpostazioneRapida1.Seleziona(false);
				cImpostazioneRapida2.Seleziona(false);
				cImpostazioneRapida3.Seleziona(false);

				Selezionato = CMAINMIG_SELEZIONATO_VOID;
			}
			else
			{
				//__________________Valuta quale impostazione rapida selezionare
				sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
				if(sA)
				{
					sA += IndiceImpostazioneRapida;
					if(sA>=1)
						sA = 1;
					else if(sA<0)
						sA = 0;
					IndiceImpostazioneRapida = (uint8_t)sA;
				}

				if(OldIndiceImpostazioneRapida != IndiceImpostazioneRapida)
				{
					OldIndiceImpostazioneRapida = IndiceImpostazioneRapida;
					switch(IndiceImpostazioneRapida)
					{
						case 0:
							cImpostazioneRapida1.Seleziona(true);
							cImpostazioneRapida2.Seleziona(false);
							cImpostazioneRapida3.Seleziona(false);
						break;
						case 1:
							cImpostazioneRapida1.Seleziona(false);
							cImpostazioneRapida2.Seleziona(true);
							cImpostazioneRapida3.Seleziona(false);
						break;
						case 2:
							cImpostazioneRapida1.Seleziona(false);
							cImpostazioneRapida2.Seleziona(false);
							cImpostazioneRapida3.Seleziona(true);
						break;
					}
				}
			}
		break;
	}

	//______________________________________________Evidenzia cosa è selezionato
	if(OldSelezionato!=Selezionato)
	{
		OldSelezionato=Selezionato;

		bSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bSX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],0.2)));
		bSX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],0.2)));
		bSX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],0.2)));
		tASX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],0.2)));
		tASX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],0.2)));
		tASX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],0.2)));
		tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

		bDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bDX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],0.2)));
		bDX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],0.2)));
		tADX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],0.2)));
		tADX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],0.2)));
		tAValDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

		bRapida1.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bRapida2.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));
		bRapida3.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

		if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
		{
			bSX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
			tASX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));

			bDX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
			tADX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
		}
		else
		{
			switch((viperdef_MigSynSXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UISXVIS]))
			{
				default:
				break;
				case VIPERDEF_MIGSYNSXVIS_CORRENTE:
					bSX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
					tASX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
					tASX1.setY(PosYWhatDataOriginal+10);
					tASX2.setY(PosYWhatDataOriginal);
					tASX3.setY(PosYWhatDataOriginal);
					tASX1Val.setY(-30);	// Sparisce
					tASX2Val.setY(PosYValDataOriginal);
					tASX3Val.setY(PosYValDataOriginal);
				break;
				case VIPERDEF_MIGSYNSXVIS_DVELFILO:
					bSX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
					tASX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
					tASX1.setY(PosYWhatDataOriginal);
					tASX2.setY(PosYWhatDataOriginal+10);
					tASX3.setY(PosYWhatDataOriginal);
					tASX1Val.setY(PosYValDataOriginal);
					tASX2Val.setY(-30);	// Sparisce
					tASX3Val.setY(PosYValDataOriginal);
				break;
				case VIPERDEF_MIGSYNSXVIS_DSPESSORE:
					bSX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
					tASX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
					tASX1.setY(PosYWhatDataOriginal);
					tASX2.setY(PosYWhatDataOriginal);
					tASX3.setY(PosYWhatDataOriginal+10);
					tASX1Val.setY(PosYValDataOriginal);
					tASX2Val.setY(PosYValDataOriginal);
					tASX3Val.setY(-30);	// Sparisce
				break;
			}
			switch((viperdef_MigSynSXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UIDXVIS]))
			{
				default:
				break;
				case VIPERDEF_MIGSYNDXVIS_DTENSIONE:
					bDX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
					tADX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
					tADX1.setY(PosYWhatDataOriginal+10);
					tADX3.setY(PosYWhatDataOriginal);
					tADX1Val.setY(-30);	// Sparisce
					tADX3Val.setY(PosYValDataOriginal);
				break;
				case VIPERDEF_MIGSYNDXVIS_DBILANCIAMENTOTENSIONE:
					bDX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],1)));
					tADX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
					tADX1.setY(PosYWhatDataOriginal);
					tADX3.setY(PosYWhatDataOriginal+10);
					tADX1Val.setY(PosYValDataOriginal);
					tADX3Val.setY(-30);	// Sparisce
				break;
			}
			switch(Selezionato)
			{
				default:
				break;
				case CMAINMIG_SELEZIONATO_VOID:
				break;
				case CMAINMIG_SELEZIONATO_EDITSX:
					tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
					tAValDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

					bSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				break;
				case CMAINMIG_SELEZIONATO_EDITDX:
					tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
					tAValDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

					bDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
				break;
				case CMAINMIG_SELEZIONATO_WHATVISSX:
					tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
					tAValDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

					switch((viperdef_MigSynSXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UISXVIS]))
					{
						default:
						break;
						case VIPERDEF_MIGSYNSXVIS_CORRENTE:
							bSX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
							tASX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
						break;
						case VIPERDEF_MIGSYNSXVIS_DVELFILO:
							bSX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
							tASX2.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
						break;
						case VIPERDEF_MIGSYNSXVIS_DSPESSORE:
							bSX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
							tASX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
						break;
					}
				break;
				case CMAINMIG_SELEZIONATO_WHATVISDX:
					tAValSX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));
					tAValDX.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO]));

					switch((viperdef_MigSynSXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UIDXVIS]))
					{
						default:
						break;
						case VIPERDEF_MIGSYNDXVIS_DTENSIONE:
							bDX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
							tADX1.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
						break;
						case VIPERDEF_MIGSYNDXVIS_DBILANCIAMENTOTENSIONE:
							bDX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],1)));
							tADX3.setColor(colortype(viperui_OpacizzaColore(viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],1)));
						break;
					}
				break;
			}
		}

		invalidate();
	}



	//_______________________________________________Visualizza il numerone a SX
	if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
		enumData = VIPERUI_DATA_MIGMAN_DVELFILO;
	else
	{
		switch((viperdef_MigSynSXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UISXVIS]))
		{
			default:
			break;
			case VIPERDEF_MIGSYNSXVIS_CORRENTE:
				enumData = VIPERUI_DATA_MIGSYNACTUALSYNCALC_CORRENTE;
			break;
			case VIPERDEF_MIGSYNSXVIS_DVELFILO:
				enumData = VIPERUI_DATA_MIGSYN_DVELFILO;
			break;
			case VIPERDEF_MIGSYNSXVIS_DSPESSORE:
				enumData = VIPERUI_DATA_MIGSYNACTUALSYNCALC_DSPESSORE;
			break;
		}
	}
	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[enumData]);
	if(ValShowSX!=lA)
	{
		ValShowSX = lA;
		viperuisupport_GetTxtEnumData(enumData,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tAValSXBuffer,tmpStr, TAVALSX_SIZE-1);
		tAValSX.invalidate();
	}

	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_MIGSYNACTUALSYNCALC_CORRENTE]);
	if(ValShowSX1Val!=lA)
	{
		ValShowSX1Val = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_MIGSYNACTUALSYNCALC_CORRENTE,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tASX1ValBuffer,tmpStr, TASX1VAL_SIZE-1);
		tASX1Val.invalidate();
	}
	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DVELFILO]);
	if(ValShowSX2Val!=lA)
	{
		ValShowSX2Val = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_MIGSYN_DVELFILO,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tASX2ValBuffer,tmpStr, TASX2VAL_SIZE-1);
		tASX2Val.invalidate();
	}
	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_MIGSYNACTUALSYNCALC_DSPESSORE]);
	if(ValShowSX3Val!=lA)
	{
		ValShowSX3Val = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_MIGSYNACTUALSYNCALC_DSPESSORE,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tASX3ValBuffer,tmpStr, TASX3VAL_SIZE-1);
		tASX3Val.invalidate();
	}


	//_______________________________________________Visualizza il numerone a DX
	if(viperdef_Pack8GenTx_Base.TipoMig==VIPERDEF_TIPOMIG_MAN)
		enumData = VIPERUI_DATA_MIGMAN_DTENSIONE;
	else
	{
		switch((viperdef_MigSynDXVis_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_UIDXVIS]))
		{
			default:
			break;
			case VIPERDEF_MIGSYNDXVIS_DTENSIONE:
				enumData = VIPERUI_DATA_MIGSYNACTUALSYNCALC_DTENSIONE;
			break;
			case VIPERDEF_MIGSYNDXVIS_DBILANCIAMENTOTENSIONE:
				enumData = VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO;
			break;
		}
	}
	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[enumData]);
	if(ValShowDX!=lA)
	{
		ValShowDX = lA;
		viperuisupport_GetTxtEnumData(enumData,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tAValDXBuffer,tmpStr, TAVALDX_SIZE-1);
		tAValDX.invalidate();
	}

	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_MIGSYNACTUALSYNCALC_DTENSIONE]);
	if(ValShowDX1Val!=lA)
	{
		ValShowDX1Val = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_MIGSYNACTUALSYNCALC_DTENSIONE,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tADX1ValBuffer,tmpStr, TADX1VAL_SIZE-1);
		tADX1Val.invalidate();
	}
	lA = data_GetValData((const data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO]);
	if(ValShowDX3Val!=lA)
	{
		ValShowDX3Val = lA;
		viperuisupport_GetTxtEnumData(VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tADX3ValBuffer,tmpStr, TADX3VAL_SIZE-1);
		tADX3Val.invalidate();
	}

	cImpostazioneRapida1.ManagerTickEvent();
	cImpostazioneRapida2.ManagerTickEvent();
	cImpostazioneRapida3.ManagerTickEvent();
	cImpostazioneRapidaSinergia.ManagerTickEvent();

}
