/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Protocollo di comunicazione customizzato ad oggetti per essere utilizzato
	su più canali di comunicazione (CAN, Ethernet, Http in Javascript, ecc).
	Per ogni canale di comunicazione DEVE essere creato un oggetto con hprot_CreateObj.
	Necessario swtimer operativo.

	Regole:
		- Gli id partono da H0001 fino a HFFFF. L'id==0 è usato solo per definire
		  il fine array della ptrStructData e non può quindi essere utilizzato.
		- Gli id zona HFF00..HFFFF hanno precisi significati che vanno quindi
		  lasciati liberi.

	Struttura protocollo:
		<STX><address>[Record][Record]...<CHK><ETX>
		- <Address> : H00..H7F	// Usato solo per master/multislave non presente se master/slave
		- [Record]:
			<TypeRecord>[LenData][id][Data]<CR>
				- <TypeRecord> :
					B7: Sempre a 1
					B6: 1==SET, 0==GET
					B5..B3: Liberi
					B2..B0: TypeLenData
						000 : Dimensione 0 (GET)
						001 : Dimensione fissa 1 byte. [LenData] assente.
						010 : Dimensione fissa 2 byte. [LenData] assente.
						011 : Dimensione fissa 4 byte. [LenData] assente.
						100 : Dimensione fissa 8 byte. [LenData] assente.
						101 : Dimensione variabile. [LenData] presente.
		- [LenData]: composto di 2 byte presente solo per TypeLenData==H04
		- [id]: composto da 2 byte sempre.
		- [Data]: Lungo LenData solo in SET.
		- [CR]: Fine record
		- <CHK>: In maschera H7F XEOR sui byte precedenti
		- <ETX>: Fine pacchetto.



------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "HProt.h"
#include "String.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES
static hprot_Status_e ValutazionePck(hprot_Obj_t *ptrObj);
static uint32_t EsecuzionePck(hprot_Obj_t *ptrObj);
static void ExecuteSetId(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData,uint8_t *ptrData,uint32_t dimData);
static void SendDataChk(hprot_Obj_t *ptrObj,uint8_t *ptrData,uint32_t lenData);
static void SendCharChk(hprot_Obj_t *ptrObj,uint8_t carattere);
static void AddTXSetPtrStructData(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData);
static void AddTXGetPtrStructData(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData);

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
hprot_Init_t hprot_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t hprot_InitModule(void)
{
	bool errore=false;

	if(!swtimer_Init.InitOk)
		errore = true;

	if(errore)
	{
		hprot_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		hprot_Init.InitOk = true;
		return _STATUS_OK_;
	}
}


