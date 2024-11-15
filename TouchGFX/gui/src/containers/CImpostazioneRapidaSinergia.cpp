#include <gui/containers/CImpostazioneRapidaSinergia.hpp>

extern "C"
{
//	#include "CORELib/Viper/UserInterface/ViperUI.h"
//	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
//	#include "CORELib/Viper/ViperDef.h"
//	#include "ARMLib/Common/CustomString.h"
	#include "ARMLib/Common/Data.h"
	#include <ViperUISupport.h>

};


CImpostazioneRapidaSinergia::CImpostazioneRapidaSinergia()
{
    TipoFiloShow = VIPERDEF_TIPOFILO_MAX;
    DiametroFiloShow = VIPERDEF_DIAMETROFILO_MAX;
    TipoGasShow = VIPERDEF_TIPOGAS_MAX;

    ManagerTickEvent();
}

void CImpostazioneRapidaSinergia::initialize()
{
    CImpostazioneRapidaSinergiaBase::initialize();
}

void CImpostazioneRapidaSinergia::ManagerTickEvent(void)
{
	int32_t lA;
	char tmpStr[30];

    TipoFiloShow = VIPERDEF_TIPOFILO_MAX;
    DiametroFiloShow = VIPERDEF_DIAMETROFILO_MAX;
    TipoGasShow = VIPERDEF_TIPOGAS_MAX;

    lA = data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOFILO]);
    if(TipoFiloShow!=lA)
    {
    	TipoFiloShow=(viperdef_TipoFilo_e)lA;
    	viperuisupport_GetTxtPtrData((data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOFILO],tmpStr, sizeof(tmpStr));

		Unicode::strncpy(tATipoFiloBuffer,tmpStr, TATIPOFILO_SIZE-1);
		tATipoFilo.invalidate();
    }

    lA = data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_DIAMETROFILO]);
    if(DiametroFiloShow!=lA)
    {
    	DiametroFiloShow=(viperdef_DiametroFilo_e)lA;
    	viperuisupport_GetTxtPtrData((data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_BASE_DIAMETROFILO],tmpStr, sizeof(tmpStr));

		Unicode::strncpy(tADiametroBuffer,tmpStr, TADIAMETRO_SIZE-1);
		tADiametro.invalidate();
    }

    lA = data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOGAS]);
    if(TipoGasShow!=lA)
    {
    	TipoGasShow=(viperdef_TipoGas_e)lA;
    	viperuisupport_GetTxtPtrData((data_Data_t*)&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOGAS],tmpStr, sizeof(tmpStr));

		Unicode::strncpy(tATipoGasBuffer,tmpStr, TATIPOGAS_SIZE-1);
		tATipoGas.invalidate();
    }
}
