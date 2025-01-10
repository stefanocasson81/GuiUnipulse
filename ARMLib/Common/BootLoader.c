/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	23/06/20
 Descrizione:
	Bootloader manager usando le librerie HAL.
	Sono previsti più modi operativi:
		* Aggiornamento via CANBus tramite protocollo BusStack.
		   Questa modalità è sempre supportata come base sui bootloader delle schede.
		   Definire nel progetto [_ARMLIB_DEF_CANBUSSTACK_] e [_ARMLIB_DEF_HPROT_]!!!

		* Aggiornamento via USB:
	       L'uso dell'aggiornamento via USB è abilitata definendo a livello di progetto
	       la key [_ARMLIB_DEF_DRVFILESYSTEMUSB_].
	       Tipicamente questa configurazione viene usata dalla scheda display e cmq
	       chi ha l'USB.

		   Allo startup viene verificata la presenza di una chiavetta USB. Se presente
		   viene cercato il firmware con nome specificato da init. Sono attualmente
		   supportati i soli file codificati e criptati di tipo BIN o HEX(intel)


	Il modulo è quanto più completo possibile per essere integrato velocemente sui
	vari progetto e quindi iniziale da se diverse librerie

	L'uso del mobulo impone il l'init già fatto dei moduli a seguire operativi:
		- SWTimer

	************************* USO e ABUSO del modulo ***************************
	* INIZIALIZZAZIONE

		errori = false;

		if(_board_InitModule()!=_STATUS_OK_)
			errori = true;

		swtimer_Init.NumTimerDaGestire = 3;	// Default 10
		if(swtimer_InitModule()!=_STATUS_OK_)
			errori = true;

		if(_drvflashmcu_InitModule()!=_STATUS_OK_)
			errori = true;


	* USO
		bootloader_Manager();	// Da eseguire il looping





------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include <ARMLib/Common/BootLoader.h>
#include "ctype.h"
#include "ARMLib/Common/SwTimer.h"
#include "ARMLib/Common/FWUpgrade.h"

#if defined(_ARMLIB_DEF_CANBUSSTACK_)
	#include <ARMLib/Protocolli/CanBusStack/CanBusStack.h>
#endif

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES


#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	static swtimer_TimerHandler_t HTimerWaitUSB;
#endif

