#ifndef CWELDINGGENERICCONTAINER_HPP
#define CWELDINGGENERICCONTAINER_HPP

#include <gui_generated/containers/CWeldingGenericContainerBase.hpp>
#include <touchgfx/Callback.hpp>
#include <ARMLib/TGFX/color_define.hpp>

class SWeldingProcessView;


class CWeldingGenericContainer : public CWeldingGenericContainerBase
{
public:
    CWeldingGenericContainer();
    virtual ~CWeldingGenericContainer() {}
	 virtual void seletcMenu(S8 menu,S8 submenu)=0;
	 virtual void confirmMenu(S8 menu)=0;
    virtual void initialize();
    virtual void setContainerType(viperdef_Processo_e)=0;
    virtual void seletcMig_WireDIameter(S8 menu){};
    virtual void seletcMig_WireType(S8 menu){};
    virtual void seletcMig_MigType(S8 menu){};
    virtual void seletcMig_GasType(S8 menu){};
    virtual viperdef_Processo_e getContainerType(void){return containerType;}
    //Method to set the view callback
    void setInitContainer_Callback(GenericCallback<void>& callback);
	 void setMenuSelectedView_Callback(GenericCallback<S8,S8>& callback);
protected:
	GenericCallback<void>* initContainer_Callback;
	GenericCallback<S8,S8>* menuSelected_Callback;
	viperdef_Processo_e containerType;
};

#endif // CWELDINGGENERICCONTAINER_HPP
