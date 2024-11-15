#ifndef SMENUADVANCEVIEW_HPP
#define SMENUADVANCEVIEW_HPP

#include <gui_generated/smenuadvance_screen/SMenuAdvanceViewBase.hpp>
#include <gui/smenuadvance_screen/SMenuAdvancePresenter.hpp>
#include "ARMLib/GUI/TGFX/TGFXMenu.hpp"

class SMenuAdvanceView : public SMenuAdvanceViewBase
{
public:
    SMenuAdvanceView();
    virtual ~SMenuAdvanceView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    tgfxmenu_c TGFXMenu;
};

#endif // SMENUADVANCEVIEW_HPP
