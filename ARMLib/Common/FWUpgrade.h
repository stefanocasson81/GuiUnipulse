#ifndef FWUPGRADE_H_INCLUDED
#define FWUPGRADE_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Device/Flash/DrvFlash.h"

#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	#include "ARMLib/FileSystem/DrvFileSystem.h"
#endif

//====================================================================== DEFINES


typedef enum
{
	FWUPGRADE_TIPOFILE_NONVALIDO						,
	FWUPGRADE_TIPOFILE_NONRILEVATO						,

	FWUPGRADE_TIPOFILE_BINOLDSTYLE						,	// Tipo file codificato BIN old style
	FWUPGRADE_TIPOFILE_BIN								,	// Tipo file codificato BIN new style
	FWUPGRADE_TIPOFILE_HEX								,	// Tipo file codificato HEX (cmq new style)

	FWUPGRADE_TIPOFILE_MAX
}fwupgrade_TipoFile_t;

typedef enum
{																		
	FWUPGRADE_STATUS_USBNONRILEVATA,				// Usb NON rilevata
	FWUPGRADE_STATUS_FILENONRILEVATO,				// Usb rilevata ma file NON rilevato
	FWUPGRADE_STATUS_FILENONVALIDO,					// File NON valido
	FWUPGRADE_STATUS_FILEINVERIFICA,				// Verifica del file in corso
	FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO,		// Firmware in aggiornamento
	FWUPGRADE_STATUS_FIRMWAREAGGIORNATO,			// Firmware aggiornato
	FWUPGRADE_STATUS_FIRMWAREERRFLASH,				// Firmware non aggiornato per errore generico in mcuflash
	FWUPGRADE_STATUS_ERROREGENERICO,				// Altro errore generico
	FWUPGRADE_STATUS_AGGIORNAMENTONONNECESSARIO,	// Aggiornamento non necessario

	FWUPGRADE_STATUS_OK                  		// Lettura file corretta
	
}fwupgrade_Status_e;

typedef enum
{
	FWUPGRADE_SLVUGETDATA_OPENFILEHEX,
	FWUPGRADE_SLVUGETDATA_OPENFILEBIN,
	FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEHEX	,
	FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEBIN	,
	FWUPGRADE_SLVUGETDATA_POSTHEADERDATAFILEBIN_NEXT	,
	//FWUPGRADE_SLVUGETDATA_HEADERDATAFILE		,
	FWUPGRADE_SLVUGETDATA_FIRMWAREAGGIORNATO	,
}fwupgrade_SLVUGetData_e;


typedef enum
{
	FWUPGRADE_VUTYPEDATA_VOID		,
	FWUPGRADE_VUTYPEDATA_STARTED	,
	FWUPGRADE_VUTYPEDATA_SENDDATA	,
	FWUPGRADE_VUTYPEDATA_END		,
	FWUPGRADE_VUTYPEDATA_ANOMALIA
}fwupgrade_VUTypeData_e;

typedef struct
{
	fwupgrade_SLVUGetData_e SLVUGetData;

	uint32_t ActualAddress;

	uint32_t SizeFile;

	uint8_t StatusLoop;

	struct
	{
		uint8_t Buffer32[32];
		uint8_t IndiceWrBuffer32;

		uint8_t Buffer4K[0x1000];	// Buffer di appoggio per memorizzazione nuovo settore verso la flash in HProt
		uint32_t IndiceWrBuffer4K;
		uint32_t NByteToWrBuffer4K;

		uint32_t StartAddressFlash;	// L'address di scrittura inflash del nuovo Buffer4K
	}Info;


}fwupgrade_VirtualizeUpgrade_t;


typedef struct
{
	char* PtrNomeFile;			
	uint8_t SizeNomeFile;
	uint32_t StartSeekFile;		// Address start da inizio file
	uint32_t SizeFile;			// Size dal inizio StartSeekFile

//	DRVCUSTOMFS_HANDLEFILE *PtrHandleFilePck; // Handle del Pck di origine, NON chiuderlo e NON modificarlo, serve solo per recuperare info
}fwupgrade_RigaIntesta_t;

