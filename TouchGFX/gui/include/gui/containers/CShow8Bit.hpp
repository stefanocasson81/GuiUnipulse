#ifndef CSHOW8BIT_HPP
#define CSHOW8BIT_HPP

#include <gui_generated/containers/CShow8BitBase.hpp>

class CShow8Bit : public CShow8BitBase
{
public:
    CShow8Bit();
    virtual ~CShow8Bit() {}

    virtual void initialize();
    void ManagerTickEvent(void);

    void SetPtrtData(uint8_t *ptrData);
protected:
    uint8_t *PtrData;
    uint8_t OldValue;
    bool PrimoGiro;
};

#endif // CSHOW8BIT_HPP
