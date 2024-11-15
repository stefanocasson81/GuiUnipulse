#ifndef CSTRUMENTO_HPP
#define CSTRUMENTO_HPP

#include <gui_generated/containers/CStrumentoBase.hpp>

class CStrumento : public CStrumentoBase
{
public:
    CStrumento();
    virtual ~CStrumento() {}

    virtual void initialize();
    bool ManagerTickEvent(void);
protected:
    uint16_t ValShowCorrente;
    uint16_t ValShowTensione;
    uint32_t OldSwTimerTickCounter;
};

#endif // CSTRUMENTO_HPP
