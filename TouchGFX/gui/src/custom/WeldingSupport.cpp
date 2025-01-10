/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	06/04/23
 Descrizione:
	Manager core Viper (nuova generazione saldatrici) lato user interface.
	Questo modulo è la madre di tutto il core gestione user interface su base
	TouchGFX.
	All'InitModule è necessario avere scheda e swtimer inizilizzato.
------------------------------------------------------------------------------*/

#include <ARMLib/TGFX/cpp_define.hpp>
#include <gui/custom/WeldingSupport.hpp>

#if 0
extern "C"
{
   //===================================================================== INCLUDES
   //#include "CORELib/UserInterface/ViperUI.h"
   #include "CORELib/Common/ViperDef.h"
   #include "ARMLib/TGFX/TGFXCustom.h"
   #include "ARMLib/Common/SwTimer.h"
   #include "ARMLib/Common/Data8Pack.h"
   #include "ARMLib/Common/Data.h"
   #include "ARMLib/Protocolli/HProt/HProt.h"
   #include "ARMLib/Protocolli/HProt/HProtFU.h"
   #include "ARMLib/Protocolli/CanBusStack/CanBusStack.h"
   #include "Features.h"
   #include "_Board.h"
   #include "cmsis_os.h"
   #include "stdio.h"	//niknik

   //====================================================================== DEFINES

   //=========================================================== PRIVATE PROTOTYPES
   static void AdapterEncoderGetStatus(tgfxcustom_Encoder_e numEncoder,bool *ptrPremuto,int16_t *ptrCounter);
   static void PostSetData(const data_Data_t *ptrStructData);
   static void TaskCANBUS(void *argument);

   //============================================================= STATIC VARIABLES
   static const osThreadAttr_t TaskCANBUSThreadAttr =
   {
      .name = "CANBUS",
      .stack_size = 2048 * 4,	//niknik era 512
      .priority = (osPriority_t) osPriorityNormal,
   };


   //____________________________________________________________________HProt zone
   //____________________________________________________________________HProt zone
   //____________________________________________________________________HProt zone
   typedef struct
   {
      uint8_t StatusBoard;
      uint32_t TypeExecuteReboot;
   }application_HProtData_t;

   static application_HProtData_t HProtData=
   {
      .StatusBoard = HPROT_STATUSBOARD_POWEREDON,
   };

   static int32_t FunHProt_StatusBoard(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
   {
      if(typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTGET)	// Una volta letto lo resetto
         HProtData.StatusBoard &= ~(HPROT_STATUSBOARD_POWEREDON|HPROT_STATUSBOARD_REBOOTED);

      if(_board_GetStatusForceBootLoader())
         HProtData.StatusBoard |= HPROT_STATUSBOARD_BOOTLOADERMODE;

      return _STATUS_OK_;
   }
   static int32_t FunHProt_ExecuteReboot(void* ptrStructData,hprot_TypeFunExecute_e typeFunExecute)
   {
      if(typeFunExecute==HPROT_TYPEFUNEXECUTE_POSTSET)
      {
         switch(HProtData.TypeExecuteReboot)
         {
            case 0xaa556666:	// Reboot
               _board_ExecuteReboot(false);
            break;
            case 0x55aa9999:	// Reboot in bootloader
               _board_ExecuteReboot(true);
            break;
         }
      }

      return _STATUS_OK_;
   }

   // Lista Id HProt specifico per il canale CAN
   static const hprot_StructData_t HProtStructDataCan[]=
   {
      {HPROT_IDSTD_R_STATUSBOARD,									&HProtData.StatusBoard,																_TYPEDATA_UINT8_,		sizeof(HProtData.StatusBoard),												FunHProt_StatusBoard,		HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_CODICESCHEDAESTESOBOOTLOADERCOMPATIBILITY,	&viperui_Init.InfoFirmware.CodiceEstesoBootLoaderCompatibility,						_TYPEDATA_UINT32_,		sizeof(viperui_Init.InfoFirmware.CodiceEstesoBootLoaderCompatibility),		_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_CODICESCHEDAESTESO,							&viperui_Init.InfoFirmware.CodiceEsteso,											_TYPEDATA_UINT32_,		sizeof(viperui_Init.InfoFirmware.CodiceEsteso),								_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_FIRMWAREVER,									&viperui_Init.InfoFirmware.Version,													_TYPEDATA_UINT16_,		sizeof(viperui_Init.InfoFirmware.Version),									_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_FIRMWARECRC32,								&viperui_Init.InfoFirmware.Crc32Firmware,											_TYPEDATA_UINT32_,		sizeof(viperui_Init.InfoFirmware.Crc32Firmware),							_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_CODICEUNIVOCOSCHEDA,							&viperui_Init.InfoFirmware.CodiceUnivocoScheda,										_TYPEDATA_UINT32_,		sizeof(viperui_Init.InfoFirmware.CodiceUnivocoScheda),						_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_R_FIRMWAREDATA,								&viperui_Init.InfoFirmware.FirmwareData,											_TYPEDATA_UINT32_,		sizeof(viperui_Init.InfoFirmware.FirmwareData),								_NULL_,						HPROT_PERMISSION_NOWR},
      {HPROT_IDSTD_RW_EXECUTEREBOOT,								&HProtData.TypeExecuteReboot,														_TYPEDATA_UINT32_,		sizeof(HProtData.TypeExecuteReboot),										FunHProt_ExecuteReboot,		0},

      {_NULL_},
   };


   //============================================================= GLOBAL VARIABLES
   viperui_Init_t viperui_Init;
   //viperui_Info_t viperui_Info;

   uint32_t viperui_ListStdColor[VIPERUI_STDCOLOR_MAX]=
   {
      FEATURES_RGB_COLOREAZIENDALE,
      FEATURES_RGB_NERO,
      FEATURES_RGB_GRIGIOSCURO,
      FEATURES_RGB_GRIGIOMEDIO,
      FEATURES_RGB_GRIGIOCHIARO,
      FEATURES_RGB_BIANCO,
   };

   #define __DATU__(a) {(&a) , (sizeof(a))}
   const data8pack_Data_t viperui_StructData8TX[]=	// Questa è la struttura di link al CAN tramesso in uscita
   {	// Terminare la tabella con 0 !!!
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMAN,					   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigMan),},  					},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANHOTSTART,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigManHotStart),},  			},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_MigManCraterFiller),},  		},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN,					   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSyn),},  					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART,			   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_MigSynHotStart),},  			},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER,		DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSynCraterFiller),},  		},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC,	   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSynActualSynCalc),}, 		},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG,					      DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Mig),}, 						},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGLIMITICRATERFILLER,	DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigLimitiCraterFiller),},   	},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGLIMITIHOTSTART,		DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigLimitiHotStart),},   		},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MMA,					      DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Mma),}, 						},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TIG,					      DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Tig),}, 						},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASICSETUP,				   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_BasicSetup),},				},
      {   VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE,					      DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_Base),},  					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE,				   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenRx_FunExecute),},				},	// NOTA: Deve essere RX
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN,			   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen),},  			},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN1,			   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen1),},  			},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN2,			   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen2),},  			},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN3,			   DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen3),},  			},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTSINT32,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetRx_VarTestSInt32),}, 					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTSINT16,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestSInt16),}, 					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTSINT8,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestSInt8),}, 					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTUINT32,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt32),}, 					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTUINT16,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt16),}, 					},
      {	VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_VARTESTUINT8,			   DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt8),}, 					},


      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+0,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[0]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+1,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[1]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+2,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[2]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+3,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[3]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+4,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[4]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+5,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[5]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+6,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[6]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+7,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[7]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+8,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[8]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+9,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[9]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+10,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[10]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+11,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[11]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+12,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[12]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+13,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[13]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+14,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[14]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+15,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.RawDataToSend[15]),},  			},
      {	CANBUSSTACK_IDCAN_BUSSTACKEVENT,									DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(canbusstack_Info.EventToSend),}  					},

      {	_NULL_,																_NULL_,					_NULL_, {{_NULL_,0},}   											},
   };

   static uint8_t TmpBuffer8PackRX[DATA8PACK_DIMDATA];	// Usato come buffer di passaggio in ricezione CAN
   const data8pack_Data_t viperui_StructData8RX[]=	// Questa è la struttura di link al CAN ricevuta
   {	// Terminare la tabella con 0 !!!
      { 	VIPERDEF_CANID_TRAINOTX_HP_INFOSTATUS,								DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8TrainoTX_InfoStatus),},  			},

      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_INFOSTATUS,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_InfoStatus),},  				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_STRUMENTO,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Strumento),},   				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMAN,					DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigMan),},  					},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANHOTSTART,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigManHotStart),},  			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_MigManCraterFiller),},  		},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANLIMITI2,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigManLimiti2),},   			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANLIMITI1,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_MigManLimiti1),},   			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYN,					DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSyn),},  					},
      {	 VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_MigSynHotStart),},  			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER,		DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSynCraterFiller),},  		},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC,	DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSynActualSynCalc),}, 		},
   /*
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_NOMECURVAMIGA1,			DATA8PACK_NOAUTOTX,		_NULL_, {{ &(viperdef_GenTx_CurvaSyn.NomeCurva[0]), 8 }}, 			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_NOMECURVAMIGA2,			DATA8PACK_NOAUTOTX,		_NULL_, {{ &(viperdef_GenTx_CurvaSyn.NomeCurva[8]), 8 }}, 			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_NOMECURVAMIGA3,			DATA8PACK_NOAUTOTX,		_NULL_, {{ &(viperdef_GenTx_CurvaSyn.NomeCurva[16]), 8 }},			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_NOMECURVAMIGA4,			DATA8PACK_NOAUTOTX,		_NULL_, {{ &(viperdef_GenTx_CurvaSyn.NomeCurva[24]), 7 },__DATU__(viperdef_GenTx_CurvaSyn.IndiceCurva)},	},
   */
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNLIMITI1,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigSynLimiti1),},   			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIG,					DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Mig),}, 						},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITICRATERFILLER,	DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigLimitiCraterFiller),},   	},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITIHOTSTART,		DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigLimitiHotStart),},   		},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITI1,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MigLimiti1),},  				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MMA,					DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Mma),}, 						},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MMALIMITI1,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_MmaLimiti1),},  				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TIG,					DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_Tig),}, 						},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TIGLIMITI1,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_TigLimiti1),},  				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_BASICSETUP,				DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_BasicSetup),},				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_BASE,					DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_Base),},  					},
   //	{   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TARATGENTOSLAVE,		DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_Pack8GenTx_TaratGenToSlave),}, 			},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_INFOFIRMWARE,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_InfoFirmware),},  			},

      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TARATURAESSEN,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen),},  			},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TARATURAESSEN1,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen1),},  			},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TARATURAESSEN2,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen2),},  			},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TARATURAESSEN3,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_Pack8GenTx_TaraturaEssen3),},  			},

      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTSINT32,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestSInt32),}, 					},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTSINT16,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestSInt16),}, 					},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTSINT8,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestSInt8),}, 					},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTUINT32,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt32),}, 					},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTUINT16,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt16),}, 					},
      {	VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_VARTESTUINT8,			DATA8PACK_NOAUTOTX,		_NULL_, {__DATU__(viperdef_GetTx_VarTestUInt8),}, 					},

      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKDAC,				DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckDac),},						},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKADC1,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckAdc1),},					},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKADC0,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckAdc0),},					},
      //	{   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN3,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortIn3),},					},
      //	{   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN2,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortIn2),},					},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN1,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortIn1),},					},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN0,			DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortIn0),},					},
      //	{   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT3,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortOut3),},				},
      //	{   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT2,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortOut2),},				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT1,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortOut1),},				},
      {   VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT0,		DATA8PACK_NOAUTOTX,		_NULL_,	{__DATU__(viperdef_GetTx_HwCheckPortOut0),},				},

      //_______________________________________________________________Zona traino
      {	VIPERDEF_CANID_TRAINOTX_HWSTATUS,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(viperdef_Pack8TrainoTX_HWStatus), },	},
      {	VIPERDEF_CANID_TRAINOTX_HWSTATUS2,									DATA8PACK_NOAUTOTX,		_NULL_,								{__DATU__(viperdef_Pack8TrainoTX_HWStatus2), },	},

      //_____________________________________________________________Zona BusStack
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+0,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+1,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+2,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+3,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+4,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+5,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+6,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+7,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+8,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+9,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+10,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+11,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+12,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+13,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+14,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKBASE+15,									DATA8PACK_NOAUTOTX,		canbusstack_PushReceivedRawDataFromCAN,	{__DATU__(TmpBuffer8PackRX),},  				},
      {	CANBUSSTACK_IDCAN_BUSSTACKEVENT,									DATA8PACK_NOAUTOTX,		canbusstack_PushEventFromCAN,			{__DATU__(TmpBuffer8PackRX),},  				},

      {	_NULL_,																_NULL_,					_NULL_, {{_NULL_,0},}   											},
   };



   const data_Data_t viperui_StrutturaDati[VIPERUI_DATA_MIGSYN_MAX] =
   {	// Tenere allineata la struttura con gli enum del tipo VIPERUI_DATA_xxxxx
      { &viperdef_Pack8GenTx_MigMan.DTensione,						2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMAN	},
      { &viperdef_Pack8GenTx_MigMan.DVelFilo,							2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMAN	},
      { &viperdef_Pack8GenTx_MigMan.Induttanza,						2,0,	_TYPEDATA_UINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti2.Induttanza_Min,					(uint32_t)&viperdef_Pack8GenTx_MigManLimiti2.Induttanza_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMAN	},

      { &viperdef_Pack8GenTx_MigManHotStart.DVelFilo,					2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANHOTSTART	},
      { &viperdef_Pack8GenTx_MigManHotStart.DTensione,				2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANHOTSTART	},
      { &viperdef_Pack8GenTx_MigManHotStart.DSecDurata,				2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),					   											(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecDurata_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANHOTSTART	},
      { &viperdef_Pack8GenTx_MigManHotStart.DSecSlope,				2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecSlope_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANHOTSTART	},
      { &viperdef_Pack8GenTx_MigManCraterFiller.DVelFilo,				2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DVelFilo_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigManCraterFiller.DTensione,			2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Min,						(uint32_t)&viperdef_Pack8GenTx_MigManLimiti1.DTensione_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigManCraterFiller.DSecSlope,			2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),					   											(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecSlope_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigManCraterFiller.DSecDurata,			2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecDurata_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER	},

      { &viperdef_Pack8GenTx_MigSyn.UISXVis,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_MIGSYNSXVIS_MAX-1),										VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN	},
      { &viperdef_Pack8GenTx_MigSyn.UIDXVis,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_MIGSYNDXVIS_MAX-1),										VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN	},
      { &viperdef_Pack8GenTx_MigSyn.DVelFilo,							2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.DVelFilo_Min,						(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.DVelFilo_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN	},
      { &viperdef_Pack8GenTx_MigSyn.DTensBilanciamento,				2,1,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.DTensBilanciamento_Min,			(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.DTensBilanciamento_Max,			VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN	},
      { &viperdef_Pack8GenTx_MigSyn.InduttanzaBilanciamento,			3,0,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.InduttanzaBilanciamento_Min,		(uint32_t)&viperdef_Pack8GenTx_MigSynLimiti1.InduttanzaBilanciamento_Max,		VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYN	},

      { &viperdef_Pack8GenTx_MigSynHotStart.PercSpeed,				2,1,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.PercSpeed_Min,					(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.PercSpeed_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART	},
      { &viperdef_Pack8GenTx_MigSynHotStart.DBalanceV,				2,1,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.BalanceV_Min,					(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.BalanceV_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART	},
      { &viperdef_Pack8GenTx_MigSynHotStart.DSecDurata,				2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),					   											(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecDurata_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART	},
      { &viperdef_Pack8GenTx_MigSynHotStart.DSecSlope,				2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimitiHotStart.DSecSlope_Max,					VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART	},
      { &viperdef_Pack8GenTx_MigSynCraterFiller.PercSpeed,			2,1,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.PercSpeed_Min,				(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.PercSpeed_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigSynCraterFiller.DBalanceV,			2,1,	_TYPEDATA_SINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.BalanceV_Min,				(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.BalanceV_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigSynCraterFiller.DSecSlope,			2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),					   											(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.DSecSlope_Max,				VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER	},
      { &viperdef_Pack8GenTx_MigSynCraterFiller.DSecDurata,			2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimitiCraterFiller.DSecDurata_Max,			VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER	},

      { &viperdef_Pack8GenTx_MigSynActualSynCalc.Corrente,			3,0,	_TYPEDATA_UINT16_,	DATA_SCALARE(0),																DATA_SCALARE(0xffff),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC },
      { &viperdef_Pack8GenTx_MigSynActualSynCalc.DSpessore,			2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(0xff),																VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC },
      { &viperdef_Pack8GenTx_MigSynActualSynCalc.DTensione,			2,1,	_TYPEDATA_UINT16_,	DATA_SCALARE(0),																DATA_SCALARE(0xffff),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC },
   /*	viperdef_CurvaSyn_t							viperdef_GenTx_CurvaSyn;
   */

      { &viperdef_Pack8GenTx_Mig.CurvaAttuale,						3,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.CurvaAttuale_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.Tempi,								1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_TEMPI_MAX-1),												VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.BBT,									3,0,	_TYPEDATA_UINT8_,	(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.BBT_Min,								(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.BBT_Max,								VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.CSWireSlope,							1,2,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.CSWireSlope_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.DSPostGas,							2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.DSPostGas_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.DSPreGas,							2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.DSPreGas_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },
      { &viperdef_Pack8GenTx_Mig.DSSpot,								2,1,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.DSSpot_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MIG },

      /*
      viperdef_Pack8Gen_MigLimitiCraterFiller_t   viperdef_Pack8GenRx_MigLimitiCraterFiller;
      viperdef_Pack8Gen_MigLimitiHotStart_t       viperdef_Pack8GenRx_MigLimitiHotStart;
      viperdef_Pack8Gen_MigLimiti1_t              viperdef_Pack8GenRx_MigLimiti1;
   */
      { &viperdef_Pack8GenTx_Mma.Corrente,							3,0,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_MmaLimiti1.Corrente_Min,							(uint32_t)&viperdef_Pack8GenTx_MmaLimiti1.Corrente_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MMA },
      { &viperdef_Pack8GenTx_Mma.HotStart,							3,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MmaLimiti1.HotStart_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MMA },
      { &viperdef_Pack8GenTx_Mma.ArcForce,							3,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																(uint32_t)&viperdef_Pack8GenTx_MmaLimiti1.ArcForce_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_MMA },

      { &viperdef_Pack8GenTx_Tig.Corrente,                     		3,0,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_TigLimiti1.Corrente_Min,							(uint32_t)&viperdef_Pack8GenTx_TigLimiti1.Corrente_Max,							VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TIG },
      { &viperdef_Pack8GenTx_Tig.DTensCutOff,                     	2,1,	_TYPEDATA_UINT16_,	(uint32_t)&viperdef_Pack8GenTx_TigLimiti1.DTensCutOff_Min,						(uint32_t)&viperdef_Pack8GenTx_TigLimiti1.DTensCutOff_Max,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TIG },

      { &viperdef_Pack8GenTx_Base.Processo,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_PROCESSO_MAX-1),											VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },
      { &viperdef_Pack8GenTx_Base.PtoOperatore,						2,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(1),																(uint32_t)&viperdef_Pack8GenTx_MigLimiti1.MaxPtiOperatore,						VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },
      { &viperdef_Pack8GenTx_Base.TipoMig,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_TIPOMIG_MAX-1),											VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },
      { &viperdef_Pack8GenTx_Base.TipoFilo,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_TIPOFILO_MAX-1),											VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },
      { &viperdef_Pack8GenTx_Base.TipoGas,							1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_TIPOGAS_MAX-1),											VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },
      { &viperdef_Pack8GenTx_Base.DiametroFilo,						1,0,	_TYPEDATA_UINT8_,	DATA_SCALARE(0),																DATA_SCALARE(VIPERDEF_DIAMETROFILO_MAX-1),										VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASE },

      { &viperdef_Pack8GenTx_BasicSetup.GruppoH2OEn,					1,0,	_TYPEDATA_BOOL_,	DATA_SCALARE(0),																DATA_SCALARE(1),																VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_BASICSETUP },
   /*
      viperdef_Pack8Gen_TaratGenToSlave_t         viperdef_Pack8GenRx_TaratGenToSlave;
   */

      /*

      viperdef_Pack8Traino_InfoStatus_t			viperdef_Pack8TrainoTx_InfoStatus;
      viperdef_Pack8Traino_CmdSet_t				viperdef_Pack8TrainoTx_CmdSet;
   */

      { &viperdef_Pack8GenTx_TaraturaEssen.KIo,						9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN },
      { &viperdef_Pack8GenTx_TaraturaEssen.K0Io,						9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN },
      { &viperdef_Pack8GenTx_TaraturaEssen1.KVo,						9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN1 },
      { &viperdef_Pack8GenTx_TaraturaEssen1.K0Vo,						9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN1 },
      { &viperdef_Pack8GenTx_TaraturaEssen2.KISet,					9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN2 },
      { &viperdef_Pack8GenTx_TaraturaEssen2.K0ISet,					9,0,	_TYPEDATA_SINT32_,	DATA_SCALARE(-9999999),															DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN2 },
      { &viperdef_Pack8GenTx_TaraturaEssen3.FascioCavi_MilliOhmResistenza,9,0,_TYPEDATA_UINT32_,	DATA_SCALARE(0),																DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN3 },
      { &viperdef_Pack8GenTx_TaraturaEssen3.FascioCavi_MilliHenryInduttanza,9,0,_TYPEDATA_UINT32_,DATA_SCALARE(0),																DATA_SCALARE(9999999),															VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_TARATURAESSEN3 },

   };

   //______________________________________________________________________________
   // Desc:  Opacizza il colore passato
   // Arg: - colore: Colore tipo RGB
   //      opacita: 0== trasparente ... 1.0==totalmente opaco
   // Ret: - void
   //______________________________________________________________________________
   uint32_t viperui_OpacizzaColore(uint32_t colore, float opacita)
   {
      uint32_t valRet=0;

      valRet = (uint32_t)(((colore>>16)&0xff)*opacita);
      valRet<<=8;
      valRet |= (uint32_t)(((colore>>8)&0xff)*opacita);
      valRet<<=8;
      valRet |= (uint32_t)(((colore)&0xff)*opacita);

      return valRet;
   }

   //______________________________________________________________________________
   // Desc:  Manager del modulo da eseguire ogni ms
   // Arg: - Nessun argomento.
   // Ret: - void
   //______________________________________________________________________________
   void viperui_Manager1MS(void)
   {
      tgfxcustom_Manager1MS();
      data8pack_Manager1MS();
   }

   int32_t viperui_InitModule(void)
   {
      bool errore=false;
         uint16_t indice=0;

         if(!swtimer_Init.InitOk)
            errore = true;

         data_Init.PtrFunPostSetData = PostSetData;
         if(data_InitModule()!=_STATUS_OK_)
            errore = true;

         data8pack_Init.PtrStructDataTX = viperui_StructData8TX;
         if(data8pack_InitModule()!=_STATUS_OK_)
            errore = true;

         tgfxcustom_Init.PtrFunGetStatusEncoder = AdapterEncoderGetStatus;
         tgfxcustom_Init.PtrFunBeep = _board_Beep_Set;
         if(tgfxcustom_InitModule()!=_STATUS_OK_)
            errore = true;

         //__________________________________________________________Init CanBusStack
         canbusstack_Init.GatewayMode = true;
         canbusstack_Init.DimBufferRawData = 5000; // Dimensionare in modo opportuno... (4K + qualcosa in  più)
         canbusstack_Init.CodiceUnivocoScheda = _board_GetCodiceUnivocoScheda();
         if(canbusstack_InitModule()!=_STATUS_OK_)
            errore = true;

         //________________________________________________________________Init HProt
         if(hprot_InitModule()==_STATUS_OK_)
         {
            if(canbusstack_Init.GatewayMode)
            {
               hprotfu_Init.PtrHProtObjRef = hprot_CreateObj((hprot_StructData_t*)hprotfu_HProtStructData,true,5000,canbusstack_RawDataGetChar,canbusstack_RawDataSendData,canbusstack_RawDataFlushRX,canbusstack_RawDataIsEmptyTX,canbusstack_RawDataSetAddressDest,canbusstack_RawDataGetAddressOrg);
               if(!hprotfu_Init.PtrHProtObjRef)
                  errore = true;

               if(hprotfu_InitModule()!=_STATUS_OK_)
                  errore = true;
            }
            else
               hprotfu_Init.PtrHProtObjRef = hprot_CreateObj((hprot_StructData_t*)HProtStructDataCan,false,5000,canbusstack_RawDataGetChar,canbusstack_RawDataSendData,canbusstack_RawDataFlushRX,canbusstack_RawDataIsEmptyTX,canbusstack_RawDataSetAddressDest,canbusstack_RawDataGetAddressOrg);
         }
         else
            errore = true;


      osThreadNew(TaskCANBUS, NULL, &TaskCANBUSThreadAttr);

      if(errore)
      {
         viperui_Init.InitOk = false;
         return _STATUS_FAIL_;
      }
      else
      {
         viperui_Init.InitOk = true;
         return _STATUS_OK_;
      }
   }

   static void TaskCANBUS(void *argument)
   {
      // Serie di richieste iniziali
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_INFOSTATUS);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_STRUMENTO);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMAN);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANHOTSTART);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANCRATERFILLER);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANLIMITI2);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGMANLIMITI1);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYN);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNHOTSTART);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNCRATERFILLER);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNACTUALSYNCALC);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGSYNLIMITI1);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIG);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITICRATERFILLER);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITIHOTSTART);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MIGLIMITI1);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MMA);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_MMALIMITI1);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TIG);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_TIGLIMITI1);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_BASICSETUP);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_BASE);
      _board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_INFOFIRMWARE);

      while(1)
      {
         canbusstack_Manager();
         hprot_ManagerObj(hprotfu_Init.PtrHProtObjRef);
         hprotfu_ManagerGateway();
         _board_CAN_Manager();

         osDelay(3); // Tasmissione in CAN rilassata perchè il micro è fin troppo cattivo per il resto delle schede...
      }
   }


   //______________________________________________________________________________
   // Desc:  Manager del modulo
   // Arg: - Nessun argomento.
   // Ret: - void
   //______________________________________________________________________________
   //void viperui_ZZZ(void)
   //{
   //
   //}


   //============================================================ PRIVATE FUNCTIONS
   // Funzione adapter
   static void AdapterEncoderGetStatus(tgfxcustom_Encoder_e numEncoder,bool *ptrPremuto,int16_t *ptrCounter)
   {
      switch(numEncoder)
      {
         default:
         break;
         case TGFXCUSTOM_ENCODER_LEFT:
            _board_Encoder_GetStatus(_BOARD_JOY_LEFT, ptrPremuto, ptrCounter);
         break;
         case TGFXCUSTOM_ENCODER_CENTER:
            _board_Encoder_GetStatus(_BOARD_JOY_CENTER, ptrPremuto, ptrCounter);
         break;
         case TGFXCUSTOM_ENCODER_RIGHT:
            _board_Encoder_GetStatus(_BOARD_JOY_RIGHT, ptrPremuto, ptrCounter);
         break;
      }
   }

   static void PostSetData(const data_Data_t *ptrStructData)
   {
      data8pack_ForceTX(ptrStructData->Id);
   }


} //extern C



