#include <gui/containers/CTileMenu.hpp>

extern "C"
{
	#include "CORELib/Viper/UserInterface/ViperUI.h"
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	#include "CORELib/Viper/Common/ViperDef.h"
	#include "ARMLib/Common/CustomString.h"
};

CTileMenu::CTileMenu()
{
	Abilita(true);
}

void CTileMenu::initialize()
{
    CTileMenuBase::initialize();
}

void CTileMenu::Seleziona(bool stato)
{
	if(stato)
		bSfondo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE]));
	else
		bSfondo.setColor(colortype(viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO]));

	invalidate();
}

void CTileMenu::Abilita(bool stato)
{
	if(stato)
	{
		bOscuramento.setVisible(false);
		iLucchetto.setVisible(false);
	}
	else
	{
		bOscuramento.setVisible(true);
		iLucchetto.setVisible(true);
	}

	Abilitato = stato;

	invalidate();
}

void CTileMenu::SetDescrizione(char *ptrStr)
{
	Unicode::strncpy(tADescrizioneBuffer,ptrStr, TADESCRIZIONE_SIZE-1);

	tADescrizione.invalidate();
}

void CTileMenu::SetDescrizione(const Unicode::UnicodeChar* ptrUnicodeChar)
{
	Unicode::strncpy(tADescrizioneBuffer,ptrUnicodeChar, TADESCRIZIONE_SIZE-1);

	tADescrizione.invalidate();
}

void CTileMenu::SetIco(uint32_t icoId)
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

