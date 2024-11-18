/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	01/05/23
 Descrizione:
	Interfaccia utente viper project basato su STM32H730.


	NOTE:
	- Il file FixHeaderDebugFirmwareSection.bin contenuto nel progetto è l'intestazione
	per la protezione e aggiornamento del firmware. I primi H200 byte vengono forzati
	in debug per bypassare velocemente la fase di check firmware.



------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "Applicazione.h"
#include "_Board.h"
#include "ARMLib/Common/SwTimer.h"
#include "ARMLib/Common/Data8Pack.h"
#include "ARMLib/Common/Data.h"
#include "ARMLib/Common/CustomRtc.h"
#include "ARMLib/FileSystem/_DrvFileSystemUSB.h"
#include "ARMLib/Device/Flash/_DrvFlashEEProm.h"
#include "ARMLib/FileSystem/Xml.h"
#include "ARMLib/Common/FWUpgrade.h"
#include "XmlSupport.h"
#include "Features.h"
#include "CORELib/Viper/Common/ViperDef.h"

#include "ARMLib/GUI/TGFX/TGFXCustom.h"
#include "CORELib/Viper/UserInterface/ViperUI.h"



//====================================================================== DEFINES
#define APPLICAZIONE_TASKPRIORITY_LOWGUI				osPriorityBelowNormal3

//=========================================================== PRIVATE PROTOTYPES
static void TaskLowGui(void *argument);

//============================================================= STATIC VARIABLES


static const osThreadAttr_t TaskLowGuiAttr =
{
	.name = "LowGui",
	.stack_size = 8192*2,
	.priority = APPLICAZIONE_TASKPRIORITY_LOWGUI,
};

static const uint8_t TabellaTranscodifica[]={24,2,26,6,4,7,31,30,29,28,27,25,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,5,3,1,0};

//============================================================= GLOBAL VARIABLES


//extern ADC_HandleTypeDef hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;

