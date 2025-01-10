#ifndef VIPERUI_H_INCLUDED
#define VIPERUI_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Common/Data8Pack.h"
#include "ARMLib/Common/Data.h"
#include "CORELib/Common/ViperDef.h"

//====================================================================== DEFINES
#define VIPERUI_NMAXCURVEINFODISPONIBILI 300

typedef enum
{
	VIPERUI_STDCOLOR_AZIENDALE,
	VIPERUI_STDCOLOR_NERO,
	VIPERUI_STDCOLOR_GRIGIOSCURO,
	VIPERUI_STDCOLOR_GRIGIOMEDIO,
	VIPERUI_STDCOLOR_GRIGIOCHIARO,
	VIPERUI_STDCOLOR_BIANCO,

	VIPERUI_STDCOLOR_MAX
}viperui_ObjColor_e;

typedef enum
{	// Tenere allineati gli enum con la struttura viperui_StrutturaDati
	VIPERUI_DATA_MIGMAN_DTENSIONE,
	VIPERUI_DATA_MIGMAN_DVELFILO,
	VIPERUI_DATA_MIGMAN_INDUTTANZA,

	VIPERUI_DATA_MIGMANHOTSTART_DVELFILO,
	VIPERUI_DATA_MIGMANHOTSTART_DTENSIONE,
	VIPERUI_DATA_MIGMANHOTSTART_DSECDURATA,
	VIPERUI_DATA_MIGMANHOTSTART_DSECSLOPE,
	VIPERUI_DATA_MIGMANCRATERFILLER_DVELFILO,
	VIPERUI_DATA_MIGMANCRATERFILLER_DTENSIONE,
	VIPERUI_DATA_MIGMANCRATERFILLER_DSECSLOPE,
	VIPERUI_DATA_MIGMANCRATERFILLER_DSECDURATA,

	VIPERUI_DATA_MIGSYN_UISXVIS,
	VIPERUI_DATA_MIGSYN_UIDXVIS,
	VIPERUI_DATA_MIGSYN_DVELFILO,
	VIPERUI_DATA_MIGSYN_DTENSBILANCIAMENTO,
	VIPERUI_DATA_MIGSYN_INDUTTANZABILANCIAMENTO,

	VIPERUI_DATA_MIGSYNHOTSTART_PERCSPEED,
	VIPERUI_DATA_MIGSYNHOTSTART_DBALANCEV,
	VIPERUI_DATA_MIGSYNHOTSTART_DSECDURATA,
	VIPERUI_DATA_MIGSYNHOTSTART_DSECSLOPE,
	VIPERUI_DATA_MIGSYNCRATERFILLER_PERCSPEED,
	VIPERUI_DATA_MIGSYNCRATERFILLER_DBALANCEV,
	VIPERUI_DATA_MIGSYNCRATERFILLER_DSECSLOPE,
	VIPERUI_DATA_MIGSYNCRATERFILLER_DSECDURATA,

	VIPERUI_DATA_MIGSYNACTUALSYNCALC_CORRENTE,
	VIPERUI_DATA_MIGSYNACTUALSYNCALC_DSPESSORE,
	VIPERUI_DATA_MIGSYNACTUALSYNCALC_DTENSIONE,

	VIPERUI_DATA_MIG_CURVAATTUALE,
	VIPERUI_DATA_MIG_TEMPI,
	VIPERUI_DATA_MIG_BBT,
	VIPERUI_DATA_MIG_CSWIRESLOPE,
	VIPERUI_DATA_MIG_DSPOSTGAS,
	VIPERUI_DATA_MIG_DSPREGAS,
	VIPERUI_DATA_MIG_DSSPOT,

	VIPERUI_DATA_MMA_CORRENTE,
	VIPERUI_DATA_MMA_HOTSTART,
	VIPERUI_DATA_MMA_ARCFORCE,

	VIPERUI_DATA_TIG_CORRENTE,
	VIPERUI_DATA_TIG_DTENSCUTOFF,

	VIPERUI_DATA_BASE_PROCESSO,
	VIPERUI_DATA_BASE_PTOOPERATORE,
	VIPERUI_DATA_BASE_TIPOMIG,
	VIPERUI_DATA_BASE_TIPOFILO,
	VIPERUI_DATA_BASE_TIPOGAS,
	VIPERUI_DATA_BASE_DIAMETROFILO,

	VIPERUI_DATA_BASICSETUP_GRUPPOH2OEN,

	VIPERUI_DATA_TARATURAESSEN_KIO,
	VIPERUI_DATA_TARATURAESSEN_K0IO,
	VIPERUI_DATA_TARATURAESSEN_KVO,
	VIPERUI_DATA_TARATURAESSEN_K0VO,
	VIPERUI_DATA_TARATURAESSEN_KISET,
	VIPERUI_DATA_TARATURAESSEN_K0ISET,
	VIPERUI_DATA_TARATURAESSEN_FASCIOCAVIRESISTENZA,
	VIPERUI_DATA_TARATURAESSEN_FASCIOCAVIINDUTTANZA,

	VIPERUI_DATA_MIGSYN_MAX
}viperui_Data_e;

