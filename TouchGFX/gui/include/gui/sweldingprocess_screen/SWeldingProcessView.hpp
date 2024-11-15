#ifndef SWELDINGPROCESSVIEW_HPP
#define SWELDINGPROCESSVIEW_HPP

#include <gui_generated/sweldingprocess_screen/SWeldingProcessViewBase.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Viper/ViperDef.h"
};




class SWeldingProcessView : public SWeldingProcessViewBase
{
public:
    SWeldingProcessView();
    virtual ~SWeldingProcessView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    Drawable *PtrQuadro;
    viperdef_Processo_e ProcessoAllocato;

};

#endif // SWELDINGPROCESSVIEW_HPP
