#include <gui/containers/CShow8Bit.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>

extern "C"
{
	#include "CORELib/Common/ViperDef.h"
};



CShow8Bit::CShow8Bit()
{
	OldValue = 0xaa;
	PtrData = _NULL_;
	PrimoGiro = true;

	ManagerTickEvent();	// Prima rigenerata
}

void CShow8Bit::initialize()
{
    CShow8BitBase::initialize();
}

void CShow8Bit::SetPtrtData(uint8_t *ptrData)
{
	PtrData = ptrData;
}

void CShow8Bit::ManagerTickEvent(void)
{
	uint8_t value;
	uint8_t uCA;

	touchgfx::Box *tabBox[]={&bB0,&bB1,&bB2,&bB3,&bB4,&bB5,&bB6,&bB7};

	if(PtrData)
	{
		value = *PtrData;
		if(PrimoGiro)
		{
			PrimoGiro = false;
			OldValue=value^0xff;	// Tanto per rinfrescare la prima volta
		}

		if(OldValue!=value)
		{
			uCA = OldValue^value;

			for(uCA=0;uCA<8;uCA++)
			{
				if(((OldValue^value)&(_B0_<<uCA)))
				{
					if(value&(_B0_<<uCA))
						tabBox[uCA]->setColor(touchgfx::Color::getColorFromRGB(0xff, 0, 0));
					else
						tabBox[uCA]->setColor(touchgfx::Color::getColorFromRGB(0, 0xff, 00));

					tabBox[uCA]->invalidate();
				}
			}

			OldValue=value;
		}
	}
}
