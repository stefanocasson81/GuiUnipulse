/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	29/06/20
 Descrizione:
	Libreria gestione file codificati in Custom style per aggiornamento ed estrazione file tramite la
	funzione fwupgrade_CheckFileAndShoot.
	Il modulo usa le librerie _DrvMcuFlash e CustomFS che DEVONO essere preventivamente
	inizializzate.
	Questo modulo viene usato dal gateway con usb per aggiornare le vaire periferiche, le periferiche
	slave NON lo usano.
	La funzione fwupgrade_VUGetData è l'unica utilizzata da gateway per aggiornare uno slave.
	
	USO e ABUSO
	Esempio lettura file .pck e aggiornamento automatico con un timeout presenza 
	USB di 3000 ms
	
		swtimer_TimerHandler_t hTimer = swtimer_Create(_NULL_);
		swtimer_Set(hTimer,5000);	// Timeout presenza chiavetta usb
		if(fwupgrade_InitModule() != _STATUS_OK_)
			erroriInitModuleRilevati++; 

		while(1)
		{
			// ...oppure se vuole aggiornare tramite binario nudo e crudo
			fwupgrade_Status_e status = fwupgrade_CheckAndShoot("DB521_BootLoader.bin");
		
			switch(status)
			{
				case FWUPGRADE_STATUS_USBNONRILEVATA:					// Usb NON rilevata
					if(!swtimer_Test(hTimer))
						ExecuteNormalApp();
				break;
				case FWUPGRADE_STATUS_FILENONRILEVATO:					// Usb rilevata ma file NON rilevato
				case FWUPGRADE_STATUS_PCKNONVALIDO:						// File PCK NON valido
				case FWUPGRADE_STATUS_FIRMWAREAGGIORNATO:			// Firmware su PCK aggiornato
				case FWUPGRADE_STATUS_FIRMWAREERRFLASH:				// Firmware su PCK NON aggiornato per errore generico in mcuflash
				case FWUPGRADE_STATUS_OK:                      // Lettura file corretta
				case FWUPGRADE_STATUS_ERROREGENERICO:					// Altro errore generico
					ExecuteNormalApp();
				break;
			}
			
	#ifdef RTE_CMSIS_RTOS2
			osDelay(5);
	#endif
		}

------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/FWUpgrade.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"
#include "ARMLib/Common/Crc.h"


//====================================================================== DEFINES
typedef enum
{
	FWUPGRADE_SL_SEARCHTAGPOSTNOMEFILE,
	FWUPGRADE_SL_SEARCHTAGPOSTOFFSETFILE,
	FWUPGRADE_SL_SEARCHTAGPOSTSIZEFILE,
	FWUPGRADE_SL_SEARCHLF,
}fwupgrade_SL_e;

typedef struct
{
	char StartCode;
	uint8_t ByteCount;
	uint16_t Address;
	uint8_t RecordType;
	uint8_t Data[16];
	uint8_t CheckSum;
}fwupgrade_HexRecord_t;




//=========================================================== PRIVATE PROTOTYPES
static uint8_t AsciiHexToByte(char c);
static fwupgrade_Status_e ShootFileBin(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato);
static fwupgrade_Status_e ShootFileHex(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato);
static uint32_t GetCrc32HEXFileCodifiato(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato);
static uint16_t GetCrc16BINFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato);
static uint32_t GetCrc32BINFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato);
static int32_t CompilaHexRecordFromHandleFile(drvfilesystem_HandleFile_t *ptrHandleFile,fwupgrade_HexRecord_t *ptrHexRecord);
static void DeCodificaBuffer32(uint8_t *ptrBuffer32);

//============================================================= STATIC VARIABLES


//============================================================= GLOBAL VARIABLES
fwupgrade_Init_t fwupgrade_Init=
{
	.AddressFirmware 			= 0xffffffff,
	.CodiceSchedaEsteso 				= 0,
};
fwupgrade_Info_t fwupgrade_Info=
{
	.Status = FWUPGRADE_STATUS_USBNONRILEVATA,
};

