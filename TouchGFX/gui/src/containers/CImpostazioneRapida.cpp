#include <gui/containers/CImpostazioneRapida.hpp>

extern "C"
{
	#include "ARMLib/Common/Data.h"
	#include "ViperUISupport.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
};


CImpostazioneRapida::CImpostazioneRapida()
{
	PtrData = _NULL_;
	ValShow = 0xffff;

	bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));

	Seleziona(false);
}

void CImpostazioneRapida::initialize()
{
    CImpostazioneRapidaBase::initialize();
}

void CImpostazioneRapida::Seleziona(bool seleziona)
{
	Selezionato = seleziona;
	if(seleziona)
		bBase.setVisible(true);
	else
		bBase.setVisible(false);

	bBase.invalidate();
}

void CImpostazioneRapida::SetInfo(data_Data_t *ptrData,uint32_t enumTypeText)
{
	PtrData = ptrData;
	Unicode::strncpy(tATitoloBuffer,touchgfx::TypedText(enumTypeText).getText(), TATITOLO_SIZE-1);
	tATitolo.invalidate();
}

void CImpostazioneRapida::SetInfo(data_Data_t *ptrData,char *ptrStrTitolo)
{
	PtrData = ptrData;
	Unicode::strncpy(tATitoloBuffer,ptrStrTitolo, TATITOLO_SIZE-1);
	tATitolo.invalidate();
}

void CImpostazioneRapida::SetIco(uint32_t icoId)
{
	int16_t sA;

	if(icoId==0xffffffff)
		image.setVisible(false);
	else
	{
		image.setVisible(true);
		image.setBitmap(touchgfx::Bitmap(icoId));

		// Riposiziono al centro
		sA = getWidth()/2;
		sA-= (image.getWidth()/2);
		image.setX(sA);
	}

	image.invalidate();
}


void CImpostazioneRapida::ManagerTickEvent(void)
{
	int32_t value;
	char tmpStr[20];
	int16_t sA;

	if(PtrData && visible)
	{
		if(Selezionato)
		{
			sA=tgfxcustom_GetOffsetEncoder(TGFXCUSTOM_ENCODER_RIGHT);
			if(sA)
			{
				sA += data_GetValData(PtrData);
				data_SetValData(PtrData,sA);
			}
		}

		value = data_GetValData(PtrData);

		if(ValShow!=value)
		{
			ValShow = value;

			viperuisupport_GetTxtPtrData(PtrData,tmpStr, sizeof(tmpStr));
			Unicode::strncpy(tAValueBuffer,tmpStr, TAVALUE_SIZE-1);
			tAValue.invalidate();
		}
	}
}