//______________________________________________________________________________
// Desc:  Crea un oggetto HProt
// Arg: - ptrStructData: Ptr alla struttura dati
//		- masterMode: a true indica connessione master
//		- ptrFunGetch: Ptr alla funzione di lettura dato
//		- ptrFunSendData : Ptr alla funzione di scrittura data
// Ret: - ptr all'oggetto, altrimenti _NULL_ se non creato per un qualsiasi problema
//______________________________________________________________________________
hprot_Obj_t* hprot_CreateObj(hprot_StructData_t *ptrStructData,bool masterMode,uint32_t dimBufferAppoggio,
							int32_t (*ptrFunGetChar)(void),
							int32_t (*ptrFunSendData)(uint8_t *ptrData,uint32_t lenData),
							void (*ptrFunFlushRX)(void),
							bool (*ptrFunIsEmptyTX)(void),
							void (*ptrFunSetAddressDest)(uint8_t address),
							uint8_t (*PtrFunGetAddressOrg)(void))
{
	hprot_Obj_t *valRet=_NULL_;
	bool errore = false;

	valRet = _MALLOC_(sizeof(hprot_Obj_t));

	if((valRet!=0) && ptrFunGetChar && ptrFunSendData && ptrFunFlushRX && ptrFunIsEmptyTX && dimBufferAppoggio)
	{
		memset(valRet,0,sizeof(hprot_Obj_t));

		valRet->MasterModeInfo.HTimerAttesaRisposta = swtimer_Create();

		valRet->MasterMode = masterMode;
		valRet->PtrStructData = ptrStructData;
		valRet->PtrFunGetChar = ptrFunGetChar;
		valRet->PtrFunSendData = ptrFunSendData;
		valRet->PtrFunFlushRX = ptrFunFlushRX;
		valRet->PtrFunIsEmptyTX = ptrFunIsEmptyTX;
		valRet->PtrFunSetAddressDest = ptrFunSetAddressDest;
		valRet->PtrFunGetAddressOrg = PtrFunGetAddressOrg;

		valRet->SLObjPV = HPROT_SLOBJPV_VOID;

		valRet->SizeBufferAppoggio = dimBufferAppoggio;
		valRet->PtrBufferAppoggio = _MALLOC_(dimBufferAppoggio);

		if(!valRet->PtrBufferAppoggio)
			errore = true;
	}
	else	// Errore, non allocato o parametro non validi
		errore = true;

	if(errore)
	{
		if(valRet)
			_FREE_(valRet->PtrBufferAppoggio);

		_FREE_(valRet);
	}

	return valRet;
}





//______________________________________________________________________________
// Desc:  Manager dell'oggeto. La funzione è thread safe
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - enum del tipo hprot_Status_t
//______________________________________________________________________________
hprot_Status_e hprot_ManagerObj(hprot_Obj_t *ptrObj)
{
	hprot_Status_e valRet = HPROT_STATUS_VOID;

	if(ptrObj->MasterMode)
	{
		valRet=ValutazionePck(ptrObj);
		switch(valRet)
		{
			case HPROT_STATUS_VOID:
				switch(ptrObj->MasterModeInfo.StatusPckMaster)
				{
					case HPROT_STATUSPCKMASTER_INATTESARISPOSTA:
						if(!ptrObj->PtrFunIsEmptyTX())
							swtimer_Set(ptrObj->MasterModeInfo.HTimerAttesaRisposta,HPROT_MSATTESARISPOSTA);
						// ATTENZIONE: Volutamente senza break;
					case HPROT_STATUSPCKMASTER_RISPOSTAINCORSO:
						if(!swtimer_Test(ptrObj->MasterModeInfo.HTimerAttesaRisposta))
							ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_RISPOSTANONAVVENUTA;
					break;
					default:
					break;
				}
			break;

			case HPROT_STATUS_PCKINARRIVO:
				ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_RISPOSTAINCORSO;
				swtimer_Set(ptrObj->MasterModeInfo.HTimerAttesaRisposta,HPROT_MSATTESARISPOSTA);
			break;

			case HPROT_STATUS_PCKRICEVUTO:
				ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA;
				EsecuzionePck(ptrObj);
				// ATTENZIONE: volutamente senza break
			case HPROT_STATUS_PCKCONFERMATO:
				ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA;
			break;

			case HPROT_STATUS_PCKERRATO:
				SendCharChk(ptrObj,0x15);	// NAK
			break;

			default:
			break;
		}
	}
	else
	{
		valRet=ValutazionePck(ptrObj);
		switch(valRet)
		{
			case HPROT_STATUS_PCKRICEVUTO:
				if(ptrObj->PtrFunSetAddressDest && ptrObj->PtrFunGetAddressOrg)
					ptrObj->PtrFunSetAddressDest(ptrObj->PtrFunGetAddressOrg());
				EsecuzionePck(ptrObj);
			break;
			case HPROT_STATUS_PCKERRATO:
				SendCharChk(ptrObj,0x15);	// NAK
			break;
			default:
			break;
		}
	}

	return valRet;
}


