/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	06/04/23
 Descrizione:
	Modulo definizioni comuni core Viper
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ViperDef.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES

//------------------------------------------------------------------------------
viperdef_Pack8Gen_InfoStatus_t  			viperdef_Pack8GenRx_InfoStatus;

viperdef_Pack8Gen_InfoStatus_t  			viperdef_Pack8GenTx_InfoStatus;

//------------------------------------------------------------------------------
viperdef_Pack8Gen_MigMan_t                  viperdef_Pack8GenRx_MigMan;
viperdef_Pack8Gen_MigManHotStart_t          viperdef_Pack8GenRx_MigManHotStart;
viperdef_Pack8Gen_MigManCraterFiller_t      viperdef_Pack8GenRx_MigManCraterFiller;
viperdef_Pack8Gen_MigManLimiti2_t           viperdef_Pack8GenRx_MigManLimiti2;
viperdef_Pack8Gen_MigManLimiti1_t           viperdef_Pack8GenRx_MigManLimiti1;

viperdef_Pack8Gen_MigMan_t                  viperdef_Pack8GenTx_MigMan;
viperdef_Pack8Gen_MigManHotStart_t          viperdef_Pack8GenTx_MigManHotStart;
viperdef_Pack8Gen_MigManCraterFiller_t      viperdef_Pack8GenTx_MigManCraterFiller;
viperdef_Pack8Gen_MigManLimiti2_t           viperdef_Pack8GenTx_MigManLimiti2;
viperdef_Pack8Gen_MigManLimiti1_t           viperdef_Pack8GenTx_MigManLimiti1;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_MigSyn_t                  viperdef_Pack8GenRx_MigSyn;
viperdef_Pack8Gen_MigSynHotStart_t          viperdef_Pack8GenRx_MigSynHotStart;
viperdef_Pack8Gen_MigSynCraterFiller_t      viperdef_Pack8GenRx_MigSynCraterFiller;
viperdef_Pack8Gen_MigSynActualSynCalc_t     viperdef_Pack8GenRx_MigSynActualSynCalc;
viperdef_Pack8Gen_MigSynLimiti1_t           viperdef_Pack8GenRx_MigSynLimiti1;

viperdef_Pack8Gen_MigSyn_t                  viperdef_Pack8GenTx_MigSyn;
viperdef_Pack8Gen_MigSynHotStart_t          viperdef_Pack8GenTx_MigSynHotStart;
viperdef_Pack8Gen_MigSynCraterFiller_t      viperdef_Pack8GenTx_MigSynCraterFiller;
viperdef_Pack8Gen_MigSynActualSynCalc_t     viperdef_Pack8GenTx_MigSynActualSynCalc;
viperdef_CurvaSyn_t							viperdef_GenTx_CurvaSyn;
viperdef_Pack8Gen_MigSynLimiti1_t           viperdef_Pack8GenTx_MigSynLimiti1;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_Mig_t                     viperdef_Pack8GenRx_Mig;
viperdef_Pack8Gen_MigLimitiCraterFiller_t   viperdef_Pack8GenRx_MigLimitiCraterFiller;
viperdef_Pack8Gen_MigLimitiHotStart_t       viperdef_Pack8GenRx_MigLimitiHotStart;
viperdef_Pack8Gen_MigLimiti1_t              viperdef_Pack8GenRx_MigLimiti1;

viperdef_Pack8Gen_Mig_t                     viperdef_Pack8GenTx_Mig;
viperdef_Pack8Gen_MigLimitiCraterFiller_t   viperdef_Pack8GenTx_MigLimitiCraterFiller;
viperdef_Pack8Gen_MigLimitiHotStart_t       viperdef_Pack8GenTx_MigLimitiHotStart;
viperdef_Pack8Gen_MigLimiti1_t              viperdef_Pack8GenTx_MigLimiti1;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_Mma_t                     viperdef_Pack8GenRx_Mma;
viperdef_Pack8Gen_MmaLimiti1_t              viperdef_Pack8GenRx_MmaLimiti1;

viperdef_Pack8Gen_Mma_t                     viperdef_Pack8GenTx_Mma;
viperdef_Pack8Gen_MmaLimiti1_t              viperdef_Pack8GenTx_MmaLimiti1;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_Tig_t                     viperdef_Pack8GenRx_Tig;
viperdef_Pack8Gen_TigLimiti1_t              viperdef_Pack8GenRx_TigLimiti1;

viperdef_Pack8Gen_Tig_t                     viperdef_Pack8GenTx_Tig;
viperdef_Pack8Gen_TigLimiti1_t              viperdef_Pack8GenTx_TigLimiti1;


