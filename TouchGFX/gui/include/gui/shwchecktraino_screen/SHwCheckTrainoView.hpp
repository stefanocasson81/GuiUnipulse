#ifndef SHWCHECKTRAINOVIEW_HPP
#define SHWCHECKTRAINOVIEW_HPP

#include <gui_generated/shwchecktraino_screen/SHwCheckTrainoViewBase.hpp>
#include <gui/shwchecktraino_screen/SHwCheckTrainoPresenter.hpp>


extern "C"
{
	#include "CORELib/Common/ViperDef.h"
};


class SHwCheckTrainoView : public SHwCheckTrainoViewBase
{
public:
    SHwCheckTrainoView();
    virtual ~SHwCheckTrainoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    uint32_t OldTickCounter;

    uint16_t OldDVBus;

    uint16_t OldDVEvGas;
    uint16_t OldMAEvGas;

    uint16_t OldDVMot;
    uint16_t OldMAMot;
    uint16_t OldMMVelMot;

    viperdef_Pack8TrainoTX_InfoStatus_t OldTrainoInfoStatus;
};

#endif // SHWCHECKTRAINOVIEW_HPP
