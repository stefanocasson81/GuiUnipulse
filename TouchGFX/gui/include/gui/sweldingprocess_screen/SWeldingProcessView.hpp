#ifndef SWELDINGPROCESSVIEW_HPP
#define SWELDINGPROCESSVIEW_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui_generated/sweldingprocess_screen/SWeldingProcessViewBase.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>
#include <touchgfx/Callback.hpp>




class SWeldingProcessView : public SWeldingProcessViewBase
{
public:
    SWeldingProcessView();
    virtual ~SWeldingProcessView();
    virtual void setupScreen();
    virtual void tearDownScreen();

//    virtual void handleTickEvent();

    void processChanged(uint8_t prc);
    void setEncoderPression(U8 i, U8 p);
    void GoBack(void);
    void encLdown(void);
    void encLup(void);
//    void encRdown(void);
    void encRX_incDec(S8 toward);
    void encSX_incDec(S8 toward);
    void setMenuContainer(viperui_AttualeSelezioneProcesso_e m);
    void encSoffset(S16 off);
//    void encRup(void);
    void encRXconfirm(void);

    void updateWireType(viperdef_TipoFilo_e wiretype );
    void updateWireDiameter(viperdef_DiametroFilo_e wireDiameter);
    void updateGasType(viperdef_TipoGas_e gastype );
    void updateMigType(viperdef_TipoMig_e  migtype);

    viperdef_TipoFilo_e getWireType(void);
    viperdef_DiametroFilo_e getWireDiameter(void);
    viperdef_TipoGas_e getGasType(void);
    viperdef_TipoMig_e getMigType(void);

    void fromContainerInitContainer_Callback(void);
    void fromContainerMenuSelected_Callback(S8 d,S8 e);
    void createSelectedProcess(viperdef_Processo_e t);

protected:
    CWeldingGenericContainer *PtrQuadro;
    Callback<SWeldingProcessView> setInitContainer_Callback;
    Callback<SWeldingProcessView, S8,S8> setMenuSelectedContainer_Callback;

private :
    S8 process;
    S8 focusMenu;
//    U8 encR;
//	 U8 encL;
//    U8 indexMenu;
};

#endif // SWELDINGPROCESSVIEW_HPP
