/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
 	 Gestore file tipo CSV. Per il momento è supportata la filesystem su USB di tipo CustomFS.

 	 Da Obj creato è possibile definire:
 	 	 - La stringa di fine riga ("\r\n" per i Windows like o "\r" per il resto del mondo)
 	 	 - Il carattere separazione campi
 	 	 - Il nome file
 	 	 - Riferimento alla struttura dati inclusa l'intestazione del file quando NON ancora creato.

	Il modulo richiede l'inizializzazione valida di:
		- CustomFS	(Il gestore file system)

	USO & ABUSO

		- Inizializzazione:
			if(csv_InitModule() != _STATUS_OK_)
			{
				... errore serio da gestire
			}

		- Allocazione oggetto:
			uint16_t 	IDMmachine;
			char 		StartTime[10];
			uint16_t 	Durata;
			uint8_t 	IMedia;
			uint16_t 	VMedia;
			uint8_t 	Velocita;
			uint16_t 	Energia;
			uint8_t 	IMax;
			uint8_t 	VMax;
			uint8_t 	JobN;
			uint16_t 	MMin;
			int8_t 		BalanceSet;
			int16_t 	DynamicSet;
			const csv_DataRecord_t TabellaRecordCsv[]=
			{
				{(char*)"ID machine"					,&IDMmachine        	,_TYPEDATA_UINT16_	,4	,0},
				{(char*)"Start_Time[hh:mm:ss]"			,&StartTime     		,_TYPEDATA_PTRSTR_	,0	,0},
				{(char*)"Durata[s]"						,&Durata            	,_TYPEDATA_UINT16_	,4	,1},
				{(char*)"I Media [A]"					,&IMedia            	,_TYPEDATA_UINT8_	,3	,0},
				{(char*)"V Media [V]"					,&VMedia            	,_TYPEDATA_UINT16_	,3  ,1},
				{(char*)"Velocità [m/min]"				,&Velocita          	,_TYPEDATA_UINT8_	,2	,1},
				{(char*)"Energia [J]"					,&Energia           	,_TYPEDATA_UINT16_	,5	,0},
				{(char*)"I Max [A]"						,&IMax              	,_TYPEDATA_UINT8_	,3	,0},
				{(char*)"V Max [A]"						,&VMax              	,_TYPEDATA_UINT8_	,3	,0},
				{(char*)"Job N."						,&JobN              	,_TYPEDATA_UINT8_	,3	,0},
				{(char*)"m/min set"						,&MMin              	,_TYPEDATA_UINT16_	,2	,1},
				{(char*)"Balance set"					,&BalanceSet        	,_TYPEDATA_SINT8_	,3	,0},
				{(char*)"Dynamic set"					,&DynamicSet        	,_TYPEDATA_SINT16_	,3	,0},

				_NULL_	// Terminazione tabella NECESSARIA
			};

			csv_Obj_t *PtrCsvObj;

			....

			PtrCsvObj = csv_CreateObj((csv_DataRecord_t*)TabellaRecordCsv,&_drvcustomfsusb_Driver);
			if(PtrCsvObj)
			{
				PtrCsvObj->SimboloDecimale = '.';
				PtrCsvObj->SimboloRiempimento = 0;	// Ottimizza spazi, altrimenti mettere ' '
			}

		- Aggiunta record:
			csv_AddRecord(PtrCsvObj);

