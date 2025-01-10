#ifndef VIPERDEF_H_INCLUDED
#define VIPERDEF_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES

typedef enum
{
	VIPERDEF_TIPOFILO_MANUAL,
	VIPERDEF_TIPOFILO_FE,
	VIPERDEF_TIPOFILO_FEMETAL,
	VIPERDEF_TIPOFILO_FERUTIL,
	VIPERDEF_TIPOFILO_CRNI308,
	VIPERDEF_TIPOFILO_CRNI316,
	VIPERDEF_TIPOFILO_CRNI2209,
	VIPERDEF_TIPOFILO_ALSI4043,
	VIPERDEF_TIPOFILO_AL4047,
	VIPERDEF_TIPOFILO_AL4145,
	VIPERDEF_TIPOFILO_AL4557,
	VIPERDEF_TIPOFILO_ALMG5356,
	VIPERDEF_TIPOFILO_AL5554,
	VIPERDEF_TIPOFILO_CUSI3CRNI,
	VIPERDEF_TIPOFILO_CUSI3ZN,
	VIPERDEF_TIPOFILO_CUAL8CRNI,
	VIPERDEF_TIPOFILO_CUAL8ZN,
	VIPERDEF_TIPOFILO_ER80,
	VIPERDEF_TIPOFILO_ER120,
	VIPERDEF_TIPOFILO_NICR3,

	VIPERDEF_TIPOFILO_MAX
}viperdef_TipoFilo_e;

typedef enum
{
	VIPERDEF_TIPOGAS_NOGAS,
	VIPERDEF_TIPOGAS_I1_ARGON,
	VIPERDEF_TIPOGAS_M12_MIX9802,
	VIPERDEF_TIPOGAS_M20_MIX9208,
	VIPERDEF_TIPOGAS_M21_MIX8020,
	VIPERDEF_TIPOGAS_C1_CO2,
	VIPERDEF_TIPOGAS_C2_MIX9802,

	VIPERDEF_TIPOGAS_MAX
}viperdef_TipoGas_e;

typedef enum
{
	VIPERDEF_DIAMETROFILO_06,
	VIPERDEF_DIAMETROFILO_08,
	VIPERDEF_DIAMETROFILO_09,
	VIPERDEF_DIAMETROFILO_10,
	VIPERDEF_DIAMETROFILO_12,
	VIPERDEF_DIAMETROFILO_14,
	VIPERDEF_DIAMETROFILO_16,

	VIPERDEF_DIAMETROFILO_MAX
}viperdef_DiametroFilo_e;

typedef enum
{
	VIPERDEF_TIPOMIG_MAN,
	VIPERDEF_TIPOMIG_SYN,
	VIPERDEF_TIPOMIG_PULSE,

	VIPERDEF_TIPOMIG_MAX
}viperdef_TipoMig_e;

typedef enum
{
	VIPERDEF_STATOMACCHINA_WARMUP,
	VIPERDEF_STATOMACCHINA_OPERATIVA,
	VIPERDEF_STATOMACCHINA_ALLARME,
	VIPERDEF_STATOMACCHINA_POSTALLARME,   // I 10 secondi di visualizzazione allarme dopo la risoluzione del problema ma senza entrare in modalit� operativa
	VIPERDEF_STATOMACCHINA_TARATURA,
	VIPERDEF_STATOMACCHINA_CALIBRAZIONE,
	VIPERDEF_STATOMACCHINA_BASICSETUP,
	VIPERDEF_STATOMACCHINA_TEST,

	VIPERDEF_STATOMACCHINA_MAX
}viperdef_StatoMacchina_e;

typedef enum
{
	VIPERDEF_TEMPI_2T,
	VIPERDEF_TEMPI_4T,
	VIPERDEF_TEMPI_2TPLUS,
	VIPERDEF_TEMPI_4TPLUS,

	VIPERDEF_TEMPI_MAX
}viperdef_Tempi_e;

typedef enum
{
	VIPERDEF_LUNGHEZZAFASCIOCAVI_0MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_3MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_5MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_10MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_15MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_20MT,
	VIPERDEF_LUNGHEZZAFASCIOCAVI_30MT,

	VIPERDEF_LUNGHEZZAFASCIOCAVI_MAX
}viperdef_LunghezzaFascioCavi_e;

typedef enum
{
	VIPERDEF_MIGSYNSXVIS_CORRENTE,
	VIPERDEF_MIGSYNSXVIS_DVELFILO,
	VIPERDEF_MIGSYNSXVIS_DSPESSORE,

	VIPERDEF_MIGSYNSXVIS_MAX
}viperdef_MigSynSXVis_e;    // Cosa visualizzare sulla parte sinistra dello schermo in MIGSYN

typedef enum
{
	VIPERDEF_MIGSYNDXVIS_DTENSIONE,
	VIPERDEF_MIGSYNDXVIS_DBILANCIAMENTOTENSIONE,

	VIPERDEF_MIGSYNDXVIS_MAX
}viperdef_MigSynDXVis_e;    // Cosa visualizzare sulla parte destra dello schermo in MIGSYN

typedef enum
{   // Aggiungere in coda eventuali enum...
	VIPERDEF_RESET_PARAMETRI,
	VIPERDEF_RESET_FACTORY,
	VIPERDEF_RESET_RESTARTMCU
}viperdef_ResetMacchina_e;  // Tipo reset da effettuare

