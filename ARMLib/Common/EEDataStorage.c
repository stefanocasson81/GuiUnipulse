/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Gestore salvataggio strutture dati in eeprom.
	Ogni struttura dati DEVE avere in testa la var uint16_t Crc16 ed in coda il
	uint32_t TagApp (EETag dell'init).
	Il modulo parte dalla versione scritta per STM8 ed espansa per architetture
	32but.
	
	Esempio:
					
		#pragma pack(1)
		typedef struct
		{
			uint16_t Crc16;
		
			traino_PtoAdjDVelMot_t PtiAdjDVelMot[TRAINO_NPTIADJDVELMOT];
			
			uint8_t WhatCalib;	// Gestito a bit indica cosa è stato calibrato. B0==Vel filo, B1==VisIO, B2==VisVO
			
			int8_t OffsetVisIO_Min;	// Offset di visualizzazione alla TARATURA_CORRENTEMIN
			int8_t OffsetVisIO_Max;	// Offset di visualizzazione alla TARATURA_CORRENTEMAX
			
			int8_t OffsetVisVO_Min;	// Offset di visualizzazione alla TARATURA_DTENSTARATMIN
			int8_t OffsetVisVO_Max;	// Offset di visualizzazione alla TARATURA_DTENSTARATMAX
				
			uint8_t DummyAlign60[41];
			uint32_t TagApp;  			// DEVE essere uguale a APPLICAZIONE_EETAG per considerare valida la struttura
		}applicazione_EECalibrazione_t;	// Dimensione fissa a 64 byte
		#pragma pack()
		 
		typedef enum
		{
			APPLICAZIONE_TIPOEEDATA_TARATURA		,
			APPLICAZIONE_TIPOEEDATA_CALIBRAZIONE	,
			APPLICAZIONE_TIPOEEDATA_MENUBASIC		,
			APPLICAZIONE_TIPOEEDATA_MENUMIGTIGMMA	,
			APPLICAZIONE_TIPOEEDATA_USER			,
			APPLICAZIONE_TIPOEEDATA_PTOOPERATORE	,
			
			APPLICAZIONE_TIPOEEDATA_MAX
		}applicazione_TipoEEData_e;


		static const eedatastorage_MappaturaEEProm_t MappaturaEEProm[APPLICAZIONE_TIPOEEDATA_MAX] =
		{
			{	APPLICAZIONE_TIPOEEDATA_TARATURA,		&applicazione_EETaratura,		0, 		sizeof(applicazione_EETaratura_t),									true, 	true},
			{	APPLICAZIONE_TIPOEEDATA_CALIBRAZIONE,	&applicazione_EECalibrazione,	64, 	sizeof(applicazione_EECalibrazione_t),								true, 	true},
			{	APPLICAZIONE_TIPOEEDATA_MENUBASIC,		&applicazione_EEMenuBasic,		128, 	sizeof(applicazione_EEMenuBasic_t),									true,	true},
			{	APPLICAZIONE_TIPOEEDATA_MENUMIGTIGMMA,	&applicazione_EEMenuMigTigMma,	192, 	sizeof(applicazione_EEMenuMigTigMma_t),								true, 	true},
			{	APPLICAZIONE_TIPOEEDATA_USER,			&applicazione_EEUser,			256, 	sizeof(applicazione_EEUser_t),										false,	true},
			{	APPLICAZIONE_TIPOEEDATA_PTOOPERATORE,	0,                 				320, 	APPLICAZIONE_NUMPTIOPERATORE * APPLICAZIONE_DIMPTIOPERATORE,		false,	false},
		};


	
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/EEDataStorage.h"
#include "ARMLib/Common/Crc.h"


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
eedatastorage_Init_t eedatastorage_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t eedatastorage_InitModule(void)
{
	uint8_t numMappatura;

	if( (eedatastorage_Init.PtrArrayMappaturaEEProm) && (eedatastorage_Init.PtrFunCheckValidataParametri) &&
		(eedatastorage_Init.PtrFunSetDefaultParametri) && (eedatastorage_Init.PtrFunReadData) &&
		(eedatastorage_Init.PtrFunWriteData) )
	{
		// Leggo tutti i dati da eeprom e li storo in RAM sulle opportune strutture
		for(numMappatura = 0; numMappatura < eedatastorage_Init.NumMappature; numMappatura++)
		{
			if(eedatastorage_Init.PtrArrayMappaturaEEProm[numMappatura].FlgAutoLoadInit)
				eedatastorage_Read(&eedatastorage_Init.PtrArrayMappaturaEEProm[numMappatura]);
		}

		eedatastorage_Init.InitOk = true;
		return _STATUS_OK_;
	}
	else
	{
		eedatastorage_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
}


//______________________________________________________________________________
// Desc:  Scrive in EEProm la struttura MappaturaEEProm passata
// Arg: - ptrMappaturaEEProm: Per alla eedatastorage_MappaturaEEProm_t da salvare
// Ret: - true se qualcosa di diverso trovato e quindi effettivamente salvato
//______________________________________________________________________________
bool eedatastorage_Write(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm)
{
	uint8_t *ptrUCa;
	uint16_t uSA;
	bool valRet = false;

	if(ptrMappaturaEEProm->FlgCrc16ETagCheck)
	{
		// Setto i 4 byte finali corrispondente al tag della struttura
		ptrUCa = ptrMappaturaEEProm->PtrData;
		ptrUCa += ptrMappaturaEEProm->SizeData;
		ptrUCa -= 4;
		memcpy(ptrUCa, &eedatastorage_Init.EETag, 4);
		
		// Ricalcolo Crc16
		uSA = crc_CalcCrc16(((uint8_t *)ptrMappaturaEEProm->PtrData) + 2, ptrMappaturaEEProm->SizeData - 2, 0);
		memcpy(ptrMappaturaEEProm->PtrData, &uSA, 2);	// Potrebbe essere address dispari
	}
	
	eedatastorage_Init.PtrFunWriteData((void *)ptrMappaturaEEProm->OffsetData,ptrMappaturaEEProm->PtrData,  ptrMappaturaEEProm->SizeData);

	return valRet;
}

//______________________________________________________________________________
// Desc:  Legge da EEProm la struttura MappaturaEEProm passata. Se la lettura
// 		  non è conforme o crc errato viene cmq ripristinato al valore di default
// 		  incluso il salvataggio effettivo.
// Arg: - ptrMappaturaEEProm: Per alla eedatastorage_MappaturaEEProm_t da salvare
// Ret: - _STATUS_OK_ se lettura eseguit con successo
//______________________________________________________________________________
int32_t eedatastorage_Read(const eedatastorage_MappaturaEEProm_t *ptrMappaturaEEProm)
{
	int32_t valRet = _STATUS_OK_;
	bool reset = false;
	uint16_t crc16;
	uint8_t *ptrUCa;
	uint32_t tag;

	eedatastorage_Init.PtrFunReadData(ptrMappaturaEEProm->PtrData, (void *)ptrMappaturaEEProm->OffsetData, ptrMappaturaEEProm->SizeData);

	if(ptrMappaturaEEProm->FlgCrc16ETagCheck)
	{
		// Ottengo Crc16 e Tag della struttura
		ptrUCa = (uint8_t*)ptrMappaturaEEProm->PtrData;
		memcpy(&crc16,ptrUCa, 2);	// Ottengo il crc16
		ptrUCa += ptrMappaturaEEProm->SizeData;
		ptrUCa -= 4;
		memcpy(&tag,ptrUCa, 4); 	// Ottengo il tag

		if(tag == eedatastorage_Init.EETag)
		{
			if(crc16 != crc_CalcCrc16(((uint8_t *)ptrMappaturaEEProm->PtrData) + 2, ptrMappaturaEEProm->SizeData - 2, 0))
				reset = true;
		}
		else
			reset = true;
	}

	if(!eedatastorage_Init.PtrFunCheckValidataParametri(ptrMappaturaEEProm))
		reset = true;

	if(reset)
	{
		eedatastorage_Init.PtrFunSetDefaultParametri(ptrMappaturaEEProm);
		eedatastorage_Write(ptrMappaturaEEProm);
	}
	
	return valRet;
}



//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicità dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void eedatastorage_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS

