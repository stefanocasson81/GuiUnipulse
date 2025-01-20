#include <gui/ssetup_screen/SSetupView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

extern "C"
{
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include <ViperUISupport.h>
	#include "CORELib/UserInterface/ViperUI.h"
};



static const viperuisupport_StructParametri_t TabellaParametriMigMan[]=
{
	{ _NULL_,		T_DINAMICA,					"%",			VIPERUI_DATA_MIGMAN_INDUTTANZA				},
	{ _NULL_,		T_TEMPI,					_NULL,			VIPERUI_DATA_MIG_TEMPI						},
	{ _NULL_,		T_PREGAS,					"s",			VIPERUI_DATA_MIG_DSPREGAS					},
	{ _NULL_,		T_SLOPE,					"s",			VIPERUI_DATA_MIG_CSWIRESLOPE				},

	{ _NULL_,		T_HOTSTART_SPEED,			"m/min",		VIPERUI_DATA_MIGMANHOTSTART_DVELFILO		},
	{ _NULL_,		T_HOTSTART_VOLTAGE,			"V",			VIPERUI_DATA_MIGMANHOTSTART_DTENSIONE		},
	{ _NULL_,		T_HOTSTART_TIME,			"s",			VIPERUI_DATA_MIGMANHOTSTART_DSECDURATA		},
	{ _NULL_,		T_HOTSTART_SLOPE,			"s",			VIPERUI_DATA_MIGMANHOTSTART_DSECSLOPE		},
	{ _NULL_,		T_CRATERFILLER_SPEED,		"m/min",		VIPERUI_DATA_MIGMANCRATERFILLER_DVELFILO	},
	{ _NULL_,		T_CRATERFILLER_VOLTAGE,		"V",			VIPERUI_DATA_MIGMANCRATERFILLER_DTENSIONE	},
	{ _NULL_,		T_CRATERFILLER_SLOPE,		"s",			VIPERUI_DATA_MIGMANCRATERFILLER_DSECSLOPE	},
	{ _NULL_,		T_CRATERFILLER_TIME,		"s",			VIPERUI_DATA_MIGMANCRATERFILLER_DSECDURATA	},

	{ _NULL_,		T_BBT,						_NULL_,			VIPERUI_DATA_MIG_BBT						},
	{ _NULL_,		T_POSTGAS,					"s",			VIPERUI_DATA_MIG_DSPOSTGAS					},

	{ _NULL_ }
};


static const viperuisupport_StructParametri_t TabellaParametriMigSyn[]=
{
	{ _NULL_,		T_DINAMICA,					"%",			VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO	},
	{ _NULL_,		T_TEMPI,					_NULL,			VIPERUI_DATA_MIG_TEMPI						},
	{ _NULL_,		T_PREGAS,					"s",			VIPERUI_DATA_MIG_DSPREGAS					},
	{ _NULL_,		T_SLOPE,					"s",			VIPERUI_DATA_MIG_CSWIRESLOPE				},

	{ _NULL_,		T_HOTSTART_SPEEDPERC,		"% m/min",		VIPERUI_DATA_MIGSYNHOTSTART_PERCSPEED		},
	{ _NULL_,		T_HOTSTART_BALANCEV,		"V",			VIPERUI_DATA_MIGSYNHOTSTART_DBALANCEV		},
	{ _NULL_,		T_HOTSTART_TIME,			"s",			VIPERUI_DATA_MIGSYNHOTSTART_DSECDURATA		},
	{ _NULL_,		T_HOTSTART_SLOPE,			"s",			VIPERUI_DATA_MIGSYNHOTSTART_DSECSLOPE		},
	{ _NULL_,		T_CRATERFILLER_SLOPE,		"s",			VIPERUI_DATA_MIGSYNCRATERFILLER_DSECSLOPE	},
	{ _NULL_,		T_CRATERFILLER_SPEEDPERC,	"% m/min",		VIPERUI_DATA_MIGSYNCRATERFILLER_PERCSPEED	},
	{ _NULL_,		T_CRATERFILLER_BALANCEV,	"V",			VIPERUI_DATA_MIGSYNCRATERFILLER_DBALANCEV	},
	{ _NULL_,		T_CRATERFILLER_TIME,		"s",			VIPERUI_DATA_MIGSYNCRATERFILLER_DSECDURATA	},

	{ _NULL_,		T_BBT,						_NULL_,			VIPERUI_DATA_MIG_BBT						},
	{ _NULL_,		T_POSTGAS,					"s",			VIPERUI_DATA_MIG_DSPOSTGAS					},

	{ _NULL_ }
};