//______________________________________________________________________________
// Desc:  Inizia la creazione di un pacchetto  da spedire lato Master
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - _STATUS_OK_ se è possibile iniziare la compilaziona del pacchetto.
//        Potrebbe essere che la manager stia ancora gestendo il precedente
// 		  pacchetto e non essre pronto per produrne un altro. In questo caso
//		  ritorna _STATUS_FAIL_
//______________________________________________________________________________
int32_t hprot_Master_PckStart(hprot_Obj_t *ptrObj)
{
	if(hprot_Master_PckIsPossibleToStart(ptrObj))
	{
		ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_PCKINCOSTRUZIONE;
		ptrObj->InfoPck.CheckSumCalcolataTX = 0;

		SendCharChk(ptrObj,0x02);	// Inizio pacchetto

		return _STATUS_OK_;
	}
	else
		return _STATUS_FAIL_;
}

//______________________________________________________________________________
// Desc:  Abortisce la costruzione di un packet
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - void
//______________________________________________________________________________
void hprot_Master_PckAbort(hprot_Obj_t *ptrObj)
{
	ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_VOID;
	ptrObj->InfoPck.CheckSumCalcolataTX = 0;
}


//______________________________________________________________________________
// Desc:  Ritorna a true se è possibile procedere con la creazione di un nuovo pacchetto
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - trur sse è possibile procedere
//______________________________________________________________________________
bool hprot_Master_PckIsPossibleToStart(hprot_Obj_t *ptrObj)
{
	bool valRet = false;

	switch(ptrObj->MasterModeInfo.StatusPckMaster)
	{
		case HPROT_STATUSPCKMASTER_VOID:
		case HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA:
		case HPROT_STATUSPCKMASTER_RISPOSTANONAVVENUTA:
			valRet = true;
		break;
		default:
		break;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Aggiunge un id in Set al pacchetto in trasmissione lato Master
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		  trStructData : Riferimento alla formattazione parametro
// Ret: - void
//______________________________________________________________________________
void hprot_Master_PckAddIdSet(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData)
{
	AddTXSetPtrStructData(ptrObj,ptrStructData);
}

//______________________________________________________________________________
// Desc:  Aggiunge un id in Get alla pacchetto in trasmissione lato Master
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		  trStructData : Riferimento alla formattazione parametro
// Ret: - void
//______________________________________________________________________________
void hprot_Master_PckAddIdGet(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData)
{
	AddTXGetPtrStructData(ptrObj,ptrStructData);
}

//______________________________________________________________________________
// Desc:  Termina ikl pacchetto in trasmissione lato server
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		  trStructData : Riferimento alla formattazione parametro
// Ret: - void
//______________________________________________________________________________
void hprot_Master_PckEnd(hprot_Obj_t *ptrObj)
{
	SendCharChk(ptrObj,ptrObj->InfoPck.CheckSumCalcolataTX&0x7f);	// CheckSum
	SendCharChk(ptrObj,0x03);	// Fine pacchetto

	swtimer_Set(ptrObj->MasterModeInfo.HTimerAttesaRisposta,HPROT_MSATTESARISPOSTA);
	ptrObj->MasterModeInfo.StatusPckMaster = HPROT_STATUSPCKMASTER_INATTESARISPOSTA;
}


//______________________________________________________________________________
// Desc:  Ritorna il ptr al StructData dall id voluto
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		- id : id da cercare3
// Ret: - ptr al StructData se trovato altrimenti _NULL_
//______________________________________________________________________________
hprot_StructData_t* hprot_GetStructDataFromId(hprot_Obj_t *ptrObj,uint16_t id)
{
	hprot_StructData_t *valRet = _NULL_;
	hprot_StructData_t *ptrStructData = ptrObj->PtrStructData;

	while(ptrStructData->Id)
	{
		if(ptrStructData->Id == id)
		{	// Beccato e quidni esco
			valRet = ptrStructData;
			break;
		}
		ptrStructData++;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Ritorna il ptr al StructData dall id voluto
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		- ptrVar: Riferimento alla var da ricercare
// Ret: - ptr al StructData se trovato altrimenti _NULL_
//______________________________________________________________________________
hprot_StructData_t* hprot_GetStructDataFromPtrVar(hprot_Obj_t *ptrObj,void* ptrVar)
{
	hprot_StructData_t *valRet = _NULL_;
	hprot_StructData_t *ptrStructData = ptrObj->PtrStructData;

	while(ptrStructData->Id)
	{
		if(ptrStructData->PtrData == ptrVar)
		{	// Beccato e quidni esco
			valRet = ptrStructData;
			break;
		}
		ptrStructData++;
	}

	return valRet;
}


//============================================================ PRIVATE FUNCTIONS

//______________________________________________________________________________
// Desc:  Valutazione del pacchetto in arrivo
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - true, se pck valido ed eseguibile
//______________________________________________________________________________
static hprot_Status_e ValutazionePck(hprot_Obj_t *ptrObj)
{
	hprot_Status_e valRet = HPROT_STATUS_VOID;
	int8_t carattere;
	int32_t ricex;
	uint8_t uCA;

	ricex = ptrObj->PtrFunGetChar();
	while(ricex!=-1)
	{
		valRet = HPROT_STATUS_PCKINARRIVO;
		carattere = (uint8_t)ricex;
		ptrObj->InfoPck.CheckSumCalcolataRX ^= carattere;
		switch(ptrObj->SLObjPV)
		{
			case HPROT_SLOBJPV_VOID:
				switch(carattere)
				{
					case 0x02:	// STX
						// Se carattere ricevuto procedo con l'analisi del pacchetto
						ptrObj->IndiceWrBufferAppoggio = 0;
						memset(&ptrObj->InfoPck,0,sizeof(ptrObj->InfoPck));

						ptrObj->SLObjPV = HPROT_SLOBJPV_WAITPOSTSTX;
						ptrObj->InfoPck.CheckSumCalcolataRX = carattere;
					break;
					case 0x06:	// ACK
						valRet = HPROT_STATUS_PCKCONFERMATO;
					break;
				}
			break;

			case HPROT_SLOBJPV_WAITPOSTSTX:
				if(carattere&_B7_)
				{	// Inizia un record e quindi un <TypeRecord>
					ptrObj->InfoPck.TypeRecord = carattere;

					switch((hprot_TypeLenData_e)(ptrObj->InfoPck.TypeRecord&0x07))
					{
						case HPROT_TYPELENDATA_GET:
							ptrObj->InfoPck.NumRecordGETRilevati++;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITID1;
						break;
						case HPROT_TYPELENDATA_DIMFIX1:
							ptrObj->InfoPck.NumRecordSETRilevati++;
							ptrObj->InfoPck.LenData = 1;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITID1;
						break;
						case HPROT_TYPELENDATA_DIMFIX2:
							ptrObj->InfoPck.NumRecordSETRilevati++;
							ptrObj->InfoPck.LenData = 2;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITID1;
						break;
						case HPROT_TYPELENDATA_DIMFIX4:
							ptrObj->InfoPck.NumRecordSETRilevati++;
							ptrObj->InfoPck.LenData = 4;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITID1;
						break;
						case HPROT_TYPELENDATA_DIMFIX8:
							ptrObj->InfoPck.NumRecordSETRilevati++;
							ptrObj->InfoPck.LenData = 8;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITID1;
						break;
						case HPROT_TYPELENDATA_DIMVAR:
							ptrObj->InfoPck.LenData = 0;
							ptrObj->InfoPck.NumRecordSETRilevati++;
							ptrObj->SLObjPV=HPROT_SLOBJPV_WAITLENDATA1;
						break;
						default:
						break;
					}
				}
				else
				{	// Se è un <address> ...
					// NOTA: Se un record è già stato rilevato significa che questo è la
					// checksum di fine pacchetto altrimenti è l'address destinatario del pacchetto
					if(ptrObj->InfoPck.NumRecordGETRilevati || ptrObj->InfoPck.NumRecordSETRilevati)
					{
						ptrObj->InfoPck.CheckSumCalcolataRX ^= carattere;	// Ritolgo l'eorata aggiunta in testa...
						if((ptrObj->InfoPck.CheckSumCalcolataRX&0x7f)==carattere)	// La chk ha sempre B7 basso
						{	// Checksum valida, procedo con l'esecuzione effettina del pacchetto
							ptrObj->SLObjPV = HPROT_SLOBJPV_WAITETX;	// Attendi l'ETX finale
						}
						else
							valRet = HPROT_STATUS_PCKERRATO;	//Nik valutare qui di richiedere il pacchetto...
					}
					else
					{	// Questo è un address e quindi il statusloop rimane qui per procedere con l'analisi del pacchetto

						ptrObj->InfoPck.Address = carattere;
					}
				}
			break;

			case HPROT_SLOBJPV_WAITLENDATA1:
				uCA = (uint8_t)carattere;	// Accrocchio per evitare un signed sulla conversione che mi rende negativo tutto il uit32
				ptrObj->InfoPck.LenData = uCA;
				ptrObj->SLObjPV++;
			break;
			case HPROT_SLOBJPV_WAITLENDATA2:
				uCA = (uint8_t)carattere;	// Accrocchio per evitare un signed sulla conversione che mi rende negativo tutto il uit32
				ptrObj->InfoPck.LenData |= ((uint32_t)uCA)<<8;
				ptrObj->SLObjPV++;
			break;
			case HPROT_SLOBJPV_WAITLENDATA3:
				uCA = (uint8_t)carattere;	// Accrocchio per evitare un signed sulla conversione che mi rende negativo tutto il uit32
				ptrObj->InfoPck.LenData |= ((uint32_t)uCA)<<16;
				ptrObj->SLObjPV++;
			break;

			case HPROT_SLOBJPV_WAITID1:
				ptrObj->InfoPck.Id = carattere;
				ptrObj->SLObjPV++;
			break;
			case HPROT_SLOBJPV_WAITID2:
				ptrObj->InfoPck.Id |= ((uint16_t)carattere)<<8;

				if(ptrObj->InfoPck.TypeRecord==0x80)
				{	// Se è una domanda vado in cerca del CR di fine record
					ptrObj->SLObjPV = HPROT_SLOBJPV_WAITCR;
				}
				else	// Altrimenti continua con la lettura dati
				{
					ptrObj->SLObjPV++;
				}
			break;

			case HPROT_SLOBJPV_WAITDATA:
				if(ptrObj->InfoPck.LenData)
				{
					ptrObj->InfoPck.LenData--;
					if(!ptrObj->InfoPck.LenData)
					{	// Tutti i data ricevuti vado alla ricerca del CR
						ptrObj->SLObjPV = HPROT_SLOBJPV_WAITCR;
					}
				}
				else
					valRet = HPROT_STATUS_PCKERRATO;	// Qualcosa è andato storto se mi trovo qui con un LenData==0
			break;

			case HPROT_SLOBJPV_WAITCR:
				if(carattere==0x0d)
					ptrObj->SLObjPV = HPROT_SLOBJPV_WAITPOSTSTX;	// Valuta se altro record o fine pacchetto
				else
				{	// Pacchetto malformattato
					//Nik valutare qui regole di ingaggio
					valRet = HPROT_STATUS_PCKERRATO;
				}
			break;

			case HPROT_SLOBJPV_WAITETX:
				if(carattere==0x03)
					valRet = HPROT_STATUS_PCKRICEVUTO;
				else
					valRet = HPROT_STATUS_PCKERRATO;
			break;
		}

		switch(valRet)
		{
			case HPROT_STATUS_PCKERRATO:
				ptrObj->PtrFunFlushRX();
			case HPROT_STATUS_PCKRICEVUTO:
				ptrObj->SLObjPV = HPROT_SLOBJPV_VOID;	// Parti dacapo

				ricex = -1;	// Per uscire dal ciclo
			break;
			case HPROT_STATUS_PCKCONFERMATO:
				// Qui valutazione particolare. Se ACK senza nessun seguito lo segnalo, altrimenti
				// roseguo con analisi senza metterlo nel buffer di appoggio
				ricex = ptrObj->PtrFunGetChar();
			break;

			default:
				ptrObj->PtrBufferAppoggio[ptrObj->IndiceWrBufferAppoggio++] = carattere;
				if(ptrObj->IndiceWrBufferAppoggio>=ptrObj->SizeBufferAppoggio)
					ptrObj->IndiceWrBufferAppoggio=ptrObj->SizeBufferAppoggio-1;

				ricex = ptrObj->PtrFunGetChar();
			break;
		}

	}	// Fine while(ricex!=-1)

	return valRet;
}



//______________________________________________________________________________
// Desc:  Scrive sull'id il ptrData passati
// Arg: - id : l'id da aggiungere
//		- ptrData: ptr al data array
//		- dimData: dimensione passata
// Ret: - void
//______________________________________________________________________________
static void ExecuteSetId(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData,uint8_t *ptrData,uint32_t dimData)
{
	int32_t status = _STATUS_OK_;

	if(ptrStructData->PtrFunExecute)
		status = ptrStructData->PtrFunExecute(ptrStructData,HPROT_TYPEFUNEXECUTE_PRESET);

	if(status==_STATUS_OK_)
	{
		dimData = _MIN_(dimData,ptrStructData->MaxLen);
		memcpy(ptrStructData->PtrData,ptrData,dimData);
		if(ptrStructData->TypeData == _TYPEDATA_PTRSTR_)
		{	// Termino la stringa alla dimensione massima in ogni caso sempre e cmq (utile se il master non passa anche lo zero finale)
			((uint8_t*)ptrStructData->PtrData)[ptrStructData->MaxLen-1] = 0;
		}
	}

	if(ptrStructData->PtrFunExecute)
		ptrStructData->PtrFunExecute(ptrStructData,HPROT_TYPEFUNEXECUTE_POSTSET);
}


//______________________________________________________________________________
// Desc:  Compone la trasmissione del record in SET
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		- ptrStructData : Riferimento alla formattazione parametro
// Ret: - void
//______________________________________________________________________________
static void AddTXSetPtrStructData(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData)
{
	uint32_t sizeToCopy=0;
	uint8_t typeData=0;

	if(ptrStructData)
	{
		switch(ptrStructData->TypeData)
		{
			case _TYPEDATA_BOOL_:
			case _TYPEDATA_UINT8_:
			case _TYPEDATA_SINT8_:
				sizeToCopy = 1;
				typeData = 0xc1;
			break;
			case _TYPEDATA_UINT16_:
			case _TYPEDATA_SINT16_:
				sizeToCopy = 2;
				typeData = 0xc2;
			break;
			case _TYPEDATA_UINT32_:
			case _TYPEDATA_SINT32_:
			case _TYPEDATA_FLOAT_:
				sizeToCopy = 4;
				typeData = 0xc3;
			break;
			case _TYPEDATA_DOUBLE_:
				sizeToCopy = 8;
				typeData = 0xc4;
			break;
			case _TYPEDATA_PTRSTR_:
				sizeToCopy = strlen(ptrStructData->PtrData);
				typeData = 0xc5;
			break;

			case _TYPEDATA_ARRAYOFBOOL_:
			case _TYPEDATA_ARRAYOFUINT8_:
			case _TYPEDATA_ARRAYOFSINT8_:
			case _TYPEDATA_ARRAYOFUINT16_:
			case _TYPEDATA_ARRAYOFSINT16_:
			case _TYPEDATA_ARRAYOFUINT32_:
			case _TYPEDATA_ARRAYOFSINT32_:
			case _TYPEDATA_ARRAYOFFLOAT_:
			case _TYPEDATA_ARRAYOFDOUBLE_:
			case _TYPEDATA_ARRAYOFPTRSTR_:
				sizeToCopy = ptrStructData->MaxLen;
				typeData = 0xc5;
			break;

			default:
			break;
		}

		// <TypeRecord>
		SendCharChk(ptrObj,typeData);
		// [LenData] se necessario
		if(typeData==0xc5)
		{	// Aggiungo i due byte della dimensione variabile
			SendDataChk(ptrObj,(uint8_t*)&sizeToCopy,3);
		}
		// [id] due byte
		SendDataChk(ptrObj,(uint8_t*)&ptrStructData->Id,2);
		// [Data]
		SendDataChk(ptrObj,(uint8_t*)ptrStructData->PtrData,sizeToCopy);
		// <CR> fine record
		SendCharChk(ptrObj,0x0d);
	}
}

//______________________________________________________________________________
// Desc:  Compone la trasmissione del record in GET
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
//		- ptrStructData : Riferimento alla formattazione parametro
// Ret: - void
//______________________________________________________________________________
static void AddTXGetPtrStructData(hprot_Obj_t *ptrObj,hprot_StructData_t *ptrStructData)
{
	if(ptrStructData)
	{
		// <TypeRecord>
		SendCharChk(ptrObj,0x80);
		// [id] due byte
		SendDataChk(ptrObj,(uint8_t*)&ptrStructData->Id,2);
		// <CR> fine record
		SendCharChk(ptrObj,0x0d);
	}
}


//______________________________________________________________________________
// Desc:  Funzione di appogio per spedire un buffer calcolando la checksum da poter riutilizzare
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - Il numero di byte da trasmettere
//______________________________________________________________________________
static void SendDataChk(hprot_Obj_t *ptrObj,uint8_t *ptrData,uint32_t lenData)
{
	uint8_t tmpBuffer[100];
	uint32_t len;

	// NOTA: Qui c'è la reale necessità di calcolare la checksum sugli esatti dati
	// trasmessi. Si pensi al caso di trasmissione della video ram dove l'array
	// è in continua e veloce variazione come pure le variabili stesse... Esattamente
	// quello che trasmette DEVE essre usato per il calcolo della checksum
	while(lenData)
	{
		len = _MIN_(lenData,sizeof(tmpBuffer));

		memcpy(tmpBuffer,ptrData,len);
		ptrObj->PtrFunSendData(tmpBuffer,len);
		ptrData+=len;

		lenData-=len;
		while(len)
		{
			len--;
			ptrObj->InfoPck.CheckSumCalcolataTX^=tmpBuffer[len];
		}
	}
}

//______________________________________________________________________________
// Desc:  Funzione di appogio per spedire un carattere calcolando la checksum da poter riutilizzare
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - Il numero di byte da trasmettere
//______________________________________________________________________________
static void SendCharChk(hprot_Obj_t *ptrObj,uint8_t carattere)
{
	SendDataChk(ptrObj,&carattere,1);
}

//______________________________________________________________________________
// Desc:  Esecuzione del pacchetto arrivato
// Arg: - ptrObj : Ptr all'oggetto creato con hprot_CreateObj
// Ret: - Il numero di byte da trasmettere
//______________________________________________________________________________
static uint32_t EsecuzionePck(hprot_Obj_t *ptrObj)
{
	uint32_t numRecord;
	uint8_t *ptrRdData = ptrObj->PtrBufferAppoggio;
	uint16_t *ptrId;
	hprot_StructData_t *ptrStructData;
	uint32_t dimData=0;
	uint8_t typeRecord;
	uint32_t valRet = 0;
	bool errore = false;

	// In questa parte del codice NON serve valutare la conformità del pacchetto
	// perchè è già stata fatta dalla ValutazionePck() precedente. Quindi se arrivo
	// qui il pck in ricezione è valido

	ptrRdData++;	// Salto l'STX iniziale e eventuale <address>
	if(!((*ptrRdData)&_B7_))
		ptrRdData++;

	if(!ptrObj->MasterMode)
	{	// In modalità master NON si manda l'ACK
		// Tecnicamente in modalità master non si dovrebbe ricevere domande dal slave
		if(!ptrObj->InfoPck.NumRecordGETRilevati)
			SendCharChk(ptrObj,0x06);	// Se non sono attese risposte do l'ack e vado oltre
	}

	ptrObj->InfoPck.CheckSumCalcolataTX = 0;

	if(ptrObj->InfoPck.NumRecordGETRilevati)	// Se è da compilare una risposta inizio il pck
		SendCharChk(ptrObj,0x02);	// STX di apertura

	for(numRecord=0;numRecord<(ptrObj->InfoPck.NumRecordGETRilevati+ptrObj->InfoPck.NumRecordSETRilevati);numRecord++)
	{
		typeRecord = *ptrRdData;
		ptrRdData++;
		if(typeRecord==0x80)
		{	// Richiesta dal master
			// Successivi 2 byte l'id
			ptrId = (uint16_t*)ptrRdData;
			ptrRdData+=2;

			ptrStructData = hprot_GetStructDataFromId(ptrObj, *ptrId);
			if(ptrStructData)
			{
				if(ptrStructData->PtrFunExecute)
					ptrStructData->PtrFunExecute(ptrStructData,HPROT_TYPEFUNEXECUTE_PRESET);

				AddTXSetPtrStructData(ptrObj,ptrStructData);

				if(ptrStructData->PtrFunExecute)
					ptrStructData->PtrFunExecute(ptrStructData,HPROT_TYPEFUNEXECUTE_POSTSET);
			}
			else
			{
				// Anche se non trova l'id mappato poco male, semplicemente non aggiunge il SET e fa finta di niente...
			}

			ptrRdData++;	// Salto anche CR
		}
		else
		{	// Impostazione dal master
			switch(typeRecord&0x07)
			{
				case 1:
					dimData = 1;
				break;
				case 2:
					dimData = 2;
				break;
				case 3:
					dimData = 4;
				break;
				case 4:
					dimData = 8;
				break;
				case 5:
					dimData=(uint32_t)(*ptrRdData);
					ptrRdData++;
					dimData|=((uint32_t)(*ptrRdData))<<8;
					ptrRdData++;
					dimData|=((uint32_t)(*ptrRdData))<<16;
					ptrRdData++;
				break;

			}

			// Successivi 2 byte l'id
			ptrId = (uint16_t*)ptrRdData;
			ptrRdData+=2;

			ptrStructData = hprot_GetStructDataFromId(ptrObj, *ptrId);

			// Un minimo fi protezione alle sovrascritture
			if(dimData>ptrStructData->MaxLen)
				dimData=ptrStructData->MaxLen;

			// Qui ptrData punta proprio al data
			if(ptrStructData && !(ptrStructData->Permission&HPROT_PERMISSION_NOWR))
				ExecuteSetId(ptrObj, ptrStructData, ptrRdData, dimData);

			ptrRdData+= dimData+1;	// Salto anche il CR finale
		}

		if(errore)
			break;
	}	// fine for(numRecord=0;numRecord<ptrObj->InfoPck.NumRecordRilevati;numRecord++)

	if(ptrObj->InfoPck.NumRecordGETRilevati)
	{
		SendCharChk(ptrObj,(ptrObj->InfoPck.CheckSumCalcolataTX&0x7f));
		SendCharChk(ptrObj,0x03);	// ETX di fine pck
	}

	return valRet;
}








