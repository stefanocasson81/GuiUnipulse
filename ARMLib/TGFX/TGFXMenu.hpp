
#ifndef TGFXMENU_H_INCLUDED
#define TGFXMENU_H_INCLUDED

#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Color.hpp>
#include <gui/containers/CTileMenu.hpp>

/*
extern "C"
{
	#include "ARMLib/Common/ComDef.h"
};
*/

#define TGFXMENU_MAXTILE 8

class tgfxmenu_c : public touchgfx::Container
{

public:
	tgfxmenu_c();
	virtual ~tgfxmenu_c();

	void CreateTile(CTileMenu* pContainer,touchgfx::TypedText ptext,uint32_t iconId,bool abilitato,uint8_t tag);
	uint8_t ManagerTickEvent(void);

protected:
    int16_t CoordX;
    int16_t CoordY;
};

/*extern guiutil_Init_t guiutil_Init;
touchgfx::colortype guiutil_GetStdColor(customgui_Colore_e colore,float opacita);
*/

#endif /* TGFXMENU_H_INCLUDED */
