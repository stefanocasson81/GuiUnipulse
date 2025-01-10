/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione: Driver scrittura in eeprom

	NOTE:
		- Gli address a cui fanno riferimento le funzioni partono da 0.

		- Il ChipSelect DEVE essere pilotato a mano perchè usando le librerie
		  HHAL non è gestita la trasmissione di due pacchetti senza fissare il CS...
 
------------------------------------------------------------------------------*/

//===================================================================== INCLUDES
#include "ARMLib/Device/Flash/_DrvFlashEEProm.h"

//====================================================================== DEFINES

// Comandi specifici per la M95512 e M95M01
#define _DRVFLASHEEPROM_CMD_WREN 	0x06	// Write enable 0000 0110
#define _DRVFLASHEEPROM_CMD_WRDI 	0x04	// Write disable 0000 0100
#define _DRVFLASHEEPROM_CMD_RDSR	0x05	// Read Status register 0000 0101
#define _DRVFLASHEEPROM_CMD_WRSR	0x01	// Write Status register 0000 0001
#define _DRVFLASHEEPROM_CMD_READ	0x03	// Read from Memory array 0000 0011
#define _DRVFLASHEEPROM_CMD_WRITE	0x02	// Write to Memory array 0000 0010

#define _DRVFLASHEEPROM_STATUSREGISTER_SRWD	_B7_	// Status register write protect
#define _DRVFLASHEEPROM_STATUSREGISTER_BP1	_B3_	// Block Protect bit 1
#define _DRVFLASHEEPROM_STATUSREGISTER_BP0	_B2_	// Block Protect bit 0
#define _DRVFLASHEEPROM_STATUSREGISTER_WEL	_B1_	// Write Enable Latch bit
#define _DRVFLASHEEPROM_STATUSREGISTER_WIP	_B0_	// Wrire in Progress bit


//=========================================================== PRIVATE PROTOTYPES
static dvrflash_Status_u GetStatus(void);
static drvflash_Info_t* GetInfo(void);
static int32_t ReadData(uint32_t address, void* ptrData, uint32_t dimData);
static int32_t WriteData(uint32_t address, void *ptrData, uint32_t dimData);
static uint8_t ReadStatusRegister(void);
static int32_t GetSizeSector(uint32_t numSector);
static int32_t GetNumSectorFromAddress(uint32_t address);
static bool IsAddressStartSector(uint32_t address);
static int32_t EraseChip(void);
static int32_t LockChip(bool status);
static int32_t EraseSector(uint32_t numSector);
static int32_t EraseMultipleSector(uint32_t address, uint32_t dimData);
static void DelayMS(uint32_t ms);

//============================================================= STATIC VARIABLES
static drvflash_Info_t Info;

//============================================================= GLOBAL VARIABLES
_drvflasheeprom_Init_t _drvflasheeprom_Init;
const drvflash_Driver_t _drvflasheeprom_Driver =
{
	.GetStatus					= GetStatus,				// Iniquo
	.GetInfo					= GetInfo,					// Iniquo
	.GetSizeSector				= GetSizeSector,			// Iniquo
	.GetNumSectorFromAddress	= GetNumSectorFromAddress,	// Iniquo
	.ReadData					= ReadData,
	.WriteData					= WriteData,
	.EraseSector				= EraseSector,				// Iniquo
	.EraseMultipleSector		= EraseMultipleSector,		// Iniquo
	.IsAddressStartSector		= IsAddressStartSector,		// Iniquo
	.LockChip					= LockChip,
	.EraseChip					= EraseChip,
};

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Descrizione della funzione.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvflasheeprom_InitModule(void)
{
	bool errore = false;
	
#if defined(_STM32H7XX_) || defined(_STM32G4XX_)
	if(!_drvflasheeprom_Init.PtrSpiHandleTypeDef)
		errore = true;
#else
	#error "_DrvFlashEEProm.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	switch(_drvflasheeprom_Init.ChipName)
	{
		default:
			Info.NumeroSettori = 0;
			Info.DimensioneTotale = 0;
			errore = true;
		break;

		case _DRVFLASHEEPROM_CHIPNAME_M95512:
			Info.NumeroSettori = 1;
			Info.DimensioneTotale = 0x10000;
		break;
		case _DRVFLASHEEPROM_CHIPNAME_M95M01:
			Info.NumeroSettori = 1;
			Info.DimensioneTotale = 0x20000;
		break;
	}

	if(errore)
	{
		_drvflasheeprom_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		_drvflasheeprom_Init.InitOk = true;
		return _STATUS_OK_;
	}
}


