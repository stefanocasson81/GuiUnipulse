#ifndef CUSTOMSTRING_H_INCLUDED
#define CUSTOMSTRING_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES

//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES
uint8_t customstring_FormatFix(char *ptrStr,int64_t valFix,uint8_t numInteri,uint8_t numDecimali,char simboloRiempimento,char simboloDecimale,bool segnato);
int64_t customstring_GetInt64t(void *ptrData,_TypeData_e typeData);
int32_t customstring_AtoX(char *ptrStr,void *ptrData,_TypeData_e typeData);
void customstring_SetValTypeInt(void *ptrData,_TypeData_e typeData,int64_t val);


#endif /* CUSTOMSTRING_H_INCLUDED */

