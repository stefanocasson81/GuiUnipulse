/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione: Gestione flash interna del micro in Raw Mode per cui vengono rese
			  disponibili le primitive per cancellare e scrivere settori.
			  Attenzione, i comandi sono diretti e distruttivi nel senso che
			  se si indica di operare in una zona dove risiede il firmware,
			  semplicemente viene fottuto!!!
			  Attualmente vengono supportate solamente le libreria HAL.
			  MicroP supportati:
			  	  	  STM32F373C8
			  	  	  STM32F405xx

 
 USO e ABUSO del modulo
 
 * _drvflashmcu_InitModule()
 
	Tipica inizializzazione del modulo. Per il momento è anche inutile.
 
 A seuire le funzioni driver utilizzabili....
 
 * _drvflashmcu_Driver.EraseSector(uint32_t address, uint32_t dimData);
	Erase dei blocchi necessari a partire dall'address fisico passato e per dimData
	byte. Esempio:
			_drvflashmcu_Driver.EraseSector (0x00078000,123);
 
 * _drvflashmcu_Driver.WriteData(uint32_t address, void *ptrData, uint32_t dimData);
  	Scrittura sui blocchi specificati dall'address.


**************************************NOTE**************************************
	
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include <ARMLib/Device/Flash/_DrvFlashMcu.h>

#if defined(STM32H750xx) || defined(STM32H743xx)
#elif defined(STM32F405xx)
#elif defined(STM32F373xC)
#elif defined(STM32G474xx)
#else
	#error "_DrvMcuFlash.c : Microprocessore NON supportato. Implementarlo!!!"
#endif

//====================================================================== DEFINES
typedef struct
{
	uint32_t StartAddress;
	uint32_t Dimensione;
}_drvflashmcu_Sector_t; 

#if defined(STM32H750xx)
	static const _drvflashmcu_Sector_t StrutturaSettori[] =	// 128K totali
	{
		{	0x08000000,	0x00020000	},	// 128K
	};
#elif defined(STM32H743xx)
	static const _drvflashmcu_Sector_t StrutturaSettori[] =	// 2M totali
	{
		{	0x08000000,	0x00020000	},	// 128K
		{	0x08020000,	0x00020000	},	// 128K
		{	0x08040000,	0x00020000	},	// 128K
		{	0x08060000,	0x00020000	},	// 128K
		{	0x08080000,	0x00020000	},	// 128K
		{	0x080a0000,	0x00020000	},	// 128K
		{	0x080c0000,	0x00020000	},	// 128K
		{	0x080e0000,	0x00020000	},	// 128K
		{	0x08100000,	0x00020000	},	// 128K
		{	0x08120000,	0x00020000	},	// 128K
		{	0x08140000,	0x00020000	},	// 128K
		{	0x08160000,	0x00020000	},	// 128K
		{	0x08180000,	0x00020000	},	// 128K
		{	0x081a0000,	0x00020000	},	// 128K
		{	0x081c0000,	0x00020000	},	// 128K
		{	0x081e0000,	0x00020000	},	// 128K
	};
#elif defined(STM32F405xx)
	static const _drvflashmcu_Sector_t StrutturaSettori[] =	// 2M totali
	{
		{	0x08000000,	0x00004000	},	// 16K
		{	0x08004000,	0x00004000	},	// 16K
		{	0x08008000,	0x00004000	},	// 16K
		{	0x0800c000,	0x00004000	},	// 16K
		{	0x08010000,	0x00010000	},	// 64K
		{	0x08020000,	0x00020000	},	// 128K
		{	0x08040000,	0x00020000	},	// 128K
		{	0x08060000,	0x00020000	},	// 128K
		{	0x08080000,	0x00020000	},	// 128K
		{	0x080a0000,	0x00020000	},	// 128K
		{	0x080c0000,	0x00020000	},	// 128K
		{	0x080e0000,	0x00020000	},	// 128K
	};
#elif defined(STM32F373xC)

#elif defined(STM32G474xx)
	// NOTA: i parametri fanno riferimeto al dual bank mode attivo di default

	#define _DRVFLASHMCU_DIMFLASH				0x00080000			// 512KB
	#define _DRVFLASHMCU_DIMFIXSECTOR 			0x800				// 4KB
	#define _DRVFLASHMCU_NBYTEPAGINADISCRITTURA	8
#endif


//=========================================================== PRIVATE PROTOTYPES

static int32_t GetSizeSector(uint32_t numSector);

