#ifndef SBASICPRESENTER_HPP
#define SBASICPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SBasicView;

class SBasicPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SBasicPresenter(SBasicView& v);

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

    virtual ~SBasicPresenter() {};

private:
    SBasicPresenter();

    SBasicView& view;
};

#endif // SBASICPRESENTER_HPP
