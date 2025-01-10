/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Primitive gestione stringhe
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Common/CustomString.h"


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES
static const uint64_t Pot[]={           1,
                                       10,
                                      100,
                                     1000,
                                    10000,
                                   100000,
                                  1000000,
                                 10000000,
                                100000000,
                               1000000000,
                              10000000000,
                               0xffffffff};


//============================================================= GLOBAL VARIABLES

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________
 Desc:  Formatta il valore fisso in stringa
 Arg: - ptrStr: ptr alla stirnga da compilare (consigliato almeno 12+1 char)
      - valFix: Il valore
      - numInteri: Numero degli interi
      - numDecimali: numero dei decimali, verrà aggiunta la virgola come separatore
      - simboloRiempimento: carattere da usare come riempimento, se 0 ottimizza
      - simboloDecimale: indica che simbolo usar per il decimale, tipicamente '.' o ','
      - segnato: se usare il simbolo del segno
 Ret: - Il numero di caratteri utilizzati
______________________________________________________________________________*/
uint8_t customstring_FormatFix(char *ptrStr,int64_t valFix,uint8_t numInteri,uint8_t numDecimali,char simboloRiempimento,char simboloDecimale,bool segnato)
{
    uint8_t uCn,uCq,uCp;
	uint8_t strPos = 0;
    uint32_t uLl,uLt,uLs;
	bool scrittaPrimaCifraNonZero = false;
    uint8_t valRet=0;
    bool flgSigned=false;

    if(valFix<0)
    {   // Se il Valore e` negativo, lo rende positivo
    	valFix*=-1;
    	flgSigned = true;
    }

    uCn=numInteri+numDecimali;    // Riduzione del numero da considerare
    if(valFix>=Pot[uCn])
    {
        uLt=Pot[uCn];
        uLs=valFix/uLt;
        uLs*=uLt;
        valFix-=uLs;
    }

    uCn--;
    uCq=0x00;     // Parte dal primo carattere a sinistra

    valRet = numInteri+numDecimali;
    if(numDecimali)
        valRet++;

    if(segnato)
    {
		if(flgSigned)
			ptrStr[strPos++]='-';
		else
			ptrStr[strPos++]='+';

		valRet++;
	}

    for(;uCn!=0xff;uCn--)
    {
        uCp=0x30;
        uLl=Pot[uCn];
        while(valFix>=uLl){
        	valFix-=uLl;
            uCp++;
        }
        uCq++;
        if(uCp!='0')
			scrittaPrimaCifraNonZero = true;
		if(scrittaPrimaCifraNonZero || simboloRiempimento != 0 || uCq == numInteri)
			ptrStr[strPos++]=uCp;
		if(uCq==numInteri && numDecimali)
		{
			ptrStr[strPos++]=simboloDecimale;      // Punto decimale
    		scrittaPrimaCifraNonZero = true;
		}
	}
    ptrStr[strPos]=0x00;   // Terminatore di stringa

    strPos--;  // Per lasciare uno '0' se il Valore e` nullo

    if(simboloRiempimento != 0)
    {
		for(uCn=0x00;uCn!=strPos;uCn++)
	    {   // Sostituisce eventuali zeri  (0030->__30)
			if((ptrStr[uCn]=='-') || (ptrStr[uCn]=='+'))
			{

			}
			else
			{
				if((ptrStr[uCn]!=0x30) || (ptrStr[uCn+1]==simboloDecimale))
					break;

				ptrStr[uCn] = simboloRiempimento;
			}
	    }
	}

    return valRet;
}