static int32_t ReadData(uint32_t address, void *ptrData, uint32_t dimData);
static int32_t WriteData(uint32_t address, void *ptrData, uint32_t dimData);
static int32_t EraseSector(uint32_t numSector);
static int32_t GetNumSectorFromAddress(uint32_t address);
static int32_t EraseMultipleSector(uint32_t address, uint32_t dimData);
static dvrflash_Status_u GetStatus(void);
static drvflash_Info_t* GetInfo(void);
static bool IsAddressStartSector(uint32_t address);



//============================================================= STATIC VARIABLES
static dvrflash_Status_u Status;
static drvflash_Info_t Info;

//============================================================= GLOBAL VARIABLES
_drvflashmcu_Init_t _drvflashmcu_Init;
drvflash_Driver_t _drvflashmcu_Driver=
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
};



//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Descrizione della funzione.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t _drvflashmcu_InitModule(void)
{
	_drvflashmcu_Init.InitOk = true;

	Status.Busy = false;
	Status.Error = false;

#if defined(STM32F373xC) || defined(STM32G474xx)
	Info.NumeroSettori 		= _DRVFLASHMCU_DIMFLASH/_DRVFLASHMCU_DIMFIXSECTOR;
	Info.DimensioneTotale	= _DRVFLASHMCU_DIMFLASH;

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
#else
	uint32_t uLA;

	Info.NumeroSettori = _NELEMENTS_(StrutturaSettori);
	Info.DimensioneTotale=0;
	for(uLA=0;uLA<Info.NumeroSettori;uLA++)
	{
		Info.DimensioneTotale+=StrutturaSettori[uLA].Dimensione;
	}
#endif

	if(_drvflashmcu_Init.InitOk)
		return _STATUS_OK_;
	else
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
#if defined(STM32F373xC) || defined(STM32G474xx)
	return _DRVFLASHMCU_DIMFIXSECTOR;
#else
	if(numSector<Info.NumeroSettori)
		return StrutturaSettori[numSector].Dimensione;
	else
		return _STATUS_FAIL_;
#endif
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

#if defined(STM32F373xC) || defined(STM32G474xx)
	if(address>=_drvflashmcu_Init.AddressMapping)
	{
		valRet = (address-_drvflashmcu_Init.AddressMapping)/_DRVFLASHMCU_DIMFIXSECTOR;
	}
#else
	uint32_t ula;

	if(_drvflashmcu_Init.InitOk)
	{
		for(ula = 0; ula < Info.NumeroSettori; ula++)
		{
			if((address>=StrutturaSettori[ula].StartAddress) && (address<(StrutturaSettori[ula].StartAddress+StrutturaSettori[ula].Dimensione)))
			{
				valRet = ula;
				break;
			}
		}
	}
#endif

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
static int32_t ReadData(uint32_t address, void *ptrData, uint32_t dimData)
{
	Status.Busy=true;
	memcpy(ptrData,(void*)address,dimData);
	Status.Busy=false;
	return _STATUS_OK_;
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
	int32_t valRet = _STATUS_OK_;

#if defined(STM32G474xx)
	uint8_t *ptrLocalData = (uint8_t*)ptrData;

	if(_drvflashmcu_Init.InitOk)
	{
		Status.Busy=true;

		while(dimData)
		{
			// NOTA: la scrittura in doubleword viene usata per non avere problemi con interrupt...
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *(uint64_t*)ptrLocalData) != HAL_OK)
			{
				valRet = _STATUS_FAIL_;
				break;
			}

			address+=_DRVFLASHMCU_NBYTEPAGINADISCRITTURA;
			ptrLocalData+=_DRVFLASHMCU_NBYTEPAGINADISCRITTURA;
			if(dimData>_DRVFLASHMCU_NBYTEPAGINADISCRITTURA)
				dimData-=_DRVFLASHMCU_NBYTEPAGINADISCRITTURA;
			else
				dimData=0;
		}

		Status.Busy=false;
	}
	else
		valRet = _STATUS_FAIL_;
#endif

	if(valRet==_STATUS_FAIL_)
		Status.Error = true;

	return valRet;
}


