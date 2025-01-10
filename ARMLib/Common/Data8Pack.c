/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Definizione struttura dati a quanti da 8 byte, tipico del can ad esempio.
	Questo modulo NON è legato in alcun modo al protocollo, è solo una unterfaccia
	dati standardizzato.
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/Data8Pack.h"


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES
static uint16_t MsDecounterFreeTime = 0;
static uint16_t IndiceGiroFreeTime = 0;

static uint16_t NumDataSET=0;			// Numero di pack sulla tabella di SET
static uint16_t *PtrLocalMsToSend = _NULL_;	// Tabella timing ms

//============================================================= GLOBAL VARIABLES
data8pack_Init_t data8pack_Init=
{
	.MsFreeTimeSend=200,
};

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t data8pack_InitModule(void)
{
	bool errore=false;
	data8pack_Data_t *ptrStructData;
	uint32_t uLA;

	MsDecounterFreeTime = data8pack_Init.MsFreeTimeSend;

	if(data8pack_Init.PtrStructDataTX)
	{
		// Conto il pack in SET presenti
		ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;

		while(ptrStructData->Id)
		{
			NumDataSET++;
			ptrStructData++;
		}

		if(NumDataSET)
		{
			PtrLocalMsToSend = _CALLOC_(sizeof(uint16_t)*NumDataSET);
			if(!PtrLocalMsToSend)
				errore = true;
		}

		for(uLA = 0; uLA < NumDataSET;uLA++)
			PtrLocalMsToSend[uLA] = data8pack_Init.PtrStructDataTX[uLA].MsToSend;
	}
	
	if(errore)
	{
		data8pack_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		data8pack_Init.InitOk = true;
		return _STATUS_OK_;
	}
}


//______________________________________________________________________________
// Desc:  Ritorna a true sse lo specifico id è libero di essere trasmesso
// Arg: - id: id del pack.
// Ret: - true sse id libero di trasmettere e quindi possibile di essere ritrasmesso
//______________________________________________________________________________
bool data8pack_IsIdFreeToSend(uint16_t id)	//niknik da capire
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;
	bool valRet = false;

	for(index = 0; index < NumDataSET; index++)
	{
		if(ptrStructData[index].Id == id)
		{
			if(PtrLocalMsToSend[index] > 2)
				valRet = true;

			break;
		}
	}

	return valRet;
}


//______________________________________________________________________________
// Desc:  Ritorna a true sse lo specifico id di tipo immediate è libero di essere trasmesso
// Arg: - id: id del pack.
// Ret: - true sse id libero di trasmettere e quindi possibile di essere ritrasmesso
//______________________________________________________________________________
bool data8pack_IsIdImmediateTXFreeToSend(uint16_t id)
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;
	bool valRet = false;

	for(index = 0; index < NumDataSET; index++)
	{
		if(ptrStructData[index].Id == id)
		{
			if(PtrLocalMsToSend[index] != 0)
				valRet = true;

			break;
		}
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Segna l'id come trasmissione immediata
// Arg: - id: id del pack.
// Ret: - void
//______________________________________________________________________________
void data8pack_ForceImmediateTX(uint16_t id)
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;

	for(index = 0; index < NumDataSET; index++)
	{
		if(ptrStructData[index].Id == id)
		{
			PtrLocalMsToSend[index] = 0;
			break;
		}
	}
}

//______________________________________________________________________________
// Desc:  Segna l'id come trasmissione immediata passando direttamente l'index per velocizzare
//        Utile da usare sotto irq o per id continuamente usati
// Arg: - index: index diretto dell'arrary
// Ret: - void
//______________________________________________________________________________
void data8pack_ForceImmediateTXFastIndex(uint16_t index)
{
	PtrLocalMsToSend[index]=0;
}

//______________________________________________________________________________
// Desc:  Ritorna l'index dall'id passato
// Arg: - id: id del pack.
// Ret: - index richiesto, -1 altrimenti
//______________________________________________________________________________
int16_t data8pack_GetIndexFromId(uint16_t id)
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;

	for(index = 0; index < NumDataSET; index++)
	{
		if(ptrStructData[index].Id == id)
		{
			return index;
		}
	}

	return -1;
}



