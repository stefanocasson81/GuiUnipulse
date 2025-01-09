#ifndef SWELDINGPROCESSVIEW_HPP
#define SWELDINGPROCESSVIEW_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui_generated/sweldingprocess_screen/SWeldingProcessViewBase.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>
#include <gui/custom/CWeldingContainerMig.hpp>
//#include <gui/containers/CWeldingGenericContainerParam.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>
//#include <gui/custom/CWeldingPrsMig.hpp>
//#include <Array>
extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Common/ViperDef.h"
};




class SWeldingProcessView : public SWeldingProcessViewBase
{
public:
    SWeldingProcessView();
    virtual ~SWeldingProcessView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void gasChanged(uint8_t gas);
    void processChanged(uint8_t prc);
    void setEncoderPression(U8 i, U8 p);
    void GoBack(void);
    void encLdown(void);
    void encLup(void);
//    void encRdown(void);
    void encRX_incDec(S8 toward);
    void encSX_incDec(S8 toward);
    void encSoffset(S16 off);
//    void encRup(void);
    void encRconfirm(void);
    void setWireCB();
    void setMigConfiguration();

    void MenucSelectedeContainer(U8 d,U8 e);

protected:
    CWeldingGenericContainer *PtrQuadro;
//    CWeldingPrsMig *PtrQuadro;
//    CWeldingPrsMig CMig;
    U8 selectedProcess;
    Callback<SWeldingProcessView, U8,U8> setMenuSelectedContainer_Callback;

private :
//    static const U8 nWeldingContainerParam = 4;
//    std::array<CWeldingGenericContainerParam*,nWeldingContainerParam>pCWeldingSetParamGeneric;
    S16 Process,menuList;
    U8 ActualSelection;
    U8 encR,encL;
    U8 indexMenu;
//    ColorDefined color;

};

#endif // SWELDINGPROCESSVIEW_HPP
