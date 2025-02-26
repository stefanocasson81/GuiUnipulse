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
   void setProcessToModel(Model::Process_Type p);
   void setGas(uint8_t gas);
   void setProcess(uint8_t prc);
   void setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p);
   void setOffsetEncoder(Model::Enc_Type e, S16 offset, S8 dir);
//   void setFocusMenu(U8);
   /**
    * Get Functions
    **/
   Model::Process_Type getProcess();

//    void wireChanged(uint8_t type)
//    {
//       view.wireChanged(type);
//    }

private:
    SWeldingProcessPresenter();

    SWeldingProcessView& view;
};

#endif // SWELDINGPROCESSPRESENTER_HPP
