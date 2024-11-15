#ifndef SUPGRADEFIRMWAREVIEW_HPP
#define SUPGRADEFIRMWAREVIEW_HPP

#include <gui_generated/supgradefirmware_screen/SUpgradeFirmwareViewBase.hpp>
#include <gui/supgradefirmware_screen/SUpgradeFirmwarePresenter.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "ARMLib/FileSystem/DrvFileSystem.h"
	#include "ARMLib/Common/FWUpgrade.h"
};


#define SUPGRADEFIRMWARE_MAXSHOWABBINAMENTO 4




class SUpgradeFirmwareView : public SUpgradeFirmwareViewBase
{
public:
    SUpgradeFirmwareView();
    virtual ~SUpgradeFirmwareView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    drvfilesystem_PeripheralsStatus_e	OldPeripheralsStatus;
    uint8_t AbbinamentiTrovati;
    uint8_t Selezionato;

    fwupgrade_Abbinamento_t Abbinamento[SUPGRADEFIRMWARE_MAXSHOWABBINAMENTO];
	bool AggiornaFirmware;
};

#endif // SUPGRADEFIRMWAREVIEW_HPP
