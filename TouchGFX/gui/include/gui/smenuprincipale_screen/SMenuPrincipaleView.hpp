#ifndef SMENUPRINCIPALEVIEW_HPP
#define SMENUPRINCIPALEVIEW_HPP

#include <gui_generated/smenuprincipale_screen/SMenuPrincipaleViewBase.hpp>
#include <gui/smenuprincipale_screen/SMenuPrincipalePresenter.hpp>
#include <gui/containers/CTileMenu.hpp>
#include <ARMLib/TGFX/TGFXMenu.hpp>

#define SMENUPRINCIPALEVIEW_MAXTILE 8

class SMenuPrincipaleView : public SMenuPrincipaleViewBase
{
public:
    SMenuPrincipaleView();
    virtual ~SMenuPrincipaleView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

//    virtual void handleTickEvent();
    void GoBack(void);
    void setMenu();
    void encRup(void);
    void encRdown(void);
protected:
    tgfxmenu_c TGFXMenu;
private:
    static const U8 nCTileMenu = 4;
//    CTileMenu cMenuList[8];
    std::array<CTileMenu,nCTileMenu>cMenuList;
    std::array<CTileMenu*,nCTileMenu>pMenuList;
    S8 indexSelected;
//    Drawable& pCTileMenu[8];
};

#endif // SMENUPRINCIPALEVIEW_HPP
