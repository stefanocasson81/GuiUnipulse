#ifndef SWELDINGPROCESSVIEW_HPP
#define SWELDINGPROCESSVIEW_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui_generated/sweldingprocess_screen/SWeldingProcessViewBase.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>
//#include <gui/custom/CWeldingContainerMig.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>
#include <touchgfx/Callback.hpp>

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

    void fromContainerMenuSelected_Callback(U8 d,U8 e);
    void createSelectedProcess(Model::Process_Type t);

protected:
    CWeldingGenericContainer *PtrQuadro;
    Callback<SWeldingProcessView, U8,U8> setMenuSelectedContainer_Callback;

private :
    U8 process;
    U8 focusMenu;
    U8 encR;
	 U8 encL;
    U8 indexMenu;
};

#endif // SWELDINGPROCESSVIEW_HPP
