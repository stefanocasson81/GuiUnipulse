#ifndef SMENUADVANCEPRESENTER_HPP
#define SMENUADVANCEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SMenuAdvanceView;

class SMenuAdvancePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SMenuAdvancePresenter(SMenuAdvanceView& v);

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

    virtual ~SMenuAdvancePresenter() {}

private:
    SMenuAdvancePresenter();

    SMenuAdvanceView& view;
};

#endif // SMENUADVANCEPRESENTER_HPP
