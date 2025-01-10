
#ifndef TGFXUTIL_H_INCLUDED
#define TGFXUTIL_H_INCLUDED

#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Color.hpp>

/*
extern "C"
{
	#include "ARMLib/Common/ComDef.h"
};
*/


class tgfxguiutil_c
{
public:
	tgfxguiutil_c();
	virtual ~tgfxguiutil_c();

	//static uint16_t SpecialTextStrNCpy(touchgfx::Unicode::UnicodeChar* ptrDest, void* ptrSpecialTextOrg, uint16_t maxchars);
//	static uint16_t GetIdFromImage(void* ptrImage);
//	static void GetUnicodeStrListBox(appdata_Info_t *ptrAppDataInfo,float valore,touchgfx::Unicode::UnicodeChar *ptrUnicodeDest,uint16_t maxSize);
//	static float Riproporziona(uint16_t widthMax,uint16_t heightMax,int16_t widthOrg,int16_t heightOrg);
	//static colortype GetStdColor(uint32_t coloreRGB,float opacita);
protected:

};

/*extern guiutil_Init_t guiutil_Init;
touchgfx::colortype guiutil_GetStdColor(customgui_Colore_e colore,float opacita);
*/

#endif /* TGFXUTIL_H_INCLUDED */