fwupgrade_VirtualizeUpgrade_t fwupgrade_VirtualizeUpgrade;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Descrizione della funzione.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t fwupgrade_InitModule(void)
{
	bool errore=false;

	// Necessario driver scrittura in flash
	/* Darebbe da mettere ma per evitare errori di compilazione se no nusato evito....
	 * if(!fwupgrade_Init.PtrDrvFlashDriver)
		errore=true;*/

	// Necessario codice scheda diverso da zero
	if(!fwupgrade_Init.CodiceSchedaEsteso)
		errore = true;

	if(!fwupgrade_Init.PtrDrvfilesystem)
		errore = true;

	if(errore)
	{
		fwupgrade_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		fwupgrade_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

/*______________________________________________________________________________
 Desc: Analizza il nome file passato per ottenere info di codifica
 Arg: - strNomeFile: Nome del file es "Benito.hlv"
 	 	validaCRCFile: a true se si intende validare anche il crc calcolandolo, attenzione tempi lunghi per HEX
 Ret: - _STATUS_OK se file valido, _STATUS_FAIL_ se file valido
______________________________________________________________________________*/
int32_t fwupgrade_CheckFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato,bool validaCRCFile)
{
	uint8_t tmpBuffer[10];
	uint32_t sizeFile;
	uint32_t uLA;
	int32_t valRet = _STATUS_FAIL_;
	bool errore = false;
	drvfilesystem_HandleFile_t handleFile;

	ptrInfoFileCodificato->Crc16Originale = 0xffff;
	ptrInfoFileCodificato->Crc32Originale = 0xffffffff;
	ptrInfoFileCodificato->Crc32Codificato = 0xffffffff;
	ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONVALIDO;

	if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	{	// File aperto con successo e quindi procedi con aggiornamento firmware
		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
		sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);

		// Verifico che sia almeno grande quanto l'intestazione
		if(sizeFile>=0x200)
		{
			// Cerco il pattern fisso per capire se è un file codificato valido
			fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x0c,DRVFILESYSTEM_SEEK_SET);
			if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,8)!=_STATUS_FAIL_)
			{
				// Pattern fisso di 8 byte (02 02 07 03 05 04 02 02).
				uLA=0;
				if(tmpBuffer[0]==0x02)
					uLA++;
				if(tmpBuffer[1]==0x02)
					uLA++;
				if(tmpBuffer[2]==0x07)
					uLA++;
				if(tmpBuffer[3]==0x03)
					uLA++;
				if(tmpBuffer[4]==0x05)
					uLA++;
				if(tmpBuffer[5]==0x04)
					uLA++;
				if(tmpBuffer[6]==0x02)
					uLA++;
				if(tmpBuffer[7]==0x02)
					uLA++;

				if(uLA==8)
				{	// OK è un file codificato, vedo se è di nuova generazione, altrimenti segue un file di tipo BIN
					fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x14,DRVFILESYSTEM_SEEK_SET);
					if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,8)!=_STATUS_FAIL_)
					{	// Pattern (AA 55 66 99 55 aa 00) + 01
						uLA=0;
						if(tmpBuffer[0]==0xaa)
							uLA++;
						if(tmpBuffer[1]==0x55)
							uLA++;
						if(tmpBuffer[2]==0x66)
							uLA++;
						if(tmpBuffer[3]==0x99)
							uLA++;
						if(tmpBuffer[4]==0x55)
							uLA++;
						if(tmpBuffer[5]==0xaa)
							uLA++;
						if(tmpBuffer[6]==0x00)
							uLA++;
						if(tmpBuffer[7]==0x01)
							uLA++;

						if(uLA==8)
						{	// Ok è un file di nuova generazione
							fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x20,DRVFILESYSTEM_SEEK_SET);
							if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,1)!=_STATUS_FAIL_)
							{
								if(tmpBuffer[0]==0)
								{	// File di tipo BIN
									ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_BIN;
								}
								else if(tmpBuffer[0]==1)
								{	// File di tipo HEX
									ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_HEX;
								}

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x00,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->CodiceSchedaEsteso,4)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x04,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->TotalSizeAlign32,4)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x08,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->Crc16Originale,2)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x1c,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->Crc32Originale,4)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x21,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->Crc32Codificato,4)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x26,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->VersioneFirmware,1)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x27,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->DataCreazione,4)==_STATUS_FAIL_)
									errore = true;

								fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x2B,DRVFILESYSTEM_SEEK_SET);
								if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->CodiceSchedaEstesoBootLoaderCompatibility,4)==_STATUS_FAIL_)
									errore = true;

								if(!errore)
									valRet = _STATUS_OK_;
							}
						}
						else
						{	// ... è un file di vecchia generazione e quindi solo BIN file
							fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x04,DRVFILESYSTEM_SEEK_SET);
							if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->TotalSizeAlign32,4)==_STATUS_FAIL_)
								errore = true;

							fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x08,DRVFILESYSTEM_SEEK_SET);
							if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,&ptrInfoFileCodificato->Crc16Originale,2)==_STATUS_FAIL_)
								errore = true;

							if(!errore)
							{
								ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_BINOLDSTYLE;
								valRet = _STATUS_OK_;
							}
						}
					}
				}
			}
		}	// Fine if(sizeFile>=0x200)
		else
			valRet = _STATUS_FAIL_;

		if(valRet==_STATUS_FAIL_)
			ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONVALIDO;

		fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
	}	// Fine if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	else
		ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONRILEVATO;

	ptrInfoFileCodificato->CrcValidato = false;
	if(validaCRCFile)
	{
		switch(ptrInfoFileCodificato->TipoFile)
		{
			case FWUPGRADE_TIPOFILE_BINOLDSTYLE:
				uLA = GetCrc16BINFile(strNomeFile,ptrInfoFileCodificato);
				if(uLA==ptrInfoFileCodificato->Crc16Originale)
					ptrInfoFileCodificato->CrcValidato = true;
				else
				{
					valRet =_STATUS_FAIL_;
					ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONVALIDO;
				}
			break;
			case FWUPGRADE_TIPOFILE_BIN:
				uLA = GetCrc32BINFile(strNomeFile,ptrInfoFileCodificato);
				if(uLA==ptrInfoFileCodificato->Crc32Originale)
					ptrInfoFileCodificato->CrcValidato = true;
				else
				{
					valRet =_STATUS_FAIL_;
					ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONVALIDO;
				}
			break;
			case FWUPGRADE_TIPOFILE_HEX:
				uLA = GetCrc32HEXFileCodifiato(strNomeFile,ptrInfoFileCodificato);
				if(uLA==ptrInfoFileCodificato->Crc32Codificato)
					ptrInfoFileCodificato->CrcValidato = true;
				else
				{
					valRet =_STATUS_FAIL_;
					ptrInfoFileCodificato->TipoFile = FWUPGRADE_TIPOFILE_NONVALIDO;
				}
			break;
			default:
			break;
		}
	}
	else
		ptrInfoFileCodificato->CrcValidato = true;;

	return valRet;
}

/*______________________________________________________________________________
 Desc: Cerca su USB il file passato incluso di estensione e aggiorna il firmware di brutto.
 	 Nel caso del formato Hex la posizione in flash è dettata dal file stesso ma in
 	 ogni caso la zona flash del bootloader rimane intatta.
 	 Il file dovrà essere di tipo codificato e i formati supportati sono BIN e HEX.

 Arg: - strNomeFile: Nome del file es "Benito.hlv"
 	  - virtualize: a true usa le funzioni di virtualizzazione per aggiornamento via HProt
 Ret: - enum del tipo fwupgrade_Status_e
______________________________________________________________________________*/
fwupgrade_Status_e fwupgrade_CheckFileAndShoot(char* strNomeFile,bool virtualize,uint32_t codiceSchedaVirtualize)
{
	fwupgrade_InfoFileCodificato_t infoFileCodificato;
	fwupgrade_Status_e valRet = FWUPGRADE_STATUS_FILENONRILEVATO;
	bool procediAggiornamento;

	if(fwupgrade_CheckFile(strNomeFile,&infoFileCodificato,false)==_STATUS_OK_)
	{
		switch(infoFileCodificato.TipoFile)
		{
			case FWUPGRADE_TIPOFILE_NONVALIDO:
				fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;
			break;
			case FWUPGRADE_TIPOFILE_NONRILEVATO:
				fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONRILEVATO;
			break;
			case FWUPGRADE_TIPOFILE_BINOLDSTYLE:
				if(fwupgrade_CheckFile(strNomeFile,&infoFileCodificato,true)==_STATUS_OK_)
				{
					if(infoFileCodificato.Crc16Originale==fwupgrade_Init.Crc16FirmwareAttuale)
						fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_AGGIORNAMENTONONNECESSARIO;
					else
					{
						if(infoFileCodificato.CrcValidato)
							fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_OPENFILEBIN;
						else
							fwupgrade_Info.Status = valRet = ShootFileBin(strNomeFile, &infoFileCodificato);
					}
				}
			break;
			case FWUPGRADE_TIPOFILE_BIN:
			case FWUPGRADE_TIPOFILE_HEX:
				procediAggiornamento = false;
				if(virtualize)
				{
					// Controllo le ultime 5 cifre del codice scheda
					if(infoFileCodificato.CodiceSchedaEsteso==codiceSchedaVirtualize)
						procediAggiornamento = true;
					else if(infoFileCodificato.CodiceSchedaEstesoBootLoaderCompatibility==codiceSchedaVirtualize)	// Questo è il caso di scheda ancora non battezzata
						procediAggiornamento = true;
				}
				else
				{
					if(infoFileCodificato.CodiceSchedaEsteso==fwupgrade_Init.CodiceSchedaEsteso)
					{	// Il codice scheda esteso è lo stesso e quindi si può procedere
						procediAggiornamento = true;
					}
					else
					{
						if(fwupgrade_Init.ProcediAlCambioFirmware)
						{
							if(infoFileCodificato.CodiceSchedaEstesoBootLoaderCompatibility==fwupgrade_Init.CodiceSchedaEstesoBootLoaderCompatibility)
								procediAggiornamento = true;
						}
					}
				}

				if(procediAggiornamento)
				{
					if((infoFileCodificato.Crc32Originale==fwupgrade_Init.Crc32FirmwareAttuale) && (!virtualize))
						fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_AGGIORNAMENTONONNECESSARIO;
					else
					{
						if(fwupgrade_CheckFile(strNomeFile,&infoFileCodificato,true)==_STATUS_OK_)	// a false per evitare controllo completo da usare in test per velocizzare...
						{
							if(infoFileCodificato.CrcValidato)
							{
								if(virtualize)
								{
									if(infoFileCodificato.TipoFile==FWUPGRADE_TIPOFILE_HEX)
										fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_OPENFILEHEX;
									else
										fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_OPENFILEBIN;
								}
								else
								{
									if(infoFileCodificato.TipoFile==FWUPGRADE_TIPOFILE_HEX)
										fwupgrade_Info.Status = valRet = ShootFileHex(strNomeFile, &infoFileCodificato);
									else
										fwupgrade_Info.Status = valRet = ShootFileBin(strNomeFile, &infoFileCodificato);
								}
							}
						}
						else
							fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;
					}
				}
				else
					fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;
			break;

			default:
			break;
		}
	}

	return valRet;
}