#if defined(_ARMLIB_DEF_HPROT_)
	static void DeCodificaBuffer32(uint8_t *ptrBuffer32);

	//____________________________________________________________________HProt zone
	static hprot_Obj_t *PtrHProtObjLocalCAN = _NULL_;	// Per HProt su bus CAN

	typedef struct
	{
		uint8_t StatusBoard;
		uint32_t TypeExecuteReboot;

		// Identificativi usati per aggiornamento del firmware
		uint32_t ModPermissionFlash;
		uint32_t StartAddressFlash;	// L'address di scrittura in flash del nuovo Buffer4K
		uint8_t Buffer4K[0x1000];	// Buffer di appoggio per memorizzazione nuovo settore verso la flash in HProt
		uint32_t NByteToWrBuffer4K;
		uint32_t CoreDestroy;
	}application_HProtData_t;

	static application_HProtData_t HProtData=
	{
		.StatusBoard = HPROT_STATUSBOARD_POWEREDON,
	};

	static int32_t FunHProt_StatusBoard(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
	{
		if(typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTGET)	// Una volta letto lo resetto
			HProtData.StatusBoard &= ~(HPROT_STATUSBOARD_POWEREDON|HPROT_STATUSBOARD_REBOOTED);

		//if(_board_GetStatusForceBootLoader())
			HProtData.StatusBoard |= HPROT_STATUSBOARD_BOOTLOADERMODE;

		return _STATUS_OK_;
	}
	static int32_t FunHProt_ExecuteReboot(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
	{
		if(typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTSET)
		{
			switch(HProtData.TypeExecuteReboot)
			{
				case 0xaa556666:	// Reboot
					bootloader_Init.PtrFunExecuteReboot(false);
				break;
				case 0x55aa9999:	// Reboot in bootloader
					bootloader_Init.PtrFunExecuteReboot(true);
				break;
			}
		}

		return _STATUS_OK_;
	}
	static int32_t FunHProt_WriteFlash(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
	{
		uint32_t uLA;
		uint8_t *ptrData;

		if((typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTSET) && HProtData.ModPermissionFlash==0xaa551001)
		{	// Se permesso di scrittura concordato
			// Verifico di essere in zona concessa per la programmazione
			if((HProtData.StartAddressFlash>=bootloader_Init.AddressFirmware-0x200) && (HProtData.StartAddressFlash<(bootloader_Init.AddressFirmware+bootloader_Init.MaxDimFirmware)))
			{
				if(bootloader_Init.PtrDrvFlashDriver)
				{
					if(bootloader_Init.PtrDrvFlashDriver->IsAddressStartSector(HProtData.StartAddressFlash))
						bootloader_Init.PtrDrvFlashDriver->EraseMultipleSector(HProtData.StartAddressFlash,sizeof(HProtData.Buffer4K));

					// Sotto la zona 0x200 non devo transcodificare il buffer32 ma scriverlo come lo ricevo
					if(HProtData.StartAddressFlash<(bootloader_Init.AddressFirmware+0x200))
					{
						ptrData = HProtData.Buffer4K;
						ptrData+=0x200;
						uLA = HProtData.NByteToWrBuffer4K;
						uLA-=0x200;
					}
					else
					{
						ptrData = HProtData.Buffer4K;
						uLA = HProtData.NByteToWrBuffer4K;

					}
					while(uLA)
					{
						DeCodificaBuffer32(ptrData);
						ptrData+=32;
						if(uLA>=32)
							uLA-=32;
						else
							uLA = 0;
					}

					bootloader_Init.PtrDrvFlashDriver->WriteData(HProtData.StartAddressFlash,HProtData.Buffer4K,HProtData.NByteToWrBuffer4K);
				}
			}
		}

		return _STATUS_OK_;
	}
	static int32_t FunHProt_CoreDestroy(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
	{
		if((typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTSET) && (HProtData.ModPermissionFlash==0xaa559009) && (HProtData.CoreDestroy==0xcacca666))
		{	// Se permesso di scrittura concordato
			if(bootloader_Init.PtrDrvFlashDriver)
			{
				bootloader_Init.PtrDrvFlashDriver->EraseMultipleSector(bootloader_Init.AddressFirmware-0x200,sizeof(HProtData.Buffer4K));	// Erase di 4k, tanto per essere sicuri
			}
		}

		return _STATUS_OK_;
	}

	// Lista Id HProt specifico per il canale CAN
	static const hprot_StructData_t HProtStructDataCan[]=
	{
		{HPROT_IDSTD_R_STATUSBOARD,											&HProtData.StatusBoard,																_TYPEDATA_UINT8_,		sizeof(HProtData.StatusBoard),														FunHProt_StatusBoard,		HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_CODICESCHEDAESTESOBOOTLOADERCOMPATIBILITY,			&bootloader_Init.InfoFirmware.CodiceEstesoSchedaBootLoaderCompatibility,			_TYPEDATA_UINT32_,		sizeof(bootloader_Init.InfoFirmware.CodiceEstesoSchedaBootLoaderCompatibility),		_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_CODICESCHEDAESTESO,									&bootloader_Init.InfoFirmware.CodiceEstesoScheda,									_TYPEDATA_UINT32_,		sizeof(bootloader_Init.InfoFirmware.CodiceEstesoScheda),							_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_FIRMWAREVER,											&bootloader_Init.InfoFirmware.Version,												_TYPEDATA_UINT16_,		sizeof(bootloader_Init.InfoFirmware.Version),										_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_FIRMWARECRC32,										&bootloader_Init.InfoFirmware.Crc32Firmware,										_TYPEDATA_UINT32_,		sizeof(bootloader_Init.InfoFirmware.Crc32Firmware),									_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_CODICEUNIVOCOSCHEDA,									&bootloader_Init.InfoFirmware.CodiceUnivocoScheda,									_TYPEDATA_UINT32_,		sizeof(bootloader_Init.InfoFirmware.CodiceUnivocoScheda),							_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_R_FIRMWAREDATA,										&bootloader_Init.InfoFirmware.FirmwareData,											_TYPEDATA_UINT32_,		sizeof(bootloader_Init.InfoFirmware.FirmwareData),									_NULL_,						HPROT_PERMISSION_NOWR},
		{HPROT_IDSTD_RW_EXECUTEREBOOT,										&HProtData.TypeExecuteReboot,														_TYPEDATA_UINT32_,		sizeof(HProtData.TypeExecuteReboot),												FunHProt_ExecuteReboot,		0},

		{HPROT_IDSTD_RW_WRFLASH_MODPERMISSION,								&HProtData.ModPermissionFlash,														_TYPEDATA_UINT32_,		sizeof(HProtData.ModPermissionFlash),												_NULL_,						0},
		{HPROT_IDSTD_RW_WRFLASH_ADDRESS,									&HProtData.StartAddressFlash,														_TYPEDATA_UINT32_,      sizeof(HProtData.StartAddressFlash),												_NULL_,						0},
		{HPROT_IDSTD_RW_WRFLASH_BUFFER4K,									HProtData.Buffer4K,																	_TYPEDATA_ARRAYOFUINT8_,sizeof(HProtData.Buffer4K),															_NULL_,						0},
		{HPROT_IDSTD_RW_WRFLASH_NBYTETOWRITE,								&HProtData.NByteToWrBuffer4K,														_TYPEDATA_UINT32_,      sizeof(HProtData.NByteToWrBuffer4K),												FunHProt_WriteFlash,		0},
		{HPROT_IDSTD_RW_WRFLASH_COREDESTROY,								&HProtData.CoreDestroy,																_TYPEDATA_UINT32_,		sizeof(HProtData.CoreDestroy),														FunHProt_CoreDestroy,		0},


		{_NULL_},
	};
#endif


//============================================================= GLOBAL VARIABLES


bootloader_Init_t bootloader_Init=
{
#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	.MsTimerWaitUSB=3333	,
#endif
};
bootloader_Info_t bootloader_Info;

#if defined(_ARMLIB_DEF_CANBUSSTACK_)
	#define __DATU__(a) {(&a) , (sizeof(a))}

	//static uint8_t TmpBuffer8PackTX[DATA8PACK_DIMDATA];	// Usato per comporre il packet da spedire in CAN
	const data8pack_Data_t bootloader_StructData8TX[]=
	{   // Terminare la tabella con 0 !!!
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+0,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[0]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+1,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[1]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+2,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[2]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+3,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[3]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+4,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[4]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+5,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[5]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+6,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[6]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+7,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[7]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+8,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[8]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+9,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[9]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+10,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[10]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+11,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[11]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+12,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[12]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+13,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[13]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+14,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[14]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+15,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.RawDataToSend[15]),},  	},
		{	CANBUSSTACK_IDCAN_BUSSTACKEVENT,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(canbusstack_Info.EventToSend),},  		},

		{	_NULL_,																_NULL_,					_NULL_, 							{{_NULL_,0},}   									},
	};



	static uint8_t TmpBuffer8PackRX[DATA8PACK_DIMDATA];	// Usato come buffer di passaggio in ricezione CAN
	const data8pack_Data_t bootloader_StructData8RX[]=
	{	// Terminare la tabella con 0 !!!

		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+0,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+1,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+2,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+3,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+4,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+5,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+6,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+7,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+8,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+9,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+10,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+11,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+12,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+13,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+14,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKBASE+15,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
		{	CANBUSSTACK_IDCAN_BUSSTACKEVENT,									DATA8PACK_NOAUTOTX,		canbusstack_PushEventFromCAN,			{__DATU__(TmpBuffer8PackRX),},  				},

		{	_NULL_,																_NULL_,					_NULL_, 								{{_NULL_,0},}   								},
	};
