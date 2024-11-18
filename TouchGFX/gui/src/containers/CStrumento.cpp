#include <gui/containers/CStrumento.hpp>

extern "C"
{
	#include "CORELib/Viper/UserInterface/ViperUI.h"
	#include "CORELib/Viper/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
	#include "ARMLib/Common/SwTimer.h"
};



CStrumento::CStrumento()
{
    ValShowCorrente=0xffff;
    ValShowTensione=0xffff;
    OldSwTimerTickCounter=0;
}

void CStrumento::initialize()
{
    CStrumentoBase::initialize();
}

// Ritorna true se deve essere rivisualizzato in primo piano
bool CStrumento::ManagerTickEvent(void)
{
	bool valRet=false;
	uint16_t uSA;
	char tmpStr[20];

	if(viperdef_Pack8GenTx_InfoStatus.GenInfo.Bits.CorrenteDiStartRilevata)
	{
		//OldSwTimerTickCounter=swtimer_TickCounter+(viperdef_Pack8GenTx_BasicSetup.DsDigitalMeterShow*100);

		OldSwTimerTickCounter=swtimer_TickCounter+(20*100);
	}

	if(OldSwTimerTickCounter>swtimer_TickCounter)
	{
		if(!isVisible())
		{
			bA.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			bV.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			linePainter.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
			line.setPainter(linePainter);
			boxWithBorder.setBorderColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

			valRet = true;
			setVisible(true);
			invalidate();
		}
	}
	else
	{
		if(isVisible())
		{
			setVisible(false);
			invalidate();
		}
	}

	if(isVisible())
	{
		uSA = viperdef_Pack8GenTx_Strumento.IoMedia;
		if(ValShowCorrente!=uSA)
		{
			ValShowCorrente=uSA;
			customstring_FormatFix(tmpStr, ValShowCorrente, 3,0, ' ',',', false);
			Unicode::strncpy(tACorrenteBuffer,tmpStr, TACORRENTE_SIZE);
			tACorrente.invalidate();
		}

		uSA = viperdef_Pack8GenTx_Strumento.VoDMedia;
		if(ValShowTensione!=uSA)
		{
			ValShowTensione=uSA;
			customstring_FormatFix(tmpStr, ValShowTensione, 2,1, ' ',',', false);
			// Tolgo il punto decimale
			tmpStr[2]=tmpStr[3];
			tmpStr[3]=0;
			Unicode::strncpy(tATensioneBuffer,tmpStr, TATENSIONE_SIZE);
			tATensione.invalidate();
			tAPuntino.invalidate();
		}
	}

	return valRet;
}
