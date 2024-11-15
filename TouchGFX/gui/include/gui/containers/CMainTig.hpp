#ifndef CMAINTIG_HPP
#define CMAINTIG_HPP

#include <gui_generated/containers/CMainTigBase.hpp>

class CMainTig : public CMainTigBase
{
public:
    CMainTig();
    virtual ~CMainTig() {}

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
	int32_t ValShowSX;
};

#endif // CMAINTIG_HPP
