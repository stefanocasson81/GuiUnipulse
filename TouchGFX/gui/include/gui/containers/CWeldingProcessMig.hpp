#ifndef CWELDINGPROCESSMIG_HPP
#define CWELDINGPROCESSMIG_HPP

#include <gui_generated/containers/CWeldingProcessMigBase.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/UserInterface/ViperUI.h"
};


class CWeldingProcessMig : public CWeldingProcessMigBase
{
public:
    CWeldingProcessMig();
    virtual ~CWeldingProcessMig() {}

    virtual void initialize();

    void Invalida(void);
    void ManagerTickEvent(void);
protected:
    viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;
	
	uint8_t TabEnumDiametro[32];
	uint8_t TabEnumTipoGas[32];
	uint8_t TabEnumTipoMig[32];
	uint8_t IndiceMaxEnumDiametro;
	uint8_t IndiceMaxEnumTipoGas;
	uint8_t IndiceMaxEnumTipoMig;
};

#endif // CWELDINGPROCESSMIG_HPP
