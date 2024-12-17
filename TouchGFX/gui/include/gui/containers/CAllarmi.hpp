#ifndef CALLARMI_HPP
#define CALLARMI_HPP

#include <gui_generated/containers/CAllarmiBase.hpp>

extern "C"
{
	#include "CORELib/Common/ViperDef.h"
};


class CAllarmi : public CAllarmiBase
{
public:
    CAllarmi();
    virtual ~CAllarmi() {}

    virtual void initialize();

    bool ManagerTickEvent(void);
protected:
    viperdef_TrainoAnomalie_u AnomalieTrainoShow;	// Gestito a bit enum del tipo viperdef_InfoStatus_AnomalieTraino_e
    viperdef_GenAnomalieBoard_u AnomalieBoardShow;  // Gestito a bit enum del tipo viperdef_InfoStatus_AnomalieBoard_e
    viperdef_GenAnomaliePower_u AnomaliePowerShow;  // Gestito a bit enum del tipo viperdef_InfoStatus_AnomaliePower_e
};

#endif // CALLARMI_HPP
