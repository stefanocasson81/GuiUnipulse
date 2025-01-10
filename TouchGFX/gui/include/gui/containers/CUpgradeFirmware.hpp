#ifndef CUPGRADEFIRMWARE_HPP
#define CUPGRADEFIRMWARE_HPP

#include <gui_generated/containers/CUpgradeFirmwareBase.hpp>

extern "C"
{
	#include "ARMLib/Common/FWUpgrade.h"
};

class CUpgradeFirmware : public CUpgradeFirmwareBase
{
public:
    CUpgradeFirmware();
    virtual ~CUpgradeFirmware() {}

    virtual void initialize();

    void SetPtrAbbinamento(fwupgrade_Abbinamento_t *ptrAbbinamento);

    void Refresh(void);
protected:
    fwupgrade_Abbinamento_t *PtrAbbinamento;
};

#endif // CUPGRADEFIRMWARE_HPP