#endif

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
cViper_Info::cViper_Info()
{
   viperui_InitData();
   SelezioneProcesso.AttualeSelezione = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
   memset(&SelezioneProcesso,0x00,(sizeof(SelezioneProcesso)/sizeof(U8)));
   memset(&SelezioneProcesso,0x00,(sizeof(SelezioneProcesso)/sizeof(U8)));
}


void cViper_Info::viperui_InitData()
{
   U16 indice=0;
	memset(ListaCurveInfoDisponibili,0xff,sizeof(ListaCurveInfoDisponibili));
	//_______________________________________________________________________SYN
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_06;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	viperui_Info.ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FERUTIL;
//	viperui_Info.ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	viperui_Info.ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_C1_CO2;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4145;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4557;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3ZN;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8ZN;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER80;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER80;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_SYN;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;




	/*__________________________________________________________PULSE____________________________*/
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FE;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_FEMETAL;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;
//
//
	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI308;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI316;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CRNI2209;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M12_MIX9802;;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALSI4043;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4043;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4047;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4145;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL4557;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_09;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ALMG5356;
	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;

//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5356;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_16;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_AL5554;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_12;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;


//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUSI3CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;



//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_CUAL8CRNI;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;
//
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M21_MIX8020;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_ER120;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_08;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_M20_MIX9208;
//
//	ListaCurveInfoDisponibili[indice].TipoMig		= VIPERDEF_TIPOMIG_PULSE;
//	ListaCurveInfoDisponibili[indice].TipoFilo   	= VIPERDEF_TIPOFILO_NICR3;
//	ListaCurveInfoDisponibili[indice].DiametroFilo	= VIPERDEF_DIAMETROFILO_10;
//	ListaCurveInfoDisponibili[indice++].TipoGas	= VIPERDEF_TIPOGAS_I1_ARGON;




}



