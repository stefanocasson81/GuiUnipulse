#ifndef SINFOVIEW_HPP
#define SINFOVIEW_HPP

#include <gui_generated/sinfo_screen/SInfoViewBase.hpp>
#include <gui/sinfo_screen/SInfoPresenter.hpp>

class SInfoView : public SInfoViewBase
{
public:
    SInfoView();
    virtual ~SInfoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
};

#endif // SINFOVIEW_HPP
