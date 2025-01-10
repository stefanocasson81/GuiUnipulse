#ifndef SDEBUGVARPRESENTER_HPP
#define SDEBUGVARPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SDebugVarView;

class SDebugVarPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SDebugVarPresenter(SDebugVarView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~SDebugVarPresenter() {};

private:
    SDebugVarPresenter();

    SDebugVarView& view;
};

#endif // SDEBUGVARPRESENTER_HPP