//______________________________________________________________________________
// Desc:  Segna l'id come trasmissione con priorità leggermente più bassa della
//        massima immediata
// Arg: - id: id del pack.
// Ret: - true se beccato, false altrimenti
//______________________________________________________________________________
bool data8pack_ForceTX(uint16_t id)
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;

	for(index = 0; index < NumDataSET; index++)
	{
		if(ptrStructData->Id == id)
		{
			PtrLocalMsToSend[index] = 1;
			return true;
		}
		ptrStructData++;
	}

	return false;
}

//______________________________________________________________________________
// Desc:  Manager da eseguire sul main
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void data8pack_Manager1MS(void)
{
	uint32_t index;
	data8pack_Data_t* ptrStructData = (data8pack_Data_t*)data8pack_Init.PtrStructDataTX;

	for(index = 0; index < NumDataSET; index++)
	{
		if(PtrLocalMsToSend[index] && (PtrLocalMsToSend[index]<0xff00))
			PtrLocalMsToSend[index]--;

		ptrStructData++;
	}

	if(MsDecounterFreeTime)
		MsDecounterFreeTime--;
}

//______________________________________________________________________________
// Desc:  Ritorna il ptr al pack da spedire via proto
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
const data8pack_Data_t* data8pack_PopData(void)
{
	uint32_t index;
	uint16_t indiceGiroFreeTimePartenza = IndiceGiroFreeTime;

	for(index = 0; index < NumDataSET; index++)
	{
		if(!PtrLocalMsToSend[index])
		{
			PtrLocalMsToSend[index] = data8pack_Init.PtrStructDataTX[index].MsToSend;
			return &data8pack_Init.PtrStructDataTX[index];
		}
	}

	// Se si arriva qui significa che nulla è stato spedito, valuto il freetime
	if(!MsDecounterFreeTime)
	{
		MsDecounterFreeTime = data8pack_Init.MsFreeTimeSend;

		IndiceGiroFreeTime++;
		if(IndiceGiroFreeTime>=NumDataSET)
			IndiceGiroFreeTime = 0;
		while(indiceGiroFreeTimePartenza != IndiceGiroFreeTime)
		{
			//if(data8pack_Init.PtrStructDataTX[IndiceGiroFreeTime].MsToSend == DATA8PACK_FREETIME)
			if(PtrLocalMsToSend[IndiceGiroFreeTime] == DATA8PACK_FREETIME)
			{	// Beccato da spedire
				PtrLocalMsToSend[IndiceGiroFreeTime] = 0;
				break;
			}

			IndiceGiroFreeTime++;
			if(IndiceGiroFreeTime>=NumDataSET)
				IndiceGiroFreeTime = 0;
		}
	}

	return _NULL_;
}

//______________________________________________________________________________
// Desc:  Comprime su ptrData l'esploso ptrData8PackData.
// Arg: - Nessun argomento.
// Ret: - Ritorna la size compresso
//______________________________________________________________________________
uint8_t data8pack_ZipData(uint8_t *ptrData,data8pack_Data_t* ptrData8PackData)
{
	uint8_t index;
	uint8_t valRet = 0;
	uint8_t uCA;

	for(index = 0; index < DATA8PACK_DIMDATA; index++)
	{
		if(ptrData8PackData->DataEsploso[index].PtrData!=_NULL_)
		{
			uCA = ptrData8PackData->DataEsploso[index].Size;
			memcpy(ptrData,ptrData8PackData->DataEsploso[index].PtrData, uCA );
			ptrData += uCA;
			valRet += uCA;
		}
		else
			break;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Decomprime sull'esploso ptrData8PackData il ptrData
// Arg: - Nessun argomento.
// Ret: - Ritorna la size decompresso
//______________________________________________________________________________
uint8_t data8pack_UnzipData(data8pack_Data_t* ptrData8PackData,uint8_t *ptrData)
{
	uint8_t index;
	uint8_t valRet = 0;
	uint8_t uCA;

	for(index = 0; index < DATA8PACK_DIMDATA; index++)
	{
		if(ptrData8PackData->DataEsploso[index].PtrData!=_NULL_)
		{
			uCA = ptrData8PackData->DataEsploso[index].Size;
			memcpy(ptrData8PackData->DataEsploso[index].PtrData, ptrData, uCA );
			ptrData += uCA;
			valRet += uCA;
		}
		else
			break;
	}

	return valRet;
}



//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicità dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void data8pack_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS

