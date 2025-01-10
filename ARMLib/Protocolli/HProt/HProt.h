#ifndef HPROT_H_INCLUDED
#define HPROT_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

#include "ARMLib/Common/SwTimer.h"

//====================================================================== DEFINES
#define HPROT_MSATTESARISPOSTA 1000

typedef enum	// Questi sono valori a bit assegnati all id standard HPROT_IDSTD_STATUSBOARD
{
	HPROT_STATUSBOARD_POWEREDON			= _B0_,	// Accesa elettricamente
	HPROT_STATUSBOARD_REBOOTED			= _B1_,	// Riavviata (dopo un reset o reboot)
	HPROT_STATUSBOARD_FAIL				= _B2_,	// Problema grave HW o SW tale da inibire funzionamente della scheda
	HPROT_STATUSBOARD_BOOTLOADERMODE	= _B3_,	// Modalità boot loader operativa
}hprot_StatusBoard_e;

// Serie di identificativi standard
#define HPROT_IDSTD_RW_WRFLASH_COREDESTROY							0xfff2		// Messo a 0xcacca666 distrugge l'anima della scheda (il firmware)
#define HPROT_IDSTD_R_CODICESCHEDAESTESOBOOTLOADERCOMPATIBILITY		0xfff3		// Codice scheda esteso boot loader compatibility (uint32_t)
#define HPROT_IDSTD_R_VIDEOMEMORY 									0xfff4		// Video Memory
#define HPROT_IDSTD_RW_WRFLASH_NBYTETOWRITE							0xfff5		// Numero di byte da scrivere in flash dal Buffer4K
#define HPROT_IDSTD_RW_WRFLASH_ADDRESS								0xfff6		// Address di scrittura in flashdel Buffer di appoggio
#define HPROT_IDSTD_RW_WRFLASH_BUFFER4K								0xfff7 		// Buffer di appoggio 4K
#define HPROT_IDSTD_RW_WRFLASH_MODPERMISSION						0xfff8		// Messo a 0xaa551001 concede la scrittura in flash per aggiornamento, a 0xaa559009 per distruzione anima
#define HPROT_IDSTD_RW_EXECUTEREBOOT								0xfff9		// Esegue un reboot se vale 0xaa556666 o in bootloader se vale 0x55aa9999
#define HPROT_IDSTD_R_FIRMWAREDATA									0xfffa		// Data compilazione firmware AAAMMGG (uint32_t)
#define HPROT_IDSTD_R_CODICEUNIVOCOSCHEDA							0xfffb		// Codice univoco scheda (uint32_t)
#define HPROT_IDSTD_R_FIRMWARECRC32									0xfffc		// Crc32 del firmware (uint32_t)
#define HPROT_IDSTD_R_FIRMWAREVER									0xfffd		// Versione firmware (uint16_t) MSB:Stage(!,b,V) LSB:ver
#define HPROT_IDSTD_R_CODICESCHEDAESTESO							0xfffe		// Codice scheda esteso (uint32_t)
#define HPROT_IDSTD_R_STATUSBOARD 									0xffff		// Status scheda (uint8_t)

typedef enum
{
	HPROT_STATUS_VOID,			// Stato void
	HPROT_STATUS_PCKINARRIVO,	// Pacchetto in arrivo
	HPROT_STATUS_PCKERRATO,		// Formattazione del pacchetto errata
	HPROT_STATUS_PCKCONFERMATO,	// ACK di risposta arrivata
	HPROT_STATUS_PCKRICEVUTO,	// Pacchetto ricevuto e corretttamente formattato
}hprot_Status_e;

typedef enum
{
	HPROT_SLOBJPV_VOID,				// In attesa di qualcosa da fare

	HPROT_SLOBJPV_WAITPOSTSTX,		// Analisi carattere post STX

	HPROT_SLOBJPV_WAITLENDATA1,		// Attesa primo byte LenData
	HPROT_SLOBJPV_WAITLENDATA2,		// Attesa secondo byte LenData
	HPROT_SLOBJPV_WAITLENDATA3,		// Attesa terzo byte LenData

	HPROT_SLOBJPV_WAITID1,			// Attesa primo byte id
	HPROT_SLOBJPV_WAITID2,			// Attesa secondo byte id

	HPROT_SLOBJPV_WAITDATA,

	HPROT_SLOBJPV_WAITCR,

	HPROT_SLOBJPV_WAITETX,
}hprot_SLObjPV_e;	// Status loop dell'oggetto Prima Valutazione del pck

typedef enum
{
	HPROT_TYPEFUNEXECUTE_PREGET,	// Prima di ritornare la lettura di id
	HPROT_TYPEFUNEXECUTE_POSTGET,	// Dopo il ritorno di una lettura di un id
	HPROT_TYPEFUNEXECUTE_PRESET,	// Prima di impostare un id, se la PtrFunExecute ritorna _STATUS_OK_ leffettivo SET può essere eseguito altrimenti no
	HPROT_TYPEFUNEXECUTE_POSTSET,	// Dopo il ritorno di una lettura di un id

}hprot_TypeFunExecute_e;

typedef enum
{
	HPROT_PERMISSION_NOWR = _B0_,	// Id NON scrivibile da fuori, solo lettura
}hprot_Permission_e;

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}hprot_Init_t;

typedef struct
{
	uint16_t Id;				// Id del record
	void *PtrData;				// Ptr al dato
	_TypeData_e TypeData;		// Tipo dato
	uint32_t MaxLen;			// Dimensione massima (nel caso di stringhe includere anche lo zero di fine stringa)
	int32_t (*PtrFunExecute)(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute);
	uint8_t Permission;			// Gestito a bit enum del tipo HPROT_PERMISSION_xxx
}hprot_StructData_t;

typedef enum
{
	HPROT_TYPELENDATA_GET,
	HPROT_TYPELENDATA_DIMFIX1,
	HPROT_TYPELENDATA_DIMFIX2,
	HPROT_TYPELENDATA_DIMFIX4,
	HPROT_TYPELENDATA_DIMFIX8,
	HPROT_TYPELENDATA_DIMVAR,

	HPROT_TYPELENDATA_MAX
}hprot_TypeLenData_e;

typedef enum
{
	HPROT_STATUSPCKMASTER_VOID,
	HPROT_STATUSPCKMASTER_PCKINCOSTRUZIONE,
	HPROT_STATUSPCKMASTER_INATTESARISPOSTA,
	HPROT_STATUSPCKMASTER_RISPOSTAINCORSO,
	HPROT_STATUSPCKMASTER_RISPOSTANONAVVENUTA,
	HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA,
}hprot_StatusPckMaster_e;

typedef struct
{
	hprot_StructData_t *PtrStructData;
	int32_t (*PtrFunGetChar)(void);											// Funzione di lettura byte ricevuti
	int32_t (*PtrFunSendData)(uint8_t *ptrData,uint32_t lenData);			// Funzione di trasmissione dati
	void (*PtrFunFlushRX)(void);											// Funzione di flush buffer di ricezione
	bool (*PtrFunIsEmptyTX)(void);											// Ritorna true sse buffer di trasmissione vuoto (trasmesso tutto il trasmissibile)
	void (*PtrFunSetAddressDest)(uint8_t address);							// setta l'address di destinazione dei dati a venire
	uint8_t (*PtrFunGetAddressOrg)(void);									// Ritorna l'address di origine pacchetto

	hprot_SLObjPV_e SLObjPV;

	// Il buffer appoggio serve per i client memorizzare il protocollo in arrivo per poi analizzarlo solo alla fine quando
	// si è verificata la validità della formattazione. In fase di trasmissione il protocollo esce dalla periferica mano a mano che viene composto
	uint8_t *PtrBufferAppoggio;
	uint32_t IndiceWrBufferAppoggio;
	uint32_t SizeBufferAppoggio;

	bool MasterMode;	// A true indica che l'oggetto è di tipo master
	struct
	{
		hprot_StatusPckMaster_e StatusPckMaster;
		swtimer_TimerHandler_t HTimerAttesaRisposta;
		uint32_t MsAttesaRisposta;	// msec di wait state per la risposta
	}MasterModeInfo;

	struct
	{
		uint8_t Address;
		uint8_t TypeRecord;
		uint32_t LenData;
		uint16_t Id;

		uint32_t NumRecordSETRilevati;
		uint32_t NumRecordGETRilevati;

		uint8_t CheckSumCalcolataTX;
		uint8_t CheckSumCalcolataRX;
	}InfoPck;	// Info gestione pacchetto
}hprot_Obj_t;	// Oggetto HProt


//============================================================= GLOBAL VARIABLES
extern hprot_Init_t hprot_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t hprot_InitModule(void);
hprot_Obj_t* hprot_CreateObj(hprot_StructData_t *ptrStructData,bool masterMode,uint32_t dimBufferAppoggio,
							int32_t (*ptrFunGetChar)(void),
							int32_t (*ptrFunSendData)(uint8_t *ptrData,uint32_t lenData),
							void (*ptrFunFlushRX)(void),
							bool (*ptrFunIsEmptyTX)(void),
							void (*ptrFunSetAddressDest)(uint8_t address),
							uint8_t (*PtrFunGetAddressOrg)(void));
hprot_Status_e hprot_ManagerObj(hprot_Obj_t *ptrObj);

hprot_StructData_t* hprot_GetStructDataFromId(hprot_Obj_t *ptrObj,uint16_t id);
hprot_StructData_t* hprot_GetStructDataFromPtrVar(hprot_Obj_t *ptrObj,void *ptrVar);

bool hprot_Master_PckIsPossibleToStart(hprot_Obj_t *ptrObj);
int32_t hprot_Master_PckStart(hprot_Obj_t *ptrObj);
void hprot_Master_PckAddIdSet(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData);
void hprot_Master_PckAddIdGet(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData);
void hprot_Master_PckEnd(hprot_Obj_t *ptrObj);
void hprot_Master_PckAbort(hprot_Obj_t *ptrObj);


#endif /* HPROT_H_INCLUDED */

