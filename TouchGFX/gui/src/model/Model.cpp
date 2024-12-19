#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <ARMLib/TGFX/cpp_define.hpp>
extern "C"
{
   #include "CORELib/UserInterface/ViperUI.h"
   #include "ARMLib/TGFX/TGFXCustom.h"
   #include "CORELib/Common/ViperDef.h"
};


Model::Model() : modelListener(0),OldAttualeSelezione(VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO),OldTipoFilo(VIPERDEF_TIPOFILO_MANUAL)\
               ,OldDiametroFilo(VIPERDEF_DIAMETROFILO_06),ProcessoAllocato(VIPERDEF_PROCESSO_MMA)
{
   viperui_Info.SelezioneProcesso.Processo = viperdef_Pack8GenTx_Base.Processo;
   viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas = viperdef_Pack8GenTx_Base.TipoGas;
   viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo = viperdef_Pack8GenTx_Base.DiametroFilo;

   viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoMig     = viperui_Info.SelezioneProcesso.CurvaInfo.TipoMig;
   viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoFilo    = viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;
   viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.TipoGas     = viperui_Info.SelezioneProcesso.CurvaInfo.TipoGas;
   viperui_Info.SelezioneProcesso.CurvaInfoDiPartenza.DiametroFilo = viperui_Info.SelezioneProcesso.CurvaInfo.DiametroFilo;
}

void Model::tick()
{
   U8 i = TGFXCUSTOM_ENCODER_LEFT;
   S16 tmp;

   if(viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo != OldTipoFilo)
   {
      OldTipoFilo = viperui_Info.SelezioneProcesso.CurvaInfo.TipoFilo;
      modelListener->wireChanged(OldTipoFilo);
   }

   if(viperui_Info.SelezioneProcesso.Processo != ProcessoAllocato)
   {
      ProcessoAllocato = viperui_Info.SelezioneProcesso.Processo;
      modelListener->processChanged(ProcessoAllocato);
   }
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

//   TGFXMenu.ManagerTickEvent()

}

U8 Model::getProcess()
{
   return (U8)ProcessoAllocato;
}



tgfxcustom_PressioneEncoder_e Model::encGetPression(Model::Enc_Type encoder)
{
   return (tgfxcustom_GetPressioneEncoder((tgfxcustom_Encoder_e)encoder));
}


S16 Model::encGetOffset(Model::Enc_Type encoder)
{
   return (tgfxcustom_GetOffsetEncoder((tgfxcustom_Encoder_e)encoder)) ;
}


