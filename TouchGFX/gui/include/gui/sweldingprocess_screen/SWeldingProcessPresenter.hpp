#ifndef SWELDINGPROCESSPRESENTER_HPP
#define SWELDINGPROCESSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SWeldingProcessView;

class SWeldingProcessPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SWeldingProcessPresenter(SWeldingProcessView& v);

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

    virtual ~SWeldingProcessPresenter() {};

private:
    SWeldingProcessPresenter();

    SWeldingProcessView& view;
};

#endif // SWELDINGPROCESSPRESENTER_HPP
