/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione: Gestione flash in OCTA SPI dual bank del STM32H730 per cui vengono
 	 	 	  rese disponibili le primitive per cancellare e scrivere settori.
			  Attenzione, i comandi sono diretti e distruttivi nel senso che
			  se si indica di operare in una zona dove risiede il firmware,
			  semplicemente viene fottuto!!!
			  Attualmente vengono supportate solamente le libreria HAL.
			  L'init del modulo configura una dual port QSPI DTR (Double Transfer Raw)
			  e quindi la dimensione settori e massima di archiviazione riguarda
			  l'unione dei due chip gemelli.
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include <ARMLib/Device/Flash/_DrvFlashOSPI.h>

//====================================================================== DEFINES

#define _DRVFLASHOSPI_W25Q_WRITEENABLE 					0x06
#define _DRVFLASHOSPI_W25Q_VOLATILESRWRITEENABLE 		0x50
#define _DRVFLASHOSPI_W25Q_WRITEDISABLE 				0x04

#define _DRVFLASHOSPI_W25Q_RELEASEPOWERDOWN				0xab
#define _DRVFLASHOSPI_W25Q_MANUFACTURER					0x90
#define _DRVFLASHOSPI_W25Q_JEDEC						0x9f
#define _DRVFLASHOSPI_W25Q_READUNIQUEID					0x4b

#define _DRVFLASHOSPI_W25Q_READDATA						0x03
#define _DRVFLASHOSPI_W25Q_FASTREAD						0x0b

#define _DRVFLASHOSPI_W25Q_PAGEPROGRAM					0x02

#define _DRVFLASHOSPI_W25Q_SECTORERASE_4KB				0x20
#define _DRVFLASHOSPI_W25Q_BLOCKERASE_32KB				0x52
#define _DRVFLASHOSPI_W25Q_BLOCKERASE_64KB				0xd8
#define _DRVFLASHOSPI_W25Q_CHIPERASE					0xc7	// 0x60

#define _DRVFLASHOSPI_W25Q_READSTATUSREGISTER_1			0x05
#define _DRVFLASHOSPI_W25Q_WRITESTATUSREGISTER_1		0x01
#define _DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2 		0x35
#define _DRVFLASHOSPI_W25Q_WRITESTATUSREGISTER_2 		0x31
#define _DRVFLASHOSPI_W25Q_READSTATUSREGISTER_3 		0x15
#define _DRVFLASHOSPI_W25Q_WRITESTATUSREGISTER_3		0x11

#define _DRVFLASHOSPI_W25Q_READSFDPREGISTER				0x5a
#define _DRVFLASHOSPI_W25Q_ERASESECURITYREGISTER		0x44
#define _DRVFLASHOSPI_W25Q_PROGRAMSECURITYREGISTER		0x42
#define _DRVFLASHOSPI_W25Q_READSECURITYREGISTER			0x48

#define _DRVFLASHOSPI_W25Q_GLOBALBLOCKLOCK				0x7e
#define _DRVFLASHOSPI_W25Q_GLOBALBLOCKUNLOCK			0x98
#define _DRVFLASHOSPI_W25Q_READBLOCKLOCK				0x3d
#define _DRVFLASHOSPI_W25Q_INDIVIDUALBLOCKLOCK			0x36
#define _DRVFLASHOSPI_W25Q_INDIVIDUALBLOCKUNLOCK		0x39
#define _DRVFLASHOSPI_W25Q_ERASEPROGRAMSUSPEND			0x75
#define _DRVFLASHOSPI_W25Q_ERASEPROGRAMRESUME			0x7a
#define _DRVFLASHOSPI_W25Q_POWERDOWN					0xb9
#define _DRVFLASHOSPI_W25Q_ENABLERESET					0x66
#define _DRVFLASHOSPI_W25Q_RESETDEVICE					0x99

#define _DRVFLASHOSPI_W25Q_FASTREADQUADIO				0xeb

#define _DRVFLASHOSPI_W25Q_QUADINPUTPAGEPROGRAM			0x32


