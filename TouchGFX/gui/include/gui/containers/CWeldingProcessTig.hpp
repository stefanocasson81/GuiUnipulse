#ifndef CWELDINGPROCESSTIG_HPP
#define CWELDINGPROCESSTIG_HPP

#include <gui_generated/containers/CWeldingProcessTigBase.hpp>

class CWeldingProcessTig : public CWeldingProcessTigBase
{
public:
    CWeldingProcessTig();
    virtual ~CWeldingProcessTig() {}

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
};

#endif // CWELDINGPROCESSTIG_HPP