typedef enum
{
	VIPERDEF_PROCESSO_MMA,
	VIPERDEF_PROCESSO_TIG,
	VIPERDEF_PROCESSO_MIG,

	VIPERDEF_PROCESSO_MAX
}viperdef_Processo_e;       // Tipo processo operativo

typedef struct
{
	viperdef_TipoMig_e TipoMig;
	viperdef_TipoFilo_e TipoFilo;
	viperdef_TipoGas_e TipoGas;
	viperdef_DiametroFilo_e DiametroFilo;
}viperdef_MigCurvaInfo_t;


typedef enum
{
	VIPERDEF_WIRESTARTMODE_SOFT,  // Partenza in avvicinamento e poi in rampa alla rilevazione corrente
	VIPERDEF_WIRESTARTMODE_HARD,  // Partenza subito in rampa

	VIPERDEF_WIRESTARTMODE_MAX
}viperdef_WireStartMode_e;  // Tipo partenza vel filo

typedef enum
{	// NikNik da rivedere
	VIPERDEF_FUNEXECUTE_CARICAPTOOPERATORE,
	VIPERDEF_FUNEXECUTE_SALVAPTOOPERATORE,
	VIPERDEF_FUNEXECUTE_SETDEFAULTPARAMETRI,
	VIPERDEF_FUNEXECUTE_SALVAPARAMETRIINEEPROM,
	VIPERDEF_FUNEXECUTE_CAMBIAPROCESSO,
	VIPERDEF_FUNEXECUTE_RESETMACCHINA,

	VIPERDEF_FUNEXECUTE_STARTTARATURA,
	VIPERDEF_FUNEXECUTE_PUSHDATATARATURA,
	VIPERDEF_FUNEXECUTE_PUSHDATAAUXTARATURA,

	VIPERDEF_FUNEXECUTE_FORZASTATOMACCHINA,

	VIPERDEF_FUNEXECUTE_SALVATARATURA,
}viperdef_FunExecute_e;


typedef struct
{
	uint32_t CodiceEstesoBootLoaderCompatibility;	// Esempio 26060437 legato al pcb
	uint32_t CodiceEsteso;	// Esempio 22755016
	uint16_t Version;		// MSB Stage(A,B,V) LSB:0.0 ... 25.5
	uint32_t Crc32Firmware;
	uint32_t CodiceUnivocoScheda;
	uint32_t FirmwareData;
}viperdef_InfoFirmware_t;







#define VIPERDEF_CANID_HIGHPRIORITY								0x000
#define VIPERDEF_BASECANID_GENTX								0x100
#define VIPERDEF_BASECANID_GENRX								0x200
#define VIPERDEF_BASECANID_TRAINO								0x300 	//niknik da capire come gestire
//ZONE 0x500..0x550 dedidicato al busstack


//_________________________________________________Trasmissione ad alta priorità
#define VIPERDEF_CANID_TRAINORX_HP_CMDSET					VIPERDEF_CANID_HIGHPRIORITY+0x03 	// viperdef_Pack8TrainoRX_CmdSet_t
#define VIPERDEF_CANID_TRAINOTX_HP_INFOSTATUS				VIPERDEF_CANID_HIGHPRIORITY+0x20	// viperdef_Pack8TrainoTX_InfoStatus_t




//____________________________________________________________Zona generatore TX

#define VIPERDEF_CANID_GEN_INFOSTATUS						0x00 		// viperdef_Pack8Gen_InfoStatus_t
#define VIPERDEF_CANID_GEN_STRUMENTO						0x01   		// viperdef_Pack8Gen_Strumento_t

#define VIPERDEF_CANID_GEN_MIGMAN							0x10		// viperdef_Pack8Gen_MigMan_t
#define VIPERDEF_CANID_GEN_MIGMANHOTSTART					0x11   		// viperdef_Pack8Gen_MigManHotStart_t
#define VIPERDEF_CANID_GEN_MIGMANCRATERFILLER				0x12   		// viperdef_Pack8Gen_MigManCraterFiller_t
#define VIPERDEF_CANID_GEN_MIGMANLIMITI2					0x1e   		// viperdef_Pack8Gen_MigManLimiti2_t
#define VIPERDEF_CANID_GEN_MIGMANLIMITI1					0x1f   		// viperdef_Pack8Gen_MigManLimiti1_t

#define VIPERDEF_CANID_GEN_MIGSYN							0x20		// viperdef_Pack8Gen_MigSyn_t
#define VIPERDEF_CANID_GEN_MIGSYNHOTSTART	   				0x21   		// viperdef_Pack8Gen_MigSynHotStart_t
#define VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER				0x22   	   	// viperdef_Pack8Gen_MigSynCraterFiller_t
#define VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC				0x23   		// viperdef_Pack8Gen_MigSynActualSynCalc_t
#define VIPERDEF_CANID_GEN_MIGSYNLIMITI1					0x2f   		// viperdef_Pack8Gen_MigSynLimiti1_t

