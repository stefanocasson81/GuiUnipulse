#ifndef CWELDINGPROCESSMIG2_HPP
#define CWELDINGPROCESSMIG2_HPP

#include <gui_generated/containers/CWeldingProcessMig2Base.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/UserInterface/ViperUI.h"
};

class CWeldingProcessMig2 : public CWeldingProcessMig2Base
{
public:
    CWeldingProcessMig2();
    virtual ~CWeldingProcessMig2() {}

    virtual void initialize();

    void Invalida(void);
    void ManagerTickEvent(void);
protected:
    viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;

	viperdef_TipoFilo_e OldTipoFilo;
	viperdef_DiametroFilo_e OldDiametroFilo;
};

#endif // CWELDINGPROCESSMIG2_HPP