//============================================================ PRIVATE FUNCTIONS
static uint8_t AsciiHexToByte(char c)
{
	char carattere = toupper(c);


	if((carattere>='A') && (carattere<='F'))
		return 0x0a+carattere-'A';
	else if((carattere>='0') && (carattere<='9'))
		return carattere-'0';

	return 0;
}

//______________________________________________________________________________
// Desc:  Estrapola l'HexRecord dall'handle file passato
// Arg: - ptrHandleFile: handle del file in corso
//		  ptrHexRecord: ptr al HexRecord da compilare
// Ret: - il CRC32 o 0 se errore
//______________________________________________________________________________
static int32_t CompilaHexRecordFromHandleFile(drvfilesystem_HandleFile_t *ptrHandleFile,fwupgrade_HexRecord_t *ptrHexRecord)
{
	uint8_t tmpBuffer[32];
	int32_t valRet = _STATUS_OK_;
	uint32_t uLA;

	if(fwupgrade_Init.PtrDrvfilesystem->FRead(ptrHandleFile,tmpBuffer,9)==_STATUS_FAIL_)
		valRet = _STATUS_FAIL_;

	//________________________________________________________________Start code
	if(tmpBuffer[0]!=':')	// Il primo byte DEVE essere un ':'
		valRet = _STATUS_FAIL_;
	ptrHexRecord->StartCode = tmpBuffer[0];

	//________________________________________________________________Byte count
	ptrHexRecord->ByteCount = 0;
	for(uLA=0;uLA<2;uLA++)
	{
		ptrHexRecord->ByteCount<<=4;
		ptrHexRecord->ByteCount |= AsciiHexToByte(tmpBuffer[1+uLA]);
	}

	//___________________________________________________________________Address
	ptrHexRecord->Address = 0;
	for(uLA=0;uLA<4;uLA++)
	{
		ptrHexRecord->Address<<=4;
		ptrHexRecord->Address |= AsciiHexToByte(tmpBuffer[3+uLA]);
	}

	//_______________________________________________________________Record type
	ptrHexRecord->RecordType = 0;
	for(uLA=0;uLA<2;uLA++)
	{
		ptrHexRecord->RecordType<<=4;
		ptrHexRecord->RecordType |= AsciiHexToByte(tmpBuffer[7+uLA]);
	}

	//______________________________________________________________________Data
	if(ptrHexRecord->ByteCount<=sizeof(ptrHexRecord->Data))
	{
		if(fwupgrade_Init.PtrDrvfilesystem->FRead(ptrHandleFile,tmpBuffer,ptrHexRecord->ByteCount*2)==_STATUS_FAIL_)
			valRet = _STATUS_FAIL_;
		for(uLA=0;uLA<ptrHexRecord->ByteCount*2;uLA++)
		{
			ptrHexRecord->Data[uLA/2]<<=4;
			ptrHexRecord->Data[uLA/2] |= AsciiHexToByte(tmpBuffer[uLA]);
		}
	}
	else
		valRet = _STATUS_FAIL_;

	//__________________________________________________________________Checksum
	ptrHexRecord->CheckSum = 0;
	if(fwupgrade_Init.PtrDrvfilesystem->FRead(ptrHandleFile,tmpBuffer,2)==_STATUS_FAIL_)
		valRet = _STATUS_FAIL_;
	for(uLA=0;uLA<2;uLA++)
	{
		ptrHexRecord->CheckSum<<=4;
		ptrHexRecord->CheckSum |= AsciiHexToByte(tmpBuffer[uLA]);
	}

	//_____________________Verifico la sincronizzazione di fine riga (0x0d,0x0a)
	if(fwupgrade_Init.PtrDrvfilesystem->FRead(ptrHandleFile,tmpBuffer,1)==_STATUS_FAIL_)
		valRet = _STATUS_FAIL_;
	if(tmpBuffer[0]==0x0a)
	{	// Linux merda mode: 0x0d}

	}
	else
	{	// Resto del mondo: 0x0d 0x0a
		if(tmpBuffer[0]!=0x0d)
			valRet = _STATUS_FAIL_;
		if(fwupgrade_Init.PtrDrvfilesystem->FRead(ptrHandleFile,tmpBuffer,1)==_STATUS_FAIL_)
			valRet = _STATUS_FAIL_;
		if(tmpBuffer[0]!=0x0a)
			valRet = _STATUS_FAIL_;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc: Cerca su USB il file passato incluso di estensione e aggiorna il firmware
			 di brutto
 Arg: - strNomeFile: Nome del file es "Benito.bin"
 Ret: - enum del tipo fwupgrade_Status_e
______________________________________________________________________________*/
static fwupgrade_Status_e ShootFileBin(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato)
{
	uint8_t tmpBuffer[32];
	fwupgrade_Status_e valRet = FWUPGRADE_STATUS_USBNONRILEVATA;
	uint32_t sizeData;
	uint32_t dimData,offset;
	drvfilesystem_HandleFile_t handleFile;
	uint32_t uLA;
	bool errore = false;
	uint16_t crc16=0;
	uint32_t counterAggiornamento=0;
	uint64_t percentuale;
	uint32_t sizeFile;

	// Un minimo di controllo
	if(ptrInfoFileCodificato->TipoFile == FWUPGRADE_TIPOFILE_BIN)
	{
		if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
		{	// File aperto con successo e quindi procedi con aggiornamento firmware
			fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILEINVERIFICA;
			fwupgrade_Info.DecPercentualeEsecuzione = 0;
			// Eventualmente utile per aggiornare informazioni su display
			if(fwupgrade_Init.PtrFunFunInfoStatus)
				fwupgrade_Init.PtrFunFunInfoStatus(false);


			fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
			sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
			sizeData = sizeFile-0x200;

			fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x200,DRVFILESYSTEM_SEEK_SET);
			// Verifico la validità della checksum
			for(uLA=0;uLA<sizeData;uLA+=32)
			{
				if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,32)!=_STATUS_FAIL_)
				{
					DeCodificaBuffer32(tmpBuffer);
					crc16 = crc_CalcCrc16(tmpBuffer, 32,crc16^0xffff);	// Crc ModBus
				}
				else
					errore = true;
			}
			if(!errore && (crc16==ptrInfoFileCodificato->Crc16Originale) && (ptrInfoFileCodificato->TotalSizeAlign32==sizeData))
			{
				fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO;
				fwupgrade_Info.DecPercentualeEsecuzione = 0;
				// Eventualmente utile per aggiornare informazioni su display
				if(fwupgrade_Init.PtrFunFunInfoStatus)
					fwupgrade_Init.PtrFunFunInfoStatus(false);

				if(fwupgrade_Init.PtrDrvFlashDriver->EraseMultipleSector(fwupgrade_Init.AddressFirmware,ptrInfoFileCodificato->TotalSizeAlign32)==_STATUS_OK_)
				{
					// OCCHIO: sizeFile cambia di significato nel loop a seguire...
					fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x200,DRVFILESYSTEM_SEEK_SET);
					offset = 0;
					while(sizeData)
					{
						dimData = _MIN_(sizeData,sizeof(tmpBuffer));
						fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,dimData);
						DeCodificaBuffer32(tmpBuffer);
						if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(fwupgrade_Init.AddressFirmware+offset,tmpBuffer,dimData)==_STATUS_FAIL_)
						{
							fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
							break;
						}
						offset+=dimData;
						sizeData-=dimData;

						counterAggiornamento++;
						if(counterAggiornamento>1000)
						{	// Limito la frequenza di aggiornamento display
							counterAggiornamento = 0;
							percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
							percentuale = (percentuale*1000)/sizeFile;
							fwupgrade_Info.DecPercentualeEsecuzione = percentuale;

							// Eventualmente utile per aggiornare informazioni su display
							if(fwupgrade_Init.PtrFunFunInfoStatus)
								fwupgrade_Init.PtrFunFunInfoStatus(false);
						}
					}

					if(!sizeData)
					{
						// Alla fine copia l'intestazione di 512 del file all'inizio della zona firmware
						fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_SET);

						for(uLA=0;uLA<fwupgrade_Init.DimInfoHeader;uLA+=32)
						{
							if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,32)!=_STATUS_FAIL_)
							{
								if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(fwupgrade_Init.AddressInfoHeader+uLA,tmpBuffer,32)!=_STATUS_OK_)
									fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
							}
						}

						if(valRet != FWUPGRADE_STATUS_FIRMWAREERRFLASH)
							fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREAGGIORNATO;
					}
				}
				else
					fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
			}
			else
				fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;

			fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
		}
		else
		{
			fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONRILEVATO;
		}
	}

	return valRet;
}

