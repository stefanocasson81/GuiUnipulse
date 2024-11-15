#ifndef VIPERUISUPPORT_H_INCLUDED
#define VIPERUISUPPORT_H_INCLUDED

//===================================================================== INCLUDES

#include "ARMLib/Common/ComDef.h"
#include "CORELib/Viper/ViperDef.h"
#include "ARMLib/Common/CustomString.h"
#include "CORELib/Viper/UserInterface/ViperUI.h"

//====================================================================== DEFINES
typedef struct
{
	const char *PtrTestoParametro;
	uint32_t EnumTypedText;
	const char *PtrStrUdm;
	const viperui_Data_e EnumData;
	//bool *PtrBoolVarShow;		// Se !=_NULL_ indica il ptr alla var di parametro visualizzabile
}viperuisupport_StructParametri_t;

typedef struct
{
	uint8_t IndiceParametroSelezionato;
	uint8_t NumeroParametri;

	const viperuisupport_StructParametri_t *PtrStructParametri;
}viperuisupport_ObjTabParametri_t;

//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES
void viperuisupport_GetTxtEnumData(viperui_Data_e dataEnum,char* ptrStr,uint16_t maxLen);
void viperuisupport_GetTxtPtrData(data_Data_t *ptrData,char* ptrStr,uint16_t maxLen);

void viperuisupport_ObjTabParametri_Create(viperuisupport_ObjTabParametri_t *PtrObjTabParametri,const viperuisupport_StructParametri_t *PtrStructParametri);
void viperuisupport_ObjTabParametri_SelezioneOffset(viperuisupport_ObjTabParametri_t *ptrObjTabParametri,int16_t offsetSel);

#endif /* VIPERUISUPPORT_H_INCLUDED */

