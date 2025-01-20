/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	06/04/23
 Descrizione:
	Manager core Viper (nuova generazione saldatrici) lato user interface.
	Questo modulo è la madre di tutto il core gestione user interface su base
	TouchGFX.
	All'InitModule è necessario avere scheda e swtimer inizilizzato.
------------------------------------------------------------------------------*/
#include <ARMLib/TGFX/cpp_define.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <gui/custom/WeldingSupport.hpp>
#include <ARMLib/TGFX/TGFXCustom.h>
#include <gui/model/ModelListener.hpp>
#include <TouchGFX/Color.hpp>
extern "C"
{
   #include "gui/custom/CommonDefine.h"
   #include "Features.h"
}

/*******************************************************
 * GLOBAL DEFINES
 *******************************************************/

//#define COLOREAZIENDALE     Color::getColorFrom24BitRGB(0, 0, 255)
//#define NERO                Color::getColorFrom24BitRGB(0, 0, 255)
//#define GRIGIOSCURO         Color::getColorFrom24BitRGB(0, 0, 255)
//#define GRIGIOMEDIO         Color::getColorFrom24BitRGB(0, 0, 255)
//#define GRIGIOCHIARO        Color::getColorFrom24BitRGB(0, 0, 255)
//#define BIANCO              Color::getColorFrom24BitRGB(0, 0, 255)


/*******************************************************
 * GLOBAL VARIABLES
 *******************************************************/

U32 viperui_ListStdColor[VIPERUI_STDCOLOR_MAX]=
{
   FEATURES_RGB_COLOREAZIENDALE,
   FEATURES_RGB_NERO,
   FEATURES_RGB_GRIGIOSCURO,
   FEATURES_RGB_GRIGIOMEDIO,
   FEATURES_RGB_GRIGIOCHIARO,
   FEATURES_RGB_BIANCO,
};



//============================================================= GLOBAL FUNCTIONS


//ColorDefined ColorContainer;

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
cViper_Info::cViper_Info():sm_State(SM_Init),pModelListner(0)
{
   memset(&SelezioneProcesso, 0x00, sizeof(SelezioneProcesso));
   viperui_InitData();
//   ProcessoAllocato = VIPERDEF_PROCESSO_MAX;
   SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
   SelezioneProcesso.IndiceDiametroFilo = 0;
   SelezioneProcesso.IndiceTipoGas = 0;
   SelezioneProcesso.IndiceTipoMig = 0;
//   viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, SelezioneProcesso.CurvaInfo.DiametroFilo, SelezioneProcesso.CurvaInfo.TipoGas);
}


void cViper_Info::viperui_LoadData(void)
{

//   SelezioneProcesso.IndiceDiametroFilo = 0;
//   SelezioneProcesso.IndiceTipoGas = 0;
//   SelezioneProcesso.IndiceTipoMig = 0;
//   viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, SelezioneProcesso.CurvaInfo.DiametroFilo, SelezioneProcesso.CurvaInfo.TipoGas);

   SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
   SelezioneProcesso.Processo = viperdef_Pack8GenTx_Base.Processo;

   //todo da eliminare una volta che si legge ilprocesso da generatore
   SelezioneProcesso.Processo = VIPERDEF_PROCESSO_MIG;
   //

   if ( SelezioneProcesso.Processo == VIPERDEF_PROCESSO_MIG )
   {
      SelezioneProcesso.CurvaInfo.TipoMig = viperdef_Pack8GenTx_Base.TipoMig;

      if ( viperdef_Pack8GenTx_Base.TipoMig == VIPERDEF_TIPOMIG_MAN )
         SelezioneProcesso.CurvaInfo.TipoFilo = VIPERDEF_TIPOFILO_MANUAL;
      else
         SelezioneProcesso.CurvaInfo.TipoFilo = viperdef_Pack8GenTx_Base.TipoFilo;

      SelezioneProcesso.CurvaInfo.TipoGas = viperdef_Pack8GenTx_Base.TipoGas;
      SelezioneProcesso.CurvaInfo.DiametroFilo = viperdef_Pack8GenTx_Base.DiametroFilo;

      SelezioneProcesso.CurvaInfoDiPartenza.TipoMig = SelezioneProcesso.CurvaInfo.TipoMig;
      SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo = SelezioneProcesso.CurvaInfo.TipoFilo;
      SelezioneProcesso.CurvaInfoDiPartenza.TipoGas = SelezioneProcesso.CurvaInfo.TipoGas;
      SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = SelezioneProcesso.CurvaInfo.DiametroFilo;
   }

}