//------------------------------------------------------------------------------
//viperdef_Pack8Gen_Base_t                    viperdef_Pack8GenRx_Base;

viperdef_Pack8Gen_Base_t                    viperdef_Pack8GenTx_Base;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_BasicSetup_t              viperdef_Pack8GenRx_BasicSetup;
viperdef_Pack8Gen_BasicSetup2_t             viperdef_Pack8GenRx_BasicSetup2;

viperdef_Pack8Gen_BasicSetup_t              viperdef_Pack8GenTx_BasicSetup;
viperdef_Pack8Gen_BasicSetup2_t             viperdef_Pack8GenTx_BasicSetup2;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_TaratGenToSlave_t         viperdef_Pack8GenTx_TaratGenToSlave;

//------------------------------------------------------------------------------
viperdef_Pack8Gen_Strumento_t               viperdef_Pack8GenTx_Strumento;

//------------------------------------------------------------------------------
viperdef_Pack8Gen_FunExecute_t				viperdef_Pack8GenRx_FunExecute;

//------------------------------------------------------------------------------
viperdef_Pack8Gen_InfoFirmware_t			viperdef_Pack8GenTx_InfoFirmware;


//------------------------------------------------------------------------------
viperdef_Pack8Gen_TaraturaEssen_t			viperdef_Pack8GenRx_TaraturaEssen;
viperdef_Pack8Gen_TaraturaEssen1_t			viperdef_Pack8GenRx_TaraturaEssen1;
viperdef_Pack8Gen_TaraturaEssen2_t			viperdef_Pack8GenRx_TaraturaEssen2;
viperdef_Pack8Gen_TaraturaEssen3_t			viperdef_Pack8GenRx_TaraturaEssen3;

viperdef_Pack8Gen_TaraturaEssen_t			viperdef_Pack8GenTx_TaraturaEssen;
viperdef_Pack8Gen_TaraturaEssen1_t			viperdef_Pack8GenTx_TaraturaEssen1;
viperdef_Pack8Gen_TaraturaEssen2_t			viperdef_Pack8GenTx_TaraturaEssen2;
viperdef_Pack8Gen_TaraturaEssen3_t			viperdef_Pack8GenTx_TaraturaEssen3;

//------------------------------------------------------------------------------
viperdef_Pack8TrainoTX_InfoStatus_t			viperdef_Pack8TrainoTX_InfoStatus;
viperdef_Pack8TrainoTX_HWStatus_t			viperdef_Pack8TrainoTX_HWStatus;
viperdef_Pack8TrainoTX_HWStatus2_t			viperdef_Pack8TrainoTX_HWStatus2;
viperdef_Pack8TrainoRX_CmdSet_t				viperdef_Pack8TrainoRX_CmdSet;

//------------------------------------------------------------------------------
int32_t viperdef_GetRx_VarTestSInt32[2];
int16_t viperdef_GetRx_VarTestSInt16[4];
int8_t viperdef_GetRx_VarTestSInt8[8];
uint32_t viperdef_GetRx_VarTestUInt32[2];
uint16_t viperdef_GetRx_VarTestUInt16[4];
uint8_t viperdef_GetRx_VarTestUInt8[8];

int32_t viperdef_GetTx_VarTestSInt32[2];
int16_t viperdef_GetTx_VarTestSInt16[4];
int8_t viperdef_GetTx_VarTestSInt8[8];
uint32_t viperdef_GetTx_VarTestUInt32[2];
uint16_t viperdef_GetTx_VarTestUInt16[4];
uint8_t viperdef_GetTx_VarTestUInt8[8];
uint8_t viperdef_GetTx_HwCheckDac[8];
uint8_t viperdef_GetTx_HwCheckAdc1[8];
uint8_t viperdef_GetTx_HwCheckAdc0[8];
uint8_t viperdef_GetTx_HwCheckPortIn3[8];
uint8_t viperdef_GetTx_HwCheckPortIn2[8];
uint8_t viperdef_GetTx_HwCheckPortIn1[8];
uint8_t viperdef_GetTx_HwCheckPortIn0[8];
uint8_t viperdef_GetTx_HwCheckPortOut3[8];
uint8_t viperdef_GetTx_HwCheckPortOut2[8];
uint8_t viperdef_GetTx_HwCheckPortOut1[8];
uint8_t viperdef_GetTx_HwCheckPortOut0[8];


//============================================================= GLOBAL FUNCTIONS


//============================================================ PRIVATE FUNCTIONS

