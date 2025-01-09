#ifndef CWELDINGPRSMIG_HPP
#define CWELDINGPRSMIG_HPP

//#include <gui/custom/CWeldingProcess__.hpp>
#include <touchgfx/Callback.hpp>

#include <gui/containers/CWeldingGenericContainerParam.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>

//#include <array>

extern "C"
{
   #include "ARMLib/Common/ComDef.h"
   #include "CORELib/Common/ViperDef.h"
   #include "CORELib/UserInterface/ViperUI.h"
};

class CWeldingContainerMig : public CWeldingGenericContainer
{
private:

    U8 indexMenu;
    ColorDefined colorSelection;
    static const U8 nCWeldParam = 3;
    std::array<CWeldingGenericContainerParam,nCWeldParam>CWeldingGenContParm;

public:
	 CWeldingContainerMig();
    ~CWeldingContainerMig(){}


   virtual void initialize();

   void seletcMenu(S8 menu);
   void seletcSubMenu(S8 menu);
   void confirmMenu(S8 preessed);
//   void setPtr(SWeldingProcessView* ptr);
//    void setMenuList(U8 list);
//    void setPtr(SWeldingProcessView* ptr);
//    void encDxOffset(S16 v);
//    void encSxOffset(S16 v);
//   void setViewCallback(GenericCallback<uint8_t>& callback);
protected:
//    Callback<CustomContainer1, const Box&, const ClickEvent&> BoxClickedCallback;
//    SWeldingProcessView* viewPtr;
//   CWeldingGenericContainerParam CWeldingGenContParm[3];
//   SWeldingProcessView* viewPtr;
};

#endif // CWELDINGPRSMIG__HPP
