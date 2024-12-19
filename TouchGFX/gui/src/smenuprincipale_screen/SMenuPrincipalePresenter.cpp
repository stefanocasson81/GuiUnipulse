#include <gui/smenuprincipale_screen/SMenuPrincipaleView.hpp>
#include <gui/smenuprincipale_screen/SMenuPrincipalePresenter.hpp>

SMenuPrincipalePresenter::SMenuPrincipalePresenter(SMenuPrincipaleView& v)
    : view(v)
{

}

void SMenuPrincipalePresenter::activate()
{

}

void SMenuPrincipalePresenter::deactivate()
{

}

void SMenuPrincipalePresenter::setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p)
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
            view.setMenu();
         }
      break;
      default:
      break;
   }
}

void SMenuPrincipalePresenter::setOffsetEncoder(Model::Enc_Type e, S16 offset, S8 dir)
{
   switch (e)
   {
      case Model::TGFXCUSTOM_ENCODER_LEFT:

      break;
      case Model::TGFXCUSTOM_ENCODER_CENTER:

      break;

      case Model::TGFXCUSTOM_ENCODER_RIGHT:
         if ( dir > 0 )
            view.encRup();
         else
            view.encRdown();
      break;

      default:
      break;
   }
}
