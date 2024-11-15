#ifndef SHWCHECKVIEW_HPP
#define SHWCHECKVIEW_HPP

#include <gui_generated/shwcheck_screen/SHwCheckViewBase.hpp>
#include <gui/shwcheck_screen/SHwCheckPresenter.hpp>

class SHwCheckView : public SHwCheckViewBase
{
public:
    SHwCheckView();
    virtual ~SHwCheckView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    uint32_t OldTickCounter;
    uint32_t OldTickCounterFast;
    uint16_t OldADCIo;
    uint16_t OldADCVo;
    uint16_t OldDACCnt;
};

#endif // SHWCHECKVIEW_HPP