static const viperuisupport_StructParametri_t TabellaParametriMigPulse[]=
{
	{ _NULL_,		T_DINAMICA,					"%",			VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO	},
	{ _NULL_,		T_TEMPI,					_NULL,			VIPERUI_DATA_MIG_TEMPI						},
	{ _NULL_,		T_PREGAS,					"s",			VIPERUI_DATA_MIG_DSPREGAS					},
	{ _NULL_,		T_SLOPE,					"s",			VIPERUI_DATA_MIG_CSWIRESLOPE				},

	{ _NULL_,		T_HOTSTART_SPEEDPERC,		"% m/min",		VIPERUI_DATA_MIGSYNHOTSTART_PERCSPEED		},
	{ _NULL_,		T_HOTSTART_BALANCEV,		"V",			VIPERUI_DATA_MIGSYNHOTSTART_DBALANCEV		},
	{ _NULL_,		T_HOTSTART_TIME,			"s",			VIPERUI_DATA_MIGSYNHOTSTART_DSECDURATA		},
	{ _NULL_,		T_HOTSTART_SLOPE,			"s",			VIPERUI_DATA_MIGSYNHOTSTART_DSECSLOPE		},
	{ _NULL_,		T_CRATERFILLER_SLOPE,		"s",			VIPERUI_DATA_MIGSYNCRATERFILLER_DSECSLOPE	},
	{ _NULL_,		T_CRATERFILLER_SPEEDPERC,	"% m/min",		VIPERUI_DATA_MIGSYNCRATERFILLER_PERCSPEED	},
	{ _NULL_,		T_CRATERFILLER_BALANCEV,	"V",			VIPERUI_DATA_MIGSYNCRATERFILLER_DBALANCEV	},
	{ _NULL_,		T_CRATERFILLER_TIME,		"s",			VIPERUI_DATA_MIGSYNCRATERFILLER_DSECDURATA	},

	{ _NULL_,		T_BBT,						_NULL_,			VIPERUI_DATA_MIG_BBT						},
	{ _NULL_,		T_POSTGAS,					"s",			VIPERUI_DATA_MIG_DSPOSTGAS					},

	{ _NULL_ }
};

static const viperuisupport_StructParametri_t TabellaParametriTig[]=
{
	{ _NULL_,		T_VCUT,			"V",		VIPERUI_DATA_TIG_DTENSCUTOFF			},

	{ _NULL_ }
};

static const viperuisupport_StructParametri_t TabellaParametriMma[]=
{
	{ _NULL_,		T_MMA_ARCFORCE,		_NULL_,					VIPERUI_DATA_MMA_ARCFORCE	},
	{ _NULL_,		T_MMA_HOTSTART,		"A",					VIPERUI_DATA_MMA_HOTSTART	},

	{ _NULL_ }
};