//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_CambiaProcesso(viperdef_Processo_e processo,viperdef_TipoMig_e tipoMig,viperdef_TipoFilo_e tipoFilo,viperdef_TipoGas_e tipoGas,viperdef_DiametroFilo_e diametroFilo)
{
	uint32_t uLA;

	uLA = (uint8_t)diametroFilo;
	uLA<<=8;
	uLA |= (uint8_t)tipoGas;
	uLA<<=8;
	uLA|=(uint8_t)tipoFilo;
	uLA<<=4;
	uLA|=(uint8_t)tipoMig;
	uLA<<=4;
	uLA|=processo;


	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_CAMBIAPROCESSO;   // Funzione da eseguiore
	viperdef_Pack8GenRx_FunExecute.ValueFunction = uLA;

	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_SalvaTaratura(void)
{
	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_SALVATARATURA;   // Funzione da eseguire
	viperdef_Pack8GenRx_FunExecute.ValueFunction = 0x00;
	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Esegue il comando di cambio processo verso il CNTInverter
// Arg: - processo: Il processo da impostare
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_Generatore_SalvaParametriInEEProm(void)
{
	viperdef_Pack8GenRx_FunExecute.ByteAA = 0xaa;
	viperdef_Pack8GenRx_FunExecute.Byte55 = 0x55;     // Deve essere 0x55 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Byte66 = 0x66;     // Deve essere 0x66 per eseguire la funzione
	viperdef_Pack8GenRx_FunExecute.Function = VIPERDEF_FUNEXECUTE_SALVAPARAMETRIINEEPROM;   // Funzione da eseguire
	viperdef_Pack8GenRx_FunExecute.ValueFunction = 0x00;
	data8pack_ForceImmediateTX(VIPERDEF_BASECANID_GENRX+VIPERDEF_CANID_GEN_FUNEXECUTE);
}

//______________________________________________________________________________
// Desc:  Ricalcolo combinazioni di selezioni possibili per processo MIG
// Arg: -
// Ret: - void
//______________________________________________________________________________

void cViper_Info::viperui_RecalcBitMappedFiloDiametroGasMig(viperdef_TipoFilo_e tipoFilo,viperdef_DiametroFilo_e diametroFilo,viperdef_TipoGas_e tipoGas)
{
	uint32_t indice;
	uint8_t beccato;
	uint8_t uCA;

	SelezioneProcesso.BitMappedDiametroFilo = 0;
	SelezioneProcesso.BitMappedTipoGas = 0;
	SelezioneProcesso.BitMappedTipoMig = 0;

	indice = 0;
	while(ListaCurveInfoDisponibili[indice].DiametroFilo!=0xff)
	{
		beccato = 0;

		if(ListaCurveInfoDisponibili[indice].TipoFilo==tipoFilo)
			beccato |= _B0_;

		if(diametroFilo==VIPERDEF_DIAMETROFILO_MAX)
			beccato |= _B1_;
		else
		{
			if(ListaCurveInfoDisponibili[indice].DiametroFilo==diametroFilo)
				beccato |= _B1_;
		}

		if(tipoGas==VIPERDEF_TIPOGAS_MAX)
			beccato |= _B2_;
		else
		{
			if(ListaCurveInfoDisponibili[indice].TipoGas==tipoGas)
				beccato |= _B2_;
		}

		if(beccato==0x07)
		{
			SelezioneProcesso.BitMappedDiametroFilo 	|= _B0_<< ListaCurveInfoDisponibili[indice].DiametroFilo;
			SelezioneProcesso.BitMappedTipoGas 		|= _B0_<< ListaCurveInfoDisponibili[indice].TipoGas;
			SelezioneProcesso.BitMappedTipoMig			|= _B0_<< ListaCurveInfoDisponibili[indice].TipoMig;
		}

		indice++;
	}

	SelezioneProcesso.IndiceMaxEnumDiametro=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedDiametroFilo&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumDiametro[SelezioneProcesso.IndiceMaxEnumDiametro]=uCA;
			SelezioneProcesso.IndiceMaxEnumDiametro++;
		}
	}
	SelezioneProcesso.IndiceMaxEnumTipoGas=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedTipoGas&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumTipoGas[SelezioneProcesso.IndiceMaxEnumTipoGas]=uCA;
			SelezioneProcesso.IndiceMaxEnumTipoGas++;
		}
	}
	SelezioneProcesso.IndiceMaxEnumTipoMig=0;
	for(uCA=0;uCA<32;uCA++)
	{
		if(SelezioneProcesso.BitMappedTipoMig&(_B0_<<uCA))
		{
			SelezioneProcesso.TabEnumTipoMig[SelezioneProcesso.IndiceMaxEnumTipoMig]=uCA;
			SelezioneProcesso.IndiceMaxEnumTipoMig++;
		}
	}
}

//void cViper_Info::viperui_ManagerTickEvent(U8 t, U8 p)
//{
//   switch (this->SelezioneProcesso.Processo)
//   {
//      case VIPERDEF_PROCESSO_MMA:
//
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//      break;
//      case VIPERDEF_PROCESSO_MIG:
//
//
//
//
//
//      break;
//      case VIPERDEF_PROCESSO_MAX:
//
//
//         break;
//   }
//   switch(this->SelezioneProcesso.AttualeSelezione)
//   {
//      case VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO:
//         break;
//   }
//}






