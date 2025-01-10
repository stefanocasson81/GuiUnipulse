#ifndef SSETUPPRESENTER_HPP
#define SSETUPPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SSetupView;

class SSetupPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SSetupPresenter(SSetupView& v);

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

    virtual ~SSetupPresenter() {};

private:
    SSetupPresenter();

    SSetupView& view;
};

#endif // SSETUPPRESENTER_HPP
