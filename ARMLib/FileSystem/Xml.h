#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/FileSystem/Xml.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"

//====================================================================== DEFINES
typedef enum
{
	XML_STATUS_FAIL 	= _STATUS_FAIL_		,
	XML_STATUS_OK 		= _STATUS_OK_		,
	XML_STATUS_FSERROR						,	// Errore file system
	XML_STATUS_TAGSBILANCIATI				,
	XML_STATUS_CHECKSUMERRATA				,
	XML_STATUS_ERRGENERICO					,
	XML_STATUS_GRAMMATICAERRATA				,
}xml_Status_e;

typedef struct
{
	char* PtrStrTag;							// Stringa completa del Tag del tipo PrimoLivello\\SecondoLivello\\TerzoLivello\\...
	void* PtrData;								// Ptr al dato
	_TypeData_e TypeData;						// Tipo dato standard
	float ValMin;								// Valore minimo concesso
	float ValMax;								// Valore massimo concesso, nel caso di stringhe il numero di caratteri massimi (escluso lo zero di fine stringa)
	void (*PtrFunPostWr)(void* ptrVoidDataTag);	// Ptr alla funzione da eseguire post scrittura valore
}xml_DataTag_t;	// Informazioni struttura data del record

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}xml_Init_t;

//============================================================= GLOBAL VARIABLES
extern xml_Init_t xml_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t xml_InitModule(void);
xml_Status_e xml_CaricaXml(char* ptrStrNomeFile,drvfilesystem_Driver_t *ptrDrvCustomFS, xml_DataTag_t *ptrXmlDataTag,bool(*ptrFunCheckPermission)(xml_DataTag_t *ptrDataTag));

#endif /* XML_H_INCLUDED */

