#include <gui/containers/CInfoBar.hpp>
#include <touchgfx/Unicode.hpp>
#include "FreeRTOS.h"


extern "C"
{
	#include "ARMLib/Common/CustomString.h"
	#include "CORELib/Common/ViperDef.h"
	#include "ARMLib/Common/Data.h"
	#include "CORELib/UserInterface/ViperUI.h"
	#include "_Board.h"
	#include "ARMLib/Common/SwTimer.h"
	#include "Applicazione.h"
};


CInfoBar::CInfoBar()
{
	OldMsTickCounter = 0;

	if(applicazione_Info.ShowDebug)
		tAHeap.setVisible(true);
	else
		tAHeap.setVisible(false);

	ManagerTickEvent();	// Tanto per fare la prima generata
}

void CInfoBar::initialize()
{
    CInfoBarBase::initialize();
}

void CInfoBar::SetTitolo(touchgfx::TypedText ptrStr)
{
	tATitolo.setTypedText(touchgfx::TypedText(ptrStr));
	tATitolo.invalidate();
}

void CInfoBar::ManagerTickEvent(void)
{
	char tmpStr[50];

	if(swtimer_TickCounter-OldMsTickCounter>500)
	{
		OldMsTickCounter = swtimer_TickCounter;
		snprintf(tmpStr,sizeof(tmpStr)-1,"H:%06d/Tx:%05d/Rx:%05d",(int)xPortGetFreeHeapSize(),(int)_board_Info.PtrCanBusObj->TotNPacketTrasmessi,(int)_board_Info.PtrCanBusObj->TotNPacketRicevuti);
		Unicode::strncpy(tAHeapBuffer,tmpStr, TAHEAP_SIZE-1);
		tAHeap.invalidate();
	}
}


void CInfoBar::SetIco(uint32_t icoId)
{
	if(icoId==0xffffffff)
		image.setVisible(false);
	else
	{
		image.setVisible(true);
		image.setBitmap(touchgfx::Bitmap(icoId));
	}

	image.invalidate();
}



