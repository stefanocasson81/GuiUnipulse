#ifndef SMAINPRESENTER_HPP
#define SMAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SMainView;

class SMainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SMainPresenter(SMainView& v);

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

    virtual ~SMainPresenter() {};

private:
    SMainPresenter();

    SMainView& view;
};

#endif // SMAINPRESENTER_HPP
