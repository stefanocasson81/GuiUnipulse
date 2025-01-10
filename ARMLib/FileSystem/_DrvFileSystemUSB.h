#ifndef _DRVFILESYSTEMUSB_H_INCLUDED
#define _DRVFILESYSTEMUSB_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"


//====================================================================== DEFINES
typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

}_drvfilesystemusb_Init_t;


//============================================================= GLOBAL VARIABLES
extern _drvfilesystemusb_Init_t _drvfilesystemusb_Init;
extern drvfilesystem_Driver_t _drvfilesystemusb_Driver;

//============================================================ GLOBAL PROTOTYPES
int32_t _drvfilesystemusb_InitModule(void);
void _drvfilesystemusb_Manager(void);
void _drvfilesystemusb_DeInitModule(void);

#endif /* _DRVFILESYSTEMUSB_H_INCLUDED */


