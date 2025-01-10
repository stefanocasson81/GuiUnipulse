/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Schedulatore parametri con formattazione e limiti ed esecuzione funzione in set o get
	Sono supportati i tipi:
		_TYPEDATA_BOOL_
		_TYPEDATA_UINT8_
		_TYPEDATA_SINT8_
		_TYPEDATA_UINT16_
		_TYPEDATA_SINT16_
		_TYPEDATA_UINT32_
		_TYPEDATA_SINT32_

	SCHEDULAZIONE TIPICA:
		const data_Data_t vipergen_StrutturaDati[APPDEF_ENUMDATA_MAX] =
		{   // ATTENZIONE: Aggiungere i parametri in coda ed in ogni caso con coerenza allenum del tipo APPDEF_ENUMDATA_xxx che DEVE essere comune a tutte le app della stessa famiglia
			{ &vipergen_EEUser.MigMan_DTensione,							&vipergen_EEUser.MigMan_DTensione,						_TYPEDATA_UINT16_,		DATA_SCALARE(FEATURES_MIGMANUI_DTENSIONEMIN),				DATA_SCALARE(FEATURES_MIGMANUI_DTENSIONEMAX),  										APPDEF_IDEASYPROT_MIGMAN																		},
			{ &vipergen_EEUser.MigMan_DVelFilo,							&vipergen_EEUser.MigMan_DVelFilo,						_TYPEDATA_UINT16_,		DATA_SCALARE(FEATURES_MIGMANUI_DVELFILOMIN),				DATA_SCALARE(FEATURES_MIGMANUI_DVELFILOMAX),             APPDEF_IDEASYPROT_MIGMAN																		},
			...
    	};
 
		data_Init.PtrFunPostSetData = DataForceImmediateTX;
		data_InitModule();

------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/Data.h"
#include "ARMLib/Common/CustomString.h"


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES
static uint8_t GetSizeData(_TypeData_e typeData);

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
data_Init_t data_Init;