void cViper_Info::viperui_InitData()
{
   U16 indice=0;
	memset(ListaCurveInfoDisponibili,0xff,sizeof(ListaCurveInfoDisponibili));
	memset(&oldCurva,0xff,sizeof(oldCurva));
	//_______________________________________________________________________SYN
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4145;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4557;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER80;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER80;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;




	/*__________________________________________________________PULSE____________________________*/
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;
//
//
	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI2209;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4145;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4557;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;



//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_NICR3;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;




}



//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_CambiaProcesso(viperdef_Processo_e processo,viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_TipoGas_e tipoGas,viperdef_DiametroFilo_e diametroFilo)
{
	uint32_t uLA;

	uLA = (uint8_t)diametroFilo;
	uLA<<=8;
	uLA |= (uint8_t)tipoGas;
	uLA<<=8;
	uLA|=(uint8_t)tipoFilo;
	uLA<<=4;
	uLA|=(uint8_t)tipoMig;
	uLA<<=4;
	uLA|=processo;


	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_CAMBIAPROCESSO;   // Funzione da eseguiore
	viperdef_Pack8GenRx_FunExecute.ValueFunction = uLA;

	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_SalvaTaratura(void)
{
	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_SALVATARATURA;   // Funzione da eseguire
	viperdef_Pack8GenRx_FunExecute.ValueFunction = 0x00;
	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_SalvaParametriInEEProm(void)
{
	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_SALVAPARAMETRIINEEPROM;   // Funzione da eseguire
	viperdef_Pack8GenRx_FunExecute.ValueFunction = 0x00;
	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Ricalcolo combinazioni di selezioni possibili per processo MIG
// Arg: -
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_RecalcBitMappedFiloDiametroGasMig(viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_DiametroFilo_e diametroFilo,viperdef_TipoGas_e tipoGas)
{
	uint32_t indice;
	uint8_t beccato;
	uint8_t uCA;

	SelezioneProcesso.BitMappedDiametroFilo = 0;
	SelezioneProcesso.BitMappedTipoGas = 0;
	SelezioneProcesso.BitMappedTipoMig = 0;
	SelezioneProcesso.BitMappedTipoFilo = 0;

	indice = 0;
	while(ListaCurveInfoDisponibili[indice].DiametroFilo!=0xff)
   {
      beccato = 0;

      if ( tipoFilo == VIPERDEF_TIPOFILO_MAX )
         beccato |= _B0_;
      else
      {
         if ( ListaCurveInfoDisponibili[indice].TipoFilo == tipoFilo )
            beccato |= _B0_;
      }

      if ( diametroFilo == VIPERDEF_DIAMETROFILO_MAX )
         beccato |= _B1_;
      else
      {
         if ( ListaCurveInfoDisponibili[indice].DiametroFilo == diametroFilo )
            beccato |= _B1_;
      }

      if ( tipoGas == VIPERDEF_TIPOGAS_MAX )
         beccato |= _B2_;
      else
      {
         if ( ListaCurveInfoDisponibili[indice].TipoGas == tipoGas )
            beccato |= _B2_;
      }

//         if ( tipoMig == VIPERDEF_TIPOMIG_MAX )
//              beccato |= _B3_;
//           else
//           {
      if ( ListaCurveInfoDisponibili[indice].TipoMig == tipoMig )
         beccato |= _B3_;
//           }

      if ( beccato == 0x0F )
      {
         SelezioneProcesso.BitMappedDiametroFilo |= _B0_ << ListaCurveInfoDisponibili[indice].DiametroFilo;
         SelezioneProcesso.BitMappedTipoGas |= _B0_ << ListaCurveInfoDisponibili[indice].TipoGas;
         SelezioneProcesso.BitMappedTipoMig |= _B0_ << ListaCurveInfoDisponibili[indice].TipoMig;
         SelezioneProcesso.BitMappedTipoFilo |= _B0_ << ListaCurveInfoDisponibili[indice].TipoFilo;
      }

      indice++;
   }

	SelezioneProcesso.IndiceMaxEnumDiametro=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedDiametroFilo&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumDiametro[SelezioneProcesso.IndiceMaxEnumDiametro]=uCA;
			SelezioneProcesso.IndiceMaxEnumDiametro++;
		}
	}
	SelezioneProcesso.IndiceMaxEnumTipoGas=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedTipoGas&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumTipoGas[SelezioneProcesso.IndiceMaxEnumTipoGas]=uCA;
			SelezioneProcesso.IndiceMaxEnumTipoGas++;
		}
	}
	SelezioneProcesso.IndiceMaxEnumTipoMig=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedTipoMig&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumTipoMig[SelezioneProcesso.IndiceMaxEnumTipoMig]=uCA;
			SelezioneProcesso.IndiceMaxEnumTipoMig++;
		}
	}
	SelezioneProcesso.IndiceMaxEnumTipoFilo = 0;
	for(uCA=0;uCA<32;uCA++)
	   {
	      if(SelezioneProcesso.BitMappedTipoFilo&(_B0_<<uCA))
	      {
	         SelezioneProcesso.TabEnumTipoFilo[SelezioneProcesso.IndiceMaxEnumTipoFilo]=uCA;
	         SelezioneProcesso.IndiceMaxEnumTipoFilo++;
	      }
	   }
}



