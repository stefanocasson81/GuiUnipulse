#ifndef SWELDINGMIGVIEW_HPP
#define SWELDINGMIGVIEW_HPP

#include <gui_generated/sweldingmig_screen/SWeldingMigViewBase.hpp>
#include <gui/sweldingmig_screen/SWeldingMigPresenter.hpp>

class SWeldingMigView : public SWeldingMigViewBase
{
public:
    SWeldingMigView();
    virtual ~SWeldingMigView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SWELDINGMIGVIEW_HPP