#define VIPERDEF_CANID_GEN_MIG								0x30		// viperdef_Pack8Gen_Mig_t
#define VIPERDEF_CANID_GEN_MIGLIMITICRATERFILLER			0x3d   		// viperdef_Pack8Gen_MigLimitiCraterFiller_t
#define VIPERDEF_CANID_GEN_MIGLIMITIHOTSTART	   			0x3e   		// viperdef_Pack8Gen_MigLimitiHotStart_t
#define VIPERDEF_CANID_GEN_MIGLIMITI1						0x3f  		// viperdef_Pack8Gen_MigLimiti1_t

#define VIPERDEF_CANID_GEN_MMA								0x40  		// viperdef_Pack8Gen_Mma_t
#define VIPERDEF_CANID_GEN_MMALIMITI1						0x4f		// viperdef_Pack8Gen_MmaLimiti1_t

#define VIPERDEF_CANID_GEN_TIG								0x50 		// viperdef_Pack8Gen_Tig_t
#define VIPERDEF_CANID_GEN_TIGLIMITI1						0x5f   		// viperdef_Pack8Gen_TigLimiti1_t

#define VIPERDEF_CANID_GEN_BASICSETUP						0x60 	   	// viperdef_Pack8Gen_BasicSetup_t
#define VIPERDEF_CANID_GEN_BASICSETUP2						0x61 	   	// viperdef_Pack8Gen_BasicSetup2_t

#define VIPERDEF_CANID_GEN_BASE								0x70		// viperdef_Pack8Gen_Base_t
#define VIPERDEF_CANID_GEN_FUNEXECUTE						0x71   	 	// viperdef_Pack8Gen_FunExecute_t
#define VIPERDEF_CANID_GEN_TARATGENTOSLAVE					0x72   		// viperdef_Pack8Gen_TaratGenToSlave_t
#define VIPERDEF_CANID_GEN_INFOFIRMWARE						0x73   		// viperdef_Pack8Gen_InfoFirmware_t

#define VIPERDEF_CANID_GEN_TARATURAESSEN					0x80		// viperdef_Pack8Gen_TaraturaEssen_t 	niknik da togliere
#define VIPERDEF_CANID_GEN_TARATURAESSEN1					0x81		// viperdef_Pack8Gen_TaraturaEssen1_t 	niknik da togliere
#define VIPERDEF_CANID_GEN_TARATURAESSEN2					0x82		// viperdef_Pack8Gen_TaraturaEssen2_t 	niknik da togliere
#define VIPERDEF_CANID_GEN_TARATURAESSEN3					0x83		// viperdef_Pack8Gen_TaraturaEssen3_t 	niknik da togliere



#define VIPERDEF_CANID_GEN_VARTESTSINT32					0xdb		// Usato per debug generici
#define VIPERDEF_CANID_GEN_VARTESTSINT16					0xdc		// Usato per debug generici
#define VIPERDEF_CANID_GEN_VARTESTSINT8						0xdd		// Usato per debug generici
#define VIPERDEF_CANID_GEN_VARTESTUINT32					0xde		// Usato per debug generici
#define VIPERDEF_CANID_GEN_VARTESTUINT16					0xdf		// Usato per debug generici
#define VIPERDEF_CANID_GEN_VARTESTUINT8						0xea		// Usato per debug generici
#define VIPERDEF_CANID_GEN_HWCHECKDAC						0xeb  		// Gestito a bit mask ottimizzato per analisi scheda DAC prime 4
#define VIPERDEF_CANID_GEN_HWCHECKADC1						0xec  		// Gestito a bit mask ottimizzato per analisi scheda ADC seconde 4
#define VIPERDEF_CANID_GEN_HWCHECKADC0						0xed  		// Gestito a bit mask ottimizzato per analisi scheda ADC prime 4
#define VIPERDEF_CANID_GEN_HWCHECKPORTIN3					0xee  		// Gestito a bit mask ottimizzato per analisi scheda IN quarti 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTIN2					0xef   		// Gestito a bit mask ottimizzato per analisi scheda IN terzi 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTIN1					0xfa  		// Gestito a bit mask ottimizzato per analisi scheda IN secondi 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTIN0					0xfb  		// Gestito a bit mask ottimizzato per analisi scheda IN primi 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTOUT3					0xfc   		// Gestito a bit mask ottimizzato per analisi scheda OUT quarti 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTOUT2					0xfd   		// Gestito a bit mask ottimizzato per analisi scheda OUT terzi 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTOUT1					0xfe   		// Gestito a bit mask ottimizzato per analisi scheda OUT secondi 64 out
#define VIPERDEF_CANID_GEN_HWCHECKPORTOUT0					0xff   		// Gestito a bit mask ottimizzato per analisi scheda OUT primi 64 out



//____________________________________________________________Zona generatore RX
// NOTA: Gli stessi id trasmessi dal generatore nela zona VIPERDEF_CANID_GENTX possono essere impostati
// da fuori. Ad ogni id impostato nella zona VIPERDEF_CANID_GENRX verrà ritrasmesso l'analogo Id della
// zona VIPERDEF_CANID_GENTX
				    										  
				    										  

