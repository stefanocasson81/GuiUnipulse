#include <gui/containers/CEditVal.hpp>
#include <touchgfx/Unicode.hpp>
#include <gui/custom/WeldingSupport.hpp>


extern "C"
{
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "CORELib/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
	#include "ARMLib/Common/Data.h"
};



CEditVal::CEditVal()
{
	OldValue = 0xffffffff;
	PtrStructData = _NULL_;
	PtrFunStrDedicShow = _NULL_;


	Unicode::strncpy(tAValBuffer,"XxX", TAVAL_SIZE-1);
	tAVal.invalidate();

	Unicode::strncpy(tAUdmBuffer," ", TAUDM_SIZE-1);
	tAUdm.invalidate();

	Seleziona(false);

	OldValue = 0xffffff;	// Tanto per rigenerare la prima visualizzazione
}

// Ritorna true sse dato modificato da utente
bool CEditVal::ManagerTickEvent(void)
{
	bool valRet = false;

	if(PtrStructData)
	{
		int32_t lA=0;
		int32_t value = data_GetValData(PtrStructData);
		char tmpStr[15];

		if(Selezionato)
		{	// Solo se selezionato è editabile
			lA=tgfxcustom_GetOffsetEncoder(ENCODER_RIGHT);
			if(lA)
			{
				value+=lA;
				value = data_SetValData(PtrStructData,value);
				valRet = true;
			}
		}

		if(OldValue!=value)
		{
			OldValue=value;

			if(PtrFunStrDedicShow)
				Unicode::strncpy(tAValBuffer,PtrFunStrDedicShow(value), TAVAL_SIZE-1);
			else
			{
				data_GetStrData(PtrStructData, tmpStr);

				Unicode::strncpy(tAValBuffer,tmpStr, TAVAL_SIZE-1);
			}
			tAVal.invalidate();
		}
	}

	return valRet;
}

void CEditVal::Seleziona(bool stato)
{
	Selezionato = stato;
	if(stato)
	{
		bBase.setAlpha(0xff);
		bBase.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	}
	else
		bBase.setAlpha(0x00);

	bBase.invalidate();
}


void CEditVal::SetPtrStructData(const data_Data_t *ptrStructData)
{
	PtrStructData = ptrStructData;
}

void CEditVal::initialize()
{
    CEditValBase::initialize();
}
