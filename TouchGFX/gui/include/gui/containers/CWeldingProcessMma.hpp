#ifndef CWELDINGPROCESSMMA_HPP
#define CWELDINGPROCESSMMA_HPP

#include <gui_generated/containers/CWeldingProcessMmaBase.hpp>

class CWeldingProcessMma : public CWeldingProcessMmaBase
{
public:
    CWeldingProcessMma();
    virtual ~CWeldingProcessMma() {}

    virtual void initialize();

    void ManagerTickEvent(void);
protected:
};

#endif // CWELDINGPROCESSMMA_HPP
