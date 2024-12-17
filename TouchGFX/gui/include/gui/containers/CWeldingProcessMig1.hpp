#ifndef CWELDINGPROCESSMIG1_HPP
#define CWELDINGPROCESSMIG1_HPP

#include <gui_generated/containers/CWeldingProcessMig1Base.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/UserInterface/ViperUI.h"
};


class CWeldingProcessMig1 : public CWeldingProcessMig1Base
{
public:
    CWeldingProcessMig1();
    virtual ~CWeldingProcessMig1() {}

    virtual void initialize();

    void Invalida(void);
    void ManagerTickEvent(void);
protected:
    viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;
};

#endif // CWELDINGPROCESSMIG1_HPP
