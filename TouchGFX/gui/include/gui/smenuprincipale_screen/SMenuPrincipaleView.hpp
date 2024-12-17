#ifndef SMENUPRINCIPALEVIEW_HPP
#define SMENUPRINCIPALEVIEW_HPP

#include <gui_generated/smenuprincipale_screen/SMenuPrincipaleViewBase.hpp>
#include <gui/smenuprincipale_screen/SMenuPrincipalePresenter.hpp>
#include <ARMLib/TGFX/TGFXMenu.hpp>

#define SMENUPRINCIPALEVIEW_MAXTILE 8

class SMenuPrincipaleView : public SMenuPrincipaleViewBase
{
public:
    SMenuPrincipaleView();
    virtual ~SMenuPrincipaleView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    tgfxmenu_c TGFXMenu;
};

#endif // SMENUPRINCIPALEVIEW_HPP