SSetupView::SSetupView()
{
	OldIndiceParametroSelezionato = 0xff;

	ShowTempi = VIPERDEF_TEMPI_MAX;

	switch(((viperdef_Processo_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_PROCESSO])))
	{
		default:
		break;

		case VIPERDEF_PROCESSO_MMA:
			iCiclo.setVisible(false);
			bSfondoBase.setVisible(false);
			bMobileBase.setVisible(false);

			Unicode::strncpy(tATitoloBuffer,"MMA", TAVALUE_SIZE-1);

			viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametriMma);
		break;
		case VIPERDEF_PROCESSO_TIG:
			iCiclo.setVisible(false);
			bSfondoBase.setVisible(false);
			bMobileBase.setVisible(false);

			Unicode::strncpy(tATitoloBuffer,"TIG", TAVALUE_SIZE-1);

			viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametriTig);
		break;
		case VIPERDEF_PROCESSO_MIG:
			iCiclo.setVisible(true);
			bSfondoBase.setVisible(true);
			switch(((viperdef_TipoMig_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOMIG])))
			{
				default:
				break;

				case VIPERDEF_TIPOMIG_MAN:
					viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametriMigMan);
				break;
				case VIPERDEF_TIPOMIG_SYN:
					viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametriMigSyn);
				break;
				case VIPERDEF_TIPOMIG_PULSE:
					viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametriMigPulse);
				break;
			}
		break;
	}

	ValShow = 0xffff;

	bSelezionato.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	bMobileBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	handleTickEvent();	// Prima rigenerata quadro forzato
}

void SSetupView::setupScreen()
{
    SSetupViewBase::setupScreen();
}

void SSetupView::tearDownScreen()
{
    SSetupViewBase::tearDownScreen();
}

