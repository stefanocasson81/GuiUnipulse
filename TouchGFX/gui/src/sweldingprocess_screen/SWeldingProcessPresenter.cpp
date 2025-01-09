#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/sweldingprocess_screen/SWeldingProcessPresenter.hpp>
#include <gui/model/ModelListener.hpp>
#include <ARMLib/TGFX/cpp_define.hpp>

SWeldingProcessPresenter::SWeldingProcessPresenter(SWeldingProcessView& v)
    : view(v)
{

}

void SWeldingProcessPresenter::activate()
{

}

void SWeldingProcessPresenter::deactivate()
{

}


void SWeldingProcessPresenter::setGas(uint8_t gas)
{
   view.gasChanged(gas);
}

void SWeldingProcessPresenter::setProcess(uint8_t prc)
{
   view.processChanged(prc);
}


U8 SWeldingProcessPresenter::getProcess(void)
{
   return model->getProcess();
}



void SWeldingProcessPresenter::setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p)
{
   switch (e)
   {
      case Model::TGFXCUSTOM_ENCODER_LEFT:
         if ( p == Model::TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO )
         {
            view.GoBack();
         }
      break;
      case Model::TGFXCUSTOM_ENCODER_CENTER:

      break;
      case Model::TGFXCUSTOM_ENCODER_RIGHT:
         if ( p == Model::TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO )
         {
//            view.setMenu();
         }
      break;
      default:
      break;
   }
}

void SWeldingProcessPresenter::setOffsetEncoder(Model::Enc_Type e, S16 offset, S8 dir)
{
	switch (e)
	{
		case Model::TGFXCUSTOM_ENCODER_LEFT:
			if ( dir > 0 )
				view.encSX_incDec(1);
			else
				view.encSX_incDec(-1);
		break;
		case Model::TGFXCUSTOM_ENCODER_CENTER:

		break;

		case Model::TGFXCUSTOM_ENCODER_RIGHT:
			if ( dir > 0 )
				view.encRX_incDec(1);
			else
				view.encRX_incDec(-1);
		break;

		default:
		break;
   }
//   view.encRoffset(offset);

}




