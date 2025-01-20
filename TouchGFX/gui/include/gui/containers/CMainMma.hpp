#ifndef CMAINMMA_HPP
#define CMAINMMA_HPP

#include <gui_generated/containers/CMainMmaBase.hpp>


extern "C"
{
	#include "ARMLib/Common/ComDef.h"
};



class CMainMma : public CMainMmaBase
{
public:
    CMainMma();
    virtual ~CMainMma();

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
	int32_t ValShowSX=0xffff;
private:
	U32 IdCan;
};

#endif // CMAINMMA_HPP
