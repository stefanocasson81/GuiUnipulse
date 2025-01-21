#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <ARMLib/TGFX/cpp_define.hpp>
#include <ARMLib/TGFX/color_define.hpp>

//extern "C"
//{
//   #include "CORELib/UserInterface/ViperUI.h"
//   #include "ARMLib/TGFX/TGFXCustom.h"
//   #include "CORELib/Common/ViperDef.h"
//   #include "gui/custom/CommonDefine.h"
//};


Model::Model() : modelListener(0)
{

}

void Model::tick()
{
   U8 i = ENCODER_LEFT;
   Encoder_t Enc[3] = {0};
   U8 encPression = ENC_PRESSION_NOT_PRESSED;
   S16 encOffset = 0;

   while(i != TGFXCUSTOM_ENCODER_MAX)
   {
      switch(encGetPression((Model::Enc_Type)i))
      {
         case Model::ENC_PRESSION_NOT_PRESSED:
         break;
         case Model::ENC_PRESSION_PRESSED:
            encPression = ENC_PRESSION_PRESSED;
         break;
         case Model::ENC_PRESSION_RELEASED:
            encPression = ENC_PRESSION_RELEASED;
         break;
         case Model::ENC_PRESSION_LONG_PRESSED:
            encPression = ENC_PRESSION_LONG_PRESSED;
         break;
         case Model::ENC_PRESSION_VERY_LONG_PRESSED:
            encPression = ENC_PRESSION_VERY_LONG_PRESSED;
         break;
         default:
         break;
      }

      encOffset = encGetOffset((Model::Enc_Type)i);
      modelListener->setPressionEncoder(Model::Enc_Type(i),Model::Enc_Pression(encPression));

      if(encOffset)
      {
         modelListener->setOffsetEncoder((Model::Enc_Type)i,encOffset,encOffset);
      }
      Enc[i].Type = i;
      Enc[i].Pression = encPression;
      Enc[i].Offset = encOffset;

      i++;
   }

   if ( getCurrentScreen() == View_WeldingProcess )
         viper_Info.viperui_ManagerTickEvents(Enc[ENC_LEFT], Enc[ENC_RIGHT]);

}



/********************************************
 * FUNCTION SET DATA FROM UI
 *******************************************/
void Model::setCurrentScreen(Views v)
{
   currentView = v;
}



/*********************************************
 * FUNCTION GET SOMETHING TO Model
 **********************************************/
Model::Views Model::getCurrentScreen(void)
{
   return currentView;
}


/*********************************************
 * FUNCTION GET SOMETHING TO CONTROLLER
 **********************************************/
S32 Model::getDataFromCanController(viperui_Data_e iD)
{
   return data_GetValData(&viperui_StrutturaDati[iD]);
}

/**@SET Functions ***/


/**@Generic Functions  **/

Model::Enc_Pression Model::encGetPression(Model::Enc_Type encoder)
{
   return Model::Enc_Pression(tgfxcustom_GetPressioneEncoder((tgfxcustom_Encoder_e)encoder));
}


S16 Model::encGetOffset(Model::Enc_Type encoder)
{
   return (tgfxcustom_GetOffsetEncoder((tgfxcustom_Encoder_e)encoder));
}


viperdef_TipoFilo_e Model::getWireTypeFromController(void)
{
   return viper_Info.viperui_getWiteType();
}
viperdef_DiametroFilo_e Model::getWireDiameterFromController(void)
{
   return viper_Info.viperui_getWireDiameter();
}
viperdef_TipoGas_e Model::getGasTypeFromController(void)
{
   return viper_Info.viperui_getGasType();
}
viperdef_TipoMig_e Model::getMigTypeFromController(void)
{
   return viper_Info.viperui_getMigType();
}
