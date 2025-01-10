#ifndef SSETUPVIEW_HPP
#define SSETUPVIEW_HPP

#include <gui_generated/ssetup_screen/SSetupViewBase.hpp>
#include <gui/ssetup_screen/SSetupPresenter.hpp>

extern "C"
{
	#include "ViperUISupport.h"
	#include "CORELib/Common/ViperDef.h"
};


class SSetupView : public SSetupViewBase
{
public:
    SSetupView();
    virtual ~SSetupView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    viperuisupport_ObjTabParametri_t ObjTabParametri;
    uint8_t OldIndiceParametroSelezionato;

    int32_t ValShow;
    viperdef_Tempi_e ShowTempi;
};

#endif // SSETUPVIEW_HPP
