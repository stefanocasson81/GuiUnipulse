#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <ARMLib/TGFX/cpp_define.hpp>

extern "C"
{
   #include "CORELib/UserInterface/ViperUI.h"
   #include "ARMLib/TGFX/TGFXCustom.h"
   #include "CORELib/Common/ViperDef.h"
};


Model::Model() : modelListener(0)
{

}

void Model::tick()
{
   U8 i = TGFXCUSTOM_ENCODER_LEFT;
   S16 tmp;
   Enc_Pression prsRead = TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO;


   while(i != TGFXCUSTOM_ENCODER_MAX)
   {
      switch(encGetPression((Model::Enc_Type)i))
      {
         default:
         break;
         case TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO:
//            modelListener->setPressionEncoder(i,TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO);

         break;
         case TGFXCUSTOM_PRESSIONEENCODER_PREMUTO:
            modelListener->setPressionEncoder((Model::Enc_Type)i,TGFXCUSTOM_PRESSIONEENCODER_PREMUTO);
         break;
         case TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO:
            modelListener->setPressionEncoder((Model::Enc_Type)i,TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO);
         break;
         case TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO:
            modelListener->setPressionEncoder((Model::Enc_Type)i,TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO);
         break;
         case TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO:
            modelListener->setPressionEncoder((Model::Enc_Type)i,TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO);
         break;
      }
      Enc.setPressione(i, prsRead);
      tmp = encGetOffset((Model::Enc_Type)i);
      if(tmp)
      {
         modelListener->setOffsetEncoder((Model::Enc_Type)i,tmp,tmp);
      }
//      if(tmp != encArray[i])
//      {
//         S16 direction = 0;
//         if(encArray[i] < tmp)
//            direction = 1;
//         else
//            direction = -1;
//         encArray[i] = tmp;
//         modelListener->setOffsetEncoder((Model::Enc_Type)i,tmp,direction);
//      }
      i++;
   }

//   switch(viper_Info.))

//   TGFXMenu.ManagerTickEvent()

}

/**@GET Functions ***/

//Model::Process_Type Model::getProcess()
//{
//
//}


/**@SET Functions ***/


/**@Generic Functions  **/

tgfxcustom_PressioneEncoder_e Model::encGetPression(Model::Enc_Type encoder)
{
   return (tgfxcustom_GetPressioneEncoder((tgfxcustom_Encoder_e)encoder));
}


S16 Model::encGetOffset(Model::Enc_Type encoder)
{
   return (tgfxcustom_GetOffsetEncoder((tgfxcustom_Encoder_e)encoder)) ;
}