------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/FileSystem/Csv.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"
#include "ARMLib/Common/CustomString.h"


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
csv_Init_t csv_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t csv_InitModule(void)
{
	bool errore=false;

	if(errore)
	{
		csv_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		csv_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

//______________________________________________________________________________
// Desc:  Creazione dell'oggetto csv
// Arg: - ptrDataRecord: ptr alla struittura del record
// Ret: - Ptr all'ggetto creato, se _NULL_ oggetto non creato
//______________________________________________________________________________
csv_Obj_t* csv_CreateObj(csv_DataRecord_t *ptrDataRecord,drvfilesystem_Driver_t *ptrDrvCustomFS)
{
	csv_Obj_t* valRet;

	valRet = (csv_Obj_t*)calloc(1,sizeof(csv_Obj_t));

	if(valRet)
	{
		valRet->PtrDrvCustomFS = ptrDrvCustomFS;
		valRet->PtrDataRecord = ptrDataRecord;
		valRet->CarattereSeparatoreCampi = (char)';';
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Aggiunto un nuovo record al file csv con l'attuale stato delle informazioni
// Arg: - ptrObj: ptr all'oggetto csv creato con csv_CreateObj
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t csv_AddRecord(csv_Obj_t *ptrObj)
{
	int32_t valRet = _STATUS_FAIL_;
	drvfilesystem_HandleFile_t handleFile;
	char* ptrStrNomeFile;
	char* ptrStrFineRecord;
	bool fileEsistente;
	csv_DataRecord_t* ptrDataRecord;
	bool primoData;
	char tmpStr[20];

	// Un minimodi controllo
	if(ptrObj)
	{
		if(csv_Init.InitOk && (ptrObj->PtrDrvCustomFS->PeripheralsStatus()==DRVFILESYSTEM_PERIPHERALSSTATUS_READY))
		{
			if(ptrObj->PtrStrNomeFile)
				ptrStrNomeFile = ptrObj->PtrStrNomeFile;
			else
				ptrStrNomeFile = "Log.csv";

			if(ptrObj->PtrStrFineRecord)
				ptrStrFineRecord = ptrObj->PtrStrFineRecord;
			else
				ptrStrFineRecord = "\r\n";

			// Verifico se esiste già il file
			fileEsistente = false;
			if(ptrObj->PtrDrvCustomFS->FFind(true,_NULL_,ptrStrNomeFile))
				fileEsistente = true;

			if(ptrObj->PtrDrvCustomFS->FOpen(&handleFile,ptrStrNomeFile,DRVFILESYSTEM_FOPENTYPE_APPEND)==_STATUS_OK_)
			{	// File aperto con successo
				ptrObj->PtrDrvCustomFS->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);

				//_____________________Se file NON esistente creo l'intestazione
				if(!fileEsistente)
				{
					ptrDataRecord = ptrObj->PtrDataRecord;
					primoData = true;

					while(ptrDataRecord && ptrDataRecord->PtrDescrittivo)
					{
						if(!primoData)
							ptrObj->PtrDrvCustomFS->FWrite(&handleFile,&ptrObj->CarattereSeparatoreCampi,1);

						ptrObj->PtrDrvCustomFS->FWrite(&handleFile,ptrDataRecord->PtrDescrittivo,strlen(ptrDataRecord->PtrDescrittivo));

						ptrDataRecord++;
						primoData = false;
					}

					ptrObj->PtrDrvCustomFS->FWrite(&handleFile,ptrStrFineRecord,strlen(ptrStrFineRecord));	// Fine riga
				}

				//___________________________________________Aggiunta del record
				ptrDataRecord = ptrObj->PtrDataRecord;
				primoData = true;

				while(ptrDataRecord && ptrDataRecord->PtrDescrittivo)
				{
					if(!primoData)
						ptrObj->PtrDrvCustomFS->FWrite(&handleFile,&ptrObj->CarattereSeparatoreCampi,1);

					switch(ptrDataRecord->TypeData)
					{
						case _TYPEDATA_BOOL_:
						case _TYPEDATA_UINT8_:
						case _TYPEDATA_UINT16_:
						case _TYPEDATA_UINT32_:
							customstring_FormatFix(tmpStr,customstring_GetInt64t(ptrDataRecord->PtrData,ptrDataRecord->TypeData),ptrDataRecord->NumInteri,ptrDataRecord->NumDecimali,ptrObj->SimboloRiempimento,ptrObj->SimboloDecimale,false);
							ptrObj->PtrDrvCustomFS->FWrite(&handleFile,tmpStr,strlen(tmpStr));
						break;

						case _TYPEDATA_SINT8_:
						case _TYPEDATA_SINT16_:
						case _TYPEDATA_SINT32_:
							customstring_FormatFix(tmpStr,customstring_GetInt64t(ptrDataRecord->PtrData,ptrDataRecord->TypeData),ptrDataRecord->NumInteri,ptrDataRecord->NumDecimali,ptrObj->SimboloRiempimento,ptrObj->SimboloDecimale,true);
							ptrObj->PtrDrvCustomFS->FWrite(&handleFile,tmpStr,strlen(tmpStr));
						break;

						case _TYPEDATA_PTRSTR_:
							if(ptrDataRecord->PtrData)
								ptrObj->PtrDrvCustomFS->FWrite(&handleFile,ptrDataRecord->PtrData,strlen(ptrDataRecord->PtrData));
						break;

						default:
						break;
					}

					ptrDataRecord++;
					primoData = false;
				}

				ptrObj->PtrDrvCustomFS->FWrite(&handleFile,ptrStrFineRecord,strlen(ptrStrFineRecord));	// Fine riga

				ptrObj->PtrDrvCustomFS->FClose(&handleFile);

				if(fileEsistente)
					ptrObj->StatusObj = CSV_STATUSOBJ_RECORDAGGIUNTO_NF;
				else
					ptrObj->StatusObj = CSV_STATUSOBJ_RECORDAGGIUNTO_OF;
			}
			else
				valRet = _STATUS_FAIL_;
		}

		if(valRet == _STATUS_FAIL_)
			ptrObj->StatusObj = CSV_STATUSOBJ_FAIL;
	}

	return valRet;
}


//============================================================ PRIVATE FUNCTIONS

