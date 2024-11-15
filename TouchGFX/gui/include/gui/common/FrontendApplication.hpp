#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

    void VisualizzaSMain();
    void VisualizzaSBasic();
    void VisualizzaSSetup();
    void VisualizzaSWeldingProcess();
    void VisualizzaSMenuPrincipale();
    void VisualizzaSProveGUI();
    void VisualizzaSHwCheck();
    void VisualizzaSDebugVar();
    void VisualizzaSOrologio();
    void VisualizzaSInfo();
    void VisualizzaSMenuAdvance();
    void VisualizzaSMenuDebug();
    void VisualizzaSUpgradeFirmware();
    void VisualizzaSHwCheckTraino();

private:
    touchgfx::Callback<FrontendApplication> customTransitionCallback;

    void VisualizzaSMainImpl();
    void VisualizzaSBasicImpl();
    void VisualizzaSSetupImpl();
    void VisualizzaSWeldingProcessImpl();
    void VisualizzaSMenuPrincipaleImpl();
    void VisualizzaSProveGUIImpl();
    void VisualizzaSHwCheckImpl();
    void VisualizzaSDebugVarImpl();
    void VisualizzaSOrologioImpl();
    void VisualizzaSInfoImpl();
    void VisualizzaSMenuAdvanceImpl();
    void VisualizzaSMenuDebugImpl();
    void VisualizzaSUpgradeFirmwareImpl();
    void VisualizzaSHwCheckTrainoImpl();
};

#endif // FRONTENDAPPLICATION_HPP
