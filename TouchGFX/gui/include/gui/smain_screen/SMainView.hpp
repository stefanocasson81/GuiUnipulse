#ifndef SMAINVIEW_HPP
#define SMAINVIEW_HPP

#include <gui_generated/smain_screen/SMainViewBase.hpp>
#include <gui/smain_screen/SMainPresenter.hpp>

extern "C"
{
	#include "CORELib/Common/ViperDef.h"
};




class SMainView : public SMainViewBase
{
public:
    SMainView();
    virtual ~SMainView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

protected:

    Drawable *PtrQuadro;
    viperdef_Processo_e ShowProcesso;
    viperdef_TipoMig_e ShowTipoMig;

};

#endif // SMAINVIEW_HPP
