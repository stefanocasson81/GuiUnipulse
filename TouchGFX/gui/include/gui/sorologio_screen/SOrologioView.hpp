#ifndef SOROLOGIOVIEW_HPP
#define SOROLOGIOVIEW_HPP

#include <gui_generated/sorologio_screen/SOrologioViewBase.hpp>
#include <gui/sorologio_screen/SOrologioPresenter.hpp>

extern "C"
{
	#include "ARMLib/Common/CustomRtc.h"
};

class SOrologioView : public SOrologioViewBase
{
public:
    SOrologioView();
    virtual ~SOrologioView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    uint32_t OldTickCounter;
    uint8_t IndiceSelezione;
    uint8_t OldIndiceSelezione;

    customrtc_Time_t OldRtcTime;
};

#endif // SOROLOGIOVIEW_HPP