/*______________________________________________________________________________ 
 Desc:  Erase dei settore interessati dall'address e per il dimData necessario
 Arg: - address: Address di riferimento settore
 Ret: - 0+ il numero del settore
				_STATUS_FAIL_ se address fuori range
______________________________________________________________________________*/
static int32_t EraseMultipleSector(uint32_t address, uint32_t dimData)
{
	int32_t settore;
	int32_t valRet = _STATUS_OK_;

	if(_drvflashmcu_Init.InitOk)
	{
		Status.Busy=true;
		while(dimData)
		{
			settore = GetNumSectorFromAddress(address);
			if(settore == _STATUS_FAIL_)
				valRet = _STATUS_FAIL_;
			else
			{
				if(EraseSector(settore) == _STATUS_OK_)
				{
#if defined(STM32F373xC) || defined(STM32G474xx)
					if(dimData<=_DRVFLASHMCU_DIMFIXSECTOR)
						break;
					else
					{
						address += _DRVFLASHMCU_DIMFIXSECTOR;
						dimData -= _DRVFLASHMCU_DIMFIXSECTOR;
					}
#else
					if(dimData <= StrutturaSettori[settore].Dimensione)
						break;
					else
					{
						address += StrutturaSettori[settore].Dimensione;
						if(dimData>StrutturaSettori[settore].Dimensione)
							dimData -= StrutturaSettori[settore].Dimensione;
						else
							dimData=0;
					}
#endif
				}
				else
				{
					valRet = _STATUS_FAIL_;
					break;
				}
			}
		}
		Status.Busy=false;
	}
	else
		valRet = _STATUS_FAIL_;

	if(valRet==_STATUS_FAIL_)
		Status.Error = true;

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Funzione specifica per erasare un settore
 Arg: - numSector: Numero del settore da erasare
 Ret: - _STATUS_OK_ se ttok
				_STATUS_FAIL_ se anomalia generica
______________________________________________________________________________*/
static int32_t EraseSector(uint32_t numSector)
{
	int32_t valRet = _STATUS_OK_;

#if defined(STM32F405xx) || defined(STM32H750xx)  || defined(STM32H743xx)
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t sectorError;

	Status.Busy = true;

	HAL_FLASH_Unlock();

	eraseInit.Banks		= FLASH_BANK_1;
	eraseInit.TypeErase	= FLASH_TYPEERASE_SECTORS;
	if(numSector > 7)
	{
		numSector -= 8;
		eraseInit.Banks++;
	}
	eraseInit.Sector       = numSector;
	eraseInit.NbSectors    = 1;
	eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	if(_drvflashmcu_Init.InitOk)
	{
		HAL_FLASHEx_Erase( &eraseInit, &sectorError );
		if(sectorError!=0xffffffff)
			valRet =  _STATUS_FAIL_;
	}

	Status.Busy = false;

#elif defined(STM32F373xC)
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t pageError;

	Status.Busy = true;

	eraseInit.PageAddress = _drvflashmcu_Init.AddressMapping+(_DRVFLASHMCU_DIMFIXSECTOR*numSector);
	eraseInit.NbPages = 1;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	HAL_FLASH_Unlock();

	if(HAL_FLASHEx_Erase(&eraseInit, &pageError)!=HAL_OK)
		valRet = _STATUS_FAIL_;

//	HAL_FLASH_Lock();

	Status.Busy = false;
#elif defined(STM32G474xx)
	FLASH_EraseInitTypeDef eraseInit;
	uint32_t pageError;

	Status.Busy = true;

	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.NbPages = 1;
	eraseInit.Page = numSector;
	eraseInit.Banks = FLASH_BANK_1;

	HAL_FLASH_Unlock();

	if(HAL_FLASHEx_Erase(&eraseInit, &pageError)!=HAL_OK)
		valRet = _STATUS_FAIL_;

	Status.Busy = false;

#endif

	if(valRet==_STATUS_FAIL_)
		Status.Error = true;

	return valRet;
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
 Desc:  Informa se l'address passato è all'inizio di un settore, serve per
        capire se è il caso di erasare il settore per una successiva scrittura
 Arg: - address: Address di riferimento
 Ret: -	true se primo address di un settore
        false altrimenti
______________________________________________________________________________*/
static bool IsAddressStartSector(uint32_t address)
{
	bool valRet = false;

#if defined(STM32F373xC) || defined(STM32G474xx)
	if(address%_DRVFLASHMCU_DIMFIXSECTOR)
		valRet = false;
	else
		valRet = true;
#else
	uint32_t ula;

	if(_drvflashmcu_Init.InitOk)
	{
		for(ula = 0; ula < Info.NumeroSettori; ula++)
		{
			if((address>=StrutturaSettori[ula].StartAddress) && (address<(StrutturaSettori[ula].StartAddress+StrutturaSettori[ula].Dimensione)))
			{
				if(address==StrutturaSettori[ula].StartAddress)
					valRet=true;
				break;
			}
		}
	}
#endif

	return valRet;
}





