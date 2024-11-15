#ifndef SDEBUGVARVIEW_HPP
#define SDEBUGVARVIEW_HPP

#include <gui_generated/sdebugvar_screen/SDebugVarViewBase.hpp>
#include <gui/sdebugvar_screen/SDebugVarPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/containers/CEditVal.hpp>

#define SDEBUGVARVIEW_NELEMENTCEDITVAL	8*2+4*2+2*2

class SDebugVarView : public SDebugVarViewBase
{
public:
    SDebugVarView();
    virtual ~SDebugVarView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    CEditVal cEditValTab[SDEBUGVARVIEW_NELEMENTCEDITVAL];

    uint8_t IndiceSelezionato;
    uint8_t IndiceMaxSelezionabile;
    uint8_t OldIndiceSelezionato;
};

#endif // SDEBUGVARVIEW_HPP