//=========================================================== PRIVATE PROTOTYPES
static int32_t ChipAutoPollingMemReady(void);
//static int32_t ChipAutoPollingWEL(void);
static int32_t ChipEnterQPI(void);
static int32_t ChipEnableMemoryMappedMode_4L(void);
static int32_t ChipWriteData_4L(uint32_t address, void *ptrData, uint32_t dimData);
static int32_t GetSizeSector(uint32_t numSector);
static int32_t ReadData(uint32_t address, void *ptrData, uint32_t dimData);
static int32_t WriteData(uint32_t address, void *ptrData, uint32_t dimData);
static int32_t EraseSector(uint32_t numSector);
static int32_t GetNumSectorFromAddress(uint32_t address);
static int32_t EraseMultipleSector(uint32_t address, uint32_t dimData);
static dvrflash_Status_u GetStatus(void);
static drvflash_Info_t* GetInfo(void);
static bool IsAddressStartSector(uint32_t address);
static int32_t ReadRegister(uint32_t registro,uint8_t *ptrData,uint32_t lenData,bool qpiMode,uint32_t address);
static int32_t WriteRegister(uint32_t registro,uint8_t *ptrData,uint32_t lenData,bool qpiMode,uint32_t address);
static int32_t LockChip(bool status);
static int32_t EraseChip(void);

//============================================================= STATIC VARIABLES
static dvrflash_Status_u Status;
static drvflash_Info_t Info;

//============================================================= GLOBAL VARIABLES
_drvflashospi_Init_t _drvflashospi_Init;
drvflash_Driver_t _drvflashospi_Driver =
{
	.GetStatus					= GetStatus,
	.GetInfo					= GetInfo,
	.GetSizeSector				= GetSizeSector,
	.GetNumSectorFromAddress	= GetNumSectorFromAddress,
	.ReadData					= ReadData,
	.WriteData					= WriteData,
	.EraseSector				= EraseSector,
	.EraseMultipleSector		= EraseMultipleSector,
	.IsAddressStartSector		= IsAddressStartSector,
	.LockChip					= LockChip,
	.EraseChip					= EraseChip,
};

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Descrizione della funzione.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvflashospi_InitModule(void)
{
	bool errore = false;

	Status.Busy = false;
	Status.Error = false;

	// Un minimo di controllo sull'init parametri
	if(_drvflashospi_Init.SectorSize && _drvflashospi_Init.NumSectors && _drvflashospi_Init.AddressMapping && _drvflashospi_Init.PtrOspiHandleTypeDef)
	{
		Info.NumeroSettori =_drvflashospi_Init.NumSectors;
		Info.DimensioneTotale =_drvflashospi_Init.NumSectors *_drvflashospi_Init.SectorSize;


#if defined(_STM32H7XX_)
		if(!_drvflashospi_Init.PtrOspiHandleTypeDef)
			errore = true;    // No handle qspi no party

#else
		#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

		if(!errore)
		{
			if(_drvflashospi_ChipResetMemory() != _STATUS_OK_)
				errore = true;
		}
	}
	else
		errore = true;

	if(errore)
	{
		_drvflashospi_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		_drvflashospi_Init.InitOk = true;

		return _STATUS_OK_;
	}
}

/*______________________________________________________________________________
 Desc:  Mappa in memoria la flash
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvflashospi_GoMemoryMapping(void)
{
	bool errore = false;

	// QSPI memory reset
	if(_drvflashospi_ChipResetMemory() != _STATUS_OK_)
		errore = true;
	else
	{
		if(ChipEnableMemoryMappedMode_4L() != _STATUS_OK_)
			errore = true;
	}

	if(errore)
		return _STATUS_FAIL_;
	else
		return _STATUS_OK_;
}

/*______________________________________________________________________________
 Desc:  Mappa in memoria la flash
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvflashospi_ReadJedecID(void)
{
	int32_t valRet = _STATUS_OK_;


	/*
	QSPI_CommandTypeDef regularCmd;

	// Send the command
	regularCmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	regularCmd.Instruction     = 0x9f;
	regularCmd.AddressMode     = QSPI_ADDRESS_4_LINES;
	regularCmd.DataMode        = QSPI_DATA_4_LINES;

	regularCmd.AddressSize       = QSPI_ADDRESS_32_BITS;
	//regularCmd.Address           = address;
	//regularCmd.NbData            = dimData;
	regularCmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	regularCmd.DummyCycles       = 0;
	regularCmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
	regularCmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	regularCmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

*/




	return valRet;
}

