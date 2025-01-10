#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/FileSystem/DrvFileSystem.h"

//====================================================================== DEFINES
/*
#define CSV_DEFAULTFILENAME 	"Log.csv"
#define CSV_DEFAULTENDRECORD 	"\r\n"
#define CSV_DEFAULSEPDATA		';'
*/

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}csv_Init_t;

typedef enum
{
	CSV_STATUSOBJ_VOID				,	// Nessuna funzione ancora eseguita
	CSV_STATUSOBJ_RECORDAGGIUNTO_NF	,	// Record aggiunto con creazione di Nuovo File
	CSV_STATUSOBJ_RECORDAGGIUNTO_OF	,	// Record aggiunto sul file vecchio
	CSV_STATUSOBJ_FAIL				,	// Record non aggiunto per anomalia generica
}csv_StatusObj_e;	// Stato dell'Obj

typedef struct
{
	char* PtrDescrittivo;	// Descrizione dell'intestazione colonna
	void* PtrData;			// Ptr al dato
	_TypeData_e TypeData;	// Tipo dato standard
	uint8_t NumInteri;		// Numero interi da considerare, se stringa viene scritta per intero
	uint8_t NumDecimali;	// Numero dei decimali da considerare, per il testo perde di significato
}csv_DataRecord_t;	// Informazioni struttura data del record

typedef struct
{
	char* PtrStrNomeFile;			// Default Log.csv
	char* PtrStrFineRecord;			// Stringa di fine record (default "\r\n")
	char CarattereSeparatoreCampi;	// Carattere di separazione campi (default ';')
	bool OttimizzaNumeri;			// Toglie gli spazi in più in testa
	char SimboloDecimale;			// Indica che simbolo usare per i decimali, tipicamente punto o virgola (default ',')
	char SimboloRiempimento;		// Tipicamento ' ', se 0 ottimizza i numeri (default 0);

	csv_StatusObj_e StatusObj;		// Stato attuale dell'ggetto

	csv_DataRecord_t *PtrDataRecord;

	drvfilesystem_Driver_t *PtrDrvCustomFS;
}csv_Obj_t;	// Oggetto csv




//============================================================= GLOBAL VARIABLES
extern csv_Init_t csv_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t csv_InitModule(void);
csv_Obj_t* csv_CreateObj(csv_DataRecord_t *ptrDataRecord,drvfilesystem_Driver_t *ptrDrvCustomFS);
int32_t csv_AddRecord(csv_Obj_t *ptrObj);

#endif /* CSV_H_INCLUDED */

