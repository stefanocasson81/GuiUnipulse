#ifndef XMLSUPPORT_H_INCLUDED
#define XMLSUPPORT_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/FileSystem/Xml.h"

//====================================================================== DEFINES
typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}xmlsupport_Init_t;

//============================================================= GLOBAL VARIABLES
extern xmlsupport_Init_t xmlsupport_Init;

//============================================================ GLOBAL PROTOTYPES
int32_t xmlsupport_InitModule(void);
xml_Status_e xmlsupport_LeggiImmediateCmd(void);
xml_Status_e xmlsupport_SalvaXmlActualCfg(void);


#endif /* XMLSUPPORT_H_INCLUDED */