//___________________________________________________________________Zona traino
// NOTA: Prevedere già lìesistenza di 4 traini con offeset per ogni singolo di H30
// H00..H2F	primo traino
// H30..H5F	secondo traino
// H60..H8F	terzo traino
// HA0..HCF	quarto traino
// HD0..HFF	comune ai quattro
#define VIPERDEF_CANID_TRAINOTX_INFOSTATUS				VIPERDEF_BASECANID_TRAINO+0x00	// viperdef_Pack8TrainoTX_InfoStatus_t	NOTA: Questo è l'analogo VIPERDEF_CANID_TRAINOTX_HP_INFOSTATUS ma in versione bassa priorità
#define VIPERDEF_CANID_TRAINOTX_HWSTATUS				VIPERDEF_BASECANID_TRAINO+0x01	// viperdef_Pack8TrainoTX_HWStatus_t
#define VIPERDEF_CANID_TRAINOTX_HWSTATUS2				VIPERDEF_BASECANID_TRAINO+0x02	// viperdef_Pack8TrainoTX_HWStatus2_t

#pragma pack(1)
typedef union
{
	struct
	{	// Max 8 bit
		unsigned int MassimaCorrente:1;		// Massima corrente motore rilevata
		unsigned int NonRilevato:1;
		unsigned int Sconnesso:1;;			// Se traino già rilevato ma ora disconnesso
		unsigned int EncoderFail:1;			// Ecnoder non rilevato

		unsigned int Dummy:4;
	}Bits;
	uint8_t TrainoAnomalie;
}viperdef_TrainoAnomalie_u;
typedef union
{
	struct
	{	// Max 8 bit
		unsigned int OverTemp1:1;			// OT1 rilevato
		unsigned int OverTemp2:1;			// OT2 rilevato
		unsigned int OverTemp3:1;			// OT3 rilevato
		unsigned int GruppoH2ONonRilevato:1;		// Ingresso H2O non rilevato

		unsigned int Dummy:4;
	}Bits;
	uint8_t GenAnomalieBoard;
}viperdef_GenAnomalieBoard_u;
typedef union
{
	struct
	{	// Max 8 bit
		unsigned int IoCorrenteOver:1;		// Corrente di allarme superata
		unsigned int TANonRilevato:1;		// TA non rilevato
		unsigned int IoOffsetEccessivo:1;	// Offset Io mediato all'accensione eccessivo
		unsigned int CortoInWarmUp:1;		// In solo warmup rilevato corto all'accensione
		unsigned int IntegrityError:1;		// Test integrit� da precarica/carica fallito
		unsigned int FreqPrimario:1;		// Anomalia frequenza primario

		unsigned int Dummy:2;
	}Bits;
	uint8_t GenAnomaliePower;
}viperdef_GenAnomaliePower_u;
typedef union
{
	struct
	{	// Max 16 bit
		unsigned int PotenzaAttivata:1;
		unsigned int FinestraBloccoPwmInCorso:1;
		unsigned int CorrenteDiStartRilevata:1;
		unsigned int PulsanteTorcia_A:1;
		unsigned int PulsanteTorcia_B:1;
		unsigned int TrainoInMovimento:1;
		unsigned int LimitazioneDiCorrenteInCorso:1;
		unsigned int CicloSaldaturaInCorso:1;
		unsigned int CmdAvanzaFilo:1;								// Comando di avanza filo premuto
		unsigned int CmdTestGas:1;									// Comando di test gas premuto
		unsigned int NoTarat:1;										// Taratura mancante
		unsigned int NoCalib:1;										// Calibratura mancante
		unsigned int PtoOperatoreNonSalvato:1;						// Punto operatore modificato e non salvato
		unsigned int CortoInFinestraBloccoPwmInCorso:1;				// Corto rilevato finchè siamo in finestra blocco pwm
		unsigned int AttesaConfermaAttivazionePotenza:1;			// In attesa di conferma per attivare potenza

		unsigned int Dummy:1;
	}Bits;
	uint16_t GenInfo;
}viperdef_GenInfo_u;


typedef struct
{   // Max 8 byte NikNik da rivedere
	viperdef_StatoMacchina_e StatoMacchina;
	viperdef_TrainoAnomalie_u TrainoAnomalie;		// 1 byte
	viperdef_GenAnomalieBoard_u GenAnomalieBoard;	// 1 byte
	viperdef_GenAnomaliePower_u GenAnomaliePower;	// 1 byte
	viperdef_GenInfo_u GenInfo;						// 2 byte
}viperdef_Pack8Gen_InfoStatus_t;   


// ********************************** MIGMAN ***********************************
// ********************************** MIGMAN ***********************************
// ********************************** MIGMAN ***********************************
typedef struct
{   // Max 8 byte
	uint16_t DTensione;     					// Tensione in decimi di volts
	uint16_t DVelFilo;      					// Velocità filo impostata in decimi di Mt/min  bbvbvbv
	uint8_t Induttanza;
}viperdef_Pack8Gen_MigMan_t;

typedef struct
{   // Max 8 byte
	uint16_t DVelFilo;      					// Vel filo assoluta in decimi di MT/Min
	uint16_t DTensione;     					// Tensione in decimi di volts
	uint8_t DSecDurata;     					// Durata dell'hot start
	uint8_t DSecSlope;      					// Rampa di ritorno alla normalit�
}viperdef_Pack8Gen_MigManHotStart_t;

typedef struct
{   // Max 8 byte
	uint16_t DVelFilo;      					// Vel filo assoluta in decimi di MT/Min
	uint16_t DTensione;     					// Tensione in decimi di volts
	uint8_t DSecSlope;      					// Rampa di passaggio al craterfill
	uint8_t DSecDurata;     					// Durata dell'hot start
}viperdef_Pack8Gen_MigManCraterFiller_t;

