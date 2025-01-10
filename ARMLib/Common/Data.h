#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES
#define DATA_SCALARE(indirizzo)		(uint32_t)((((uint32_t)indirizzo)|_B31_))

typedef struct
{
	void* PtrData;		// Ptr al dato
//niknik da capire se reimplementare	void* PtrDataWrite;		// Ptr al dato da scrivere
	uint8_t NumInteri;
	uint8_t NumDecimali;
	_TypeData_e TypeData;
	uint32_t DataValMin;		// Valore minimo, Se msb==1 B31 si intende dato scalare e non PTR (OCCHIO: la variabile è un mix di valore o puntatore a seconda dei casi)
	uint32_t DataValMax;		// Valore massimo,Se msb==1 B31 si intende dato scalare e non PTR (OCCHIO: la variabile è un mix di valore o puntatore a seconda dei casi)
	uint16_t Id;			// Id Can o EasyProt per trasmissione
}data_Data_t;

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
	
	void (*PtrFunPostSetData)(const data_Data_t *ptrStructData);	// Funzione da eseguire dopo avere impostato un dato
}data_Init_t;



//============================================================= GLOBAL VARIABLES
extern data_Init_t data_Init;
						 
//============================================================ GLOBAL PROTOTYPES
int32_t data_InitModule(void);
int32_t data_GetValData(const data_Data_t *ptrStructData);
int32_t data_SetValData(const data_Data_t *ptrStructData,int32_t value);
int32_t data_GetValMin(const data_Data_t *ptrStructData);
int32_t data_GetValMax(const data_Data_t *ptrStructData);
void data_GetStrData(const data_Data_t *ptrStructData,char *ptrStr);

#endif /* DATA_H_INCLUDED */

