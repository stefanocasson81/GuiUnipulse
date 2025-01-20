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
   model->setCurrentScreen(Model::View_WeldingProcess);
}

void SWeldingProcessPresenter::deactivate()
{

}


/*********************************************
 * FUNCTION SET SOMETHING TO MODEL
 **********************************************/
void SWeldingProcessPresenter::setProcessToModel(viperdef_Processo_e p)
{
   model->viper_Info.viperui_setProcess(p);
}

void SWeldingProcessPresenter::setFocusIdVariableToModel(S8 m,S8 p)
{
//   model->viper_Info.viperui_setAttualProcess(viperui_AttualeSelezioneProcesso_e(m),viperdef_Processo_e(p));
}

/*********************************************
 * FUNCTION SET SOMETHING TO VIEW
 **********************************************/

void SWeldingProcessPresenter::setPressionEncoder(Model::Enc_Type e, Model::Enc_Pression p)
{
   switch (e)
   {
      case Model::ENC_LEFT:
         if ( p == Model::ENC_PRESSION_RELEASED )
         {
            view.GoBack();
         }
      break;
      case Model::ENC_CENTER:

      break;
      case Model::ENC_RIGHT:
         if ( p == Model::ENC_PRESSION_RELEASED )
         {
            view.encRXconfirm();
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
      case Model::ENC_LEFT:
         if ( dir > 0 )
            view.encSX_incDec(1);
         else
            view.encSX_incDec(-1);
      break;
      case Model::ENC_CENTER:

      break;

      case Model::ENC_RIGHT:
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

viperui_AttualeSelezioneProcesso_e SWeldingProcessPresenter::getActualSelezioneProcesso(void)
{
   return ActualSelezioneProcesso;
}
void SWeldingProcessPresenter::setMigWeldingProcessMenu(viperui_AttualeSelezioneProcesso_e selection)
{
   ActualSelezioneProcesso = selection;
   view.setMenuContainer(selection);
}

void SWeldingProcessPresenter::setProcessWelding(viperdef_Processo_e p)
{
   view.createSelectedProcess(p);
}
void SWeldingProcessPresenter::setWireType(viperdef_TipoFilo_e wiretype)
{
   view.updateWireType(wiretype);
}
void SWeldingProcessPresenter::setWireDiameter(viperdef_DiametroFilo_e wireDiameter)
{
   view.updateWireDiameter(wireDiameter);
}
void SWeldingProcessPresenter::setGasType(viperdef_TipoGas_e gastype)
{
   view.updateGasType(gastype);
}
void SWeldingProcessPresenter::setMigType(viperdef_TipoMig_e migtype)
{
   view.updateMigType(migtype);
}


viperdef_TipoFilo_e SWeldingProcessPresenter::getWireType(void)
{
   view.updateWireType(model->getWireTypeFromController());
}
viperdef_DiametroFilo_e SWeldingProcessPresenter::getWireDiameter(void)
{
   view.updateWireDiameter(model->getWireDiameterFromController());
}
viperdef_TipoGas_e SWeldingProcessPresenter::getGasType(void)
{
   view.updateGasType(model->getGasTypeFromController());
}
viperdef_TipoMig_e SWeldingProcessPresenter::getMigType(void)
{
   view.updateMigType(model->getMigTypeFromController());
}



/*********************************************
 * FUNCTION GET SOMETHING FROM MODEL
 **********************************************/

viperdef_Processo_e SWeldingProcessPresenter::getProcess(void)
{
   return (model->viper_Info.getProcess());
}










