#ifndef CWELDINGPRSMIG_HPP
#define CWELDINGPRSMIG_HPP




#include <gui/containers/CWeldingGenericContainerParam.hpp>
#include <gui/containers/CWeldingGenericContainer.hpp>
#include <ARMLib/TGFX/color_define.hpp>
#include <touchgfx/Callback.hpp>
#include <array>


class CWeldingContainerMig : public CWeldingGenericContainer
{
private:

    U8 indexMenu;
    U8 indexSubMenu;
    static const U8 nCWeldParam = 3;
    std::array<CWeldingGenericContainerParam,nCWeldParam>CWeldingGenContParm;
    U8 maskDiameter;

public:
	 CWeldingContainerMig();
    ~CWeldingContainerMig(){}


   virtual void initialize();

   void seletcMenu(S8 menu,S8 submenu);
   void seletcMig_WireDIameter(S8 menu);
   void seletcMig_MigType(S8 menu);
   void seletcMig_WireType(S8 menu);
   void seletcMig_GasType(S8 menu);
   void confirmMenu(S8 preessed);
   void setContainerType(viperdef_Processo_e c){containerType = c;}

protected:
//    Callback<CustomContainer1, const Box&, const ClickEvent&> BoxClickedCallback;
//    SWeldingProcessView* viewPtr;
//   CWeldingGenericContainerParam CWeldingGenContParm[3];
//   SWeldingProcessView* viewPtr;
};

#endif // CWELDINGPRSMIG__HPP
