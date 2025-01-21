#ifndef MODEL_HPP
#define MODEL_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui/custom/WeldingSupport.hpp>
#include <array>

extern "C"
{
   #include "ARMLib/Common/ComDef.h"
   #include "ARMLib/Common/Data.h"
//   #include "CORELib/UserInterface/ViperUI.h"
   #include "ARMLib/TGFX/TGFXCustom.h"
   #include "CORELib/Common/ViperDef.h"
   #include "gui/custom/CommonDefine.h"
};


class ModelListener;

class Model
{
public:
   Model();

   void bind(ModelListener *listener)
   {
      modelListener = listener;
      viper_Info.viperui_setModelListnerPoint(listener);
   }

   /** Enum **/
   enum Views
   {
      View_Basic,
      View_Debug,
      View_Check,
      View_CheckTraino,
      View_Info,
      View_Main,
      View_AdvanceMenu,
      View_DebugMenu,
      View_MainMenu,
      View_Watch,
      View_Test,
      View_Setup,
      View_Splash,
      View_UpgradeFirmware,
      View_Mig,
      View_WeldingProcess,
   } currentView;

   enum Enc_Pression
   {
      ENC_PRESSION_NOT_PRESSED,
      ENC_PRESSION_PRESSED,
      ENC_PRESSION_RELEASED,            // <800ms
      ENC_PRESSION_LONG_PRESSED,        // 2000ms
      ENC_PRESSION_VERY_LONG_PRESSED,   // 4000ms

      ENCODER_PRESSION_MAX
   };

   enum Enc_Type
   {
      ENC_LEFT,
      ENC_CENTER,
      ENC_RIGHT,

      ENC_MAX
   };

   cViper_Info viper_Info;

   /** Functions**/
   void tick();

   /********** get Data from Controller **********************/
//    U8 getProcess();
   Model::Enc_Pression encGetPression(Model::Enc_Type encoder);
   S16 encGetOffset(Model::Enc_Type encoder);
   Views getCurrentScreen();
   void setCurrentScreen(Views v);

   viperdef_TipoFilo_e getWireTypeFromController(void);
   viperdef_DiametroFilo_e getWireDiameterFromController(void);
   viperdef_TipoGas_e getGasTypeFromController(void);
   viperdef_TipoMig_e getMigTypeFromController(void);
   S32 getDataFromCanController(viperui_Data_e iD);

   /************* set functions ******************/
   void setEncoderPreccesed(U8 encoder);

protected:
   ModelListener *modelListener;
private:

};

#endif // MODEL_HPP