void SSetupView::handleTickEvent()
{
	int16_t sA;
	int32_t lA;
	char tmpStr[30];

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
		application().VisualizzaSMain();

	if(tgfxcustom_GetPressioneEncoder(ENCODER_CENTER)==ENCODER_PRESSION_LONG_PRESSED)
		application().VisualizzaSMenuPrincipale();


	if(ObjTabParametri.NumeroParametri)
	{
		sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
		if(sA)
			viperuisupport_ObjTabParametri_SelezioneOffset(&ObjTabParametri,sA);

		lA=tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
		if(lA)
		{
			lA+=data_GetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData]);
			data_SetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData],lA);
		}
	}

	if(((viperdef_Processo_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_PROCESSO]))==VIPERDEF_PROCESSO_MIG)
	{
		lA = data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_MIG_TEMPI]);
		if(ShowTempi!=lA)
		{
			ShowTempi = (viperdef_Tempi_e)lA;
			switch(ShowTempi)
			{
				default:
				case VIPERDEF_TEMPI_2T:
					iCiclo.setBitmap(touchgfx::Bitmap(BITMAP_IMG2TFULL_ID));
					Unicode::strncpy(tATitoloBuffer,"2T", TAVALUE_SIZE-1);
				break;
				case VIPERDEF_TEMPI_2TPLUS:
					iCiclo.setBitmap(touchgfx::Bitmap(BITMAP_IMG2TFULL_ID));
					Unicode::strncpy(tATitoloBuffer,"2T FULL", TAVALUE_SIZE-1);
				break;

				case VIPERDEF_TEMPI_4T:
					iCiclo.setBitmap(touchgfx::Bitmap(BITMAP_IMG4TFULL_ID));
					Unicode::strncpy(tATitoloBuffer,"4T", TAVALUE_SIZE-1);
				break;
				case VIPERDEF_TEMPI_4TPLUS:
					iCiclo.setBitmap(touchgfx::Bitmap(BITMAP_IMG4TFULL_ID));
					Unicode::strncpy(tATitoloBuffer,"4T FULL", TAVALUE_SIZE-1);
				break;
			}
			invalidate();
		}
	}

	if(ObjTabParametri.NumeroParametri)
	{
		if(OldIndiceParametroSelezionato!=ObjTabParametri.IndiceParametroSelezionato)
		{
			OldIndiceParametroSelezionato=ObjTabParametri.IndiceParametroSelezionato;

			if(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].PtrTestoParametro)
				Unicode::strncpy(tAParametroBuffer,ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].PtrTestoParametro, TAPARAMETRO_SIZE-1);
			else
				Unicode::strncpy(tAParametroBuffer,touchgfx::TypedText(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumTypedText).getText(), TAPARAMETRO_SIZE-1);

			tAParametro.invalidate();

			imageSelezionato.setVisible(true);
			bMobileBase.setVisible(true);
			switch(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData)
			{
				default:
					imageSelezionato.setVisible(false);
					bMobileBase.setVisible(false);
				break;

				case VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO:
				case VIPERUI_DATA_MIGMAN_INDUTTANZA:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOINDUTTANZA30X14_ID));
					bMobileBase.setVisible(false);
				break;

				case VIPERUI_DATA_MIG_TEMPI:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICO2TWHITE54X14_ID));

					bMobileBase.setXY(7,191);
				break;
				case VIPERUI_DATA_MIG_DSPREGAS:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOPREGAS38X20_ID));

					bMobileBase.setXY(48,145);
				break;
				case VIPERUI_DATA_MIG_CSWIRESLOPE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOMOTORSLOPE46X20_ID));

					bMobileBase.setXY(74,102);
				break;


				case VIPERUI_DATA_MIGMANHOTSTART_DVELFILO:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTV38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGMANHOTSTART_DTENSIONE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTV38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGMANHOTSTART_DSECDURATA:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTT38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGMANHOTSTART_DSECSLOPE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTSLOPE38X20_ID));

					bMobileBase.setXY(178,64);
				break;
				case VIPERUI_DATA_MIGMANCRATERFILLER_DVELFILO:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERSLOPE31X20_ID));

					bMobileBase.setXY(325,111);
				break;
				case VIPERUI_DATA_MIGMANCRATERFILLER_DTENSIONE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERV31X20_ID));

					bMobileBase.setXY(325,111);
				break;
				case VIPERUI_DATA_MIGMANCRATERFILLER_DSECSLOPE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERSLOPE31X20_ID));

					bMobileBase.setXY(275,96);
				break;
				case VIPERUI_DATA_MIGMANCRATERFILLER_DSECDURATA:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERPERCENT31X20_ID));

					bMobileBase.setXY(275,96);
				break;

				case VIPERUI_DATA_MIGSYNHOTSTART_PERCSPEED:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTPERCENT38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGSYNHOTSTART_DBALANCEV:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTV38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGSYNHOTSTART_DSECDURATA:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTT38X20_ID));

					bMobileBase.setXY(133,35);
				break;
				case VIPERUI_DATA_MIGSYNHOTSTART_DSECSLOPE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOHOTSTARTT38X20_ID));

					bMobileBase.setXY(178,64);
				break;
				case VIPERUI_DATA_MIGSYNCRATERFILLER_PERCSPEED:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERPERCENT31X20_ID));

					bMobileBase.setXY(325,111);
				break;
				case VIPERUI_DATA_MIGSYNCRATERFILLER_DBALANCEV:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERV31X20_ID));

					bMobileBase.setXY(325,111);
				break;
				case VIPERUI_DATA_MIGSYNCRATERFILLER_DSECSLOPE:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERSLOPE31X20_ID));

					bMobileBase.setXY(275,96);
				break;
				case VIPERUI_DATA_MIGSYNCRATERFILLER_DSECDURATA:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOCRATERPERCENT31X20_ID));

					bMobileBase.setXY(325,111);
				break;


				case VIPERUI_DATA_MIG_BBT:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOBBT20X20_ID));

					bMobileBase.setXY(369,144);
				break;
				case VIPERUI_DATA_MIG_DSPOSTGAS:
					imageSelezionato.setBitmap(touchgfx::Bitmap(BITMAP_ICOPOSTGAS38X20_ID));

					bMobileBase.setXY(413,145);
				break;
			}
			invalidate();

			ValShow = 0xffff;	// Forza la rigenerata
		}

		lA = data_GetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData]);
		if(ValShow != lA)
		{
			ValShow = lA;
			viperuisupport_GetTxtEnumData(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData,tmpStr, sizeof(tmpStr));
			if(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].PtrStrUdm)
			{
				strcat(tmpStr," ");
				strcat(tmpStr,ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].PtrStrUdm);
			}

			Unicode::strncpy(tAValueBuffer,tmpStr, TAVALUE_SIZE-1);
			tAValue.invalidate();
		}
	}
}
