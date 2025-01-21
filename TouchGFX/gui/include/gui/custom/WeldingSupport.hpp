#ifndef WELDINGSUPPORT_H_INCLUDED
#define WELDINGSUPPORT_H_INCLUDED

//#include <gui/model/Model.hpp>
//===================================================================== INCLUDES
//#include <gui/model/ModelListener.hpp>
extern "C"
{
//   #include "ARMLib/Common/ComDef.h"
//   #include "ARMLib/Common/Data8Pack.h"
//   #include "ARMLib/Common/Data.h"
   #include "CORELib/UserInterface/ViperUI.h"
   #include "gui/custom/CommonDefine.h"
}
//====================================================================== DEFINES


/*****************************************************************************
 * Defines
 ****************************************************************************/
#define VIPERUI_NMAXCURVEINFODISPONIBILI 300

//#define FEATURES_RGB_COLOREAZIENDALE   0x66b32e
//#define FEATURES_RGB_NERO              0x000000
//#define FEATURES_RGB_GRIGIOSCURO    0x313131
//#define FEATURES_RGB_GRIGIOMEDIO    0x878787
//#define FEATURES_RGB_GRIGIOCHIARO      0x575757
//#define FEATURES_RGB_BIANCO            0xffffff


/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
//extern uint32_t viperui_ListStdColor[];

/*****************************************************************************
 * Enums
 *****************************************************************************/

typedef struct
{
   U8 Type;
   S16 Offset;
   U16 Pression;
}Encoder_t;


typedef struct
 {
    viperui_AttualeSelezioneProcesso_e AttualeSelezione;
    viperui_AttualeSelezioneProcesso_e oldAttualeSelezione;
    viperdef_Processo_e Processo;
    viperdef_Processo_e oldProcesso;

    viperdef_MigCurvaInfo_t CurvaInfo;     // Curva attualmente in selezione
    viperdef_MigCurvaInfo_t CurvaInfoDiPartenza; // Curva di partenza all'entrata del quadro

    U32 BitMappedDiametroFilo;  // Diametri filo disponibili gestito a bit enum del tipo viperdef_DiametroFilo_e
    U32 BitMappedTipoGas;    // Tipi gas disponibili gestito a bit enum del tipo viperdef_TipoGas_e
    U32 BitMappedTipoMig;    // Tipi Mig disponibili gestito a bit enum del tipo viperdef_TipoMig_e
    U32 BitMappedTipoFilo;    // Tipi Mig disponibili gestito a bit enum del tipo viperdef_TipoMig_e

    U8 IndiceDiametroFilo;
    U8 IndiceTipoGas;
    U8 IndiceTipoMig;
    U8 IndiceTipoFilo;

    U8 TabEnumDiametro[32];
    U8 TabEnumTipoGas[32];
    U8 TabEnumTipoMig[32];
    U8 TabEnumTipoFilo[32];
    U8 IndiceMaxEnumDiametro;
    U8 IndiceMaxEnumTipoGas;
    U8 IndiceMaxEnumTipoMig;
    U8 IndiceMaxEnumTipoFilo;
 }SelezioneProcesso_t;   // Usato per selezionare processo


/**************************************************************************************************
 * class
 *************************************************************************************************/
 class ModelListener;

class cViper_Info
{
public :
   cViper_Info();
   virtual ~cViper_Info(){}
   /*functions*/
   void viperui_setAttualProcess(viperui_AttualeSelezioneProcesso_e m,viperdef_Processo_e p){SelezioneProcesso.AttualeSelezione = (viperui_AttualeSelezioneProcesso_e)m;SelezioneProcesso.Processo = (viperdef_Processo_e)p;}
   void viperui_setProcess(viperdef_Processo_e p){SelezioneProcesso.Processo = p;}
   void viperui_Generatore_CambiaProcesso(viperdef_Processo_e processo,viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_TipoGas_e tipoGas,viperdef_DiametroFilo_e diametroFilo);
   void viperui_Generatore_SalvaTaratura(void);
   void viperui_Generatore_SalvaParametriInEEProm(void);
   void viperui_RecalcBitMappedFiloDiametroGasMig(viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_DiametroFilo_e diametroFilo,viperdef_TipoGas_e tipoGas);
   void viperui_InitData(void);
   void viperui_LoadData(void);
   void viperui_DestroyData(void);
   void viperui_LoadManView(void);
   void viperui_setModelListnerPoint(ModelListener* p){pModelListner = p;}
   viperdef_TipoFilo_e viperui_getWiteType(void){return SelezioneProcesso.CurvaInfo.TipoFilo;}
   viperdef_DiametroFilo_e viperui_getWireDiameter(void){return SelezioneProcesso.CurvaInfo.DiametroFilo;}
   viperdef_TipoGas_e viperui_getGasType(void){return SelezioneProcesso.CurvaInfo.TipoGas;}
   viperdef_TipoMig_e viperui_getMigType(void){return SelezioneProcesso.CurvaInfo.TipoMig;}
//   void viperui_ManagerTickEvent(U8 t, U8 p);
   viperdef_Processo_e getProcess(){return SelezioneProcesso.Processo;}
   U8 viperui_ManagerTickEvents(const Encoder_t &encS, const Encoder_t &encR);


private:
//   typeProcess prc;
   SelezioneProcesso_t SelezioneProcesso;
   viperdef_MigCurvaInfo_t ListaCurveInfoDisponibili[VIPERUI_NMAXCURVEINFODISPONIBILI];
   viperdef_MigCurvaInfo_t oldCurva;
   enum sm_States_e
   {
      SM_Init,
      SM_Run,
      SM_Stop
   }sm_State;
protected:
   ModelListener* pModelListner;

};


#endif /* WELDINGSUPPORT_H_INCLUDED */