typedef struct
{   // Max 8 byte
	uint8_t Induttanza_Min;
	uint8_t Induttanza_Max;
}viperdef_Pack8Gen_MigManLimiti2_t;

typedef struct
{   // Max 8 byte
	uint16_t DTensione_Min;
	uint16_t DTensione_Max;
	uint16_t DVelFilo_Min;
	uint16_t DVelFilo_Max;
}viperdef_Pack8Gen_MigManLimiti1_t;



// ********************************** MIGSYN ***********************************
// ********************************** MIGSYN ***********************************
// ********************************** MIGSYN ***********************************
typedef struct
{   // Max 8 byte
	viperdef_MigSynSXVis_e UISXVis;             // Cosa visualizzare sulla parte sinistra del display
	viperdef_MigSynDXVis_e UIDXVis;             // Cosa visualizzare sulla parte destra del display
	uint16_t DVelFilo;                          // Velocità filo impostata in decimi di Mt/min
	int8_t DTensBilanciamento;                  // Bilanciamento tensione in decimi di volts
	int8_t InduttanzaBilanciamento;             // Bilanciamento induttanza
}viperdef_Pack8Gen_MigSyn_t;

typedef struct
{   // Max 8 byte
	int8_t PercSpeed;       					// Percentuale vel filo di partenza in decimi di Mt/min
	int8_t DBalanceV;       					// Bilanciamento tensione in decimi di volt
	uint8_t DSecDurata;     					// Durata dell'hot start
	uint8_t DSecSlope;      					// Rampa di ritorno alla normalit�
}viperdef_Pack8Gen_MigSynHotStart_t;

typedef struct
{   // Max 8 byte
	int8_t PercSpeed;       					// Percentuale vel filo decimi di Mt/min
	int8_t DBalanceV;       					// Bilanciamento tensione in decimi di volt
	uint8_t DSecSlope;      					// Rampa di passaggio al craterfill
	uint8_t DSecDurata;     					// Durata dell'hot start
}viperdef_Pack8Gen_MigSynCraterFiller_t;

typedef struct
{   // Max 8 byte
	uint16_t Corrente;
	uint8_t DSpessore;
	uint16_t DTensione;
}viperdef_Pack8Gen_MigSynActualSynCalc_t;

typedef struct
{
	char NomeCurva[31];
	uint8_t IndiceCurva;	// Da 0 (E' l'indice arrary mappato)
	uint8_t NumeroCurva;	// Numero ufficiale della curva es 10==MIGMAN, 14==Ferro
}viperdef_CurvaSyn_t;	// ATTENZIONE: NON un pacj8 e va gestito in modo diverso....

typedef struct
{   // Max 8 byte
	uint16_t DVelFilo_Min;
	uint16_t DVelFilo_Max;
	int8_t DTensBilanciamento_Min;
	int8_t DTensBilanciamento_Max;
	int8_t InduttanzaBilanciamento_Min;
	int8_t InduttanzaBilanciamento_Max;
}viperdef_Pack8Gen_MigSynLimiti1_t;

// ************************************ MIG ************************************
// ************************************ MIG ************************************
// ************************************ MIG ************************************
typedef struct
{   // Max 8 byte
	uint8_t CurvaAttuale;               		// 0==Manuale, 1....
	viperdef_Tempi_e Tempi;               		// 2T 4T
	uint8_t BBT;                        		// Burn Back Time espresso in decimi di millimetro
	uint8_t CSWireSlope;                		// Rampa di accelerazione filo in centesimi di Mt/min 0..2,55 sec
	uint8_t DSPostGas;                  		// Post gas espresso in decimi di sec
	uint8_t DSPreGas;                   		// Pre gas espresso in decimi di sec
	uint8_t DSSpot;                     		// DS Spot, 0==Disattivato
}viperdef_Pack8Gen_Mig_t;

typedef struct
{   // Max 8 byte
	int8_t PercSpeed_Min;
	int8_t PercSpeed_Max;
	int8_t BalanceV_Min;
	int8_t BalanceV_Max;
	uint8_t DSecSlope_Max;
	uint8_t DSecDurata_Max;
}viperdef_Pack8Gen_MigLimitiCraterFiller_t; 

typedef struct
{   // Max 8 byte
	int8_t PercSpeed_Min;
	int8_t PercSpeed_Max;
	int8_t BalanceV_Min;
	int8_t BalanceV_Max;
	uint8_t DSecDurata_Max;
	uint8_t DSecSlope_Max;
}viperdef_Pack8Gen_MigLimitiHotStart_t;

typedef struct
{   // Max 8 byte
	uint8_t CurvaAttuale_Max;
	uint8_t BBT_Min;
	uint8_t BBT_Max;
	uint8_t CSWireSlope_Max;
	uint8_t DSPostGas_Max;
	uint8_t MaxPtiOperatore;
	uint8_t DSPreGas_Max;
	uint8_t DSSpot_Max;
}viperdef_Pack8Gen_MigLimiti1_t;


// ************************************ MMA ************************************
// ************************************ MMA ************************************
// ************************************ MMA ************************************
typedef struct
{   // Max 8 byte
	uint16_t Corrente;
	uint8_t HotStart;
	uint8_t ArcForce;
}viperdef_Pack8Gen_Mma_t;