/*______________________________________________________________________________
 Desc:  This function reset the QSPI memory.
 Arg: - void
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
int32_t _drvflashospi_ChipResetMemory(void)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_FAIL_;

	// Necessario per liberare il busy bit ed uscire dal memorymapped mode
	HAL_OSPI_Abort(_drvflashospi_Init.PtrOspiHandleTypeDef);

	if(WriteRegister(_DRVFLASHOSPI_W25Q_ENABLERESET,_NULL_,0,false,0xffffffff)==_STATUS_OK_)
	{
		HAL_Delay(1);
		if(WriteRegister(_DRVFLASHOSPI_W25Q_RESETDEVICE, _NULL_, 0, false,0xffffffff)==_STATUS_OK_)
		{
			HAL_Delay(1);

			valRet = ChipEnterQPI();
		}
	}

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}





//============================================================ PRIVATE FUNCTIONS


/*______________________________________________________________________________ 
 Desc:	Ritorna la dimensione in byte del settore di riferimento dell'address passato
 Arg: - address: Address di riferimento settore
 Ret: - Dimensione in byte o _STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t GetSizeSector(uint32_t numSector)
{
	if(numSector < Info.NumeroSettori)
		return (int32_t)_drvflashospi_Init.SectorSize;
	else
		return _STATUS_FAIL_;
}

/*______________________________________________________________________________
 Desc:  Ritorna il numero del settore da 0 dall'address passato
 Arg: - address: Address di riferimento settore
 Ret: - 0+ il numero del settore
				_STATUS_FAIL_ se address furoi range
______________________________________________________________________________*/
static int32_t GetNumSectorFromAddress(uint32_t address)
{
	int32_t valRet = _STATUS_FAIL_;

	if(_drvflashospi_Init.InitOk)
	{
		if(address >=_drvflashospi_Init.AddressMapping)
			address -=_drvflashospi_Init.AddressMapping;

		valRet = address /_drvflashospi_Init.SectorSize;
	}

	return valRet;
}


