/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Driver supporto chip SDRAM.
	Per il momento viene gestito il classico chip generico
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Device/SDRAM/_SDRAM.h"


//====================================================================== DEFINES





//=========================================================== PRIVATE PROTOTYPES
static void Delay(uint32_t msDelay);

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
_sdram_Init_t _sdram_Init =
{
	.TargetBank      = FMC_SDRAM_CMD_TARGET_BANK2		,
    .RefreshMode     = _SDRAM_AUTOREFRESH_MODE_CMD		,
    .RefreshRate     = ((uint32_t)0x0603)   			,	// SDRAM refresh counter (100Mhz SD clock)
    .BurstLength     = _SDRAM_BURST_LENGTH_1			,
    .BurstType       = _SDRAM_BURST_TYPE_SEQUENTIAL		,
    .CASLatency      = _SDRAM_CAS_LATENCY_3				,
    .OperationMode   = _SDRAM_OPERATING_MODE_STANDARD	,
    .WriteBurstMode  = _SDRAM_WRITEBURST_MODE_SINGLE	
};

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t _sdram_InitModule(void)
{
	bool errore=false;
	FMC_SDRAM_CommandTypeDef Command;

	if(_sdram_Init.PtrSDRAMHanldeTypeDef)
	{
	
		Command.CommandTarget          = _sdram_Init.TargetBank;
		
		//______________________________________________________________Clock enable
		Command.CommandMode            = _SDRAM_CLK_ENABLE_CMD;
		Command.AutoRefreshNumber      = 1;
		Command.ModeRegisterDefinition = 0;
		if(HAL_SDRAM_SendCommand(_sdram_Init.PtrSDRAMHanldeTypeDef, &Command, 0xffff) == HAL_OK)
		{
			// Qui andrebbe almeno 100uS di pausa, abbondiamo
			Delay(2);
			
			//_____________________________________________Precharge all sdram banks
			Command.CommandMode            = _SDRAM_PALL_CMD;
			Command.AutoRefreshNumber      = 1;
			Command.ModeRegisterDefinition = 0;
			if(HAL_SDRAM_SendCommand(_sdram_Init.PtrSDRAMHanldeTypeDef, &Command, 0xffff) == HAL_OK)
			{
				//____________________________________________Configure Refresh mode
				Command.CommandMode            = _sdram_Init.RefreshMode;
				Command.AutoRefreshNumber      = 8;
				Command.ModeRegisterDefinition = 0;
				if(HAL_SDRAM_SendCommand(_sdram_Init.PtrSDRAMHanldeTypeDef, &Command, 0xffff) == HAL_OK)
				{
					//_____________________Program the external memory mode register
					Command.CommandMode            = _SDRAM_LOAD_MODE_CMD;
					Command.AutoRefreshNumber      = 1;
					Command.ModeRegisterDefinition = _sdram_Init.BurstLength	|\
													 _sdram_Init.BurstType     	|\
													 _sdram_Init.CASLatency    	|\
													 _sdram_Init.OperationMode 	|\
													 _sdram_Init.WriteBurstMode;
					if(HAL_SDRAM_SendCommand(_sdram_Init.PtrSDRAMHanldeTypeDef, &Command, 0xffff) == HAL_OK)
					{
						//_______________________________Set the device refresh rate
						if(HAL_SDRAM_ProgramRefreshRate(_sdram_Init.PtrSDRAMHanldeTypeDef, _sdram_Init.RefreshRate) == HAL_OK)
						{
							
						}
						else
							errore = true;
					}
					else
						errore = true;
				}
				else
					errore = true;
			}
			else
				errore = true;
		}
		else
			errore = true;
	}
	else
		errore = true;

	if(errore)
	{
		_sdram_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		_sdram_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

//============================================================ PRIVATE FUNCTIONS

static void Delay(uint32_t msDelay)
{  
	uint32_t tickstart = HAL_GetTick();
	
	while((HAL_GetTick() - tickstart) < msDelay);
}