applicazione_Init_t applicazione_Init;
applicazione_Info_t applicazione_Info;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t applicazione_InitModule(void)
{
	bool errore=false;

	//_______________________________________________________Init timer software
	swtimer_Init.NumTimerDaGestire = 10;
	if(swtimer_InitModule()!=_STATUS_OK_)
		errore = true;

	//_______________________________________________________________Init _Board
	_board_Init.PtrHandleFDCan[_BOARD_CANBUS_PRIMARIO] 	= &hfdcan1;
	_board_Init.PtrHandleTIM_PwmLcd 					= &htim3;
	_board_Init.TIMChannel_PwmLcd						= TIM_CHANNEL_3;
	_board_Init.PtrHandleTIM_PwmBuzzer 					= &htim2;
	_board_Init.TIMChannel_PwmBuzzer					= TIM_CHANNEL_3;
	_board_Init.PtrHandleTIM_Joy[_BOARD_JOY_LEFT] 		= &htim5;
	_board_Init.PtrHandleTIM_Joy[_BOARD_JOY_RIGHT] 		= &htim4;
	_board_Init.PtrStructData8RX						= viperui_StructData8RX;
	//_board_Init.PtrHandleADCVBATTEMP					= &hadc3;
	if(_board_InitModule()!=_STATUS_OK_)
		errore = true;

	//_____________________________________________________________Init orologio
	customrtc_Init.PtrHrtc = &hrtc;
	if(customrtc_InitModule()!=_STATUS_OK_)
		errore = true;

	//____________________________________________________________________EEProm
	_drvflasheeprom_Init.ChipName = _DRVFLASHEEPROM_CHIPNAME_M95M01;
	_drvflasheeprom_Init.PtrSpiHandleTypeDef = &hspi1;
	_drvflasheeprom_Init.PtrFunSetCS = _board_EEProm_SetCS;
	if(_drvflasheeprom_InitModule()!=_STATUS_OK_)
		errore = true;

	//__________________________________________________________Init file system
	if(_drvfilesystemusb_InitModule()==_STATUS_OK_)
	{
		if(xml_InitModule()==_STATUS_OK_)
		{

		}
	}
	else
		errore = true;

	//_____________________________________________________Init firmware upgrade
	fwupgrade_Init.Crc32FirmwareAttuale = ((fwupgrade_InfoFileCodificato_t*)_BOARD_ADDRESSINFOHEADER)->Crc32Originale;

	fwupgrade_Init.AddressInfoHeader	= _BOARD_ADDRESSINFOHEADER;
	fwupgrade_Init.DimInfoHeader		= _BOARD_DIMINFOHEADER;

	fwupgrade_Init.AddressFirmware		= _BOARD_ADDRESSFIRMWARE;
	fwupgrade_Init.MaxDimFirmware		= _BOARD_MAXDIMFIRMWARE;

	fwupgrade_Init.PtrDrvFlashDriver	= _NULL_;

	fwupgrade_Init.CodiceSchedaEsteso 	= FEATURES_INFOFIRMWARE_CODICEESTESO;

	fwupgrade_Init.PtrFunFunInfoStatus	= _NULL_;

	fwupgrade_Init.PtrDrvfilesystem 	= &_drvfilesystemusb_Driver;
	fwupgrade_Init.PtrTabellaTranscodifica = (uint8_t*)TabellaTranscodifica;

	if(fwupgrade_InitModule()!=_STATUS_OK_)
	    errore = true;


	//______________________________________________Init appoggio user interface
	if(viperui_InitModule()!=_STATUS_OK_)
		errore = true;

	osThreadNew(TaskLowGui, 		NULL, 	&TaskLowGuiAttr);

	/*
	if(viperdef_Pack8Gen_Base.PtoOperatore==5)
		errore = true;
*/
	if(errore)
	{
		applicazione_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		applicazione_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

/*______________________________________________________________________________
 Desc:  Manager del modulo da eseguire con cadenza precisa a 1ms
 Arg: - void
 Ret: - void
______________________________________________________________________________*/
void applicazione_Manager1MS(void)
{
	_board_Manager1MS();
	viperui_Manager1MS();
}

//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicit� dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void applicazione_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS
// Task a bassa priorita sotto la gui per esguire funzioni con assolulta calma
static void TaskLowGui(void *argument)
{
	static drvfilesystem_PeripheralsStatus_e oldperipheralsStatus=DRVFILESYSTEM_PERIPHERALSSTATUS_NOTCONFIGURED;
	char tmpStr[100];

	_board_LCD_Power(50);

	while(1)
	{
		//___________________________________________________________Manager USB
		_drvfilesystemusb_Manager();

		if(oldperipheralsStatus!=_drvfilesystemusb_Driver.Info.PeripheralsStatus)
		{
			oldperipheralsStatus=_drvfilesystemusb_Driver.Info.PeripheralsStatus;
			if(oldperipheralsStatus==DRVFILESYSTEM_PERIPHERALSSTATUS_READY)
			{	// Operazioni da eseguire all'inserimento chiavetta....

				//_Gestione lettura file ImmediateCmd.xml
				if(_drvfilesystemusb_Driver.FFind(true,_NULL_,"ImmediateCmd.xml"))
				{
					if(xmlsupport_LeggiImmediateCmd()!=_STATUS_OK_)
					{
						// Segnalare qui eventuale errore
					}
				}

				xmlsupport_SalvaXmlActualCfg();

				// Se seguire uno screenshoot
				if(applicazione_Info.ExecuteScreenShoot)
				{
					applicazione_Info.ExecuteScreenShoot = false;

					customrtc_Time_t customRtcTime;

					customrtc_GetTime(&customRtcTime);
					sprintf(tmpStr,"ScreenShot_%02d%02d%02d_%02d%02d%02d",customRtcTime.Anno,customRtcTime.Mese+1,customRtcTime.GiornoDelMese,customRtcTime.Ore,customRtcTime.Minuti,customRtcTime.Secondi);
					strcat(tmpStr,".bmp");
					tgfxcustom_SaveFrameBuffer((void*)LTDC_Layer1->CFBAR,480,272,&_drvfilesystemusb_Driver,tmpStr);
				}

			}
		}

		osDelay(1);
	}
}






