/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Layer file system per slegarsi dal micro e dalle librerie di base a cui ci si
	appoggia. La libreria è operativa sia con RTOS2 che senza. Per rendere operativo
	il FileSystem richiamare _drvfilesystemusb_Manager() anche con cadenza bassa e irregolare,
	NON è bloccante.

	ATTENZIONE ATTENZIONE: - Sul CubeMX abilitare USE_FIND della FATFS
						   - Sul CubeMX disabilitare USE_MKFS della FATFS (per liberare flash)
						   - Se si vuole usare il long file name abilitare USE_LFN sul CubeMX.
						   - Se si usa il RTOS settare USBH_PROCESS_STACK_SIZE ad almeno 512 word.



	Micro supportati:
		Tutta la serie STM32 con le HAL library e MXCUBE
		* Per la USB:
			- STM32H7xx		>>> Necessarie libreria HAL e MXCUBE con o senza RTOS2
 
 
	Esempio utilizzo con Usb:
 
		//--------------Init
		if(_drvfilesystemusb_InitModule() != _STATUS_OK_)
			erroriInitModuleRilevati++; 

 
		//--------------------------------------------------------Uso per USb in FAT
		drvfilesystem_HandleFile_t usbHandleFile;
		bool oneShotWrite=true;
		char tmpBuffer[10];
		uint32_t ula;
		while(1)
		{
			_drvfilesystemusb_Manager();
 
			if(_drvfilesystemusb_Driver.PeripheralsStatus()==DRVFILESYSTEM_PERIPHERALSSTATUS_READY)
			{	// Rilevata l'USB, opero....
				if(oneShotWrite)
				{
					oneShotWrite = false;
					
					if(_drvfilesystemusb_Driver.FOpen(&usbHandleFile,"Prova.txt",DRVFILESYSTEM_FOPENTYPE_APPEND )==_STATUS_OK_)
					{
						_drvfilesystemusb_Driver.FSeek(&usbHandleFile,0,DRVFILESYSTEM_SEEK_SET);

						_drvfilesystemusb_Driver.FRead(&usbHandleFile, tmpBuffer,sizeof(tmpBuffer));

						_drvfilesystemusb_Driver.FSeek(&usbHandleFile,0,DRVFILESYSTEM_SEEK_END);
						
						strcpy(tmpBuffer,"Testo");
						_drvfilesystemusb_Driver.FWrite(&usbHandleFile,tmpBuffer,strlen(tmpBuffer));
						
						ula = _drvfilesystemusb_Driver.FTell(&usbHandleFile);
						
						sprintf(tmpBuffer,"=%d",ula);
						
						_drvfilesystemusb_Driver.FPrintf(&usbHandleFile,"Posizione %d\r\n",ula);
						
						_drvfilesystemusb_Driver.FClose(&usbHandleFile);
					}
					
					drvfilesystem_FFindInfo_t usbFFindInfo;
					bool start = true;	// <<<< ATTENZIONE!!!! NECESSARIO!!!!
					char *ptrStrFile;
					do
					{
						ptrStrFile = _drvfilesystemusb_Driver.FFind(start,_NULL_,"mio*.txt");
						start=false;
					}while(ptrStrFile);

				}
			}
		}
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/FileSystem/_DrvFileSystemUSB.h"
#include "stdarg.h"
#include "FatFs.h"
#include "usb_host.h"
#include "ff.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES
extern ApplicationTypeDef Appli_state;
static FATFS FatFs;
static DIR Dir;         	// Directory search object
static FILINFO FilInfo;    // File information

