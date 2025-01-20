#include <gui/sbasic_screen/SBasicView.hpp>

extern "C"
{
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include <ViperUISupport.h>
	#include "_Board.h"
};

static const viperuisupport_StructParametri_t TabellaParametri[]=
{
	{ "Gruppo H2O",					_NULL,		_NULL_,			VIPERUI_DATA_BASICSETUP_GRUPPOH2OEN	},

	{ "KIo",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_KIO   	},
	{ "K0Io",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_K0IO  	},
	{ "KVo",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_KVO   	},
	{ "K0Vo",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_K0VO  	},
	{ "KISet",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_KISET 	},
	{ "K0ISet",						_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_K0ISET	},
	{ "FascioCavi MilliOhm res.",	_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_FASCIOCAVIRESISTENZA	},
	{ "FascioCavi MicroHenry ind.",	_NULL,		_NULL_,			VIPERUI_DATA_TARATURAESSEN_FASCIOCAVIINDUTTANZA	},

	{ _NULL_ }
};


SBasicView::SBasicView()
{
	OldIndiceParametroSelezionato = 0xff;
	viperuisupport_ObjTabParametri_Create(&ObjTabParametri,TabellaParametri);

	ValShow = 0xffff;

//	ValShow
}

void SBasicView::setupScreen()
{
    SBasicViewBase::setupScreen();
}

void SBasicView::tearDownScreen()
{
    SBasicViewBase::tearDownScreen();
}

void SBasicView::handleTickEvent()
{
	int16_t sA;
	int32_t lA;
	char tmpStr[30];

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
	{
		presenter->setSaveParametersEeprom();
//		presenter->setSaveCalibration();
//		viperui_Generatore_SalvaTaratura();	//niknik da togliere questo accrocchio, qui NON c'è taratura da salvare nella versione ufficiale, viene usato solo per taratura fascio cavi
		application().VisualizzaSMain();
	}

	sA=tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
	if(sA)
		viperuisupport_ObjTabParametri_SelezioneOffset(&ObjTabParametri,sA);

	lA=tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
	if(lA)
	{
		lA+=data_GetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData]);
		data_SetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData],lA);

	}

	if(OldIndiceParametroSelezionato!=ObjTabParametri.IndiceParametroSelezionato)
	{
		OldIndiceParametroSelezionato=ObjTabParametri.IndiceParametroSelezionato;

		Unicode::strncpy(tATitoloBuffer,ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].PtrTestoParametro, TATITOLO_SIZE-1);
		tATitolo.invalidate();

		ValShow = 0xffff;
	}

	lA=data_GetValData(&viperui_StrutturaDati[ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData]);
	if(ValShow!=lA)
	{
		ValShow = lA;

		viperuisupport_GetTxtEnumData(ObjTabParametri.PtrStructParametri[ObjTabParametri.IndiceParametroSelezionato].EnumData,tmpStr, sizeof(tmpStr));
		Unicode::strncpy(tAValoreBuffer,tmpStr, TAVALORE_SIZE-1);
		tAValore.invalidate();
	}


}