/*______________________________________________________________________________
 Desc:  Ritorna l'int64_t del type data passato
 Arg: - ptrData ptr al dato
 	  - typeData: tipo del dato supportato
 Ret: - Il numero di caratteri utilizzati
______________________________________________________________________________*/
int64_t customstring_GetInt64t(void *ptrData,_TypeData_e typeData)
{
	int64_t valRet=0;

	switch(typeData)
	{
		case _TYPEDATA_BOOL_:
			valRet = *(bool*)ptrData;
		break;
		case _TYPEDATA_UINT8_:
			valRet = *(uint8_t*)ptrData;
		break;
		case _TYPEDATA_SINT8_:
			valRet = *(int8_t*)ptrData;
		break;
		case _TYPEDATA_UINT16_:
			valRet = *(uint16_t*)ptrData;
		break;
		case _TYPEDATA_SINT16_:
			valRet = *(int16_t*)ptrData;
		break;
		case _TYPEDATA_UINT32_:
			valRet = *(uint32_t*)ptrData;
		break;
		case _TYPEDATA_SINT32_:
			valRet = *(int32_t*)ptrData;
		break;
		default:
		break;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Setta il valore alla variabile puntata seguendo il valore di tipo int64_t passato
 Arg: - ptrData ptr al dato
 	  - typeData: tipo del dato supportato
 	  - val: Valore di tipo int64_t
 Ret: - void
______________________________________________________________________________*/
void customstring_SetValTypeInt(void *ptrData,_TypeData_e typeData,int64_t val)
{
	switch(typeData)
	{
		case _TYPEDATA_BOOL_:
			if(val)
				*(bool*)ptrData = true;
			else
				*(bool*)ptrData = false;
		break;
		case _TYPEDATA_UINT8_:
			*(uint8_t*)ptrData = (uint8_t)val;
		break;
		case _TYPEDATA_SINT8_:
			*(int8_t*)ptrData = (int8_t)val;
		break;
		case _TYPEDATA_UINT16_:
			*(uint16_t*)ptrData = (uint16_t)val;
		break;
		case _TYPEDATA_SINT16_:
			*(int16_t*)ptrData = (int16_t)val;
		break;
		case _TYPEDATA_UINT32_:
			*(uint32_t*)ptrData = (uint32_t)val;
		break;
		case _TYPEDATA_SINT32_:
			*(int32_t*)ptrData = (int32_t)val;
		break;
		default:
		break;
	}
}



//______________________________________________________________________________
// Desc:  Converte la stringa in intero a seconda del tipo passato.
//		  Nota il tipo stringa non è gestito!
// Arg: - ptrStr: ptr alla stringa origine
//		- ptrData ptr al dato
// 	  	- typeData: tipo del dato supportato
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t customstring_AtoX(char *ptrStr,void *ptrData,_TypeData_e typeData)
{
	double dA;
	int64_t lLA;
	int32_t valRet = _STATUS_OK_;

	switch(typeData)
	{
		case _TYPEDATA_BOOL_:	// Questa è una eccezione gestisco anche true, false
			lLA = 0;
			if(!strcmp(ptrStr,"true"))
				lLA = 1;
			else if(!strcmp(ptrStr,"false"))
				lLA = 0;
			else
				lLA = atoll(ptrStr);

			if(lLA==0)
				*(bool*)ptrData = false;
			else
				*(bool*)ptrData = true;
		break;
		case _TYPEDATA_UINT8_:
			lLA = atoll(ptrStr);
			*(uint8_t*)ptrData = (uint8_t)lLA;
		break;
		case _TYPEDATA_SINT8_:
			lLA = atoll(ptrStr);
			*(int8_t*)ptrData = (int8_t)lLA;
		break;
		case _TYPEDATA_UINT16_:
			lLA = atoll(ptrStr);
			*(uint16_t*)ptrData = (uint16_t)lLA;
		break;
		case _TYPEDATA_SINT16_:
			lLA = atoll(ptrStr);
			*(int16_t*)ptrData = (int16_t)lLA;
		break;
		case _TYPEDATA_UINT32_:
			lLA = atoll(ptrStr);
			*(uint32_t*)ptrData = (uint32_t)lLA;
		break;
		case _TYPEDATA_SINT32_:
			lLA = atoll(ptrStr);
			*(int32_t*)ptrData = (int32_t)lLA;
		break;
		case _TYPEDATA_FLOAT_:
			dA = atof(ptrStr);
			*(float*)ptrData = (float)dA;
		break;
		case _TYPEDATA_DOUBLE_:
			dA = atof(ptrStr);
			*(double*)ptrData = (double)dA;
		break;

		default:
			valRet = _STATUS_FAIL_;
		case _TYPEDATA_PTRSTR_:
		break;
	}

	return valRet;
}



//============================================================ PRIVATE FUNCTIONS