//============================================================= GLOBAL VARIABLES
_drvfilesystemusb_Init_t _drvfilesystemusb_Init;

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Descrizione della funzione.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvfilesystemusb_InitModule(void)
{
	bool errore=false;

	_drvfilesystemusb_Driver.Info.PeripheralsStatus = DRVFILESYSTEM_PERIPHERALSSTATUS_NOTCONFIGURED;
	_drvfilesystemusb_Driver.Info.Mounted = false;
	_drvfilesystemusb_Driver.Info.NumFileOpened = 0;

	if(errore)
	{
		_drvfilesystemusb_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		_drvfilesystemusb_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

/*______________________________________________________________________________ 
 Desc:  Da eseguire in loop o su task, è l'effettivo manager del modulo
 Arg: - Nessun argomento.
______________________________________________________________________________*/
void _drvfilesystemusb_Manager(void)
{
	drvfilesystem_PeripheralsStatus_e tmpPeriperalsStatus=DRVFILESYSTEM_PERIPHERALSSTATUS_DISCONNECT;

#if !USBH_USE_OS
	MX_USB_HOST_Process();
#endif

	switch(Appli_state)	// Appli_state è una variabile di libreria HAL, quindi si chiuda un occhio...
	{
		case APPLICATION_DISCONNECT:
			tmpPeriperalsStatus = DRVFILESYSTEM_PERIPHERALSSTATUS_DISCONNECT;
		break;

		case APPLICATION_READY:
			tmpPeriperalsStatus = DRVFILESYSTEM_PERIPHERALSSTATUS_READY;
		break;

		case APPLICATION_START:
			tmpPeriperalsStatus = DRVFILESYSTEM_PERIPHERALSSTATUS_STARTED;
		break;

		default:
		break;
	}
	if(_drvfilesystemusb_Driver.Info.PeripheralsStatus != tmpPeriperalsStatus)
	{
		// Se chiavetta connessa la monto
		switch((uint8_t)tmpPeriperalsStatus)
		{
			case DRVFILESYSTEM_PERIPHERALSSTATUS_DISCONNECT:
				_drvfilesystemusb_Driver.Info.Mounted = false;
				//	f_mount(NULL,"",0);
				break;

			case DRVFILESYSTEM_PERIPHERALSSTATUS_READY:
				if(f_mount(&FatFs, "", 0)==FR_OK)
					_drvfilesystemusb_Driver.Info.Mounted = true;
			break;
		}

		_drvfilesystemusb_Driver.Info.PeripheralsStatus = tmpPeriperalsStatus;
	}
}


/*______________________________________________________________________________
 Desc:  DeInit del modulo (In particole unmount della chiavetta
 Arg: - Nessun argomento.
______________________________________________________________________________*/
void _drvfilesystemusb_DeInitModule(void)
{
	f_mount(NULL,"",0);
}


//============================================================ PRIVATE FUNCTIONS

//_________________________________________________________________Primitive USB
//_________________________________________________________________Primitive USB
//_________________________________________________________________Primitive USB


/*______________________________________________________________________________ 
 Desc:  Per la USB ritorna lo stato della periferica
 Arg: - Nessun argomento.
 Ret: - drvfilesystem_PeripheralsStatus_e type
______________________________________________________________________________*/
static drvfilesystem_PeripheralsStatus_e Usb_PeripheralsStatus(void)
{
	return _drvfilesystemusb_Driver.Info.PeripheralsStatus;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB apre il file specificato 
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
  		-	fileName: Nome del file senza specificare l'unit�
  		. fOpenType: Tipo apertura file
 Ret: - _STATUS_OK_ Se ttok, _STATUS_FAIL_ altrimenti
______________________________________________________________________________*/
static int32_t Usb_FOpen(drvfilesystem_HandleFile_t *ptrHandleFile, const char *fileName, drvfilesystem_FOpenType_e fOpenType)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		switch(fOpenType)
		{
			case DRVFILESYSTEM_FOPENTYPE_READ:
				if(f_open(ptrHandleFile, fileName, FA_READ)==FR_OK)
					valRet = _STATUS_OK_;
			break;
			case DRVFILESYSTEM_FOPENTYPE_WRITE:
				if(f_open(ptrHandleFile, fileName, FA_CREATE_ALWAYS|FA_WRITE)==FR_OK)
					valRet = _STATUS_OK_;
			break;
			case DRVFILESYSTEM_FOPENTYPE_APPEND:
				if(f_open(ptrHandleFile, fileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ) == FR_OK)
					valRet = _STATUS_OK_;
			break;
		}
	}

	if(valRet == _STATUS_OK_)
		_drvfilesystemusb_Driver.Info.NumFileOpened++;

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB chiude il file specificato 
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
 Ret: - _STATUS_OK_ Se ttok, _STATUS_FAIL_ altrimenti
______________________________________________________________________________*/
static int32_t Usb_FClose(drvfilesystem_HandleFile_t *ptrHandleFile)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		if(_drvfilesystemusb_Driver.Info.NumFileOpened)
			_drvfilesystemusb_Driver.Info.NumFileOpened--;

		if(f_close(ptrHandleFile) == FR_OK)
			valRet = _STATUS_OK_;
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB legge size dati passati sul ptrData
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
  		-	ptrData: Buffer dove storare i dati letti
  		- size: Dimensione massima dei dati da leggere
 Ret: - il numero di byte letti, altrimenti _STATUS_FAIL_ se rilevato errore
______________________________________________________________________________*/
static int32_t Usb_FRead(drvfilesystem_HandleFile_t *ptrHandleFile,void *ptrData,uint32_t size)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		uint32_t datiLetti;
		if(f_read(ptrHandleFile, ptrData, size, (UINT*)&datiLetti) == FR_OK)
			valRet = datiLetti;
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB scrive size dati passati nel ptrData
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
			-	ptrData: Buffer di origine dati
			- size: Dimensione massima dei dati da scrivere
 Ret: - _STATUS_OK_ Se ttok, _STATUS_FAIL_ altrimenti
______________________________________________________________________________*/
static int32_t Usb_FWrite(drvfilesystem_HandleFile_t *ptrHandleFile, void *ptrData, uint32_t size)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		uint32_t datiScritti;
		if(f_write(ptrHandleFile, ptrData, size, (UINT*)&datiScritti) == FR_OK)
		{
			if(datiScritti==size)
				valRet = _STATUS_OK_;
		}
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB scrittura su file con logica printf
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
			-	format: tipica stringa printf (e. "test: val = %i fval =%f\n")
			- ... argomenti della printf
 Ret: - _STATUS_OK_ Se ttok, _STATUS_FAIL_ altrimenti
______________________________________________________________________________*/
static int32_t Usb_FPrintf(drvfilesystem_HandleFile_t *ptrHandleFile, const char *format,...)
{
	int32_t valRet = _STATUS_FAIL_;
	va_list argomenti;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		va_start(argomenti, format);
		uint32_t numChar = vsnprintf(_NULL_, 0, format, argomenti);
		va_end(argomenti);

		char tmpBuffer[numChar+1]; 

		va_start(argomenti, format);
		vsprintf(tmpBuffer, format, argomenti);
		va_end(argomenti); 
		
		Usb_FWrite(ptrHandleFile, tmpBuffer, numChar);
	}

	return valRet;
}



/*______________________________________________________________________________ 
 Desc:  Per la USB muove il read/write pointer nel file
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
			-	offset: simge offset dalla posizione dell'argomento seek
			- seek: enum del tipo drvfilesystem_Seek_e per il punto di riferimento
 Ret: - _STATUS_OK_ Se ttok, _STATUS_FAIL_ altrimenti
______________________________________________________________________________*/
static int32_t Usb_FSeek(drvfilesystem_HandleFile_t *ptrHandleFile, int32_t offset, drvfilesystem_Seek_e seek)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		switch(seek)
		{
			case DRVFILESYSTEM_SEEK_CUR:
				if(f_lseek(ptrHandleFile, f_tell(ptrHandleFile) + offset) == FR_OK)
					valRet = _STATUS_OK_; 
			break;
			case DRVFILESYSTEM_SEEK_END:
				if(f_lseek(ptrHandleFile, f_size(ptrHandleFile)+offset) == FR_OK)
					valRet = _STATUS_OK_;
			break;
			case DRVFILESYSTEM_SEEK_SET:
				if(f_lseek(ptrHandleFile, offset) == FR_OK)
					valRet = _STATUS_OK_;
			break;
		}
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB ritorna la posiziona attuale all'interno del file
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
 Ret: - indice della posiziona attuale, _STATUS_FAIL_ se errore generico rilevato
______________________________________________________________________________*/
static int32_t Usb_FTell(drvfilesystem_HandleFile_t *ptrHandleFile)
{
	int32_t valRet = _STATUS_FAIL_;

	if((ptrHandleFile != _NULL_) && _drvfilesystemusb_Init.InitOk)
	{
		valRet = f_tell(ptrHandleFile);
	}

	return valRet;
}

/*______________________________________________________________________________ 
 Desc:  Per la USB ritorna ricorsivamente la ricerca dl match voluto
 Arg: - ptrHandleFile: Handle del file del tipo drvfilesystem_HandleFile_t*
  		-	ptrPath: path di ricerca del pattern, se root passare NULL, "" o "\\"
  		- ptrPattern: pattern di ricerca. es. "*.jpg" , "abc*.*"
 Ret: - indice della posiziona attuale, _STATUS_FAIL_ se errore generico rilevato
______________________________________________________________________________*/
static char* Usb_FFind(bool startFind, const char *ptrPath, const char *ptrPattern)
{
	char *valRet=_NULL_;

	if(_drvfilesystemusb_Init.InitOk)
	{
		char ptrStrRoot[] = "\\";
		char *ptrPathValida = (char*)ptrPath;
		if(!ptrPath)
			ptrPathValida = ptrStrRoot;

		if(startFind)
		{
			if(f_findfirst(&Dir, &FilInfo, ptrPathValida, ptrPattern)==FR_OK)
			{
				if(FilInfo.fname[0])
					valRet = (char*)FilInfo.fname;
			}
		}
		else
		{
			if(f_findnext(&Dir, &FilInfo)==FR_OK)
			{
				if(FilInfo.fname[0])
					valRet = (char*)FilInfo.fname;
			}
		}
	}

	return valRet;
}

drvfilesystem_Driver_t _drvfilesystemusb_Driver=
{
	.PeripheralsStatus 	= Usb_PeripheralsStatus		,
	.FOpen				= Usb_FOpen					,
	.FClose				= Usb_FClose				,
	.FRead				= Usb_FRead					,
	.FWrite				= Usb_FWrite				,
	.FPrintf			= Usb_FPrintf				,
	.FSeek				= Usb_FSeek					,
	.FTell				= Usb_FTell					,
	.FFind				= Usb_FFind					,
};


