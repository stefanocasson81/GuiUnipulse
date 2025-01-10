#ifndef SPROVEGUIVIEW_HPP
#define SPROVEGUIVIEW_HPP

#include <gui_generated/sprovegui_screen/SProveGUIViewBase.hpp>
#include <gui/sprovegui_screen/SProveGUIPresenter.hpp>

class SProveGUIView : public SProveGUIViewBase
{
public:
    SProveGUIView();
    virtual ~SProveGUIView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    uint8_t IndiceSelezionato;
    uint8_t IndiceMaxSelezionabile;
    uint8_t OldIndiceSelezionato;
};

#endif // SPROVEGUIVIEW_HPP