//______________________________________________________________________________
// Desc: Cerca su USB il file passato incluso di estenzione e aggiorna il firmware di brutto.
// 	 Nel caso del formato Hex la posizione in flash è dettata dal file stesso ma in
// 	 goni caso la zona flash del bootloader rimane intatta.
//
// Arg: - strNomeFile: Nome del file es "Benito.hex"
// Ret: - enum del tipo fwupgrade_Status_e
//______________________________________________________________________________
static fwupgrade_Status_e ShootFileHex(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato)
{
	fwupgrade_Status_e valRet = FWUPGRADE_STATUS_USBNONRILEVATA;
	uint32_t sizeFile;
	uint32_t uLA,uLB;
	fwupgrade_HexRecord_t hexRecord;
	bool errore = false;
	uint32_t address=0;
	uint32_t lastNumSectorErased=0xffffffff;
	uint64_t percentuale;
	bool fineCiclo=false;
	uint8_t tmpBuffer[32];
	uint32_t indiceWrTmpBuffer=0;
	uint32_t lastWriteAddress=0;
	uint32_t counterAggiornamento = 0;
	drvfilesystem_HandleFile_t handleFile;

	if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	{	// File aperto con successo e quindi procedi con aggiornamento firmware

		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
		sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);

		fwupgrade_Info.DecPercentualeEsecuzione = 0;
		fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO;
		if(sizeFile>0x200)
		{
			// Eventualmente utile per aggiornare informazioni su display
			if(fwupgrade_Init.PtrFunFunInfoStatus)
				fwupgrade_Init.PtrFunFunInfoStatus(false);

			//_______Si parte con il memorizzare i primi H200 byte così come sono...
			fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_SET);
			lastWriteAddress = fwupgrade_Init.AddressFirmware-0x200;
			lastNumSectorErased = fwupgrade_Init.PtrDrvFlashDriver->GetNumSectorFromAddress(lastWriteAddress);
			if(fwupgrade_Init.PtrDrvFlashDriver->EraseSector(lastNumSectorErased)==_STATUS_OK_)
			{
				for(uLA = 0; uLA < 0x200;uLA+=32)
				{   			
					if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile, tmpBuffer, 32) == _STATUS_FAIL_)
					{
						fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
						errore = true;
					}
					else
					{
						if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(lastWriteAddress, tmpBuffer, 32) != _STATUS_OK_)
						{
							fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
							errore = true;
							break;
						}
					}
					lastWriteAddress+=32;
				}
				
				if(!errore)
				{
					// Partendo dall'inizio del file (intestazione da 512 byte esclusa) analizzo riga per riga il file
					fineCiclo=false;
					address=0;
					indiceWrTmpBuffer=0;
					lastWriteAddress=0;
					while(!fineCiclo)
					{
						if(CompilaHexRecordFromHandleFile(&handleFile,&hexRecord)==_STATUS_OK_)
						{
							switch(hexRecord.RecordType)
							{
								case 0:	// Data
									address&=0xffff0000;
									address|=hexRecord.Address;
		
									uLA = 0;
		
									if(!lastWriteAddress)
										lastWriteAddress = address;
		
									while(hexRecord.ByteCount--)
									{
										tmpBuffer[indiceWrTmpBuffer++] = hexRecord.Data[uLA++];
										address++;
										if(indiceWrTmpBuffer>=sizeof(tmpBuffer))
										{	// tmpBuffer pieno lo decodifico e lo storo in flash
											DeCodificaBuffer32(tmpBuffer);
		
											uLB = fwupgrade_Init.PtrDrvFlashDriver->GetNumSectorFromAddress(lastWriteAddress);
		
											if(lastNumSectorErased!=uLB)
											{	// Erase del blocco falsh solo se necessario
												lastNumSectorErased=uLB;
		
												if(fwupgrade_Init.PtrDrvFlashDriver->EraseSector(uLB)!=_STATUS_OK_)
												{
													fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
													errore = true;
												}
											}
		
											if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(lastWriteAddress,tmpBuffer,indiceWrTmpBuffer)!=_STATUS_OK_)
											{
												fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
												errore = true;
											}
		
											lastWriteAddress+=indiceWrTmpBuffer;
		
											indiceWrTmpBuffer = 0;
											memset(tmpBuffer,0,sizeof(tmpBuffer));
										}
									}
								break;
								case 1: // End of file
									// Se è rimasto fuori qualcosa da scrivere in flash...
									if(indiceWrTmpBuffer)
									{	// tmpBuffer pieno lo decodifico e lo storo in flash
										DeCodificaBuffer32(tmpBuffer);
		
										uLB = fwupgrade_Init.PtrDrvFlashDriver->GetNumSectorFromAddress(lastWriteAddress);
		
										if(lastNumSectorErased!=uLB)
										{	// Erase del blocco falsh solo se necessario
											lastNumSectorErased=uLB;
		
											if(fwupgrade_Init.PtrDrvFlashDriver->EraseSector(uLB)!=_STATUS_OK_)
											{
												fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
												errore = true;
											}
										}
		
										if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(lastWriteAddress,tmpBuffer,indiceWrTmpBuffer)!=_STATUS_OK_)
										{
											fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
											errore = true;
										}
		
										address+=indiceWrTmpBuffer;
										indiceWrTmpBuffer+=indiceWrTmpBuffer;
		
										indiceWrTmpBuffer = 0;
										memset(tmpBuffer,0,sizeof(tmpBuffer));
									}
		
									if(!errore)
										fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREAGGIORNATO;
		
									fineCiclo = true;
								break;
								case 2:	// Extended segment address
								break;
								case 3:	// Start segment address
								break;
								case 4:	// Extended linear address
									// Se è rimasto fuori qualcosa da scrivere in flash...
									if(indiceWrTmpBuffer)
									{	// tmpBuffer pieno lo decodifico e lo storo in flash
										DeCodificaBuffer32(tmpBuffer);
		
										uLB = fwupgrade_Init.PtrDrvFlashDriver->GetNumSectorFromAddress(lastWriteAddress);
		
										if(lastNumSectorErased!=uLB)
										{
											lastNumSectorErased=uLB;
		
											if(fwupgrade_Init.PtrDrvFlashDriver->EraseSector(uLB)!=_STATUS_OK_)
											{
												fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
												errore = true;
											}
										}
		
										if(fwupgrade_Init.PtrDrvFlashDriver->WriteData(lastWriteAddress,tmpBuffer,indiceWrTmpBuffer)!=_STATUS_OK_)
										{
											fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
											errore = true;
										}
		
										lastWriteAddress += indiceWrTmpBuffer;
										address+=indiceWrTmpBuffer;
									}
		
									if(hexRecord.Address==0)
									{
										uLA = hexRecord.Data[0];
										uLA<<=8;
										uLA |= hexRecord.Data[1];
		
										address = (uLA<<16);
									}
									else
									{
										fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;
										errore = true;
									}
		
									lastWriteAddress = 0;
		
									indiceWrTmpBuffer = 0;
									memset(tmpBuffer,0,sizeof(tmpBuffer));
								break;
								case 5:	// Start linear address
								break;
							}
						}	// fine if(CompilaHexRecordFromHandleFile(&handleFile,&hexRecord)==_STATUS_OK_)
						else
						{
							fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;
							errore = true;
						}
		
						counterAggiornamento++;
						if(counterAggiornamento>1000)
						{	// Limito la frequenza di aggiornamento display
							counterAggiornamento = 0;
							// Eventualmente utile per aggiornare informazioni su display
							percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
							percentuale = (percentuale*1000)/sizeFile;
							fwupgrade_Info.DecPercentualeEsecuzione = percentuale;
							if(fwupgrade_Init.PtrFunFunInfoStatus)
								fwupgrade_Init.PtrFunFunInfoStatus(false);
						}
		
						if(errore)
							break;
					}	// fine while(!fineCiclo)
				}	// fine if(!errore)
			}	// fine if(fwupgrade_Init.PtrDrvFlashDriver->EraseSector(lastNumSectorErased)==_STATUS_OK_)
			else
			{
				fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FIRMWAREERRFLASH;
				errore = true;
			}
		}	// fine if(sizeFile>0x200)
		else
			fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONVALIDO;

		fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
	}	// fine if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	else
		fwupgrade_Info.Status = valRet = FWUPGRADE_STATUS_FILENONRILEVATO;

	return valRet;
}