U8 cViper_Info::viperui_ManagerTickEvents(const Encoder_t &encS, const Encoder_t &encR, void *pMListner)
{
   S16 Enc_R_OffsetTmp = 0;
//   S16 Enc_S_OffsetTmp = 0;
   S16 ActualSelection;
   U8 uCA;

   switch (sm_State)
   {
      case SM_Init:
//         SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
//         SelezioneProcesso.IndiceDiametroFilo = 0;
//         SelezioneProcesso.IndiceTipoGas = 0;
//         SelezioneProcesso.IndiceTipoMig = 0;
           viperui_LoadData();
//         viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoFilo, SelezioneProcesso.CurvaInfo.DiametroFilo,SelezioneProcesso.CurvaInfo.TipoGas);

         sm_State = SM_Run;
      break;

      case SM_Run:

//         if ( !encR.Offset )
//            return 0;

//         ActualSelection = (S16)SelezioneProcesso.AttualeSelezione;
         if ( encS.Offset > 0 || (encR.Pression == ENCODER_PRESSION_RELEASED))
            ActualSelection = 1;
         else if ( encS.Offset < 0 )
            ActualSelection = -1;
         else
            ActualSelection = 0;

//         if ( ActualSelection < VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO )
//         {
//            ActualSelection = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
//         }
//         if ( ActualSelection > VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS )
//         {
//            ActualSelection = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
//         }
//         SelezioneProcesso.AttualeSelezione = (viperui_AttualeSelezioneProcesso_e)ActualSelection;
//
//         if ( SelezioneProcesso.oldAttualeSelezione != SelezioneProcesso.AttualeSelezione )
//         {
//            SelezioneProcesso.oldAttualeSelezione = SelezioneProcesso.AttualeSelezione;
//            ((ModelListener*) pMListner)->setMigWeldingProcessMenu(SelezioneProcesso.AttualeSelezione);
//         }
//         else if(!encR.Offset) //se nessuno dei 2 encoder e' stato mosso esco
//         {
//            return 0;
//         }

         if ( SelezioneProcesso.Processo < VIPERDEF_PROCESSO_MIG )
         {
            if ( SelezioneProcesso.AttualeSelezione > VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO )
            {
               SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
            }
         }

         Enc_R_OffsetTmp = encR.Offset;

         switch (SelezioneProcesso.AttualeSelezione)
         {
            case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:

               if ( Enc_R_OffsetTmp )
               {
                  Enc_R_OffsetTmp += SelezioneProcesso.Processo;
                  if ( Enc_R_OffsetTmp < 0 )
                  {
                     Enc_R_OffsetTmp = 0;
                  }
                  else if ( Enc_R_OffsetTmp >= VIPERDEF_PROCESSO_MAX )
                  {
                     Enc_R_OffsetTmp = VIPERDEF_PROCESSO_MAX - 1;
                  }
                  SelezioneProcesso.Processo = (viperdef_Processo_e) Enc_R_OffsetTmp;
                  if ( SelezioneProcesso.Processo != SelezioneProcesso.oldProcesso )
                  {
                     SelezioneProcesso.oldProcesso = SelezioneProcesso.Processo;
                     ((ModelListener*) pMListner)->setProcessWelding(SelezioneProcesso.Processo);
                  }
               }

               if(SelezioneProcesso.Processo == VIPERDEF_PROCESSO_MIG)
               {
                  if ( ActualSelection > 0 )
                  {
                     SelezioneProcesso.CurvaInfo.TipoFilo = SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo;
                     SelezioneProcesso.CurvaInfo.TipoMig = SelezioneProcesso.CurvaInfoDiPartenza.TipoMig;
                     viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,VIPERDEF_TIPOFILO_MAX, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);
                     SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
                  }
                  else if ( ActualSelection < 0 )
                     SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;

                  SelezioneProcesso.IndiceDiametroFilo = 0xff;   // Seleziona quello simile o il primo se mancante
                  SelezioneProcesso.IndiceTipoGas = 0xff;     // Seleziona quello simile o il primo se mancante
               SelezioneProcesso.IndiceTipoMig = 0xff;     // Seleziona quello simile o il primo se mancante
               }
            break;

            case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG:

               if ( Enc_R_OffsetTmp )
               {
                  Enc_R_OffsetTmp += SelezioneProcesso.IndiceTipoMig;
                  if ( Enc_R_OffsetTmp < 0 )
                  {
                     Enc_R_OffsetTmp = 0;
                  }
                  else if ( Enc_R_OffsetTmp >= SelezioneProcesso.IndiceMaxEnumTipoMig )
                  {
                     if(SelezioneProcesso.IndiceMaxEnumTipoMig)
                        Enc_R_OffsetTmp = SelezioneProcesso.IndiceMaxEnumTipoMig - 1;
                  }
                  SelezioneProcesso.IndiceTipoMig = Enc_R_OffsetTmp;
                  SelezioneProcesso.CurvaInfo.TipoMig = (viperdef_TipoMig_e) SelezioneProcesso.TabEnumTipoMig[SelezioneProcesso.IndiceTipoMig];
                  SelezioneProcesso.CurvaInfoDiPartenza.TipoMig = SelezioneProcesso.CurvaInfo.TipoMig;
//                  viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoFilo, SelezioneProcesso.CurvaInfo.DiametroFilo, SelezioneProcesso.CurvaInfo.TipoGas);
                  viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,VIPERDEF_TIPOFILO_MAX, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);
               }
               if ( ActualSelection > 0 )
               {
//                  viperui_Generatore_CambiaProcesso(SelezioneProcesso.Processo, SelezioneProcesso.CurvaInfo.TipoMig, SelezioneProcesso.CurvaInfo.TipoFilo,SelezioneProcesso.CurvaInfo.TipoGas, SelezioneProcesso.CurvaInfo.DiametroFilo);
                  //ToDo da inserire un cambio schermata,
//                  viperdef_Pack8GenTx_Base.Processo = SelezioneProcesso.Processo;  // Solo per anticipare il refresh el quadro main al nuovo processo...

                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
               }
               else if ( ActualSelection < 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
            break;

            case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO:
               if ( Enc_R_OffsetTmp )
               {
                  Enc_R_OffsetTmp += SelezioneProcesso.CurvaInfo.TipoFilo;
                  if ( Enc_R_OffsetTmp < 0 )
                  {
                     Enc_R_OffsetTmp = 0;
                  }
                  else if ( Enc_R_OffsetTmp >= VIPERDEF_TIPOFILO_MAX )
                     Enc_R_OffsetTmp = VIPERDEF_TIPOFILO_MAX - 1;

                  if ( SelezioneProcesso.CurvaInfo.TipoFilo != (viperdef_TipoFilo_e) Enc_R_OffsetTmp )
                  {
                     SelezioneProcesso.CurvaInfo.TipoFilo = (viperdef_TipoFilo_e) Enc_R_OffsetTmp;
                     SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo = SelezioneProcesso.CurvaInfo.TipoFilo;

                     viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

                     SelezioneProcesso.IndiceDiametroFilo = 0xff;   // Seleziona quello simile o il primo se mancante
                     SelezioneProcesso.IndiceTipoGas = 0xff;     // Seleziona quello simile o il primo se mancante
                     SelezioneProcesso.IndiceTipoMig = 0xff;     // Seleziona quello simile o il primo se mancante
                  }
               }
               if(ActualSelection > 0 )
               {
                if(SelezioneProcesso.CurvaInfo.TipoFilo == VIPERDEF_TIPOFILO_MANUAL)
                {
                   viperui_Generatore_CambiaProcesso(SelezioneProcesso.Processo,VIPERDEF_TIPOMIG_MAN,VIPERDEF_TIPOFILO_MANUAL,VIPERDEF_TIPOGAS_M21_MIX8020,VIPERDEF_DIAMETROFILO_10);
                   viperdef_Pack8GenTx_Base.Processo = SelezioneProcesso.Processo;  // Solo per anticipare il refresh el quadro main al nuovo processo...
                //ToDo da inserire un cambio schermata,

                }
                else
                {
                   viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);
                }
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO;
               }
               else if (ActualSelection < 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
            break;

            case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO:
               if ( Enc_R_OffsetTmp )
               {
                  Enc_R_OffsetTmp += SelezioneProcesso.IndiceDiametroFilo;
                  if ( Enc_R_OffsetTmp < 0 )
                     Enc_R_OffsetTmp = 0;
                  else if ( Enc_R_OffsetTmp >= SelezioneProcesso.IndiceMaxEnumDiametro )
                  {
                     if(SelezioneProcesso.IndiceMaxEnumDiametro)
                        Enc_R_OffsetTmp = SelezioneProcesso.IndiceMaxEnumDiametro - 1;
                  }
                  SelezioneProcesso.IndiceDiametroFilo = Enc_R_OffsetTmp;

                  SelezioneProcesso.CurvaInfo.DiametroFilo = (viperdef_DiametroFilo_e) SelezioneProcesso.TabEnumDiametro[SelezioneProcesso.IndiceDiametroFilo];
                  SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = SelezioneProcesso.CurvaInfo.DiametroFilo;

                  viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, VIPERDEF_DIAMETROFILO_MAX, VIPERDEF_TIPOGAS_MAX);

                  SelezioneProcesso.IndiceTipoGas = 0xff;     // Seleziona quello simile o il primo se mancante
                  SelezioneProcesso.IndiceTipoMig = 0xff;     // Seleziona quello simile o il primo se mancante
               }
               if(ActualSelection > 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
               else if (ActualSelection < 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO;
            break;

            case VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS:
               if ( Enc_R_OffsetTmp )
               {
                  Enc_R_OffsetTmp += SelezioneProcesso.IndiceTipoGas;
                  if ( Enc_R_OffsetTmp < 0 )
                     Enc_R_OffsetTmp = 0;
                  else if ( Enc_R_OffsetTmp >= SelezioneProcesso.IndiceMaxEnumTipoGas )
                  {
                     if(SelezioneProcesso.IndiceMaxEnumTipoGas)
                        Enc_R_OffsetTmp = SelezioneProcesso.IndiceMaxEnumTipoGas - 1;
                  }
                  SelezioneProcesso.IndiceTipoGas = Enc_R_OffsetTmp;

                  SelezioneProcesso.CurvaInfo.TipoGas = (viperdef_TipoGas_e) SelezioneProcesso.TabEnumTipoGas[SelezioneProcesso.IndiceTipoGas];
                  SelezioneProcesso.CurvaInfoDiPartenza.TipoGas = SelezioneProcesso.CurvaInfo.TipoGas;

                  viperui_RecalcBitMappedFiloDiametroGasMig(SelezioneProcesso.CurvaInfo.TipoMig,SelezioneProcesso.CurvaInfo.TipoFilo, SelezioneProcesso.CurvaInfo.DiametroFilo, VIPERDEF_TIPOGAS_MAX);
               }

               if(ActualSelection > 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS;
               else if(ActualSelection < 0 )
                  SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO;
            break;

            case VIPERUI_ATUALESELEZIONEPROCESSO_MAX:
            default:
            break;

         }

         if ( SelezioneProcesso.oldAttualeSelezione != SelezioneProcesso.AttualeSelezione )
         {
            SelezioneProcesso.oldAttualeSelezione = SelezioneProcesso.AttualeSelezione;
            ((ModelListener*) pMListner)->setMigWeldingProcessMenu(SelezioneProcesso.AttualeSelezione);
         }

         if ( SelezioneProcesso.IndiceTipoMig >= SelezioneProcesso.IndiceMaxEnumTipoMig )
            SelezioneProcesso.IndiceTipoMig = 0xff;
         if ( SelezioneProcesso.IndiceTipoMig == 0xff )
         {
            SelezioneProcesso.IndiceTipoMig = 0;
            for (uCA = 0; uCA < SelezioneProcesso.IndiceMaxEnumTipoMig; uCA++)
            {
               if ( (viperdef_TipoMig_e) SelezioneProcesso.TabEnumTipoMig[uCA] == SelezioneProcesso.CurvaInfoDiPartenza.TipoMig )
               {
                  SelezioneProcesso.IndiceTipoMig = uCA;
                  break;
               }
            }
         }

         if ( SelezioneProcesso.IndiceDiametroFilo >= SelezioneProcesso.IndiceMaxEnumDiametro )
            SelezioneProcesso.IndiceDiametroFilo = 0xff;
         if ( SelezioneProcesso.IndiceDiametroFilo == 0xff )
         {
            SelezioneProcesso.IndiceDiametroFilo = 0;
            for (uCA = 0; uCA < SelezioneProcesso.IndiceMaxEnumDiametro; uCA++)
            {
               if ( (viperdef_DiametroFilo_e) SelezioneProcesso.TabEnumDiametro[uCA] == SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo )
               {
                  SelezioneProcesso.IndiceDiametroFilo = uCA;
                  break;
               }
            }
         }

         if ( SelezioneProcesso.IndiceTipoGas >= SelezioneProcesso.IndiceMaxEnumTipoGas )
            SelezioneProcesso.IndiceTipoGas = 0xff;
         if ( SelezioneProcesso.IndiceTipoGas == 0xff )
         {
            SelezioneProcesso.IndiceTipoGas = 0;
            for (uCA = 0; uCA < SelezioneProcesso.IndiceMaxEnumTipoGas; uCA++)
            {
               if ( (viperdef_TipoGas_e) SelezioneProcesso.TabEnumTipoGas[uCA] == SelezioneProcesso.CurvaInfoDiPartenza.TipoGas )
               {
                  SelezioneProcesso.IndiceTipoGas = uCA;
                  break;
               }
            }
         }
         SelezioneProcesso.CurvaInfo.DiametroFilo = (viperdef_DiametroFilo_e) SelezioneProcesso.TabEnumDiametro[SelezioneProcesso.IndiceDiametroFilo];
         SelezioneProcesso.CurvaInfo.TipoGas = (viperdef_TipoGas_e) SelezioneProcesso.TabEnumTipoGas[SelezioneProcesso.IndiceTipoGas];
//         SelezioneProcesso.CurvaInfo.TipoMig = (viperdef_TipoMig_e) SelezioneProcesso.TabEnumTipoMig[SelezioneProcesso.IndiceTipoMig];
         SelezioneProcesso.CurvaInfo.TipoFilo = (viperdef_TipoFilo_e)SelezioneProcesso.TabEnumTipoMig[SelezioneProcesso.IndiceMaxEnumTipoFilo];

         if ( memcmp(&oldCurva, &SelezioneProcesso.CurvaInfo, sizeof(viperdef_MigCurvaInfo_t)) )
         {
            ((ModelListener*) pMListner)->setWireType(SelezioneProcesso.CurvaInfo.TipoFilo);
            ((ModelListener*) pMListner)->setGasType(SelezioneProcesso.CurvaInfo.TipoGas);
            ((ModelListener*) pMListner)->setMigType(SelezioneProcesso.CurvaInfo.TipoMig);
            ((ModelListener*) pMListner)->setWireDiameter(SelezioneProcesso.CurvaInfo.DiametroFilo);
            memcpy(&oldCurva, &SelezioneProcesso.CurvaInfo, sizeof(viperdef_MigCurvaInfo_t));
         }
      break;
      case SM_Stop:
      default:
      break;
   }
   return 1;
}


//void cViper_Info::viperui_ManagerTickEvent(U8 t, U8 p)
//{
//   switch (this->SelezioneProcesso.Processo)
//   {
//      case VIPERDEF_PROCESSO_MMA:
//
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//      break;
//      case VIPERDEF_PROCESSO_MIG:
//
//
//
//
//
//      break;
//      case VIPERDEF_PROCESSO_MAX:
//
//
//         break;
//   }
//   switch(this->SelezioneProcesso.AttualeSelezione)
//   {
//      case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
//         break;
//   }
//}






