#ifndef CEDITVAL_HPP
#define CEDITVAL_HPP

#include <gui_generated/containers/CEditValBase.hpp>

extern "C"
{
	#include "ARMLib/Common/Data.h"
};


class CEditVal : public CEditValBase
{
public:
    CEditVal();
    virtual ~CEditVal() {}

    virtual void initialize();
    bool ManagerTickEvent(void);

    void SetPtrStructData(const data_Data_t *ptrStructData);
    void Seleziona(bool stato);

    char* (*PtrFunStrDedicShow)(int32_t value);
protected:
    const data_Data_t *PtrStructData;
    bool Selezionato;
    int32_t OldValue;
};

#endif // CEDITVAL_HPP