#endif



/*
#if _LIBSUPPORT_DRVCUSTUMFS_USB
	extern ApplicationTypeDef Appli_state;
#endif
*/
//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Init del modulo.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t bootloader_InitModule(void)
{
	bool errore=false;

	if(swtimer_Init.InitOk)
	{	// Necessario SwTimer inizializzato
#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
		HTimerWaitUSB=swtimer_Create();
		swtimer_Set(HTimerWaitUSB,bootloader_Init.MsTimerWaitUSB);
#endif
	}
	else
		errore = true;

	if(!bootloader_Init.PtrFunExecuteReboot)
		errore = true;

#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	// Necessario il nome file
	if(!bootloader_Init.PtrNomeFileFirmware)
		errore = true;

	//__________________________________________________________Init filesystenm
	if(_drvfilesystemusb_InitModule() != _STATUS_OK_)
		errore = true;

	//_____________________________________________________Init firmware upgrade
	fwupgrade_Init.Crc32FirmwareAttuale = *(uint32_t*)(bootloader_Init.AddressInfoHeader+0x1c);

	fwupgrade_Init.AddressInfoHeader	= bootloader_Init.AddressInfoHeader;
	fwupgrade_Init.DimInfoHeader		= bootloader_Init.DimInfoHeader;

	fwupgrade_Init.AddressFirmware		= bootloader_Init.AddressFirmware;
	fwupgrade_Init.MaxDimFirmware		= bootloader_Init.MaxDimFirmware;

	fwupgrade_Init.PtrDrvFlashDriver	= bootloader_Init.PtrDrvFlashDriver;

	fwupgrade_Init.CodiceSchedaEsteso 	= bootloader_Init.InfoFirmware.CodiceEstesoScheda;
	fwupgrade_Init.CodiceSchedaEstesoBootLoaderCompatibility = bootloader_Init.InfoFirmware.CodiceEstesoScheda;

	fwupgrade_Init.PtrFunFunInfoStatus	= bootloader_Init.PtrFunFunInfoStatus;

	fwupgrade_Init.PtrDrvfilesystem 	= &_drvfilesystemusb_Driver;
	fwupgrade_Init.PtrTabellaTranscodifica = bootloader_Init.TabellaTranscodifica;

	if(bootloader_Init.FirmwareValidoRilevato)
		fwupgrade_Init.ProcediAlCambioFirmware = false;
	else
		fwupgrade_Init.ProcediAlCambioFirmware = true;

	if(fwupgrade_InitModule()!=_STATUS_OK_)
	    errore = true;
#endif


#if defined(_ARMLIB_DEF_CANBUSSTACK_) && defined(_ARMLIB_DEF_HPROT_)
	data8pack_Init.MsFreeTimeSend = 200;
	data8pack_Init.PtrStructDataTX = bootloader_StructData8TX;
	if(data8pack_InitModule()!=_STATUS_OK_)
		errore = true;

	//__________________________________________________________Init CanBusStack
	canbusstack_Init.GatewayMode = false;
	canbusstack_Init.DimBufferRawData = 5000;	// Dimensionare in modo opportuno... (4K + qualcosa in  più)
	canbusstack_Init.CodiceUnivocoScheda = bootloader_Init.InfoFirmware.CodiceUnivocoScheda;
	if(canbusstack_InitModule()!=_STATUS_OK_)
		errore = true;

	//________________________________________________________________Init HProt
	if(hprot_InitModule()==_STATUS_OK_)
	{
		// In questa configurazione siamo in slave e quindi gestisco il normale HProt in can senza gateway
		PtrHProtObjLocalCAN = hprot_CreateObj((hprot_StructData_t*)HProtStructDataCan,false,5000,canbusstack_RawDataGetChar,canbusstack_RawDataSendData,canbusstack_RawDataFlushRX,canbusstack_RawDataIsEmptyTX,canbusstack_RawDataSetAddressDest,canbusstack_RawDataGetAddressOrg);
	}
	else
		errore = true;
#endif

	bootloader_Info.StatusLoop = BOOTLOADER_SL_VOID;

	if(errore)
	{
		bootloader_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		bootloader_Init.InitOk = true;
		return _STATUS_OK_;
	}
}


