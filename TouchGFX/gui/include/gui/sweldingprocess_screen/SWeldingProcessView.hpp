#ifndef SWELDINGPROCESSVIEW_HPP
#define SWELDINGPROCESSVIEW_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui_generated/sweldingprocess_screen/SWeldingProcessViewBase.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Common/ViperDef.h"
};




class SWeldingProcessView : public SWeldingProcessViewBase
{
public:
    SWeldingProcessView();
    virtual ~SWeldingProcessView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void gasChanged(uint8_t gas);
    void processChanged(uint8_t prc);
    void setEncoderPression(U8 i, U8 p);
    void setMenuPrincipale(void);

    void setWireCB();

protected:
    Drawable *PtrQuadro;


private :
    uint8_t Process;
    U8 ActualSelection;
};

#endif // SWELDINGPROCESSVIEW_HPP
