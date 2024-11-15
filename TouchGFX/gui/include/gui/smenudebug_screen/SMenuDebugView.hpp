#ifndef SMENUDEBUGVIEW_HPP
#define SMENUDEBUGVIEW_HPP

#include <gui_generated/smenudebug_screen/SMenuDebugViewBase.hpp>
#include <gui/smenudebug_screen/SMenuDebugPresenter.hpp>

class SMenuDebugView : public SMenuDebugViewBase
{
public:
    SMenuDebugView();
    virtual ~SMenuDebugView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SMENUDEBUGVIEW_HPP
