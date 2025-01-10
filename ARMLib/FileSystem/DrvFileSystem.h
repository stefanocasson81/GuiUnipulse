#ifndef DRVFILESYSTEM_H_INCLUDED
#define DRVFILESYSTEM_H_INCLUDED

/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione: Layer driver per scrittura in FLASH
------------------------------------------------------------------------------*/

//===================================================================== INCLUDES


#include "ARMLib/Common/ComDef.h"
#include "ff.h"

//====================================================================== DEFINES
typedef FIL drvfilesystem_HandleFile_t;

typedef enum
{
	DRVFILESYSTEM_PERIPHERALSSTATUS_NOTCONFIGURED, 		// Periferica NON configurata/abilitata
	DRVFILESYSTEM_PERIPHERALSSTATUS_ERROR, 				// Periferica configurata ma NON operative per errori generici
	DRVFILESYSTEM_PERIPHERALSSTATUS_STARTED, 			// Periferica inizializzata correttamente
	DRVFILESYSTEM_PERIPHERALSSTATUS_READY, 				// Periferica operativa e funzionante
	DRVFILESYSTEM_PERIPHERALSSTATUS_DISCONNECT, 		// Periferica disconnessa (tipico della chiavetta USB o SDIO tolta)
}drvfilesystem_PeripheralsStatus_e;

typedef enum
{
	DRVFILESYSTEM_FOPENTYPE_READ, 		// Apre il file in sola lettura
	DRVFILESYSTEM_FOPENTYPE_WRITE, 		// Apre il file in lettura/scrittura, se il file esiste gi� viene sovrascritto
	DRVFILESYSTEM_FOPENTYPE_APPEND,  	// Apre il file in lettura/scrittura, se il file esiste gi� NON vinee sovrascritto e vi si pu� appendere data
}drvfilesystem_FOpenType_e;

typedef enum
{
	DRVFILESYSTEM_SEEK_CUR,
	DRVFILESYSTEM_SEEK_END,
	DRVFILESYSTEM_SEEK_SET
}drvfilesystem_Seek_e;

typedef struct
{
	struct
	{
		drvfilesystem_PeripheralsStatus_e PeripheralsStatus;
		bool Mounted;
		uint8_t NumFileOpened;
	}Info;

	drvfilesystem_PeripheralsStatus_e (*PeripheralsStatus)(void);
	int32_t (*FOpen)(drvfilesystem_HandleFile_t *ptrUsbHandleFile, const char *fileName, drvfilesystem_FOpenType_e fOpenType);
	int32_t (*FClose)(drvfilesystem_HandleFile_t *ptrUsbHandleFile);
	int32_t (*FRead)(drvfilesystem_HandleFile_t *ptrUsbHandleFile, void *ptrData, uint32_t size);
	int32_t (*FWrite)(drvfilesystem_HandleFile_t *ptrUsbHandleFile, void *ptrData, uint32_t size);
	int32_t (*FPrintf)(drvfilesystem_HandleFile_t *ptrUsbHandleFile, const char *format, ...);
	int32_t (*FSeek)(drvfilesystem_HandleFile_t *ptrUsbHandleFile, int32_t offset, drvfilesystem_Seek_e seek);
	int32_t (*FTell)(drvfilesystem_HandleFile_t *ptrUsbHandleFile);
	char* (*FFind)(bool startFind, const char *ptrPath, const char *ptrPattern);
}drvfilesystem_Driver_t;


//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES

#endif	// fine #if _LIBSUPPORT_DRVCUSTUMFS == _ENABLED_