//______________________________________________________________________________
// Desc:	Ritorna il CRC16 reale del file
// Arg: - strNomeFile: Nome del file es "Benito.hex"
//		- ptrInfoFileCodificato: ptr alle info file
// Ret: - la CRC16
//______________________________________________________________________________
static uint16_t GetCrc16BINFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato)
{
	uint8_t tmpBuffer[32];
	uint32_t sizeData;
	uint32_t indice;
	uint16_t valRet=0;
	uint32_t counterAggiornamento=0;
	uint64_t percentuale;
	uint32_t sizeFile;
	drvfilesystem_HandleFile_t handleFile;

	if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	{	// File aperto con successo e quindi procedi con aggiornamento firmware
		fwupgrade_Info.Status = FWUPGRADE_STATUS_FILEINVERIFICA;
		fwupgrade_Info.DecPercentualeEsecuzione = 0;
		// Eventualmente utile per aggiornare informazioni su display
		if(fwupgrade_Init.PtrFunFunInfoStatus)
			fwupgrade_Init.PtrFunFunInfoStatus(false);

		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
		sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
		sizeData = sizeFile-0x200;

		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x200,DRVFILESYSTEM_SEEK_SET);
		// Verifico la validità della checksum
		for(indice=0;indice<sizeData;indice+=32)
		{
			memset(tmpBuffer,0,sizeof(tmpBuffer));
			if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,32)!=_STATUS_FAIL_)
			{
				DeCodificaBuffer32(tmpBuffer);
				valRet = crc_CalcCrc16(tmpBuffer, 32,valRet^0xffff);	// Crc ModBus
			}
			else
				break;

			counterAggiornamento++;
			if(counterAggiornamento>1000)
			{	// Limito la frequenza di aggiornamento display
				counterAggiornamento = 0;
				// Eventualmente utile per aggiornare informazioni su display
				percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
				percentuale = (percentuale*1000)/sizeFile;
				fwupgrade_Info.DecPercentualeEsecuzione = percentuale;
				if(fwupgrade_Init.PtrFunFunInfoStatus)
					fwupgrade_Init.PtrFunFunInfoStatus(false);
			}
		}

		fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
	}
	else
	{
		fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONRILEVATO;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:	Ritorna il CRC32 reale del file
// Arg: - strNomeFile: Nome del file es "Benito.hex"
//		- ptrInfoFileCodificato: ptr alle info file
// Ret: - la CRC32
//______________________________________________________________________________
static uint32_t GetCrc32BINFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato)
{
	uint8_t tmpBuffer[32];
	uint32_t sizeData;
	uint32_t indice;
	uint32_t valRet=0;
	uint32_t counterAggiornamento=0;
	uint64_t percentuale;
	uint32_t sizeFile;
	drvfilesystem_HandleFile_t handleFile;

	if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	{	// File aperto con successo e quindi procedi con aggiornamento firmware
		fwupgrade_Info.Status = FWUPGRADE_STATUS_FILEINVERIFICA;
		fwupgrade_Info.DecPercentualeEsecuzione = 0;
		// Eventualmente utile per aggiornare informazioni su display
		if(fwupgrade_Init.PtrFunFunInfoStatus)
			fwupgrade_Init.PtrFunFunInfoStatus(false);


		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
		sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
		sizeData = sizeFile-0x200;

		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x200,DRVFILESYSTEM_SEEK_SET);
		// Verifico la validità della checksum
		for(indice=0;indice<sizeData;indice+=32)
		{
			memset(tmpBuffer,0,sizeof(tmpBuffer));
			if(fwupgrade_Init.PtrDrvfilesystem->FRead(&handleFile,tmpBuffer,32)!=_STATUS_FAIL_)
			{
				DeCodificaBuffer32(tmpBuffer);
				valRet = crc_CalcCrc32(tmpBuffer, 32,valRet);
			}
			else
				break;

			counterAggiornamento++;
			if(counterAggiornamento>1000)
			{	// Limito la frequenza di aggiornamento display
				counterAggiornamento = 0;
				// Eventualmente utile per aggiornare informazioni su display
				percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
				percentuale = (percentuale*1000)/sizeFile;
				fwupgrade_Info.DecPercentualeEsecuzione = percentuale;
				if(fwupgrade_Init.PtrFunFunInfoStatus)
					fwupgrade_Init.PtrFunFunInfoStatus(false);
			}
		}

		fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
	}
	else
	{
		fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONRILEVATO;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:	Ritorna il CRC32 reale del file
// Arg: - strNomeFile: Nome del file es "Benito.hex"
//		- ptrInfoFileCodificato: ptr alle info file
// Ret: - la CRC32
//______________________________________________________________________________
static uint32_t GetCrc32HEXFileCodifiato(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato)
{
	uint32_t sizeFile;
	uint32_t uLA;
	fwupgrade_HexRecord_t hexRecord;
	bool errore = false;
	uint32_t address=0;
	uint64_t percentuale;
	bool fineCiclo=false;
	uint8_t tmpBuffer[32];
	uint32_t indiceWrTmpBuffer=0;
	uint32_t lastWriteAddress=0;
	uint32_t valRet=0;
	uint32_t counterAggiornamento = 0;
	drvfilesystem_HandleFile_t handleFile={0};

	memset(tmpBuffer,0,sizeof(tmpBuffer));

	if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	{	// File aperto con successo e quindi procedi con aggiornamento firmware

		fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_END);
		sizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);

		fwupgrade_Info.DecPercentualeEsecuzione = 0;
		if(sizeFile)
		{
			fwupgrade_Info.Status = FWUPGRADE_STATUS_FILEINVERIFICA;
			fwupgrade_Info.DecPercentualeEsecuzione = 0;
			// Eventualmente utile per aggiornare informazioni su display
			if(fwupgrade_Init.PtrFunFunInfoStatus)
				fwupgrade_Init.PtrFunFunInfoStatus(false);

			// Verifico la validatà della checksum
			fwupgrade_Init.PtrDrvfilesystem->FSeek(&handleFile,0x200,DRVFILESYSTEM_SEEK_SET);
			fineCiclo=false;
			address=0;
			indiceWrTmpBuffer=0;
			lastWriteAddress=0;
			while(!fineCiclo)
			{
				if(CompilaHexRecordFromHandleFile(&handleFile,&hexRecord)==_STATUS_OK_)
				{
					switch(hexRecord.RecordType)
					{
						case 0:	// Data
							address&=0xffff0000;
							address|=hexRecord.Address;

							// Verifico di essere in zona concessa per la programmazione
							/*niknik capire se togliere
							if( ((address>=fwupgrade_Init.AddressFirmware) && (address<(fwupgrade_Init.AddressFirmware+fwupgrade_Init.MaxDimFirmware))) ||
								((address>=fwupgrade_Init.AddressFirmwareAux) && (address<(fwupgrade_Init.AddressFirmwareAux+fwupgrade_Init.MaxDimFirmwareAux))))
								*/
							{
								uLA = 0;

								while(hexRecord.ByteCount--)
								{
									tmpBuffer[indiceWrTmpBuffer++] = hexRecord.Data[uLA++];
									address++;
									if(indiceWrTmpBuffer>=sizeof(tmpBuffer))
									{	// tmpBuffer pieno lo decodifico e lo storo in flash
										valRet = crc_CalcCrc32(tmpBuffer, indiceWrTmpBuffer, valRet); // CRC32 normale

										indiceWrTmpBuffer = 0;
										memset(tmpBuffer,0,sizeof(tmpBuffer));
									}
								}
							}
						break;
						case 1: // End of file
							// Se è rimasto fuori qualcosa da scrivere in flash...
							if(indiceWrTmpBuffer)
							{	// tmpBuffer pieno lo decodifico e lo storo in flash
								valRet = crc_CalcCrc32(tmpBuffer, indiceWrTmpBuffer, valRet); // CRC32 normale

								address+=indiceWrTmpBuffer;
								indiceWrTmpBuffer+=indiceWrTmpBuffer;

								indiceWrTmpBuffer = 0;
								memset(tmpBuffer,0,sizeof(tmpBuffer));
							}

							fineCiclo = true;
						break;
						case 2:	// Extended segment address
						break;
						case 3:	// Start segment address
						break;
						case 4:	// Extended linear address
							// Se è rimasto fuori qualcosa da scrivere in flash...
  							if(indiceWrTmpBuffer)
							{	// tmpBuffer pieno lo decodifico e lo storo in flash
								valRet = crc_CalcCrc32(tmpBuffer, indiceWrTmpBuffer, valRet); // CRC32 normale

								lastWriteAddress += indiceWrTmpBuffer;
								address+=indiceWrTmpBuffer;
							}

							if(hexRecord.Address==0)
							{
								uLA = hexRecord.Data[0];
								uLA<<=8;
								uLA |= hexRecord.Data[1];

								address = (uLA<<16);
							}
							else
							{
								fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONVALIDO;
								errore = true;
							}

							indiceWrTmpBuffer = 0;
							memset(tmpBuffer,0,sizeof(tmpBuffer));
						break;
						case 5:	// Start linear address
						break;
					}
				}	// fine if(CompilaHexRecordFromHandleFile(&handleFile,&hexRecord)==_STATUS_OK_)
				else
				{
					fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONVALIDO;
					errore = true;
				}

				counterAggiornamento++;
				if(counterAggiornamento>1000)
				{	// Limito la frequenza di aggiornamento display
					counterAggiornamento = 0;
					// Eventualmente utile per aggiornare informazioni su display
					percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&handleFile);
					percentuale = (percentuale*1000)/sizeFile;
					fwupgrade_Info.DecPercentualeEsecuzione = percentuale;
					if(fwupgrade_Init.PtrFunFunInfoStatus)
						fwupgrade_Init.PtrFunFunInfoStatus(false);
				}

				if(errore)
					break;
			}

			if(errore)
				fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONVALIDO;
		}	// fine if(sizeFile)
		else
			fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONVALIDO;

		fwupgrade_Init.PtrDrvfilesystem->FClose(&handleFile);
	}	// fine if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&handleFile,strNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
	else
		fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONRILEVATO;

	return valRet;
}

