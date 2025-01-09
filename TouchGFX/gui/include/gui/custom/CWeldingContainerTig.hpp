#ifndef CWELDINGPROCESSTIG_HPP
#define CWELDINGPROCESSTIG_HPP


#include <touchgfx/Callback.hpp>
#include <gui/containers/CWeldingGenericContainerParam.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>

//extern "C"
//{
//#include "CORELib/UserInterface/ViperUI.h"
//#include "ARMLib/TGFX/TGFXCustom.h"
//#include "CORELib/Common/ViperDef.h"
//#include "Features.h"
//};

//class SWeldingProcessView;

class CWeldingContainerTig : public CWeldingGenericContainer
{
public:
   CWeldingContainerTig();
    ~CWeldingContainerTig(){}


    virtual void initialize();
    void seletcMenu(S8 menu,S8 submenu);
    void confirmMenu(S8 preessed);
//    void setMenuList(U8 list);
//    void setPtr(SWeldingProcessView* ptr);
//    void encDxOffset(S16 v);
//    void encSxOffset(S16 v);
//    void setViewCallback(GenericCallback<uint8_t>& callback);
protected:
//    Callback<CustomContainer1, const Box&, const ClickEvent&> BoxClickedCallback;
//    SWeldingProcessView* viewPtr;
private:
    static const U8 nCWeldParam = 2;
    U8 indexMenu;
//    CWeldingGenericContainerParam cWeldingSetParamGeneric1;
//    CWeldingGenericContainerParam cWeldingSetParamGeneric2;
//    CWeldingGenericContainerParam cWeldingSetParamGeneric3;
    ColorDefined colorSelection;
    std::array<CWeldingGenericContainerParam,nCWeldParam>CWeldingGenContParm;
};

#endif // CWELDINGPROCESSMIG__HPP
