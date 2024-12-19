#ifndef MODEL_HPP
#define MODEL_HPP

#include <ARMLib/TGFX/cpp_define.hpp>
#include <array>

extern "C"
{
   #include "ARMLib/Common/ComDef.h"
   #include "CORELib/UserInterface/ViperUI.h"
   #include "ARMLib/TGFX/TGFXCustom.h"
   #include "CORELib/Common/ViperDef.h"
};


/**
 * CLASS DEFINE
 */
//class Viper
//{
//    public:
//
//    protected:
//
//    private:
//};


//class clsEcoder
//{
//public:
//   clsEcoder();
//   ~clsEcoder();
//   tgfxcustom_PressioneEncoder_e  encGetPression(U8 encoder);
//   int16_t encGetOffset(tgfxcustom_Encoder_e encoder):
//};
//
//tgfxcustom_PressioneEncoder_e clsEcoder::encGetPression(U8 e)
//{
//   return tgfxcustom_GetPressioneEncoder((tgfxcustom_Encoder_e)e);
//}


class ModelListener;




class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }
    /** Enum **/
    enum Enc_Pression
    {
       TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO       ,
       TGFXCUSTOM_PRESSIONEENCODER_PREMUTO          ,
       TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO     ,  // <800ms
       TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO     ,   // 2000ms
       TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO  ,   // 4000ms

       TGFXCUSTOM_PRESSIONEENCODER_MAX
    };

    enum Enc_Type
    {
       TGFXCUSTOM_ENCODER_LEFT    ,
       TGFXCUSTOM_ENCODER_CENTER  ,
       TGFXCUSTOM_ENCODER_RIGHT   ,

       TGFXCUSTOM_ENCODER_MAX
    };


    /** Functions**/
    void tick();
/** get functions */
    U8 getProcess();
    tgfxcustom_PressioneEncoder_e  encGetPression(Model::Enc_Type encoder);
    S16 encGetOffset(Model::Enc_Type encoder);

/** set functions */
    void setEncoderPreccesed(U8 encoder);
//    gfxcustom_PressioneEncoder_e tgfxcustom_GetPressioneEncoder(tgfxcustom_Encoder_e encoder);

protected:

    ModelListener* modelListener;
private:

   static const U8 nSizeEncArray = 3;
   viperui_AttualeSelezioneProcesso_e OldAttualeSelezione;
   viperdef_TipoFilo_e OldTipoFilo;
   viperdef_DiametroFilo_e OldDiametroFilo;
   viperdef_Processo_e ProcessoAllocato;
   std::array<S16,nSizeEncArray>encArray;

//   clsEcoder Enc;
};

#endif // MODEL_HPP
