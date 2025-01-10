#ifndef SINFOPRESENTER_HPP
#define SINFOPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SInfoView;

class SInfoPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SInfoPresenter(SInfoView& v);

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

    virtual ~SInfoPresenter() {}

private:
    SInfoPresenter();

    SInfoView& view;
};

#endif // SINFOPRESENTER_HPP