/*______________________________________________________________________________
 Desc:  Legge dalla flash, ha senso come compatibilità con altre periferiche (es. eeprom)
 Arg: - address: Address di origine flash
		ptrDestData: Ptr destinazione
		dimData: Dimensione dati
 Ret: -	_STATUS_OK_ se ttok
		_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t ReadData(uint32_t address, void* ptrData, uint32_t dimData)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef     regularCmd;

	// ATTENZIONE: la variabile address qui cambia significato. l'address DEVE
	// essere riferito a zero in questa funzione...
	if(address >=_drvflashospi_Init.AddressMapping)
		address -=_drvflashospi_Init.AddressMapping;

	// Se dispari 1 in più
	if(dimData&_B0_)
		dimData++;

	memset(&regularCmd,0,sizeof(regularCmd));

	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;

	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;

	regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_FASTREADQUADIO;

	regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;

	/*originale funzionante
	regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_4_LINES;
	regularCmd.Address				= (address<<8)|0xff;	// Attenzione serve un 32bit address....!!!!!!
	regularCmd.AddressSize			= HAL_OSPI_ADDRESS_32_BITS;
	//regularCmdnd.AddressDtrMode

	regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE;
	//regularCmdnd.AlternateBytes
	//regularCmdnd.AlternateBytesSize
	//regularCmdnd.AlternateBytesDtrMode
	 */
	regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_4_LINES;
	regularCmd.Address				= address;
	regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	//regularCmdnd.AddressDtrMode

	regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_4_LINES;
	regularCmd.AlternateBytes       = 0xff;
	regularCmd.AlternateBytesSize	= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	//regularCmdnd.AlternateBytesDtrMode



	regularCmd.DataMode 			= HAL_OSPI_DATA_4_LINES;
	regularCmd.NbData				= dimData;
	//regularCmdnd.DataDtrMode

	regularCmd.DummyCycles 			= 4;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;

	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	if(HAL_OSPI_Receive(_drvflashospi_Init.PtrOspiHandleTypeDef,(uint8_t*)ptrData,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	return valRet;

#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}

/*______________________________________________________________________________
 Desc:  Scrive il settore interessato dall'address da origine dati e dimensione
 Arg: - address: Address di riferimento settore
				ptrData: Orgine dati
				imData: Dimensione dati
 Ret: -	_STATUS_OK_ se ttok
				_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t WriteData(uint32_t address, void *ptrData, uint32_t dimData)
{
	uint32_t scarto = address & (_drvflashospi_Init.PageSizeWrite-1);
	uint32_t stepDimData;
	uint8_t *ptrLocalData = (uint8_t *)ptrData;
	int32_t valRet = _STATUS_OK_;

	// Valutazione overlap page-size
	while(dimData)
	{
		if((scarto + dimData) <= _drvflashospi_Init.PageSizeWrite)
			stepDimData = dimData;
		else
			stepDimData = _drvflashospi_Init.PageSizeWrite - scarto;

		valRet = ChipWriteData_4L(address, ptrLocalData, stepDimData);
		address += stepDimData;
		ptrLocalData += stepDimData;
		dimData -= stepDimData;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Informa se l'address passato è all'inizio di un settore, serve per
		capire se è il caso di erasare il settore per una successiva scrittura
 Arg: - address: Address di riferimento
 Ret: -	true se primo address di un settore
		false altrimenti
______________________________________________________________________________*/
static bool IsAddressStartSector(uint32_t address)
{
	bool valRet = false;

	if(_drvflashospi_Init.InitOk)
	{
		if(address >=_drvflashospi_Init.AddressMapping)
			address -=_drvflashospi_Init.AddressMapping;

		if(address %_drvflashospi_Init.SectorSize)
			valRet = false;
		else
			valRet = true;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Scrive il settore interessato dall'address da origine dati e dimensione
 Arg: - address: Address di riferimento settore
				ptrData: Orgine dati
				imData: Dimensione dati
 Ret: -	_STATUS_OK_ se ttok
				_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t ChipWriteData_4L(uint32_t address, void *ptrData, uint32_t dimData)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef regularCmd;

	// ATTENZIONE: la variabile address qui cambia significato. l'address DEVE
	// essere riferito a zero in questa funzione...
	if(address >=_drvflashospi_Init.AddressMapping)
		address -=_drvflashospi_Init.AddressMapping;

	// Se dispari 1 in più
	if(dimData&_B0_)
		dimData++;

	if(_drvflashospi_Init.InitOk)
	{
		Status.Busy = true;

		// Enable write operations
		if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE,_NULL_,0,false,0xffffffff) != _STATUS_OK_)
			valRet = _STATUS_FAIL_;
		else
		{
			memset(&regularCmd,0,sizeof(regularCmd));
			regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;
			regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;
			regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_QUADINPUTPAGEPROGRAM;
			regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
			regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
			regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
			regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_1_LINE;
			regularCmd.Address				= address;
			regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
			//regularCmd.AddressDtrMode
			regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE;
			//regularCmd.AlternateBytes
			//regularCmd.AlternateBytesSize
			//regularCmd.AlternateBytesDtrMode
			regularCmd.DataMode 			= HAL_OSPI_DATA_4_LINES;
			regularCmd.NbData				= dimData;
			//regularCmd.DataDtrMode
			regularCmd.DummyCycles 			= 0;
			regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
			regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;


			if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
				valRet = _STATUS_FAIL_;
			else
			{
				if(HAL_OSPI_Transmit(_drvflashospi_Init.PtrOspiHandleTypeDef, (uint8_t*)ptrData,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
					valRet = _STATUS_FAIL_;
			}

			ChipAutoPollingMemReady();

			Status.Busy = false;
		}
	}
	else
		valRet = _STATUS_FAIL_;

	if(valRet == _STATUS_FAIL_)
		Status.Error = true;

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}


/*______________________________________________________________________________ 
 Desc:  Erase dei settore interessati dall'address e per il dimData necessario
 Arg: - address: Address di riferimento settore
 Ret: - 0+ il numero di byte
				_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t EraseMultipleSector(uint32_t address, uint32_t dimData)
{
	int32_t valRet = _STATUS_OK_;
	int32_t settore;

	if(_drvflashospi_Init.InitOk)
	{
		Status.Busy = true;

		while(dimData)
		{
			settore = GetNumSectorFromAddress(address);
			if(settore == _STATUS_FAIL_)
				valRet = _STATUS_FAIL_;
			else
			{
				if(EraseSector(settore) == _STATUS_OK_)
				{
					address +=_drvflashospi_Init.SectorSize;
					if(dimData >_drvflashospi_Init.SectorSize)
						dimData -=_drvflashospi_Init.SectorSize;
					else
						dimData = 0;
				}
				else
				{
					valRet = _STATUS_FAIL_;
					break;
				}
			}
		}

		Status.Busy = false;
	}
	else
		valRet = _STATUS_FAIL_;

	if(valRet == _STATUS_FAIL_)
		Status.Error = true;

	return valRet;
}


/*______________________________________________________________________________
 Desc:  Funzione specifica per erasare un settore partendo da 0
 Arg: - numSector: Numero del settore da erasare
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t EraseSector(uint32_t numSector)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_FAIL_;
	OSPI_RegularCmdTypeDef     regularCmd;

	if(_drvflashospi_Init.InitOk)
	{
		Status.Busy = true;

		if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE,_NULL_,0,false,0xffffffff)==_STATUS_OK_)
		{
			memset(&regularCmd,0,sizeof(regularCmd));

			regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;

			regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;

			regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_BLOCKERASE_64KB;

			regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
			regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
			regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;

			regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_1_LINE;
			regularCmd.Address				= numSector*_drvflashospi_Init.SectorSize;	// Occhio questo chip accetta address e non settori...
			regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
			//regularCmdnd.AddressDtrMode

			regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE;
			//regularCmdnd.AlternateBytes
			//regularCmdnd.AlternateBytesSize
			//regularCmdnd.AlternateBytesDtrMode

			regularCmd.DataMode 			= HAL_OSPI_DATA_NONE;
			//regularCmd.NbData
			//regularCmd.DataDtrMode

			regularCmd.DummyCycles 			= 0;
			regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
			regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;

			if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) == HAL_OK)
			{
				if(ChipAutoPollingMemReady() == _STATUS_OK_)
					valRet = _STATUS_OK_;
			}
		}
	}

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}

/*______________________________________________________________________________
 Desc:  Ritorna lo status della periferica
 Arg: - void
 Ret: - dvrflash_Status
______________________________________________________________________________*/
static dvrflash_Status_u GetStatus(void)
{
	return Status;
}

/*______________________________________________________________________________
 Desc:  Ritorna info della periferica
 Arg: - void
 Ret: - drvflash_Info*
______________________________________________________________________________*/
static drvflash_Info_t* GetInfo(void)
{
	return &Info;
}

/*______________________________________________________________________________
 Desc:  Mette in autopolling mem ready ma in octo 8 lines (che DEVE essre già configurato)
 Arg: - void
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t ChipAutoPollingMemReady(void)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef regularCmd;
	OSPI_AutoPollingTypeDef autoPolling;

	memset(&regularCmd,0,sizeof(regularCmd));
	memset(&autoPolling,0,sizeof(autoPolling));

	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;
	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;
	regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_READSTATUSREGISTER_1;
	regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_NONE;
/*	regularCmd.Address				= 0x00;
	regularCmd.AddressSize			= HAL_OSPI_ADDRESS_32_BITS;
	regularCmd.AddressDtrMode		= HAL_OSPI_ADDRESS_DTR_DISABLE;*/
	regularCmd.AlternateBytes		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	//regularCmdnd.AlternateBytesSize
	//regularCmdnd.AlternateBytesDtrMode
	regularCmd.DataMode 			= HAL_OSPI_DATA_1_LINE;
	regularCmd.NbData				= 1;
	regularCmd.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE;
	regularCmd.DummyCycles 			= 0;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;
	// Send Write Enable command in Octal mode
	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;
	
	autoPolling.Match				= 0x0000;
	autoPolling.Mask				= 0x0101;	// BUSY bit (_B0_) Ripetuto
	autoPolling.MatchMode			= HAL_OSPI_MATCH_MODE_AND;
	autoPolling.AutomaticStop		= HAL_OSPI_AUTOMATIC_STOP_ENABLE;
	autoPolling.Interval			= 0x10;
	if(HAL_OSPI_AutoPolling(_drvflashospi_Init.PtrOspiHandleTypeDef, &autoPolling, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}

/*______________________________________________________________________________
 Desc:  Mette in autopolling Write Enable Latch ma in octo 8 lines (che DEVE essre già configurato)
 Arg: - void
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
/*
static int32_t ChipAutoPollingWEL(void)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef regularCmd;
	OSPI_AutoPollingTypeDef autoPolling;

	memset(&regularCmd,0,sizeof(regularCmd));
	memset(&autoPolling,0,sizeof(autoPolling));

	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;
	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;
	regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_READSTATUSREGISTER_1;
	regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_NONE;
//	regularCmd.Address				= 0x00;
//	regularCmd.AddressSize			= HAL_OSPI_ADDRESS_32_BITS;
//	regularCmd.AddressDtrMode		= HAL_OSPI_ADDRESS_DTR_DISABLE;
	regularCmd.AlternateBytes		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	//regularCmdnd.AlternateBytesSize
	//regularCmdnd.AlternateBytesDtrMode
	regularCmd.DataMode 			= HAL_OSPI_DATA_1_LINE;
	regularCmd.NbData				= 1;
	regularCmd.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE;
	regularCmd.DummyCycles 			= 0;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;
	// Send Write Enable command in Octal mode
	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	autoPolling.Match				= 0x0202;
	autoPolling.Mask				= 0x0202;	// WEL bit (_B1_) Ripetuto
	autoPolling.MatchMode			= HAL_OSPI_MATCH_MODE_AND;
	autoPolling.AutomaticStop		= HAL_OSPI_AUTOMATIC_STOP_ENABLE;
	autoPolling.Interval			= 0x10;
	if(HAL_OSPI_AutoPolling(_drvflashospi_Init.PtrOspiHandleTypeDef, &autoPolling, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}
*/

/*______________________________________________________________________________
 Desc:  Legge un registro
 Arg: - register: resgistro da leggere
 	 	ptrData: Ptr al buffer in cui depositare
 	 	lenData: lunghezza del dato
 	 	qpiMode: false Se gestire qspi(1 linea) oppure a true per qpi a 4 bit
 	 	address: se address==0xffffffff è inteso senza address...

 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t ReadRegister(uint32_t registro,uint8_t *ptrData,uint32_t lenData,bool qpiMode,uint32_t address)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef     regularCmd;

	memset(&regularCmd,0,sizeof(regularCmd));
	
	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;

	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;

	regularCmd.Instruction 			= registro;

	if(qpiMode)
		regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_4_LINES;
	else
		regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;

	if(address==0xffffffff)
	{
		regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_NONE;
		//regularCmdnd.Address
		//regularCmdnd.AddressSize
		//regularCmdnd.AddressDtrMode
	}
	else
	{
		regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_1_LINE;
		regularCmd.Address				= address;
		regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	}

	regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE;
	//regularCmdnd.AlternateBytes
	//regularCmdnd.AlternateBytesSize
	//regularCmdnd.AlternateBytesDtrMode

	if(lenData)
	{
		if(qpiMode)
			regularCmd.DataMode 			= HAL_OSPI_DATA_4_LINES;
		else
			regularCmd.DataMode 			= HAL_OSPI_DATA_1_LINE;
		regularCmd.NbData				= lenData;
	}
	else
	{
		regularCmd.DataMode 			= HAL_OSPI_DATA_NONE;
		regularCmd.NbData				= 0;
	}
	//regularCmdnd.DataDtrMode

	regularCmd.DummyCycles 			= 0;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;

	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	if(lenData)
	{
		if(HAL_OSPI_Receive(_drvflashospi_Init.PtrOspiHandleTypeDef,ptrData,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			valRet = _STATUS_FAIL_;
	}

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}



/*______________________________________________________________________________
 Desc:  Srive un registro
 Arg: - register: resgistro da leggere
 	 	ptrData: Ptr al buffer da cui leggere
 	 	lenData: lunghezza del dato
 	 	qpiMode: false Se gestire qspi(1 linea) oppure a true per qpi a 4 bit

 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t WriteRegister(uint32_t registro,uint8_t *ptrData,uint32_t lenData,bool qpiMode,uint32_t address)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef     regularCmd;

	memset(&regularCmd,0,sizeof(regularCmd));
	
	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_COMMON_CFG;
	
	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;
	
	regularCmd.Instruction 			= registro;

	if(qpiMode)
		regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_4_LINES;
	else
		regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;

	if(address==0xffffffff)
	{
		regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_NONE;
		//regularCmdnd.Address
		//regularCmdnd.AddressSize
		//regularCmdnd.AddressDtrMode
	}
	else
	{
		if(qpiMode)
			regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_1_LINE;
		else
			regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_4_LINES;
		regularCmd.Address				= address;
		regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	}

	regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE;
	//regularCmdnd.AlternateBytes
	//regularCmdnd.AlternateBytesSize
	//regularCmdnd.AlternateBytesDtrMode

	if(lenData)
	{
		if(qpiMode)
			regularCmd.DataMode 			= HAL_OSPI_DATA_4_LINES;
		else
			regularCmd.DataMode 			= HAL_OSPI_DATA_1_LINE;
		regularCmd.NbData				= lenData;
	}
	else
	{
		regularCmd.DataMode 			= HAL_OSPI_DATA_NONE;
		regularCmd.NbData				= 0;
	}
	//regularCmdnd.DataDtrMode

	regularCmd.DummyCycles 			= 0;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;

	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;

	if(lenData)
	{
		if(HAL_OSPI_Transmit(_drvflashospi_Init.PtrOspiHandleTypeDef,ptrData,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			valRet = _STATUS_FAIL_;
	}

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}


/*______________________________________________________________________________
 Desc:  Lock o unlocka il chip in scrittura
 Arg: - status: true==blocca, false=sblocca

 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t LockChip(bool status)
{
	int32_t valRet = _STATUS_FAIL_;

	/*
	uint32_t uLA=0xffffffff;
	uint8_t uCA;

	uint32_t addressRef = 0;





	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READBLOCKLOCK, (uint8_t*)&uLA, 1, false,addressRef);
*/

	if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE, _NULL_,0, false,0xffffffff)==_STATUS_OK_)
	{
		if(status)
			valRet = WriteRegister(_DRVFLASHOSPI_W25Q_GLOBALBLOCKLOCK, _NULL_, 0, false,0xffffffff);
		else
			valRet = WriteRegister(_DRVFLASHOSPI_W25Q_GLOBALBLOCKUNLOCK, _NULL_, 0, false,0xffffffff);
	}
/*
	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READBLOCKLOCK, (uint8_t*)&uLA, 1, false,addressRef);


*/

/*




	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2, (uint8_t*)&uLA, 1, false,0xffffffff);


	WriteRegister(0x50, _NULL_,0, false,0xffffffff);
	if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE, _NULL_,0, false,0xffffffff)==_STATUS_OK_)
	{
		uCA = 0;
		WriteRegister(_DRVFLASHOSPI_W25Q_WRITESTATUSREGISTER_2, &uCA, 1, false,0xffffffff);
	}

	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2, (uint8_t*)&uLA, 1, false,0xffffffff);






	//niknik prove
	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_3, (uint8_t*)&uLA, 1, false,0xffffffff);

	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READBLOCKLOCK, (uint8_t*)&uLA, 1, false,addressRef);

	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2, (uint8_t*)&uLA, 1, false,0xffffffff);


	if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE, _NULL_,0, false,0xffffffff)==_STATUS_OK_)
	{
		WriteRegister(_DRVFLASHOSPI_W25Q_INDIVIDUALBLOCKUNLOCK, _NULL_,0, false,addressRef);
	}

	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2, (uint8_t*)&uLA, 1, false,0xffffffff);


	uLA=0xffffffff;
	ReadRegister(_DRVFLASHOSPI_W25Q_READBLOCKLOCK, (uint8_t*)&uLA, 1, false,addressRef);


*/






	return valRet;
}

/*______________________________________________________________________________
 Desc:  Esegue un erase completo del chip
 Arg: - void

 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t EraseChip(void)
{
	int32_t valRet = _STATUS_OK_;


	/*
	//niknik da implementare
	int32_t valRet = _STATUS_FAIL_;

	if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE, _NULL_,0, false)==_STATUS_OK_)
	{
		if(status)
			valRet = WriteRegister(_DRVFLASHOSPI_W25Q_GLOBALBLOCKLOCK, _NULL_, 0, false);
		else
			valRet = WriteRegister(_DRVFLASHOSPI_W25Q_GLOBALBLOCKUNLOCK, _NULL_, 0, false);
	}
	*/

	return valRet;
}