//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicit� dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void _drvflasheeprom_ZZZ(void)
{
	
}


//============================================================ PRIVATE FUNCTIONS


/*______________________________________________________________________________
 Desc:  Ritorna lo status della periferica
 Arg: - void
 Ret: - dvrflash_Status
______________________________________________________________________________*/
static dvrflash_Status_u GetStatus(void)
{
	dvrflash_Status_u status;

	status.Busy = 0 ;
	status.Error = 0 ;

	return status;
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
 Desc:  Legge dalla flash, ha senso come compatibilità con altre periferiche (es. eeprom)
 Arg: - address: Address di origine flash
		ptrDestData: Ptr destinazione
		dimData: Dimensione dati
 Ret: -	_STATUS_OK_ se ttok
		_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t ReadData(uint32_t address, void* ptrData, uint32_t dimData)
{
#if defined(_STM32H7XX_) || defined(_STM32G4XX_)
	int32_t valRet = _STATUS_OK_;
	uint8_t tmpBuffer[4];

	if(_drvflasheeprom_Init.InitOk)
	{
		if(_drvflasheeprom_Init.PtrFunSetCS)
			_drvflasheeprom_Init.PtrFunSetCS(_ON_);

		tmpBuffer[0] = _DRVFLASHEEPROM_CMD_READ;
		tmpBuffer[1] = (uint8_t)((address >> 16) & 0xff);
		tmpBuffer[2] = (uint8_t)((address >> 8) & 0xff);
		tmpBuffer[3] = (uint8_t)(address & 0xff);
		if(HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef,tmpBuffer,4,1000)==HAL_OK)
		{
			if(HAL_SPI_Receive(_drvflasheeprom_Init.PtrSpiHandleTypeDef,ptrData,dimData,5000)!=HAL_OK)
				valRet = _STATUS_FAIL_;
		}
		else
			valRet = _STATUS_FAIL_;

		if(_drvflasheeprom_Init.PtrFunSetCS)
		{
			_drvflasheeprom_Init.PtrFunSetCS(_OFF_);
			DelayMS(2);
		}
	}
	else
		valRet = _STATUS_FAIL_;

	return valRet;
#else
	#error "_DrvFlashEEProm.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

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
#if defined(_STM32H7XX_) || defined(_STM32G4XX_)
	// NOTA: La scrittura deve considerare il salto pagina da 256 byte...
	int32_t valRet = _STATUS_OK_;
	uint8_t tmpBuffer[4];
	uint32_t nByteToWrite;
	bool errore = false;

	if(_drvflasheeprom_Init.InitOk)
	{
		while(dimData && (!errore))
		{
			// Write Enable
			tmpBuffer[0] = _DRVFLASHEEPROM_CMD_WREN;
			if(_drvflasheeprom_Init.PtrFunSetCS)
				_drvflasheeprom_Init.PtrFunSetCS(_ON_);
			if(HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef, tmpBuffer, 1, 100)!=HAL_OK)
				errore = true;
			if(_drvflasheeprom_Init.PtrFunSetCS)
			{
				_drvflasheeprom_Init.PtrFunSetCS(_OFF_);
				DelayMS(2);
			}


			tmpBuffer[0] = _DRVFLASHEEPROM_CMD_WRITE;
			tmpBuffer[1] = (uint8_t)((address >> 16) & 0xff);
			tmpBuffer[2] = (uint8_t)((address >> 8) & 0xff);
			tmpBuffer[3] = (uint8_t)(address & 0xff);

			if(_drvflasheeprom_Init.PtrFunSetCS)
				_drvflasheeprom_Init.PtrFunSetCS(_ON_);
			if(HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef, tmpBuffer, 4, 1000) == HAL_OK)
			{
				nByteToWrite = 0x100 - (address & 0xff);
				if(nByteToWrite > dimData)
					nByteToWrite = dimData;

				if(HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef, ptrData, nByteToWrite, 5000)==HAL_OK)
				{
					ptrData += nByteToWrite;
					address += nByteToWrite;
					dimData -= nByteToWrite;
				}
				else
					errore = true;
			}
			else
				errore = true;

			if(_drvflasheeprom_Init.PtrFunSetCS)
			{
				_drvflasheeprom_Init.PtrFunSetCS(_OFF_);
				DelayMS(2);
			}

			if(!errore)
				while(ReadStatusRegister()&_DRVFLASHEEPROM_STATUSREGISTER_WIP);
		}

		// Write Disable
		tmpBuffer[0] = _DRVFLASHEEPROM_CMD_WRDI;
		if(_drvflasheeprom_Init.PtrFunSetCS)
			_drvflasheeprom_Init.PtrFunSetCS(_ON_);
		HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef, tmpBuffer, 1, 100);
		if(_drvflasheeprom_Init.PtrFunSetCS)
		{
			_drvflasheeprom_Init.PtrFunSetCS(_OFF_);
			DelayMS(2);
		}
	}
	else
		valRet = _STATUS_FAIL_;

	return valRet;
