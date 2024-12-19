#ifndef SMENUPRINCIPALEPRESENTER_HPP
#define SMENUPRINCIPALEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SMenuPrincipaleView;

class SMenuPrincipalePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SMenuPrincipalePresenter(SMenuPrincipaleView& v);

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

    virtual ~SMenuPrincipalePresenter() {};
    void setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p);
    void setOffsetEncoder(Model::Enc_Type e, S16 offset,S8 dir);
//    void setOffsetEncoder(Model::Enc_Type e,U16 offset);

private:
    SMenuPrincipalePresenter();

    SMenuPrincipaleView& view;

    U16 encValue[3];
};

#endif // SMENUPRINCIPALEPRESENTER_HPP