//______________________________________________________________________________
//  Desc:	Ritorna l'address di destinazione del buffer di 32byte letto dal file.
//			NOTA: il buffer è ancora codificato, ci penserà lo slave
// Arg: - ptrData32: Buffer di appoggio per i 32byte
// 	  	- virtualize: a true usa le funzioni di virtualizzazione per aggiornamento via BustStack
// Ret: - Comandi da eseguire del tipo FWUPGRADE_VUTYPEDATA_xxx
//______________________________________________________________________________
fwupgrade_VUTypeData_e fwupgrade_VUGetData(char *ptrStrNomeFile)
{
	static drvfilesystem_HandleFile_t HandleFile;
	static bool PrimoExtendedLinearAddress = true;
	fwupgrade_HexRecord_t hexRecord;
	uint32_t uLA;
	int32_t lA;
	uint64_t percentuale;
	fwupgrade_VUTypeData_e valRet = FWUPGRADE_VUTYPEDATA_VOID;

	while(valRet == FWUPGRADE_VUTYPEDATA_VOID)
	{
		switch(fwupgrade_VirtualizeUpgrade.SLVUGetData)
		{
			case FWUPGRADE_SLVUGETDATA_OPENFILEHEX:
			case FWUPGRADE_SLVUGETDATA_OPENFILEBIN:
				// NOTA: Qui si arriva quando tutti i test del file sono passati e quindi è inutile ripeterli
				// Come primo step si passano alla parti i primi H200 byate del file
				PrimoExtendedLinearAddress = true;
				if(fwupgrade_Init.PtrDrvfilesystem->FOpen(&HandleFile,ptrStrNomeFile,DRVFILESYSTEM_FOPENTYPE_READ )==_STATUS_OK_)
				{	// File aperto con successo e quindi procedi con aggiornamento firmware
					fwupgrade_Info.DecPercentualeEsecuzione = 0;
					fwupgrade_Info.Status = FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO;

					memset(&fwupgrade_VirtualizeUpgrade.Info,0,sizeof(fwupgrade_VirtualizeUpgrade.Info));

					fwupgrade_Init.PtrDrvfilesystem->FSeek(&HandleFile,0,DRVFILESYSTEM_SEEK_END);
					fwupgrade_VirtualizeUpgrade.SizeFile = fwupgrade_Init.PtrDrvfilesystem->FTell(&HandleFile);
					if(fwupgrade_VirtualizeUpgrade.SizeFile>0x200)
					{
						if(fwupgrade_Init.PtrFunFunInfoStatus)
							fwupgrade_Init.PtrFunFunInfoStatus(false);

						fwupgrade_Init.PtrDrvfilesystem->FSeek(&HandleFile,0,DRVFILESYSTEM_SEEK_SET);
						fwupgrade_Init.PtrDrvfilesystem->FRead(&HandleFile,fwupgrade_VirtualizeUpgrade.Info.Buffer4K,0x200);
					}
				}

				valRet = FWUPGRADE_VUTYPEDATA_STARTED;

				if(fwupgrade_VirtualizeUpgrade.SLVUGetData==FWUPGRADE_SLVUGETDATA_OPENFILEHEX)
					fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEHEX;
				else
					fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEBIN;
			break;
			case FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEHEX:
				if(CompilaHexRecordFromHandleFile(&HandleFile,&hexRecord)==_STATUS_OK_)
				{
					switch(hexRecord.RecordType)
					{
						case 0:	// Data
							fwupgrade_VirtualizeUpgrade.ActualAddress &= 0xffff0000;
							fwupgrade_VirtualizeUpgrade.ActualAddress |= hexRecord.Address;

							if(!fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K)
							{	// Se Buffer4K verginello
								// Segno il Start address del nuovo Buffer4K

								memset(fwupgrade_VirtualizeUpgrade.Info.Buffer4K,0,sizeof(fwupgrade_VirtualizeUpgrade.Info.Buffer4K));	//niknik Serve?
								fwupgrade_VirtualizeUpgrade.Info.StartAddressFlash = fwupgrade_VirtualizeUpgrade.ActualAddress;
							}

							memcpy(&fwupgrade_VirtualizeUpgrade.Info.Buffer4K[fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K],hexRecord.Data,hexRecord.ByteCount);

							//In ogni caso allineo a 16byte
							fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K+=16;
							fwupgrade_VirtualizeUpgrade.ActualAddress+=16;

							if(fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K>=sizeof(fwupgrade_VirtualizeUpgrade.Info.Buffer4K))
							{
								fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K = fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K;
								fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K = 0;

								valRet = FWUPGRADE_VUTYPEDATA_SENDDATA;
							}
						break;
						case 1: // End of file
							// Se rimanane da trasmettere il buffer4K lo caccio fuori
							if(fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K)
							{
								fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K = fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K;
								fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K = 0;
								valRet = FWUPGRADE_VUTYPEDATA_SENDDATA;
							}

							// Alla fine copia l'intestazione di 512 byte del file all'inizio della zona firmware
							fwupgrade_Init.PtrDrvfilesystem->FSeek(&HandleFile,0,DRVFILESYSTEM_SEEK_SET);

							fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_FIRMWAREAGGIORNATO;
						break;
						case 2:	// Extended segment address
						break;
						case 3:	// Start segment address
						break;
						case 4:	// Extended linear address
							if(hexRecord.Address==0)
							{
								uLA = hexRecord.Data[0];
								uLA<<=8;
								uLA |= hexRecord.Data[1];

								fwupgrade_VirtualizeUpgrade.ActualAddress = (uLA<<16);
								fwupgrade_VirtualizeUpgrade.Info.StartAddressFlash = fwupgrade_VirtualizeUpgrade.ActualAddress;

								if(fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K)
								{
									// Qui caccio fuori il buffer
									fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K = fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K;
									valRet = FWUPGRADE_VUTYPEDATA_SENDDATA;
								}

								if(PrimoExtendedLinearAddress)
								{
									PrimoExtendedLinearAddress = false;
									fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K = 0x200;
								}
								else
									fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K = 0;
							}
							else
							{
								valRet = FWUPGRADE_VUTYPEDATA_ANOMALIA;
								fwupgrade_VirtualizeUpgrade.ActualAddress = 0xffffffff;
							}
						break;
						case 5:	// Start linear address
						break;
					}
				}	// fine if(CompilaHexRecordFromHandleFile(&UsbHandleFile,&hexRecord)==_STATUS_OK_)
				else
				{
					fwupgrade_Info.Status = FWUPGRADE_STATUS_FILENONVALIDO;
					valRet = FWUPGRADE_VUTYPEDATA_ANOMALIA;
					fwupgrade_Init.PtrDrvfilesystem->FClose(&HandleFile);
				}
			break;
			case FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEBIN:
				// Primo step comandare un set address al 0x200
				fwupgrade_VirtualizeUpgrade.ActualAddress = 0x200;

				fwupgrade_VirtualizeUpgrade.SLVUGetData++;
				// ATTENZIONE: Volutamente senza break;
			case FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEBIN_NEXT:
				if(fwupgrade_Init.PtrDrvfilesystem->FTell(&HandleFile)>=fwupgrade_VirtualizeUpgrade.SizeFile)
				{	// Fine del file raggiunto, passo all'header

					// Alla fine copia l'intestazione di 512 byte del file all'inizio della zona firmware
					fwupgrade_Init.PtrDrvfilesystem->FSeek(&HandleFile,0,DRVFILESYSTEM_SEEK_SET);

					//da rimettere e capire e serve fwupgrade_VirtualizeUpgrade.SLVUGetData = FWUPGRADE_SLVUGETDATA_HEADERDATAFILE;
				}
				else
				{
					fwupgrade_VirtualizeUpgrade.Info.StartAddressFlash = fwupgrade_VirtualizeUpgrade.ActualAddress;

					lA = fwupgrade_Init.PtrDrvfilesystem->FRead(&HandleFile,fwupgrade_VirtualizeUpgrade.Info.Buffer4K,sizeof(fwupgrade_VirtualizeUpgrade.Info.Buffer4K));


					if(lA ==_STATUS_FAIL_)
					{
						valRet = FWUPGRADE_VUTYPEDATA_ANOMALIA;
						fwupgrade_VirtualizeUpgrade.ActualAddress = 0xffffffff;
					}
					else
					{
						fwupgrade_VirtualizeUpgrade.ActualAddress+=lA;
						fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K+=lA;
						fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K = fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K;
						fwupgrade_VirtualizeUpgrade.Info.IndiceWrBuffer4K = 0;
						valRet = FWUPGRADE_VUTYPEDATA_SENDDATA;
					}
				}
			break;

			case FWUPGRADE_SLVUGETDATA_FIRMWAREAGGIORNATO:
				fwupgrade_Info.Status = FWUPGRADE_STATUS_FIRMWAREAGGIORNATO;
				valRet = FWUPGRADE_VUTYPEDATA_END;
			break;

		}	// fine switch(VirtualizeUpgrade.SLVUGetData)
	}	// fine while(!fineCiclo)

	// Eventualmente utile per aggiornare informazioni su display
	percentuale = fwupgrade_Init.PtrDrvfilesystem->FTell(&HandleFile);
	percentuale = (percentuale*1000)/fwupgrade_VirtualizeUpgrade.SizeFile;
	fwupgrade_Info.DecPercentualeEsecuzione = percentuale;
	if(fwupgrade_Init.PtrFunFunInfoStatus)
		fwupgrade_Init.PtrFunFunInfoStatus(false);

	return valRet;
}


//______________________________________________________________________________
// Desc: DeCodifica il buffer di dimensione 32 byte passato
//
// Arg: - ptrBuffer32: ptr al buffer dimensionato a 32 byte
// Ret: - void
//______________________________________________________________________________
static void DeCodificaBuffer32(uint8_t *ptrBuffer32)
{
	uint8_t indiceRd;
	uint8_t tmpBuffer[32];
	uint8_t indiceTabella;

	memcpy(tmpBuffer,ptrBuffer32,32);

	indiceTabella = 0;
	for (indiceRd=0;indiceRd< 32;indiceRd++)
	{
		ptrBuffer32[fwupgrade_Init.PtrTabellaTranscodifica[indiceTabella]] = tmpBuffer[indiceRd];

		indiceTabella++;
		indiceTabella&=0x1f; // Limito 0..31
	}
}