//============================================================= GLOBAL FUNCTIONS
//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t data_InitModule(void)
{
	bool errore=false;

	if(errore)
	{
		data_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		data_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

//______________________________________________________________________________
// Desc:  Ritorna il valore del dato richiesto castato in int16_t
// Arg: - ptrStructData: Ptr alla struttura data
// Ret: - Il valore castato in int16_t
//______________________________________________________________________________
int32_t data_GetValData(const data_Data_t *ptrStructData)
{
	int32_t value=0;
	uint32_t tmpValue=0;

	// Nota: E' necessario passare per una var di appoggio perchè l'address PtrDataRead potrebbe essere dispari e non è possibile per un ARM
	memcpy(&tmpValue, ptrStructData->PtrData, GetSizeData(ptrStructData->TypeData));

	switch(ptrStructData->TypeData)
	{
		case _TYPEDATA_BOOL_:
			value = (int32_t)(*(bool *)(&tmpValue));
		break;
		case _TYPEDATA_UINT8_:
			value = (int32_t)(*(uint8_t*)(&tmpValue));
		break;
		case _TYPEDATA_SINT8_:
			value = (int32_t)(*(int8_t*)(&tmpValue));
		break;
		case _TYPEDATA_UINT16_:
			value = (int32_t)(*(uint16_t*)(&tmpValue));
		break;
		case _TYPEDATA_SINT16_:
			value = (int32_t)(*(int16_t*)(&tmpValue));
		break;
		case _TYPEDATA_UINT32_:
			value = (int32_t)(*(uint32_t*)(&tmpValue));
		break;
		case _TYPEDATA_SINT32_:
			value = (int32_t)(*(int32_t*)(&tmpValue));
		break;
		default:
		break;
	}

	return value;
}

//______________________________________________________________________________
// Desc:  Scrive il valore di tipo int16_t nella struttura data
// Arg: - ptrStructData: Ptr alla struttura data
// Ret: - Il valore castato in int32_t
//______________________________________________________________________________
int32_t data_SetValData(const data_Data_t *ptrStructData,int32_t value)
{
	int32_t valMin=data_GetValMin(ptrStructData);
	int32_t valMax=data_GetValMax(ptrStructData);

	if(value > valMax)
		value = valMax;
	else if(value < valMin)
		value = valMin;

	memcpy(ptrStructData->PtrData,&value,GetSizeData(ptrStructData->TypeData));	// Rinforzo anche il PtrRead

	if(data_Init.PtrFunPostSetData)
		data_Init.PtrFunPostSetData(ptrStructData);

	return value;
}

//______________________________________________________________________________
// Desc:  Ritorna il valore minimo della struttura data passata
// Arg: - ptrStructData: Ptr alla struttura data
// Ret: - Il valore castato in int32_t
//______________________________________________________________________________
int32_t data_GetValMin(const data_Data_t *ptrStructData)
{
	int32_t valRet = 0;
	uint32_t value;

	if(((uint32_t)ptrStructData->DataValMin) & _B31_)   // Se non è un puntatore
	{
		if(((uint32_t)ptrStructData->DataValMin)&_B30_)
		{	// Negativo
			valRet = (int32_t)(ptrStructData->DataValMin);
		}
		else
			valRet = (int32_t)(ptrStructData->DataValMin&(~_B31_));
	}
	else
	{
		memcpy(&value,(void*)ptrStructData->DataValMin,4);
		switch(ptrStructData->TypeData)
		{
			case _TYPEDATA_BOOL_:
				valRet = (int32_t)(*(bool *)(&value));
				break;
			case _TYPEDATA_UINT8_:
				valRet = (int32_t)(*(uint8_t *)(&value));
				break;
			case _TYPEDATA_SINT8_:
				valRet = (int32_t)(*(int8_t *)(&value));
				break;
			case _TYPEDATA_UINT16_:
				valRet = (int32_t)(*(uint16_t *)(&value));
				break;
			case _TYPEDATA_SINT16_:
				valRet = (int32_t)(*(int16_t *)(&value));
				break;
			case _TYPEDATA_UINT32_:
				valRet = (int32_t)(*(uint32_t *)(&value));
				break;
			case _TYPEDATA_SINT32_:
				valRet = (int32_t)(*(int32_t *)(&value));
				break;
			default:
			break;
		}
	}

	return valRet;
}


//______________________________________________________________________________
// Desc:  Ritorna il valore massimo della struttura data passata
// Arg: - ptrStructData: Ptr alla struttura data
// Ret: - Il valore castato in int16_t
//______________________________________________________________________________
int32_t data_GetValMax(const data_Data_t *ptrStructData)
{
	int32_t valRet = 0;
	uint32_t value;

	if(((uint32_t)ptrStructData->DataValMax) & _B31_)   // Se non è un puntatore
	{
		if(((uint32_t)ptrStructData->DataValMax)&_B30_)
		{	// Negativo
			valRet = (int32_t)(ptrStructData->DataValMax);
		}
		else
			valRet = (int32_t)(ptrStructData->DataValMax&(~_B31_));
	}
	else
	{
		memcpy(&value,(void*)ptrStructData->DataValMax,4);
		switch(ptrStructData->TypeData)
		{
			case _TYPEDATA_BOOL_:
				valRet = (int32_t)(*(bool *)(&value));
				break;
			case _TYPEDATA_UINT8_:
				valRet = (int32_t)(*(uint8_t *)(&value));
				break;
			case _TYPEDATA_SINT8_:
				valRet = (int32_t)(*(int8_t *)(&value));
				break;
			case _TYPEDATA_UINT16_:
				valRet = (int32_t)(*(uint16_t *)(&value));
				break;
			case _TYPEDATA_SINT16_:
				valRet = (int32_t)(*(int16_t *)(&value));
				break;
			case _TYPEDATA_UINT32_:
				valRet = (int32_t)(*(uint32_t *)(&value));
				break;
			case _TYPEDATA_SINT32_:
				valRet = (int32_t)(*(int32_t *)(&value));
				break;
			default:
			break;
		}
	}

	return valRet;
}


/*______________________________________________________________________________
 Desc:  Formatta il valore in stringa
 Arg: - ptrStructData: Ptr alla struttura data
      - ptrStr: ptr alla stirnga da compilare (consigliato almeno 12+1 char)
 Ret: - void
______________________________________________________________________________*/
void data_GetStrData(const data_Data_t *ptrStructData,char *ptrStr)
{
	bool segnato = false;
	int32_t value = data_GetValData(ptrStructData);
	int32_t valMin = data_GetValMin(ptrStructData);
	bool valido=true;

	switch(ptrStructData->TypeData)
	{
		case _TYPEDATA_BOOL_:
		case _TYPEDATA_UINT8_:
		case _TYPEDATA_UINT16_:
		case _TYPEDATA_UINT32_:
			// Puer essendo valori sempre positivi portrebbe essere una eccezione con gli offset....
			if((value<0) || (valMin<0))
				segnato = true;
		break;

		case _TYPEDATA_SINT8_:
		case _TYPEDATA_SINT16_:
		case _TYPEDATA_SINT32_:
			segnato = true;
		break;

		default:
			valido = false;
		break;
	}

	if(valido)
		customstring_FormatFix(ptrStr,value,ptrStructData->NumInteri,ptrStructData->NumDecimali,' ','.',segnato);
}


//============================================================ PRIVATE FUNCTIONS
//______________________________________________________________________________
// Desc:  Ritorna la size in memoria del dato puntato
// Arg: - ptrStructData: Ptr alla struttura data
// Ret: - la size
//______________________________________________________________________________
static uint8_t GetSizeData(_TypeData_e typeData)
{
	uint8_t valRet = 0;
	switch(typeData)
	{
		case _TYPEDATA_BOOL_:
		case _TYPEDATA_UINT8_:
		case _TYPEDATA_SINT8_:
			valRet = 1;
			break;
		case _TYPEDATA_UINT16_:
		case _TYPEDATA_SINT16_:
			valRet = 2;
			break;
		case _TYPEDATA_UINT32_:
		case _TYPEDATA_SINT32_:
			valRet = 4;
			break;
		default:
		break;
	}

	return valRet;
}