typedef enum
{
	VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO,

	VIPERUI_ATUALESELEZIONEPROCESSO_MMA,

	VIPERUI_ATUALESELEZIONEPROCESSO_TIG,

   VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG,
	VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOFILO,
	VIPERUI_ATUALESELEZIONEPROCESSO_MIG_DIAMETROFILO,
	VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOGAS,

	VIPERUI_ATUALESELEZIONEPROCESSO_MAX
}viperui_AttualeSelezioneProcesso_e;

typedef struct
{
	viperdef_InfoFirmware_t InfoFirmware;

	bool InitOk;		// a true sse modulo correttamente inizializzato
}viperui_Init_t;

//typedef struct
//{
//	struct
//	{
//		viperui_AttualeSelezioneProcesso_e AttualeSelezione;
//		viperdef_Processo_e Processo;
//
//		viperdef_MigCurvaInfo_t CurvaInfo;		// Curva attualmente in selezione
//		viperdef_MigCurvaInfo_t CurvaInfoDiPartenza;	// Curva di partenza all'entrata del quadro
//
//		uint32_t BitMappedDiametroFilo;	// Diametri filo disponibili gestito a bit enum del tipo viperdef_DiametroFilo_e
//		uint32_t BitMappedTipoGas;		// Tipi gas disponibili gestito a bit enum del tipo viperdef_TipoGas_e
//		uint32_t BitMappedTipoMig;		// Tipi Mig disponibili gestito a bit enum del tipo viperdef_TipoMig_e
//
//		uint8_t IndiceDiametroFilo;
//		uint8_t IndiceTipoGas;
//		uint8_t IndiceTipoMig;
//
//		uint8_t TabEnumDiametro[32];
//		uint8_t TabEnumTipoGas[32];
//		uint8_t TabEnumTipoMig[32];
//		uint8_t IndiceMaxEnumDiametro;
//		uint8_t IndiceMaxEnumTipoGas;
//		uint8_t IndiceMaxEnumTipoMig;
//	}SelezioneProcesso;	// Usato per selezionare processo
//
//	viperdef_MigCurvaInfo_t ListaCurveInfoDisponibili[VIPERUI_NMAXCURVEINFODISPONIBILI];
//
//	bool SuperVisorMode;
//}viperui_Info_t;

//============================================================= GLOBAL VARIABLES
extern viperui_Init_t viperui_Init;
//extern viperui_Info_t viperui_Info;
extern const data8pack_Data_t viperui_StructData8SET[];
extern uint32_t viperui_ListStdColor[];
extern const data_Data_t viperui_StrutturaDati[];
extern const data8pack_Data_t viperui_StructData8TX[];
extern const data8pack_Data_t viperui_StructData8RX[];



//============================================================ GLOBAL PROTOTYPES
int32_t viperui_InitModule(void);
uint32_t viperui_OpacizzaColore(uint32_t colore, float opacita);
//void viperui_Manager1MS(void);
//void viperui_Generatore_CambiaProcesso(viperdef_Processo_e processo,viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_TipoGas_e tipoGas,viperdef_DiametroFilo_e diametroFilo);
//void viperui_Generatore_SalvaTaratura(void);
//void viperui_Generatore_SalvaParametriInEEProm(void);
//void viperui_RecalcBitMappedFiloDiametroGasMig(viperdef_TipoFilo_e tipoFilo,viperdef_DiametroFilo_e diametroFilo,viperdef_TipoGas_e tipoGas);

#endif /* VIPERUI_H_INCLUDED */

