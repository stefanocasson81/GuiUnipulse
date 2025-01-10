#ifndef SDRAM_H_INCLUDED
#define SDRAM_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
// Command Mode 
#define _SDRAM_NORMAL_MODE_CMD             0x00000000U
#define _SDRAM_CLK_ENABLE_CMD              0x00000001U
#define _SDRAM_PALL_CMD                    0x00000002U
#define _SDRAM_AUTOREFRESH_MODE_CMD        0x00000003U
#define _SDRAM_LOAD_MODE_CMD               0x00000004U
#define _SDRAM_SELFREFRESH_MODE_CMD        0x00000005U
#define _SDRAM_POWERDOWN_MODE_CMD          0x00000006U

// Register Mode 
#define _SDRAM_BURST_LENGTH_1              0x00000000U
#define _SDRAM_BURST_LENGTH_2              0x00000001U
#define _SDRAM_BURST_LENGTH_4              0x00000002U
#define _SDRAM_BURST_LENGTH_8              0x00000004U
#define _SDRAM_BURST_TYPE_SEQUENTIAL       0x00000000U
#define _SDRAM_BURST_TYPE_INTERLEAVED      0x00000008U
#define _SDRAM_CAS_LATENCY_2               0x00000020U
#define _SDRAM_CAS_LATENCY_3               0x00000030U
#define _SDRAM_OPERATING_MODE_STANDARD     0x00000000U
#define _SDRAM_WRITEBURST_MODE_PROGRAMMED  0x00000000U 
#define _SDRAM_WRITEBURST_MODE_SINGLE      0x00000200U 



typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
	
	
	SDRAM_HandleTypeDef *PtrSDRAMHanldeTypeDef;
	
	uint32_t TargetBank;           // Target Bank, pu� essere FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2
	uint32_t RefreshMode;          // Refresh Mode
	uint32_t RefreshRate;          // Refresh Rate
	uint32_t BurstLength;          // Burst Length
	uint32_t BurstType;            // Burst Type
	uint32_t CASLatency;           // CAS Latency
	uint32_t OperationMode;        // Operation Mode
	uint32_t WriteBurstMode;       // Write Burst Mode

	
}_sdram_Init_t;

//============================================================= GLOBAL VARIABLES
extern _sdram_Init_t _sdram_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t _sdram_InitModule(void);


#endif /* _SDRAM_H_INCLUDED */