typedef struct
{   // Max 8 byte
	uint16_t Corrente_Min;
	uint16_t Corrente_Max;
	uint8_t HotStart_Max;
	uint8_t ArcForce_Max;
}viperdef_Pack8Gen_MmaLimiti1_t;

// ************************************ TIG ************************************
// ************************************ TIG ************************************
// ************************************ TIG ************************************
typedef struct
{   // Max 8 byte
	uint16_t Corrente;
	uint16_t DTensCutOff; 						// Tensione di CutOff Attenzione Min-1==AUTO Max+1==OFF
}viperdef_Pack8Gen_Tig_t;

typedef struct
{   // Max 8 byte
	uint16_t Corrente_Min;
	uint16_t Corrente_Max;
	uint16_t DTensCutOff_Min;
	uint16_t DTensCutOff_Max;
}viperdef_Pack8Gen_TigLimiti1_t;

// ************************************ BASE ***********************************
typedef struct
{   // Max 8 byte
	viperdef_Processo_e Processo;
	uint8_t PtoOperatore;
	viperdef_TipoMig_e TipoMig;
	viperdef_TipoFilo_e TipoFilo;
	viperdef_TipoGas_e TipoGas;
	viperdef_DiametroFilo_e DiametroFilo;
}viperdef_Pack8Gen_Base_t;

typedef struct
{   // Max 8 byte
	bool GruppoH2OEn;							// Attivazione gestione gruppo H2O
	uint8_t DsDigitalMeterShow;     			// Decimi di sec visualizzazione strumento
	bool FlgUnitMetric;            			 	// A true per udm metriche, imperiali altrimenti
	viperdef_WireStartMode_e WireStartMode;
	bool DropCutEn;
	bool PlusModeEn;
	viperdef_LunghezzaFascioCavi_e LunghezzaFascioCavi;
	bool SpotEn;
}viperdef_Pack8Gen_BasicSetup_t;   // 8byte max dim

typedef struct
{   // Max 8 byte
	bool VRDAttivo;
}viperdef_Pack8Gen_BasicSetup2_t;   // 8byte max dim



typedef struct
{   // Max 8 byte
	uint8_t Stato;
	uint8_t Fase;
	int16_t DatoOut;
	int16_t DatoOutAux;
}viperdef_Pack8Gen_TaratGenToSlave_t;   // 8byte max dim

typedef struct
{   // Max 8 byte
	uint16_t IoMedia;
	uint16_t VoDMedia;
}viperdef_Pack8Gen_Strumento_t;

typedef struct
{   // Max 8 byte
	uint8_t ByteAA;     // Deve essere 0xaa per eseguire la funzione
	uint8_t Byte55;     // Deve essere 0x55 per eseguire la funzione
	uint8_t Byte66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_FunExecute_e Function;   // Funzione da eseguiore

	uint32_t ValueFunction;
}viperdef_Pack8Gen_FunExecute_t;   // NikNik da rivedere


typedef struct
{
	uint32_t CodiceFirmwareEsteso;
	uint16_t Version;	// MSB:Stage(A,B,V)  LSB:0.0 ... 25.5
}viperdef_Pack8Gen_InfoFirmware_t;

// ********************************** TARATURA *********************************
typedef struct
{
	int32_t KIo;
	int32_t K0Io;
}viperdef_Pack8Gen_TaraturaEssen_t;

typedef struct
{
	int32_t KVo;
	int32_t K0Vo;
}viperdef_Pack8Gen_TaraturaEssen1_t;

typedef struct
{
	int32_t KISet;
	int32_t K0ISet;
}viperdef_Pack8Gen_TaraturaEssen2_t;

typedef struct
{
	uint32_t FascioCavi_MilliOhmResistenza;
	uint32_t FascioCavi_MilliHenryInduttanza;
}viperdef_Pack8Gen_TaraturaEssen3_t;


// *********************************** TRAINO **********************************
// *********************************** TRAINO **********************************
// *********************************** TRAINO **********************************

typedef struct
{	// Max 8 byte
	struct
	{
		unsigned int PulsanteTorcia1:1;		// 1==premuto
		unsigned int PulsanteTorcia2:1;		// 1==premuto
		unsigned int TrainoInMovimento:1;	// 1==in movimento
		unsigned int PulsanteAvanzaFilo:1;	// 1==premuto
		unsigned int PulsanteTestGas:1;		// 1==premuto
		unsigned int EvGasPilotata:1;		// 1==gas aperto
		unsigned int CadOn:1;
		unsigned int CadLeft:1;
		unsigned int CadRight:1;
		unsigned int CadUp:1;
		unsigned int CadDown:1;

		unsigned int Dummy:5;
	}Info;		// NOTA: tenere lunghezza 16 bit

	viperdef_TrainoAnomalie_u Anomalie;
}viperdef_Pack8TrainoTX_InfoStatus_t;	// Info trasmesse dal traino