/*______________________________________________________________________________
 Desc:  This function enter the QPSI memory in QPI mode
 Arg: - void
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t ChipEnterQPI(void)
{
	int32_t valRet = _STATUS_OK_;
	uint16_t uSA;
	
	ReadRegister(_DRVFLASHOSPI_W25Q_READSTATUSREGISTER_2, (uint8_t*)&uSA, 1, false,0xffffffff);
	
	if(!(uSA&0x0202))
	{	// Se il QE di entrambi i chip non sono attivi, li attiva in non volatile mode
		if(WriteRegister(_DRVFLASHOSPI_W25Q_WRITEENABLE, _NULL_,0, false,0xffffffff)==_STATUS_OK_)
		{
			uSA = 0x0202;
			valRet = WriteRegister(_DRVFLASHOSPI_W25Q_WRITESTATUSREGISTER_2, (uint8_t*)&uSA,1, false,0xffffffff);
		}
		else
			valRet = _STATUS_FAIL_;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Configure the QSPI in memory-mapped mode
 Arg: - void
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t ChipEnableMemoryMappedMode_4L(void)
{
#if defined(_STM32H7XX_)
	int32_t valRet = _STATUS_OK_;
	OSPI_RegularCmdTypeDef regularCmd;
	OSPI_MemoryMappedTypeDef memoryMapped;

	memset(&regularCmd,0,sizeof(regularCmd));
	memset(&memoryMapped,0,sizeof(memoryMapped));

	regularCmd.OperationType 		= HAL_OSPI_OPTYPE_READ_CFG;
	regularCmd.FlashId 				= HAL_OSPI_FLASH_ID_1;
	regularCmd.Instruction 			= _DRVFLASHOSPI_W25Q_FASTREADQUADIO;
	regularCmd.InstructionMode 		= HAL_OSPI_INSTRUCTION_1_LINE;
	regularCmd.InstructionSize 		= HAL_OSPI_INSTRUCTION_8_BITS;
	regularCmd.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	regularCmd.AddressMode 			= HAL_OSPI_ADDRESS_4_LINES;
	//regularCmdnd.Address
	regularCmd.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	//regularCmdnd.AddressDtrMode
	regularCmd.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_4_LINES;
	regularCmd.AlternateBytes		= 0xff;
	regularCmd.AlternateBytesSize	= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	//regularCmd.AlternateBytesDtrMode
	regularCmd.DataMode 			= HAL_OSPI_DATA_4_LINES;
	//regularCmd.NbData
	//regularCmd.DataDtrMode
	regularCmd.DummyCycles 			= 4;
	regularCmd.DQSMode 				= HAL_OSPI_DQS_DISABLE;
	regularCmd.SIOOMode 			= HAL_OSPI_SIOO_INST_EVERY_CMD;
	/* Send Write Enable command in Octal mode */
	if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		valRet = _STATUS_FAIL_;
	else
	{
		regularCmd.OperationType 		= HAL_OSPI_OPTYPE_WRITE_CFG;
		regularCmd.Instruction			= _DRVFLASHOSPI_W25Q_PAGEPROGRAM;
		regularCmd.DummyCycles 			= 0;
		if(HAL_OSPI_Command(_drvflashospi_Init.PtrOspiHandleTypeDef, &regularCmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			valRet = _STATUS_FAIL_;
		else
		{
			memoryMapped.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
			if(HAL_OSPI_MemoryMapped(_drvflashospi_Init.PtrOspiHandleTypeDef,&memoryMapped) != HAL_OK)
				valRet = _STATUS_FAIL_;
		}
	}

	return valRet;
#else
	#error "_DrvFlashOSPI.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}




