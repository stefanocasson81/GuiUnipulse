#ifndef CWELDINGPROCESSMMA_HPP
#define CWELDINGPROCESSMMA_HPP

#include <touchgfx/Callback.hpp>
#include <gui/containers/CWeldingGenericContainerParam.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>
#include <array>
//extern "C"
//{
//#include "CORELib/UserInterface/ViperUI.h"
//#include "ARMLib/TGFX/TGFXCustom.h"
//#include "CORELib/Common/ViperDef.h"
//#include "Features.h"
//};

//class SWeldingProcessView;

class CWeldingContainerMma : public CWeldingGenericContainer
{
private:
    static const U8 nCWeldParam = 1;
    U8 indexMenu;
    ColorDefined colorSelection;
    std::array<CWeldingGenericContainerParam,nCWeldParam>CWeldingGenContParm;
public:
	CWeldingContainerMma();
    ~CWeldingContainerMma(){}

    virtual void initialize();
    void seletcMenu(S8 menu,S8 submenu);
    void confirmMenu(S8 preessed);
    void setContainerType(viperdef_Processo_e c){containerType = c;}
protected:
//    Callback<CustomContainer1, const Box&, const ClickEvent&> BoxClickedCallback;
//    SWeldingProcessView* viewPtr;

};

#endif // CWELDINGPROCESSMIG__HPP