typedef struct
{
	uint16_t DVBus;						// Tensione VBus in decimi di volts
	uint16_t DVEvGas;					// Tensione ElettroValvola in decimi di volts
	uint16_t MAEvGas;					// Carico EV in mA
	uint16_t MMinVelMot;					// Millesimi di metri min vel motore
}viperdef_Pack8TrainoTX_HWStatus_t;		// Status dell'hardware
typedef struct
{
	uint16_t DVMot;						// Tensione motore in decimi di volts
	uint16_t MAMot;						// Carico motore in mA
	uint16_t AdcCadPot1;					// Potenziometro 1 cad
	uint16_t AdcCadPot2;					// Potenziometro 2 cad
}viperdef_Pack8TrainoTX_HWStatus2_t;	// Status dell'hardware estensione 2


	typedef struct
	{
		unsigned int EvGas:1;				// 1==Apri gas
		unsigned int IlluminazioneVano:1;	// 1==Attiva illuminazione vano
		unsigned int ArcoOn:1;				// 1==Attiva segnale arco ON
		unsigned int Dummy:5;
	}viperdef_EvGas_CmdSetOptions_tbw;	// Dimensionato 8 bit max
typedef struct
{	// Max 8 byte
	//niknik da rimettereint32_t SetMmMinVel;       			// Velocità da settare, 0==Fermo ATTENZIONE: qui sono millesimi di m/min, se nagativo gira al contrario
	int32_t SetMmMinVel;       			// Velocità da settare, 0==Fermo ATTENZIONE: qui sono millesimi di m/min, se nagativo gira al contrario
	uint16_t MSWireSlope;   			// Rampa in millisecondi riferito al fondo scala di
	viperdef_EvGas_CmdSetOptions_tbw Options;
}viperdef_Pack8TrainoRX_CmdSet_t;		// Cmd di esecuzione per il traino

#pragma pack()


//============================================================= GLOBAL VARIABLES
//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_InfoStatus_t  				viperdef_Pack8GenTx_InfoStatus;

//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_MigMan_t                  	viperdef_Pack8GenRx_MigMan;
extern viperdef_Pack8Gen_MigManHotStart_t          	viperdef_Pack8GenRx_MigManHotStart;
extern viperdef_Pack8Gen_MigManCraterFiller_t      	viperdef_Pack8GenRx_MigManCraterFiller;
extern viperdef_Pack8Gen_MigManLimiti2_t           	viperdef_Pack8GenRx_MigManLimiti2;
extern viperdef_Pack8Gen_MigManLimiti1_t           	viperdef_Pack8GenRx_MigManLimiti1;

extern viperdef_Pack8Gen_MigMan_t                  	viperdef_Pack8GenTx_MigMan;
extern viperdef_Pack8Gen_MigManHotStart_t          	viperdef_Pack8GenTx_MigManHotStart;
extern viperdef_Pack8Gen_MigManCraterFiller_t      	viperdef_Pack8GenTx_MigManCraterFiller;
extern viperdef_Pack8Gen_MigManLimiti2_t           	viperdef_Pack8GenTx_MigManLimiti2;
extern viperdef_Pack8Gen_MigManLimiti1_t           	viperdef_Pack8GenTx_MigManLimiti1;


//------------------------------------------------------------------------------

extern viperdef_Pack8Gen_MigSyn_t                  	viperdef_Pack8GenRx_MigSyn;
extern viperdef_Pack8Gen_MigSynHotStart_t          	viperdef_Pack8GenRx_MigSynHotStart;
extern viperdef_Pack8Gen_MigSynCraterFiller_t      	viperdef_Pack8GenRx_MigSynCraterFiller;
extern viperdef_Pack8Gen_MigSynActualSynCalc_t     	viperdef_Pack8GenRx_MigSynActualSynCalc;
extern viperdef_CurvaSyn_t							viperdef_GenTx_CurvaSyn;

extern viperdef_Pack8Gen_MigSynLimiti1_t           	viperdef_Pack8GenRx_MigSynLimiti1;

extern viperdef_Pack8Gen_MigSyn_t                  	viperdef_Pack8GenTx_MigSyn;
extern viperdef_Pack8Gen_MigSynHotStart_t          	viperdef_Pack8GenTx_MigSynHotStart;
extern viperdef_Pack8Gen_MigSynCraterFiller_t      	viperdef_Pack8GenTx_MigSynCraterFiller;
extern viperdef_Pack8Gen_MigSynActualSynCalc_t     	viperdef_Pack8GenTx_MigSynActualSynCalc;
extern viperdef_CurvaSyn_t							viperdef_GenTx_CurvaSyn;
extern viperdef_Pack8Gen_MigSynLimiti1_t           	viperdef_Pack8GenTx_MigSynLimiti1;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_Mig_t                     	viperdef_Pack8GenRx_Mig;
extern viperdef_Pack8Gen_MigLimitiCraterFiller_t   	viperdef_Pack8GenRx_MigLimitiCraterFiller;
extern viperdef_Pack8Gen_MigLimitiHotStart_t       	viperdef_Pack8GenRx_MigLimitiHotStart;
extern viperdef_Pack8Gen_MigLimiti1_t              	viperdef_Pack8GenRx_MigLimiti1;

