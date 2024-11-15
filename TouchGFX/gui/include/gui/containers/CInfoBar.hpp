#ifndef CINFOBAR_HPP
#define CINFOBAR_HPP

#include <gui_generated/containers/CInfoBarBase.hpp>

class CInfoBar : public CInfoBarBase
{
public:
    CInfoBar();
    virtual ~CInfoBar() {}

    virtual void initialize();

    void SetTitolo(char *ptrStr);

    void ManagerTickEvent(void);
    void SetIco(uint32_t icoId);
protected:
    uint32_t OldMsTickCounter;
};

#endif // CINFOBAR_HPP
