#ifndef CWELDINGPROCESSMIG3_HPP
#define CWELDINGPROCESSMIG3_HPP

#include <gui_generated/containers/CWeldingProcessMig3Base.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Viper/UserInterface/ViperUI.h"
};

class CWeldingProcessMig3 : public CWeldingProcessMig3Base
{
public:
    CWeldingProcessMig3();
    virtual ~CWeldingProcessMig3() {}

    virtual void initialize();

    void Invalida(void);
    void ManagerTickEvent(void);
protected:
    viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;

    viperdef_TipoGas_e OldTipoGas;
};

#endif // CWELDINGPROCESSMIG3_HPP
