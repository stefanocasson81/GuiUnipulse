#ifndef SBASICVIEW_HPP
#define SBASICVIEW_HPP

#include <gui_generated/sbasic_screen/SBasicViewBase.hpp>
#include <gui/sbasic_screen/SBasicPresenter.hpp>

extern "C"
{
	#include "ViperUISupport.h"
};

class SBasicView : public SBasicViewBase
{
public:
    SBasicView();
    virtual ~SBasicView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    viperuisupport_ObjTabParametri_t ObjTabParametri;
    uint8_t OldIndiceParametroSelezionato;

    int32_t ValShow;
};

#endif // SBASICVIEW_HPP