/*______________________________________________________________________________
 Desc:  Gestore del modulo
 Arg: - Nessun argomento.
 Ret: - true finchè le valutazioni sono in corso, false go jump to firmware
______________________________________________________________________________*/
bool bootloader_Manager(void)
{
	bool valRet = true;
	uint8_t checkFun=0;
	uint16_t uSA;
	(void)uSA;


#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	static drvfilesystem_PeripheralsStatus_e oldPeripheralsStatus = 0xff;
	static char* PtrStrFile;
	_drvfilesystemusb_Manager();
#endif

	if(bootloader_Init.InitOk)
	{
		switch(bootloader_Info.StatusLoop)
		{
			case BOOTLOADER_SL_VOID:
#if defined(_ARMLIB_DEF_CANBUSSTACK_) && defined(_ARMLIB_DEF_HPROT_)
				if(bootloader_Init.ForzaAggiornamentoFirmwareDaCANBUS)
				{
					checkFun |= _B0_;
					canbusstack_Manager();
					hprot_ManagerObj(PtrHProtObjLocalCAN);
					// ATTENZIONE: la _board_CAN_Manager() dovrebbe essre chiamata qui ma per coerenza di libreria DEVE essere chiamata fuori da questo modulo...
				}
#endif
#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
				checkFun |= _B1_;

	#if !defined(_ARMLIB_DEF_HPROT_)
				if(bootloader_Init.FirmwareValidoRilevato)
				{
					// Se un firmware valido è stato rilevato su flash dopo il timeout ricerca usb passa e lo esegue
					if(!swtimer_Test(HTimerWaitUSB))
						bootloader_Info.StatusLoop = BOOTLOADER_SL_ESEGUIFIRMWARE;	// Tempo scaduto jumpa al firmware reale
				}
	#endif
				if(oldPeripheralsStatus!=_drvfilesystemusb_Driver.PeripheralsStatus())
				{
					oldPeripheralsStatus=_drvfilesystemusb_Driver.PeripheralsStatus();

					if(oldPeripheralsStatus==DRVFILESYSTEM_PERIPHERALSSTATUS_READY)
					{
						// Cerco il file e becco il primo della lista
						PtrStrFile = _drvfilesystemusb_Driver.FFind(true,_NULL_,bootloader_Init.PtrNomeFileFirmware);

						if(PtrStrFile)
						{
							strncpy(bootloader_Info.Usb.NomeFile,PtrStrFile,sizeof(bootloader_Info.Usb.NomeFile)-1);
							bootloader_Info.StatusLoop = BOOTLOADER_SL_USB_CHECKFILEANDSHOOT;
						}
					}
				}
#endif
				if(!checkFun)
					bootloader_Info.StatusLoop = BOOTLOADER_SL_ESEGUIFIRMWARE;
			break;

#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
			case BOOTLOADER_SL_USB_CHECKFILEANDSHOOT:
				if(bootloader_Init.PtrFunPreAggiornamentoFirmware)
					bootloader_Init.PtrFunPreAggiornamentoFirmware();

				switch(fwupgrade_CheckFileAndShoot(bootloader_Info.Usb.NomeFile, false, _NULL_))
				{
					case FWUPGRADE_STATUS_USBNONRILEVATA:					// Usb NON rilevata
					break;
					case FWUPGRADE_STATUS_FILEINVERIFICA:
					case FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO:
					break;

					default:
					case FWUPGRADE_STATUS_FILENONRILEVATO:			// Usb rilevata ma file NON rilevato
					case FWUPGRADE_STATUS_FILENONVALIDO:			// File PCK NON valido
					case FWUPGRADE_STATUS_FIRMWAREAGGIORNATO:		// Firmware su PCK aggiornato
					case FWUPGRADE_STATUS_FIRMWAREERRFLASH:			// Firmware su PCK NON aggiornato per errore generico in mcuflash
					case FWUPGRADE_STATUS_OK:                     	// Lettura file corretta
					case FWUPGRADE_STATUS_ERROREGENERICO:			// Altro errore generico
					case FWUPGRADE_STATUS_AGGIORNAMENTONONNECESSARIO:
						bootloader_Info.StatusLoop = BOOTLOADER_SL_ESEGUIFIRMWARE;
					break;
				}
			break;
#endif	// fine #if _LIBSUPPORT_DRVCUSTUMFS_USB

			default:
			case BOOTLOADER_SL_ESEGUIFIRMWARE:
				valRet = false;
				// ATTENZIONE: volutamente senza break
			break;

		}	// fine switch(bootloader_Info.BusStakUpgrade.StatusLoop)
	}	// fine if(bootloader_Init.InitOk)

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Forza infinito tempo di attesa ricerca USB (utile quando ci sono anomalie serie)
 Arg: - Nessun argomento.
 Ret: - void
______________________________________________________________________________*/
void bootloader_ForceInfiniteWaitTime(void)
{
#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	swtimer_Set(HTimerWaitUSB,0xffffffff);
#endif
}


typedef void (*application_PtrFun)(void);
application_PtrFun JumpToApplication;
/*______________________________________________________________________________
 Desc:  Jump executor application
 Arg: - Nessun argomento.
 Ret: - void
______________________________________________________________________________*/
void bootloader_RunApplication( uint32_t address )
{
	volatile uint32_t jumpAddress;
	
	// NOTA: Prima di questa funzione settare lo stack pointer con !!!!
	// 		__set_MSP(*(uint32_t *)(address));

	jumpAddress = *(volatile unsigned long*) (address + 4);
	JumpToApplication = (application_PtrFun)jumpAddress;

	SCB->VTOR = address;

	JumpToApplication();
}


//============================================================ PRIVATE FUNCTIONS
#if defined(_ARMLIB_DEF_HPROT_)
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
			ptrBuffer32[bootloader_Init.TabellaTranscodifica[indiceTabella]] = tmpBuffer[indiceRd];

			indiceTabella++;
			indiceTabella&=0x1f; // Limito 0..31
		}
	}
#endif

