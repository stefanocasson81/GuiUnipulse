#ifndef BOOTLOADER_H_INCLUDED
#define BOOTLOADER_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Protocolli/HProt/HProt.h"
#include "ARMLib/Device/Flash/DrvFlash.h"
#include "ARMLib/Common/Data8Pack.h"


#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	#include "ARMLib/FileSystem/_DrvFileSystemUSB.h"
	#include "ARMLib/Common/FWUpgrade.h"
#endif

//====================================================================== DEFINES
#pragma pack(push,1)
typedef struct
{
	uint32_t CodiceScheda;					// Codice scheda completo
	uint32_t DimFirmwareAling32;			// Dimensione del firmware allineato a 32byte
	uint32_t Crc16FirmwareOriginaleAlign32;	// Crc16 del firmware originale non codificato allineato a 32byte
	uint8_t FixPattern[8];					// Pattern fisso 02 02 07 03 05 04 02 02
	uint8_t NewFixTypePattern[7];			// Se pattern AA 55 66 99 55 aa 00 significa file di nuovo tipo
	uint8_t VerNewType;						// Versione del nuovo pattern
	uint32_t Crc32FirmwareOriginialeALign32;
	uint8_t FileType;
	uint32_t Crc32FirmwareCodificatoALign32;
	uint8_t NoCheckFirmware;				// Salta direttamente al firmware se vale 0xa5
	uint8_t VersioneFirmware;
	uint32_t DataCreazioneFile;				// In nibble AAAAMMGG
	uint32_t CodiceSchedaEstesoBootLoaderCompatibility;
	uint8_t DummyAlign512[467];
}bootloader_InfoHeader_t;
#pragma pack(pop)

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

	uint32_t AddressInfoHeader;		// Address informazioni firmware
	uint32_t DimInfoHeader;			// Dimensione informazioni firmware

	uint32_t AddressFirmware;		// Address inizio firmware per upgrade
	uint32_t MaxDimFirmware;		// Spazio in byte dedicato al firmware

	drvflash_Driver_t *PtrDrvFlashDriver;		// Ptr al driver scrittura in flash

	void (*PtrFunFunInfoStatus)(bool forzaRefresh);	// Funzione eseguita durante l'aggiornamento utile per eventualmente aggiornare sullo stato

	uint8_t TabellaTranscodifica[32];

	bool ForzaAggiornamentoFirmwareDaCANBUS;	// Se a true si ferma in attesa di aggiornamento firmware da CAN (tipica forzatura dettata dal master esterno che vuole aggiornare la scheda)

	void (*PtrFunExecuteReboot)(bool modalitaBootLoader);
	struct
	{
		uint32_t CodiceEstesoSchedaBootLoaderCompatibility;	// Esempio 26060437	legato al pcb
		uint32_t CodiceEstesoScheda;	// Esempio 22755016
		uint16_t Version;				// MSB:Stage(!,b,V) LSB:ver
		uint32_t Crc32Firmware;
		uint32_t CodiceUnivocoScheda;
		uint32_t FirmwareData;
	}InfoFirmware;

	bool FirmwareValidoRilevato;

	uint32_t CodiceSchedaEstesoBootLoaderCompatibility;	// Codice scheda esteso tipo 26060437 legato al pcb

	int32_t (*PtrFunPreAggiornamentoFirmware)(void);
	
#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	char* PtrNomeFileFirmware;		// Nome del file da ricercare per il firmware

	uint32_t MsTimerWaitUSB;
#endif
}bootloader_Init_t;

typedef enum
{
	BOOTLOADER_SL_VOID					,

	BOOTLOADER_SL_USB_CHECKFILEANDSHOOT	,

	BOOTLOADER_SL_ESEGUIFIRMWARE		,

	BOOTLOADER_SLBUSSTACKUPGRADE_MAX
}bootloader_SL_e;

typedef struct
{
	bootloader_SL_e StatusLoop;

#if defined(_ARMLIB_DEF_DRVFILESYSTEMUSB_)
	struct
	{
		char NomeFile[100];
	}Usb;
#endif
}bootloader_Info_t;


//============================================================= GLOBAL VARIABLES
extern bootloader_Init_t bootloader_Init;
extern bootloader_Info_t bootloader_Info;
extern const data8pack_Data_t bootloader_StructData8TX[];
extern const data8pack_Data_t bootloader_StructData8RX[];

//============================================================ GLOBAL PROTOTYPES
int32_t bootloader_InitModule(void);
bool bootloader_Manager(void);
void bootloader_RunApplication( uint32_t address );
void bootloader_ForceInfiniteWaitTime(void);

#endif	// Fine #if _LIBSUPPORT_BOOTLOADER == _ENABLED_

