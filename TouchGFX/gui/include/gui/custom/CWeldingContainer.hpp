#ifndef CWELDINCONTAINER___HPP
#define CWELDINCONTAINER___HPP

#include <ARMLib/TGFX/cpp_define.hpp>
//#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
class SWeldingProcessView;

class CWeldingProcess__
{
//public:
//	CWeldingProcess__(SWeldingProcessView& v);
//    CWeldingProcess__() : viewPtr(0){}
//	 CWeldingProcess__(){}
    virtual ~CWeldingProcess__(){}

    virtual void seletcMenu(S8 menu)=0;
//    virtual void setMenuList(U8 list);
//    void setPtr(SWeldingProcessView* ptr)
//    {
//   	 viewPtr = ptr;
//    }

//    virtual void encDxOffset(S16 v);
//    virtual void encSxOffset(S16 v);
//    virtual U16 getMenuList();

protected:
//        Callback<CustomContainer1, const Box&, const ClickEvent&> BoxClickedCallback;
//        SWeldingProcessView& viewPtr;
//    SWeldingProcessView* viewPtr;
};

#endif // CWELDINCONTAINER___HPP
