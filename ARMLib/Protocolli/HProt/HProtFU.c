/*------------------------------------------------------------------------------
 Autore:	Nikis
 Descrizione:
	Funzionalità di supporto al protocollo HProt. VEngono messe a disposizioni
	le funzionalità minime necessarie per aggiotnamento del firmware.
	HProt e swtimer inizializzati necessari.

------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Protocolli/HProt/HProtFU.h"
#include "ARMLib/Protocolli/HProt/HProt.h"
#include "ARMLib/Protocolli/CanBusStack/CanBusStack.h"
#include "ARMLib/Common/FWUpgrade.h"
#include "String.h"
#include "ARMLib/Common/SwTimer.h"


//====================================================================== DEFINES
typedef struct
{
	uint32_t CodiceSchedaEstesoBootLoaderCompatibility;
	uint32_t CodiceSchedaEsteso;
	uint16_t FirmwareVer;
	uint32_t FirmwareCrc32;
	uint32_t FirmwareData;
	uint32_t CodiceUnivocoScheda;
}hprotfu_SlaveData_t;

//_________________________________________________________________GetInfoNetCAN

//_________________________________________________________________ExecuteReboot

//______________________________________________________________AggiornaFirmware



//=========================================================== PRIVATE PROTOTYPES
static bool ManagerGetInfoNetCAN();
static bool ManagerExecuteReboot();
static bool ManagerAggiornaFirmware();
static bool ManagerCoreDestroy();
static uint8_t GetAddressFromCodiceUnivocoScheda(uint32_t codiceUnivocoScheda);

//============================================================= STATIC VARIABLES
static swtimer_TimerHandler_t HTimerGetInfo;
static hprotfu_SlaveData_t SlaveData;


//============================================================= GLOBAL VARIABLES
hprotfu_Init_t hprotfu_Init;
hprotfu_Info_t hprotfu_Info;

hprotfu_InfoManager_t hprotfu_InfoManager;

const hprot_StructData_t hprotfu_HProtStructData[]=
{
	{HPROT_IDSTD_R_CODICESCHEDAESTESOBOOTLOADERCOMPATIBILITY,			&SlaveData.CodiceSchedaEstesoBootLoaderCompatibility,	_TYPEDATA_UINT32_,		sizeof(SlaveData.CodiceSchedaEstesoBootLoaderCompatibility),		_NULL_,						0},
	{HPROT_IDSTD_R_CODICESCHEDAESTESO,									&SlaveData.CodiceSchedaEsteso,							_TYPEDATA_UINT32_,		sizeof(SlaveData.CodiceSchedaEsteso),								_NULL_,						0},
	{HPROT_IDSTD_R_FIRMWAREVER,											&SlaveData.FirmwareVer,									_TYPEDATA_UINT16_,		sizeof(SlaveData.FirmwareVer),										_NULL_,						0},
	{HPROT_IDSTD_R_FIRMWARECRC32,										&SlaveData.FirmwareCrc32,								_TYPEDATA_UINT32_,		sizeof(SlaveData.FirmwareCrc32),									_NULL_,						0},
	{HPROT_IDSTD_R_CODICEUNIVOCOSCHEDA,									&SlaveData.CodiceUnivocoScheda,							_TYPEDATA_UINT32_,		sizeof(SlaveData.CodiceUnivocoScheda),								_NULL_,						0},
	{HPROT_IDSTD_R_FIRMWAREDATA,										&SlaveData.FirmwareData,								_TYPEDATA_UINT32_,		sizeof(SlaveData.FirmwareData),										_NULL_,						0},
//	{HPROT_IDSTD_RW_WRFLASH_MODPERMISSION,		&SlaveData.WrFlash.ModPermission
	//{3000,								0xc0000000,					_TYPEDATA_ARRAYOFUINT8_,800*480*2,					_NULL_,						HPROT_PERMISSION_NOWR},
	{_NULL_},
};


//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t hprotfu_InitModule(void)
{
	bool errore=false;

	if((!hprot_Init.InitOk) || (!swtimer_Init.InitOk))
		errore = true;

	memset(&hprotfu_InfoManager,0,sizeof(hprotfu_InfoManager));

	HTimerGetInfo = swtimer_Create();

	hprotfu_InfoManager.ExeceuteReboot.HTimer = swtimer_Create();
	hprotfu_InfoManager.AggiornaFirmware.HTimer = swtimer_Create();
	hprotfu_InfoManager.GetInfoNetCAN.HTimer = swtimer_Create();
	hprotfu_InfoManager.CoreDestroy.HTimer = swtimer_Create();

	if(hprotfu_Init.PtrHProtObjRef==_NULL_)
		errore = true;

	if(errore)
	{
		hprotfu_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		hprotfu_Init.InitOk = true;
		return _STATUS_OK_;
	}
}


//______________________________________________________________________________
// Desc:  Manager del modulo da eseguire anche con bassa priotità lato server
// Arg: - Nessun argomento.
// Ret: - true se operazioni in corso
//______________________________________________________________________________
bool hprotfu_ManagerGateway(void)
{
	bool (*ptrFunManagerToExecute)(void)=_NULL_;
	bool valRet = true;
	static uint32_t oldAggiornamentiInfoBoard = 0xaa55aa55;	// Solo per la prima generata
	static bool flgReqGetInfoInCorso = false;
	uint8_t presenti = 0;
	uint32_t uLA;

	if(oldAggiornamentiInfoBoard != canbusstack_Info.AggiornamentiInfoBoard)
	{
		oldAggiornamentiInfoBoard = canbusstack_Info.AggiornamentiInfoBoard;
		for(uLA=0;uLA<HPROTFU_MAXNUMADDRESS;uLA++)
		{
			hprotfu_Info.InfoBoard[uLA].Address=uLA;
			if(canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda!=0)
			{
				// NOTA: Memorizzo subito il codice univoco sul hprotfu visto che ce l'ho e potrebbe essere utile
				// per le varie funzionalità di base del hprotfu stesso. Di fatto il codice univoco scheda è la base
				// di partenza per ogni funzionalità
				hprotfu_Info.InfoBoard[uLA].Presente = true;
				hprotfu_Info.InfoBoard[uLA].CodiceUnivocoScheda = canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda;

				if(uLA!=1)	// Gateway escluso
					presenti++;
			}
			else
			{
				hprotfu_Info.InfoBoard[uLA].Presente = false;
				hprotfu_Info.InfoBoard[uLA].CodiceUnivocoScheda = 0;
			}
		}

		flgReqGetInfoInCorso = true;
		swtimer_Set(HTimerGetInfo,3000);
	}

	if(flgReqGetInfoInCorso)
	{
		if(!swtimer_Test(HTimerGetInfo))
		{
			flgReqGetInfoInCorso = false;
			hprotfu_GetInfoNetCAN(HPROTFU_CMD_START);
		}
	}

	// Becca una Managerxxx in corso
	if(hprotfu_InfoManager.GetInfoNetCAN.StatusLoop!=HPROTFU_SLGETINFONETCAN_VOID)
		ptrFunManagerToExecute = ManagerGetInfoNetCAN;
	else if(hprotfu_InfoManager.ExeceuteReboot.StatusLoop!=HPROTFU_SLEXECUTEREBOOT_VOID)
		ptrFunManagerToExecute = ManagerExecuteReboot;
	else if(hprotfu_InfoManager.AggiornaFirmware.StatusLoop!=HPROTFU_SLAGGIORNAFIRMWARE_VOID)
		ptrFunManagerToExecute = ManagerAggiornaFirmware;
	else if(hprotfu_InfoManager.CoreDestroy.StatusLoop!=HPROTFU_SLCOREDESTROY_VOID)
		ptrFunManagerToExecute = ManagerCoreDestroy;

	if(ptrFunManagerToExecute)	// Se una mManagerxxx in corso la esegue
	{
		ptrFunManagerToExecute();
		valRet = true;
	}
	else
	{
		// Se nessuna Managerxxx in corso do l'eventuale inizio di una nuova manager
		if(ManagerGetInfoNetCAN())
		{
			valRet = true;
		}
		else if(ManagerExecuteReboot())
		{
			valRet = true;
		}
		else if(ManagerAggiornaFirmware())
		{
			valRet = true;
		}
		else if(ManagerCoreDestroy())
		{
			valRet = true;
		}
	}

	return valRet;
}


//______________________________________________________________________________
// Desc:  Ottiene informazioni sulle periferiche nella rete CAN
// Arg: - cmd: HPROTFU_CMD_START inizia, HPROTFU_CMD_GETINFO ottiene info
// Ret: - info sul comando
//______________________________________________________________________________
hprotfu_StatusCmd_e hprotfu_GetInfoNetCAN(hprotfu_Cmd_e cmd)
{
	switch(cmd)
	{
		case HPROTFU_CMD_GETINFO:
		break;
		case HPROTFU_CMD_START:
			hprotfu_InfoManager.GetInfoNetCAN.FlgStart = true;
		break;
	}

	return hprotfu_InfoManager.GetInfoNetCAN.StatusCmd;
}

//______________________________________________________________________________
// Desc:  Esegue un reboot della periferica passata
// Arg: - cmd: HPROTFU_CMD_START inizia, HPROTFU_CMD_GETINFO ottiene info
//		- codiceUnivocoScheda: Il codice univoco scheda interessata
//		- bootLoaderMode: false esegue un semplice riavvio, true mette la scheda in bootloader
// Ret: - info sul comando
//______________________________________________________________________________
hprotfu_StatusCmd_e hprotfu_ExecuteReboot(hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,bool bootLoaderMode)
{
	switch(cmd)
	{
		case HPROTFU_CMD_GETINFO:
		break;
		case HPROTFU_CMD_START:
			hprotfu_InfoManager.ExeceuteReboot.FlgStart = true;
			hprotfu_InfoManager.ExeceuteReboot.CodiceUnivocoScheda = codiceUnivocoScheda;
			hprotfu_InfoManager.ExeceuteReboot.BootLoaderMode = bootLoaderMode;
		break;
	}

	return hprotfu_InfoManager.ExeceuteReboot.StatusCmd;
}

//______________________________________________________________________________
// Desc:  GEstisce l'aggiornamente del firmware di uno slave
// Arg: - cmd: HPROTFU_CMD_START inizia, HPROTFU_CMD_GETINFO ottiene info
//		- codiceUnivocoScheda: Il codice univoco scheda interessata
//		- ptrStrnomeFile: Ptr alla stringa nome File
// Ret: - info sul comando
//______________________________________________________________________________
hprotfu_StatusCmd_e hprotfu_AggiornaFirmware( hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,uint32_t codiceEstesoScheda,char *ptrStrNomeFile)
{
	switch(cmd)
	{
		case HPROTFU_CMD_GETINFO:
		break;
		case HPROTFU_CMD_START:
			hprotfu_InfoManager.AggiornaFirmware.FlgStart = true;
			hprotfu_InfoManager.AggiornaFirmware.CodiceUnivocoScheda = codiceUnivocoScheda;
			hprotfu_InfoManager.AggiornaFirmware.PtrStrNomeFile = ptrStrNomeFile;
			hprotfu_InfoManager.AggiornaFirmware.CodiceEstesoScheda = codiceEstesoScheda;
		break;
	}

	return hprotfu_InfoManager.AggiornaFirmware.StatusCmd;
}

//______________________________________________________________________________
// Desc:  Elimina il firmware (toglie l'anima)
// Arg: - cmd: HPROTFU_CMD_START inizia, HPROTFU_CMD_GETINFO ottiene info
//		- codiceUnivocoScheda: Il codice univoco scheda interessata
//		- ptrStrnomeFile: Ptr alla stringa nome File
// Ret: - info sul comando
//______________________________________________________________________________
hprotfu_StatusCmd_e hprotfu_CoreDestroy(hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,uint32_t codiceEstesoScheda)
{
	switch(cmd)
	{
		case HPROTFU_CMD_GETINFO:
		break;
		case HPROTFU_CMD_START:
			hprotfu_InfoManager.CoreDestroy.FlgStart = true;
			hprotfu_InfoManager.CoreDestroy.CodiceUnivocoScheda = codiceUnivocoScheda;
			hprotfu_InfoManager.CoreDestroy.CodiceEstesoScheda = codiceEstesoScheda;
		break;
	}

	return hprotfu_InfoManager.AggiornaFirmware.StatusCmd;
}


//============================================================ PRIVATE FUNCTIONS

//______________________________________________________________________________
// Desc:  Gestore GetInfoNetCAN
// Arg: - Nessun argomento.
// Ret: - true se in fase di elaborazione
//______________________________________________________________________________
static bool ManagerGetInfoNetCAN()
{
	switch(hprotfu_InfoManager.GetInfoNetCAN.StatusLoop)
	{
		case HPROTFU_SLGETINFONETCAN_VOID:
			if(hprotfu_InfoManager.GetInfoNetCAN.FlgStart)
			{
				hprotfu_InfoManager.GetInfoNetCAN.FlgStart = false;
				hprotfu_InfoManager.GetInfoNetCAN.StatusCmd = HPROTFU_STATUSCMD_INESECUZIONE;
				hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso=1;
				hprotfu_InfoManager.GetInfoNetCAN.StatusLoop++;
			}
		break;

		case HPROTFU_SLGETINFONETCAN_SENDREQINFO:
			while(1)
			{
				hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso++;
				if(hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso>=HPROTFU_MAXNUMADDRESS)
				{
					hprotfu_InfoManager.GetInfoNetCAN.StatusCmd = HPROTFU_STATUSCMD_ESEGUITO;
					hprotfu_InfoManager.GetInfoNetCAN.StatusLoop = HPROTFU_SLGETINFONETCAN_VOID;
					break;	// fine degli slave presenti
				}
				else
				{
					if(hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].Presente)
					{
						hprotfu_Init.PtrHProtObjRef->PtrFunSetAddressDest(hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso);
						if(hprot_Master_PckStart(hprotfu_Init.PtrHProtObjRef)==_STATUS_OK_)
						{
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.CodiceSchedaEsteso));
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.FirmwareVer));
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.FirmwareCrc32));
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.FirmwareData));
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.CodiceUnivocoScheda));
							hprot_Master_PckAddIdGet(hprotfu_Init.PtrHProtObjRef,
																hprot_GetStructDataFromPtrVar(hprotfu_Init.PtrHProtObjRef,&SlaveData.CodiceSchedaEstesoBootLoaderCompatibility));

							hprot_Master_PckEnd(hprotfu_Init.PtrHProtObjRef);

							swtimer_Set(hprotfu_InfoManager.GetInfoNetCAN.HTimer,500);	// Setto un timeout di sicurezza

							hprotfu_InfoManager.GetInfoNetCAN.StatusLoop++;
							break;
						}
						else
						{	// Qui siamo in una condizione anomala che è il caso capire e risolvere
							hprotfu_InfoManager.GetInfoNetCAN.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
							hprotfu_InfoManager.GetInfoNetCAN.StatusLoop = HPROTFU_SLGETINFONETCAN_VOID;
							break;
						}
					}
				}
			}
		break;
		case HPROTFU_SLGETINFONETCAN_WAITRESPREQINFO:
			hprotfu_Init.PtrHProtObjRef->PtrFunSetAddressDest(hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso);
			if(swtimer_Test(hprotfu_InfoManager.GetInfoNetCAN.HTimer))
			{
				if(hprot_Master_PckIsPossibleToStart(hprotfu_Init.PtrHProtObjRef))
				{
					if(hprotfu_Init.PtrHProtObjRef->MasterModeInfo.StatusPckMaster == HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA)
					{
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].CodiceSchedaEstesoBootLoaderCompatibility	= SlaveData.CodiceSchedaEstesoBootLoaderCompatibility;
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].CodiceSchedaEsteso							= SlaveData.CodiceSchedaEsteso;
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].FirmwareCrc32 								= SlaveData.FirmwareCrc32;
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].FirmwareVer 								= SlaveData.FirmwareVer;
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].CodiceUnivocoScheda						= SlaveData.CodiceUnivocoScheda;
						hprotfu_Info.InfoBoard[hprotfu_InfoManager.GetInfoNetCAN.AddressInCorso].FirmwareData 								= SlaveData.FirmwareData;
					}

					hprotfu_InfoManager.GetInfoNetCAN.StatusLoop=HPROTFU_SLGETINFONETCAN_SENDREQINFO;
				}
			}
			else
			{
				hprotfu_InfoManager.GetInfoNetCAN.StatusLoop=HPROTFU_SLGETINFONETCAN_SENDREQINFO;
			}
		default:
		break;
	}

	if(hprotfu_InfoManager.GetInfoNetCAN.StatusLoop==HPROTFU_SLGETINFONETCAN_VOID)
		return false;
	else
		return true;
}

//______________________________________________________________________________
// Desc:  Gestore ExeceuteReboot
// Arg: - Nessun argomento.
// Ret: - true se in fase di elaborazione
//______________________________________________________________________________
static bool ManagerExecuteReboot()
{
	uint8_t address;
	bool errore = false;
	uint32_t uLA;

	switch(hprotfu_InfoManager.ExeceuteReboot.StatusLoop)
	{
		case HPROTFU_SLEXECUTEREBOOT_VOID:
			if(hprotfu_InfoManager.ExeceuteReboot.FlgStart)
			{
				hprotfu_InfoManager.ExeceuteReboot.FlgStart = false;
				hprotfu_InfoManager.ExeceuteReboot.StatusCmd = HPROTFU_STATUSCMD_INESECUZIONE;
				hprotfu_InfoManager.ExeceuteReboot.StatusLoop++;
			}
		break;
		case HPROTFU_SLEXECUTEREBOOT_SENDCMDREBOOTLOADER:
			address = GetAddressFromCodiceUnivocoScheda(hprotfu_InfoManager.ExeceuteReboot.CodiceUnivocoScheda);
			if(address)
			{
				if(hprotfu_Info.InfoBoard[address].Presente)
				{
					hprotfu_Init.PtrHProtObjRef->PtrFunSetAddressDest(address);
					if(hprot_Master_PckStart(hprotfu_Init.PtrHProtObjRef)==_STATUS_OK_)
					{
						hprot_StructData_t hProtStructData;
						memset(&hProtStructData,0,sizeof(hProtStructData));

						if(hprotfu_InfoManager.ExeceuteReboot.BootLoaderMode)
							uLA = 0x55aa9999;	// Impongo un reboot in bootloader
						else
							uLA = 0xaa556666;	// Impongo un reboot normale
						hProtStructData.Id = HPROT_IDSTD_RW_EXECUTEREBOOT;
						hProtStructData.PtrData = &uLA;
						hProtStructData.TypeData = _TYPEDATA_UINT32_;
						hProtStructData.MaxLen = sizeof(uLA);


						// NOTA: Forzando un reboot non è detto che ci sia l'ACK di conferma della
						// trasmissione perchè lo slave potrebbe essersi effettivamente resettato
						// prima di dare la risposta
						hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

						hprot_Master_PckEnd(hprotfu_Init.PtrHProtObjRef);

						swtimer_Set(hprotfu_InfoManager.ExeceuteReboot.HTimer,3333);

						hprotfu_InfoManager.ExeceuteReboot.StatusLoop++;
						break;
					}
					else
					{	// Qui siamo in una condizione anomala che è il caso capire e risolvere
						errore = true;
					}
				}
				else
					errore = true;
			}
			else
				errore = true;

			if(errore)
			{
				hprotfu_InfoManager.ExeceuteReboot.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
				hprotfu_InfoManager.ExeceuteReboot.StatusLoop = HPROTFU_SLEXECUTEREBOOT_VOID;
			}
		break;
		case HPROTFU_SLEXECUTEREBOOT_WAITREBOOTLOADER:
			if(!swtimer_Test(hprotfu_InfoManager.ExeceuteReboot.HTimer))
			{	// Dovrebbe essersi riavviato in bootloader e rimappato come address...
				hprotfu_InfoManager.ExeceuteReboot.StatusCmd = HPROTFU_STATUSCMD_ESEGUITO;
				hprotfu_InfoManager.ExeceuteReboot.StatusLoop = HPROTFU_SLEXECUTEREBOOT_VOID;
			}
		default:
		break;
	}

	if(hprotfu_InfoManager.ExeceuteReboot.StatusLoop==HPROTFU_SLEXECUTEREBOOT_VOID)
		return false;
	else
		return true;
}

//______________________________________________________________________________
// Desc:  Gestore aggiornamento firmware degli slave
// Arg: - Nessun argomento.
// Ret: - true se in fase di elaborazione
//______________________________________________________________________________
static bool ManagerAggiornaFirmware()
{
	hprot_StructData_t hProtStructData;
	fwupgrade_VUTypeData_e vuTypeData;

	switch(hprotfu_InfoManager.AggiornaFirmware.StatusLoop)
	{
		case HPROTFU_SLAGGIORNAFIRMWARE_VOID:
			if(hprotfu_InfoManager.AggiornaFirmware.FlgStart)
			{
				hprotfu_InfoManager.AggiornaFirmware.FlgStart = false;
				hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_INESECUZIONE;
				hprotfu_InfoManager.AggiornaFirmware.ModPermissionFlash = 0;
				hprotfu_InfoManager.AggiornaFirmware.StatusLoop++;

				hprotfu_ExecuteReboot(HPROTFU_CMD_START,hprotfu_InfoManager.AggiornaFirmware.CodiceUnivocoScheda,true);

				// ATTENZIONE: La chiamata alla ManagerExecuteReboot qui è una eccezione per forzare
				// l'esecuzione immediata del Reboot per scavallare la logica della ManagerGateway...
				ManagerExecuteReboot();
			}
		break;

		case HPROTFU_SLAGGIORNAFIRMWARE_ATTESAMODALITABOOTLOADER:
			switch(hprotfu_InfoManager.ExeceuteReboot.StatusCmd)
			{
				case HPROTFU_STATUSCMD_ESEGUITO:	// Bene, procedi con aggiornamento
					if(fwupgrade_CheckFileAndShoot(hprotfu_InfoManager.AggiornaFirmware.PtrStrNomeFile,true,hprotfu_InfoManager.AggiornaFirmware.CodiceEstesoScheda)!=FWUPGRADE_STATUS_FILENONVALIDO)
					{
						swtimer_Set(hprotfu_InfoManager.AggiornaFirmware.HTimer,2000);
						hprotfu_InfoManager.AggiornaFirmware.StatusLoop++;
					}
					else
					{
						hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
						hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
					}
				break;
				case HPROTFU_STATUSCMD_ERRORE:	// Qualche errore avvenuto, aborto l'aggiornamento
					hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
					hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
				break;
				default:
				break;
			}
		break;

		case HPROTFU_SLAGGIORNAFIRMWARE_TICKAGGIORNAMENTO:
			if(swtimer_Test(hprotfu_InfoManager.AggiornaFirmware.HTimer))
			{
				if(hprot_Master_PckIsPossibleToStart(hprotfu_Init.PtrHProtObjRef))
				{
					// Gioco sciocco per fare la prima partenza
					vuTypeData = fwupgrade_VUGetData(hprotfu_InfoManager.AggiornaFirmware.PtrStrNomeFile);
					if(vuTypeData==FWUPGRADE_VUTYPEDATA_STARTED)
						vuTypeData = fwupgrade_VUGetData(hprotfu_InfoManager.AggiornaFirmware.PtrStrNomeFile);
					switch(vuTypeData)
					{
						case FWUPGRADE_VUTYPEDATA_SENDDATA:
							hprot_Master_PckStart(hprotfu_Init.PtrHProtObjRef);

							memset(&hProtStructData,0,sizeof(hProtStructData));

							if(!hprotfu_InfoManager.AggiornaFirmware.ModPermissionFlash)
							{	// Solo il primo pacchetto
								hprotfu_InfoManager.AggiornaFirmware.ModPermissionFlash = 0xaa551001;
								hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_MODPERMISSION;
								hProtStructData.PtrData = &hprotfu_InfoManager.AggiornaFirmware.ModPermissionFlash;
								hProtStructData.TypeData = _TYPEDATA_UINT32_;
								hProtStructData.MaxLen = sizeof(hprotfu_InfoManager.AggiornaFirmware.ModPermissionFlash);
								hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);
							}

							hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_ADDRESS;
							hProtStructData.PtrData = &fwupgrade_VirtualizeUpgrade.Info.StartAddressFlash;
							hProtStructData.TypeData = _TYPEDATA_UINT32_;
							hProtStructData.MaxLen = sizeof(fwupgrade_VirtualizeUpgrade.Info.StartAddressFlash);
							hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

							hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_BUFFER4K;
							hProtStructData.PtrData = fwupgrade_VirtualizeUpgrade.Info.Buffer4K;
							hProtStructData.TypeData = _TYPEDATA_ARRAYOFUINT8_;
							hProtStructData.MaxLen = fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K;
							hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

							hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_NBYTETOWRITE;
							hProtStructData.PtrData = &fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K;
							hProtStructData.TypeData = _TYPEDATA_UINT32_;
							hProtStructData.MaxLen = sizeof(fwupgrade_VirtualizeUpgrade.Info.NByteToWrBuffer4K);
							hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

							hprot_Master_PckEnd(hprotfu_Init.PtrHProtObjRef);

							swtimer_Set(hprotfu_InfoManager.AggiornaFirmware.HTimer,4000);

							hprotfu_InfoManager.AggiornaFirmware.StatusLoop++;
						break;

						case FWUPGRADE_VUTYPEDATA_END:
							swtimer_Set(hprotfu_InfoManager.AggiornaFirmware.HTimer,2000);

							hprotfu_ExecuteReboot(HPROTFU_CMD_START,hprotfu_InfoManager.AggiornaFirmware.CodiceUnivocoScheda,false);	// Riavvio la scheda, aggiornamento riuscito
							hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ESEGUITO;
							hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
						break;

						case FWUPGRADE_VUTYPEDATA_ANOMALIA:
							hprot_Master_PckAbort(hprotfu_Init.PtrHProtObjRef);
							hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
							hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
						break;
						default:
						break;
					}
				}
			}	// Fine if(swtimer_Test(HTimerTimeOut))
			else
			{
				hprot_Master_PckAbort(hprotfu_Init.PtrHProtObjRef);
				hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
				hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
			}
		break;

		case HPROTFU_SLAGGIORNAFIRMWARE_WAITRISPOSTAPCK:
			if(swtimer_Test(hprotfu_InfoManager.AggiornaFirmware.HTimer))
			{
				if(hprot_Master_PckIsPossibleToStart(hprotfu_Init.PtrHProtObjRef))
				{
					swtimer_Set(hprotfu_InfoManager.AggiornaFirmware.HTimer,2000);

					if(hprotfu_Init.PtrHProtObjRef->MasterModeInfo.StatusPckMaster == HPROT_STATUSPCKMASTER_RISPOSTAAVVENUTA)
					{
						hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_TICKAGGIORNAMENTO;
					}
					else
					{
						hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
						hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
					}
				}
			}
			else
			{
				hprotfu_InfoManager.AggiornaFirmware.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
				hprotfu_InfoManager.AggiornaFirmware.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
			}
		break;

		default:
		break;
	}

	if(hprotfu_InfoManager.AggiornaFirmware.StatusLoop==HPROTFU_SLAGGIORNAFIRMWARE_VOID)
		return false;
	else
		return true;
}

//______________________________________________________________________________
// Desc:  Gestore core destroy per eliminazione anima scheda
// Arg: - Nessun argomento.
// Ret: - true se in fase di elaborazione
//______________________________________________________________________________
static bool ManagerCoreDestroy()
{
	hprot_StructData_t hProtStructData;

	switch(hprotfu_InfoManager.CoreDestroy.StatusLoop)
	{
		case HPROTFU_SLCOREDESTROY_VOID:
			if(hprotfu_InfoManager.CoreDestroy.FlgStart)
			{
				hprotfu_InfoManager.CoreDestroy.FlgStart = false;
				hprotfu_InfoManager.CoreDestroy.StatusCmd = HPROTFU_STATUSCMD_INESECUZIONE;
				hprotfu_InfoManager.CoreDestroy.ModPermissionFlash = 0;
				hprotfu_InfoManager.CoreDestroy.StatusLoop++;

				hprotfu_ExecuteReboot(HPROTFU_CMD_START,hprotfu_InfoManager.CoreDestroy.CodiceUnivocoScheda,true);

				// ATTENZIONE: La chiamata alla ManagerExecuteReboot qui è una eccezione per forzare
				// l'esecuzione immediata del Reboot per scavallare la logica della ManagerGateway...
				ManagerExecuteReboot();
			}
		break;

		case HPROTFU_SLCOREDESTROY_ATTESAMODALITABOOTLOADER:
			switch(hprotfu_InfoManager.ExeceuteReboot.StatusCmd)
			{
				case HPROTFU_STATUSCMD_ESEGUITO:	// Bene, procedi con aggiornamento
					hprot_Master_PckStart(hprotfu_Init.PtrHProtObjRef);

					memset(&hProtStructData,0,sizeof(hProtStructData));

					hprotfu_InfoManager.CoreDestroy.ModPermissionFlash = 0xaa559009;
					hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_MODPERMISSION;
					hProtStructData.PtrData = &hprotfu_InfoManager.CoreDestroy.ModPermissionFlash;
					hProtStructData.TypeData = _TYPEDATA_UINT32_;
					hProtStructData.MaxLen = sizeof(hprotfu_InfoManager.CoreDestroy.ModPermissionFlash);
					hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

					hprotfu_InfoManager.CoreDestroy.CoreDestroy = 0xcacca666;
					hProtStructData.Id = HPROT_IDSTD_RW_WRFLASH_COREDESTROY;
					hProtStructData.PtrData = &hprotfu_InfoManager.CoreDestroy.CoreDestroy;
					hProtStructData.TypeData = _TYPEDATA_UINT32_;
					hProtStructData.MaxLen = sizeof(hprotfu_InfoManager.CoreDestroy.CoreDestroy);
					hprot_Master_PckAddIdSet(hprotfu_Init.PtrHProtObjRef,&hProtStructData);

					hprot_Master_PckEnd(hprotfu_Init.PtrHProtObjRef);

					swtimer_Set(hprotfu_InfoManager.CoreDestroy.HTimer,2000);

					hprotfu_InfoManager.CoreDestroy.StatusLoop++;
				break;
				case HPROTFU_STATUSCMD_ERRORE:	// Qualche errore avvenuto, aborto l'aggiornamento
					hprotfu_InfoManager.CoreDestroy.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
					hprotfu_InfoManager.CoreDestroy.StatusLoop = HPROTFU_SLCOREDESTROY_VOID;
				break;
				default:
				break;
			}
		break;

		case HPROTFU_SLCOREDESTROY_WAITRISPOSTAPCK:
			if(swtimer_Test(hprotfu_InfoManager.CoreDestroy.HTimer))
			{
				if(hprot_Master_PckIsPossibleToStart(hprotfu_Init.PtrHProtObjRef))
				{
					hprotfu_InfoManager.CoreDestroy.StatusCmd = HPROTFU_STATUSCMD_ESEGUITO;
					hprotfu_InfoManager.CoreDestroy.StatusLoop = HPROTFU_SLAGGIORNAFIRMWARE_VOID;
				}
			}
			else
			{
				hprotfu_InfoManager.CoreDestroy.StatusCmd = HPROTFU_STATUSCMD_ERRORE;
				hprotfu_InfoManager.CoreDestroy.StatusLoop = HPROTFU_SLCOREDESTROY_VOID;
			}
		break;

		default:
		break;
	}

	if(hprotfu_InfoManager.CoreDestroy.StatusLoop==HPROTFU_SLCOREDESTROY_VOID)
		return false;
	else
		return true;
}

//______________________________________________________________________________
// Desc:  Ritorna l'address scheda dal CodiceUnivoco passato
// Arg: - codiceUnivoco: Il codice univoco
// Ret: - 1..15, 0 se NON rilevato
//______________________________________________________________________________
static uint8_t GetAddressFromCodiceUnivocoScheda(uint32_t codiceUnivocoScheda)
{
	uint8_t valRet = 0;
	uint32_t indice;

	for(indice=0;indice<HPROTFU_MAXNUMADDRESS;indice++)
	{
		if(hprotfu_Info.InfoBoard[indice].CodiceUnivocoScheda==codiceUnivocoScheda)
		{
			valRet = indice;
			break;
		}
	}

	return valRet;
}


