/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
 	 Serie di funzioni utility di supporto veloce per TouchGFX per la gestione
 	 menu a tile

------------------------------------------------------------------------------*/


#include <ARMLib/TGFX/TGFXMenu.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/TypedText.hpp>
#include <gui/containers/CTileMenu.hpp>

using namespace touchgfx;

extern "C" {
#include "ARMLib/TGFX/TGFXCustom.h"
}
;

tgfxmenu_c::tgfxmenu_c():CoordX(10),CoordY(44)
{
//   CoordX = 10;
//   CoordY = 44;
}

tgfxmenu_c::~tgfxmenu_c()
{

}

void tgfxmenu_c::CreateTile(CTileMenu *pContainer, touchgfx::TypedText ptext, uint32_t iconId, bool abilitato, uint8_t tag)
{
   pContainer->setXY(CoordX, CoordY);
   pContainer->invalidate();
   pContainer->Seleziona(false);
   pContainer->SetDescrizione(ptext);
   pContainer->SetIco(iconId);
   pContainer->Abilita(abilitato);
   pContainer->Tag = tag;

   CoordX += pContainer->getWidth() + 5;
   if ( CoordX >= 450 )
   {
      CoordX = 10;
      CoordY += pContainer->getHeight() + 5;
   }
}