typedef struct
{
	uint16_t Crc16Originale;	// Crc16 del file originale letto nell'intestazione
	uint32_t Crc32Originale;	// Crc32 del file originale letto nell'intestazione
	uint32_t Crc32Codificato;	// Crc32 del file post codifica letto nell'intestazione
	uint32_t TotalSizeAlign32;	// Dimensione del file allineato a 32 byte letto nell'intestazione
	uint32_t CodiceSchedaEsteso;// Codice scheda letto nell'intestazione
	uint8_t VersioneFirmware;	// Versione derl firmware letto nell'intestazione
	uint32_t DataCreazione;		// Data creazione
	uint32_t CodiceSchedaEstesoBootLoaderCompatibility;

	uint32_t Crc32OriginaleRicalcolato;
	uint32_t Crc16OriginaleRicalcolato;
	bool CrcValidato;

	fwupgrade_TipoFile_t TipoFile;
}fwupgrade_InfoFileCodificato_t;

typedef struct
{
	char NomeFile[100];
	uint32_t Crc32File;
	uint8_t VersioneFile;
	uint32_t DataFile;
	uint32_t CodiceSchedaRidotto;
	uint8_t Tag;
}fwupgrade_Abbinamento_t;

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

	uint32_t Crc32FirmwareAttuale;	// Crc32 del firmware attuale
	uint16_t Crc16FirmwareAttuale;	// Crc16 del firmware attuale

	uint32_t AddressInfoHeader;		// Address informazioni firmware
	uint32_t DimInfoHeader;			// Dimensione informazioni firmware

	uint32_t AddressFirmware;		// Address inizio firmware per upgrade
	uint32_t MaxDimFirmware;		// Spazio in byte dedicato al firmware

	drvflash_Driver_t *PtrDrvFlashDriver;		// Ptr al driver scrittura in flash

	uint32_t CodiceSchedaEsteso;	// Codice scheda esteso tipo 22755022
	uint32_t CodiceSchedaEstesoBootLoaderCompatibility;	// Codice scheda esteso tipo 26060437 legato al pcb
	bool ProcediAlCambioFirmware;	// A true per procederfe al cambio firmware ma sse CodiceSchedaEstesoBootLoaderCompatibility è conforme

#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	drvfilesystem_Driver_t *PtrDrvfilesystem;
#endif

	void (*PtrFunFunInfoStatus)(bool forzaRefresh);	// Funzione eseguita durante l'aggiornamento utile per eventualmente aggiornare sullo stato

	uint8_t *PtrTabellaTranscodifica;
}fwupgrade_Init_t;

typedef struct
{
	fwupgrade_Status_e Status;
	uint16_t DecPercentualeEsecuzione;	// Decimo di percentuale esecuizione aggiornamento (0...100,0%)

}fwupgrade_Info_t;

//============================================================= GLOBAL VARIABLES
extern fwupgrade_Init_t fwupgrade_Init;
extern fwupgrade_Info_t fwupgrade_Info;
extern fwupgrade_VirtualizeUpgrade_t fwupgrade_VirtualizeUpgrade;

//============================================================ GLOBAL PROTOTYPES
int32_t fwupgrade_InitModule(void);
int32_t fwupgrade_CheckFile(char* strNomeFile,fwupgrade_InfoFileCodificato_t *ptrInfoFileCodificato,bool validaCRCFile);
fwupgrade_Status_e fwupgrade_CheckFileAndShoot(char* strNomeFile,bool virtualize,uint32_t codiceSchedaVirtualize);
fwupgrade_VUTypeData_e fwupgrade_VUGetData(char *ptrStrNomeFile);


#endif // fine #if _LIBSUPPORT_FWUPGRADE == _ENABLED_