#else
	#error "_DrvFlashEEProm.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return _STATUS_FAIL_;
}


//______________________________________________________________________________
// Desc:  Legge lo status register attuale
// Arg: - void
// Ret: - Il valore del status register
//______________________________________________________________________________
static uint8_t ReadStatusRegister(void)
{
#if defined(_STM32H7XX_) || defined(_STM32G4XX_)
	uint8_t valRet;

	valRet = _DRVFLASHEEPROM_CMD_RDSR;
	if(_drvflasheeprom_Init.PtrFunSetCS)
		_drvflasheeprom_Init.PtrFunSetCS(_ON_);
	HAL_SPI_Transmit(_drvflasheeprom_Init.PtrSpiHandleTypeDef, &valRet,1, 1000);
	HAL_SPI_Receive(_drvflasheeprom_Init.PtrSpiHandleTypeDef, &valRet,1, 1000);
	if(_drvflasheeprom_Init.PtrFunSetCS)
	{
		_drvflasheeprom_Init.PtrFunSetCS(_OFF_);
		DelayMS(2);
	}

	return valRet;

#else
	#error "_DrvFlashEEProm.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

	return 0;
}

/*______________________________________________________________________________
 Desc:	Ritorna la dimensione in byte del settore di riferimento dell'address passato
 Arg: - address: Address di riferimento settore
 Ret: - Dimensione in byte o _STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t GetSizeSector(uint32_t numSector)
{
	return 0;
}

/*______________________________________________________________________________
 Desc:  Ritorna il numero del settore da 0 dall'address passato
 Arg: - address: Address di riferimento settore
 Ret: - 0+ il numero del settore
				_STATUS_FAIL_ se address furoi range
______________________________________________________________________________*/
static int32_t GetNumSectorFromAddress(uint32_t address)
{
	return 0;
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
	return false;
}

/*______________________________________________________________________________
 Desc:  Esegue un erase completo del chip
 Arg: - void

 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t EraseChip(void)
{
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
	return _STATUS_FAIL_;
}

/*______________________________________________________________________________
 Desc:  Funzione specifica per erasare un settore partendo da 0
 Arg: - numSector: Numero del settore da erasare
 Ret: - _STATUS_OK_ se ttok
		_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t EraseSector(uint32_t numSector)
{
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
	return _STATUS_FAIL_;
}

static void DelayMS(uint32_t ms)
{
	uint8_t uCA = 200;
	while(uCA--);

/*
	uint32_t tickStart;
	tickStart = HAL_GetTick();
	while(HAL_GetTick() - tickStart < ms);
	*/
}

