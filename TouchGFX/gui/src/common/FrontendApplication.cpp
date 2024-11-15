#include <gui/common/FrontendApplication.hpp>
#include "gui/smain_screen/SMainView.hpp"
#include "gui/smenuprincipale_screen/SMenuPrincipaleView.hpp"
#include "gui/smenudebug_screen/SMenuDebugView.hpp"
#include "gui/smenuadvance_screen/SMenuAdvanceView.hpp"
#include "gui/shwcheck_screen/SHwCheckView.hpp"

// Aggiunti include per le transizioni
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/CoverTransition.hpp>
#include <mvp/MVPApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::VisualizzaSMain()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSMainImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSMainImpl()
{
//	touchgfx::makeTransition<QuadroMenuSetupView, QuadroMenuSetupPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
    touchgfx::makeTransition<SMainView, SMainPresenter, touchgfx::CoverTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSBasic()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSBasicImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSBasicImpl()
{
//	touchgfx::makeTransition<QuadroMenuSetupView, QuadroMenuSetupPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
    touchgfx::makeTransition<SBasicView, SBasicPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSSetup()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSSetupImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSSetupImpl()
{
//	touchgfx::makeTransition<QuadroMenuSetupView, QuadroMenuSetupPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
    touchgfx::makeTransition<SSetupView, SSetupPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSWeldingProcess()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSWeldingProcessImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSWeldingProcessImpl()
{
//	touchgfx::makeTransition<QuadroMenuSetupView, QuadroMenuSetupPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
    touchgfx::makeTransition<SWeldingProcessView, SWeldingProcessPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSMenuPrincipale()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSMenuPrincipaleImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSMenuPrincipaleImpl()
{
//	touchgfx::makeTransition<QuadroMenuSetupView, QuadroMenuSetupPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
    touchgfx::makeTransition<SMenuPrincipaleView, SMenuPrincipalePresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSProveGUI()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSProveGUIImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSProveGUIImpl()
{
    touchgfx::makeTransition<SProveGUIView, SProveGUIPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSHwCheck()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSHwCheckImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSHwCheckImpl()
{
    touchgfx::makeTransition<SHwCheckView, SHwCheckPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSHwCheckTraino()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSHwCheckTrainoImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSHwCheckTrainoImpl()
{
    touchgfx::makeTransition<SHwCheckTrainoView, SHwCheckTrainoPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSDebugVar()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSDebugVarImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSDebugVarImpl()
{
    touchgfx::makeTransition<SDebugVarView, SDebugVarPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSInfo()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSInfoImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSInfoImpl()
{
    touchgfx::makeTransition<SInfoView, SInfoPresenter, touchgfx::CoverTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSOrologio()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSOrologioImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSOrologioImpl()
{
    touchgfx::makeTransition<SOrologioView, SOrologioPresenter, touchgfx::CoverTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSMenuDebug()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSMenuDebugImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSMenuDebugImpl()
{
    touchgfx::makeTransition<SMenuDebugView, SMenuDebugPresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSMenuAdvance()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSMenuAdvanceImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSMenuAdvanceImpl()
{
    touchgfx::makeTransition<SMenuAdvanceView, SMenuAdvancePresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::VisualizzaSUpgradeFirmware()
{
    customTransitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::VisualizzaSUpgradeFirmwareImpl);
    pendingScreenTransitionCallback = &customTransitionCallback;
}
void FrontendApplication::VisualizzaSUpgradeFirmwareImpl()
{
    touchgfx::makeTransition<SUpgradeFirmwareView, SUpgradeFirmwarePresenter, touchgfx::CoverTransition<SOUTH>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