extern viperdef_Pack8Gen_Mig_t                     	viperdef_Pack8GenTx_Mig;
extern viperdef_Pack8Gen_MigLimitiCraterFiller_t   	viperdef_Pack8GenTx_MigLimitiCraterFiller;
extern viperdef_Pack8Gen_MigLimitiHotStart_t       	viperdef_Pack8GenTx_MigLimitiHotStart;
extern viperdef_Pack8Gen_MigLimiti1_t              	viperdef_Pack8GenTx_MigLimiti1;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_Mma_t                    	viperdef_Pack8GenRx_Mma;
extern viperdef_Pack8Gen_MmaLimiti1_t              	viperdef_Pack8GenRx_MmaLimiti1;

extern viperdef_Pack8Gen_Mma_t                     	viperdef_Pack8GenTx_Mma;
extern viperdef_Pack8Gen_MmaLimiti1_t              	viperdef_Pack8GenTx_MmaLimiti1;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_Tig_t                     	viperdef_Pack8GenRx_Tig;
extern viperdef_Pack8Gen_TigLimiti1_t              	viperdef_Pack8GenRx_TigLimiti1;

extern viperdef_Pack8Gen_Tig_t                     	viperdef_Pack8GenTx_Tig;
extern viperdef_Pack8Gen_TigLimiti1_t              	viperdef_Pack8GenTx_TigLimiti1;


//------------------------------------------------------------------------------
//extern viperdef_Pack8Gen_Base_t                    	viperdef_Pack8GenRx_Base;

extern viperdef_Pack8Gen_Base_t                    	viperdef_Pack8GenTx_Base;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_BasicSetup_t              	viperdef_Pack8GenRx_BasicSetup;

extern viperdef_Pack8Gen_BasicSetup_t              	viperdef_Pack8GenTx_BasicSetup;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_TaratGenToSlave_t         	viperdef_Pack8GenTx_TaratGenToSlave;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_Strumento_t               	viperdef_Pack8GenTx_Strumento;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_FunExecute_t				viperdef_Pack8GenRx_FunExecute;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_InfoFirmware_t				viperdef_Pack8GenTx_InfoFirmware;


//------------------------------------------------------------------------------
extern viperdef_Pack8Gen_TaraturaEssen_t			viperdef_Pack8GenRx_TaraturaEssen;
extern viperdef_Pack8Gen_TaraturaEssen1_t			viperdef_Pack8GenRx_TaraturaEssen1;
extern viperdef_Pack8Gen_TaraturaEssen2_t			viperdef_Pack8GenRx_TaraturaEssen2;
extern viperdef_Pack8Gen_TaraturaEssen3_t			viperdef_Pack8GenRx_TaraturaEssen3;

extern viperdef_Pack8Gen_TaraturaEssen_t			viperdef_Pack8GenTx_TaraturaEssen;
extern viperdef_Pack8Gen_TaraturaEssen1_t			viperdef_Pack8GenTx_TaraturaEssen1;
extern viperdef_Pack8Gen_TaraturaEssen2_t			viperdef_Pack8GenTx_TaraturaEssen2;
extern viperdef_Pack8Gen_TaraturaEssen3_t			viperdef_Pack8GenTx_TaraturaEssen3;

//------------------------------------------------------------------------------
extern viperdef_Pack8TrainoRX_CmdSet_t				viperdef_Pack8TrainoRX_CmdSet;
extern viperdef_Pack8TrainoTX_InfoStatus_t			viperdef_Pack8TrainoTX_InfoStatus;
extern viperdef_Pack8TrainoTX_HWStatus_t			viperdef_Pack8TrainoTX_HWStatus;
extern viperdef_Pack8TrainoTX_HWStatus2_t			viperdef_Pack8TrainoTX_HWStatus2;



//------------------------------------------------------------------------------
extern int32_t viperdef_GetRx_VarTestSInt32[2];
extern int16_t viperdef_GetRx_VarTestSInt16[4];
extern int8_t viperdef_GetRx_VarTestSInt8[8];
extern uint32_t viperdef_GetRx_VarTestUInt32[2];
extern uint16_t viperdef_GetRx_VarTestUInt16[4];
extern uint8_t viperdef_GetRx_VarTestUInt8[8];

extern int32_t viperdef_GetTx_VarTestSInt32[2];
extern int16_t viperdef_GetTx_VarTestSInt16[4];
extern int8_t viperdef_GetTx_VarTestSInt8[8];
extern uint32_t viperdef_GetTx_VarTestUInt32[2];
extern uint16_t viperdef_GetTx_VarTestUInt16[4];
extern uint8_t viperdef_GetTx_VarTestUInt8[8];
extern uint8_t viperdef_GetTx_HwCheckDac[8];
extern uint8_t viperdef_GetTx_HwCheckAdc1[8];
extern uint8_t viperdef_GetTx_HwCheckAdc0[8];
extern uint8_t viperdef_GetTx_HwCheckPortIn3[8];
extern uint8_t viperdef_GetTx_HwCheckPortIn2[8];
extern uint8_t viperdef_GetTx_HwCheckPortIn1[8];
extern uint8_t viperdef_GetTx_HwCheckPortIn0[8];
extern uint8_t viperdef_GetTx_HwCheckPortOut3[8];
extern uint8_t viperdef_GetTx_HwCheckPortOut2[8];
extern uint8_t viperdef_GetTx_HwCheckPortOut1[8];
extern uint8_t viperdef_GetTx_HwCheckPortOut0[8];


//============================================================ GLOBAL PROTOTYPES

#endif /* VIPERDEF_H_INCLUDED */

