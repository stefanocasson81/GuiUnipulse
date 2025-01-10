#ifndef CWELDINGGENERICCONTAINER_HPP
#define CWELDINGGENERICCONTAINER_HPP

#include <gui_generated/containers/CWeldingGenericContainerBase.hpp>
#include <touchgfx/Callback.hpp>

class SWeldingProcessView;


class CWeldingGenericContainer : public CWeldingGenericContainerBase
{
public:
    CWeldingGenericContainer();
    virtual ~CWeldingGenericContainer() {}
	 virtual void seletcMenu(S8 menu,S8 submenu)=0;
	 virtual void confirmMenu(S8 menu)=0;
    virtual void initialize();
    //Method to set the view callback
	 void setMenuSelectedView_Callback(GenericCallback<U8,U8>& callback);
protected:
	GenericCallback<U8,U8>* MenuSelected_Callback;
};

#endif // CWELDINGGENERICCONTAINER_HPP
