#ifndef SOROLOGIOPRESENTER_HPP
#define SOROLOGIOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SOrologioView;

class SOrologioPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SOrologioPresenter(SOrologioView& v);

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

    virtual ~SOrologioPresenter() {}

private:
    SOrologioPresenter();

    SOrologioView& view;
};

#endif // SOROLOGIOPRESENTER_HPP
