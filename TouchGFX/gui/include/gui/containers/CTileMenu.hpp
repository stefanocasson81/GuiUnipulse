#ifndef CTILEMENU_HPP
#define CTILEMENU_HPP

#include <gui_generated/containers/CTileMenuBase.hpp>

class CTileMenu : public CTileMenuBase
{
public:
    CTileMenu();
    virtual ~CTileMenu() {}

    virtual void initialize();

    void Seleziona(bool stato);
    void Abilita(bool stato);
;
    void SetDescrizione(touchgfx::TypedText ptrUnicodeChar);

    void SetIco(uint32_t icoId);
    void SetIcoSelected();

    uint32_t Tag;

    bool Abilitato;
protected:
};

#endif // CTILEMENU_HPP
