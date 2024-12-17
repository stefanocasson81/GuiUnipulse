#ifndef CWELDINGPROCESSMIG4_HPP
#define CWELDINGPROCESSMIG4_HPP

#include <gui_generated/containers/CWeldingProcessMig4Base.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/UserInterface/ViperUI.h"
};

class CWeldingProcessMig4 : public CWeldingProcessMig4Base
{
public:
    CWeldingProcessMig4();
    virtual ~CWeldingProcessMig4() {}

    virtual void initialize();

    void Invalida(void);
    void ManagerTickEvent(void);
protected:
    viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;

    viperdef_TipoMig_e OldTipoMig;
};

#endif // CWELDINGPROCESSMIG4_HPP
