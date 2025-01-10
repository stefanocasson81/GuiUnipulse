#ifndef HPROTFU_H_INCLUDED
#define HPROTFU_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Protocolli/HProt/HProtFU.h"
#include "ARMLib/Protocolli/HProt/HProt.h"

//====================================================================== DEFINES

#define HPROTFU_MAXNUMADDRESS 16

typedef enum
{
	HPROTFU_CMD_GETINFO,
	HPROTFU_CMD_START,
}hprotfu_Cmd_e;

typedef enum
{
	HPROTFU_STATUSCMD_VOID,
	HPROTFU_STATUSCMD_INESECUZIONE,
	HPROTFU_STATUSCMD_ERRORE,
	HPROTFU_STATUSCMD_ESEGUITO,

}hprotfu_StatusCmd_e;

typedef enum
{
	HPROTFU_SLEXECUTEREBOOT_VOID,
	HPROTFU_SLEXECUTEREBOOT_SENDCMDREBOOTLOADER,
	HPROTFU_SLEXECUTEREBOOT_WAITREBOOTLOADER
}hprotfu_SLExecuteReboot_e;
typedef struct
{
	bool FlgStart;
	uint32_t CodiceUnivocoScheda;
	hprotfu_StatusCmd_e StatusCmd;
	swtimer_TimerHandler_t HTimer;
	hprotfu_SLExecuteReboot_e StatusLoop;
	bool BootLoaderMode;
}hprotfu_ExecuteReboot_t;

typedef enum
{
	HPROTFU_SLGETINFONETCAN_VOID,
	HPROTFU_SLGETINFONETCAN_SENDREQINFO,
	HPROTFU_SLGETINFONETCAN_WAITRESPREQINFO,

}hprotfu_SLGetInfoNetCAN_e;
typedef struct
{
	hprotfu_SLGetInfoNetCAN_e StatusLoop;
	hprotfu_StatusCmd_e StatusCmd;
	bool FlgStart;
	uint8_t AddressInCorso;
	swtimer_TimerHandler_t HTimer;
}hprotfu_GetInfoNetCAN_t;

typedef enum
{
	HPROTFU_SLAGGIORNAFIRMWARE_VOID,
	HPROTFU_SLAGGIORNAFIRMWARE_ATTESAMODALITABOOTLOADER,
	HPROTFU_SLAGGIORNAFIRMWARE_TICKAGGIORNAMENTO,
	HPROTFU_SLAGGIORNAFIRMWARE_WAITRISPOSTAPCK,

}hprotfu_SLAggiornaFirmware_e;
typedef struct
{
	bool FlgStart;
	uint32_t CodiceUnivocoScheda;
	char *PtrStrNomeFile;
	uint32_t CodiceEstesoScheda;
	hprotfu_StatusCmd_e StatusCmd;
	swtimer_TimerHandler_t HTimer;
	hprotfu_SLAggiornaFirmware_e StatusLoop;
	uint32_t ModPermissionFlash;
}hprotfu_AggiornaFirmware_t;


typedef enum
{
	HPROTFU_SLCOREDESTROY_VOID,
	HPROTFU_SLCOREDESTROY_ATTESAMODALITABOOTLOADER,
	HPROTFU_SLCOREDESTROY_WAITRISPOSTAPCK,
}hprotfu_SLCoreDestroy_e;
typedef struct
{
	bool FlgStart;
	uint32_t CodiceUnivocoScheda;
	uint32_t CodiceEstesoScheda;
	hprotfu_StatusCmd_e StatusCmd;
	swtimer_TimerHandler_t HTimer;
	hprotfu_SLCoreDestroy_e StatusLoop;
	uint32_t ModPermissionFlash;
	uint32_t CoreDestroy;
}hprotfu_CoreDestroy_t;

typedef struct
{
	uint8_t AddressDestInCorso;

	hprotfu_GetInfoNetCAN_t GetInfoNetCAN;
	hprotfu_ExecuteReboot_t ExeceuteReboot;
	hprotfu_AggiornaFirmware_t AggiornaFirmware;
	hprotfu_CoreDestroy_t CoreDestroy;
}hprotfu_InfoManager_t;


typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

	hprot_Obj_t *PtrHProtObjRef;
}hprotfu_Init_t;

typedef struct
{
	struct
	{
		bool Presente;			// A true sse è presente (rilevato dal bus stask)
		uint32_t CodiceSchedaEstesoBootLoaderCompatibility;
		uint32_t CodiceUnivocoScheda;
		uint32_t CodiceSchedaEsteso;
		uint16_t FirmwareVer;	// MSB: Staga (A,B,V...) LSB: versione
		uint32_t FirmwareCrc32;
		uint32_t FirmwareData;	// Formato AAAAMMGG 4byte
		uint8_t Address;
	}InfoBoard[HPROTFU_MAXNUMADDRESS];
}hprotfu_Info_t;



//============================================================= GLOBAL VARIABLES
extern hprotfu_Init_t hprotfu_Init;
extern hprotfu_Info_t hprotfu_Info;
extern const hprot_StructData_t hprotfu_HProtStructData[];
extern hprotfu_InfoManager_t hprotfu_InfoManager;



//============================================================ GLOBAL PROTOTYPES
int32_t hprotfu_InitModule(void);
bool hprotfu_ManagerGateway(void);
hprotfu_StatusCmd_e hprotfu_GetInfoNetCAN(hprotfu_Cmd_e cmd);
hprotfu_StatusCmd_e hprotfu_ExecuteReboot(hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,bool bootLoaderMode);
hprotfu_StatusCmd_e hprotfu_AggiornaFirmware(hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,uint32_t codiceEstesoScheda,char *ptrStrNomeFile);
hprotfu_StatusCmd_e hprotfu_CoreDestroy(hprotfu_Cmd_e cmd,uint32_t codiceUnivocoScheda,uint32_t codiceEstesoScheda);

#endif // fine #if _LIBSUPPORT_PROTOCOLLI_HPROTFU == _ENABLED_

