#ifndef SWELDINGPROCESSPRESENTER_HPP
#define SWELDINGPROCESSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
//extern "C"
//{
//   #include <CORELib/Common/ViperDef.h>
//}

using namespace touchgfx;

class SWeldingProcessView;

class SWeldingProcessPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SWeldingProcessPresenter(SWeldingProcessView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~SWeldingProcessPresenter() {};

   /**
    * Set Functions
    **/
   void setFocusIdVariableToModel(S8 Menu,S8 Process);
   void setProcessToModel(viperdef_Processo_e p);
   void setMigWeldingProcessMenu(viperui_AttualeSelezioneProcesso_e selection);
   void setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p);
   void setOffsetEncoder(Model::Enc_Type e, S16 offset, S8 dir);
   void setWireType(viperdef_TipoFilo_e wiretype );
   void setWireDiameter(viperdef_DiametroFilo_e wireDiameter);
   void setGasType(viperdef_TipoGas_e gastype );
   void setMigType(viperdef_TipoMig_e  migtype);
   viperui_AttualeSelezioneProcesso_e getActualSelezioneProcesso(void);
   void setProcessWelding(viperdef_Processo_e process);
   viperdef_TipoFilo_e getWireType(void);
   viperdef_DiametroFilo_e getWireDiameter(void);
   viperdef_TipoGas_e getGasType(void);
   viperdef_TipoMig_e getMigType(void);
//   void setFocusMenu(U8);
   /**
    * Get Functions
    **/
   viperdef_Processo_e getProcess();

//    void wireChanged(uint8_t type)
//    {
//       view.wireChanged(type);
//    }

private:
    SWeldingProcessPresenter();

    SWeldingProcessView& view;

    /*****copy variables******/
    viperui_AttualeSelezioneProcesso_e ActualSelezioneProcesso;
};

#endif // SWELDINGPROCESSPRESENTER_HPP
