/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	08/03/23
 Descrizione:
	Definizione primitive della scheda 60437 (TinyDisplay)
	Micro supportato STM32H730ZBT

	Memory mapping:
	SRAM 				H2400.0000	H2404.FFFF	H0002.0000	320KB
	FLASH(BootLoader)	H0800.0000	H0801.FFFF	H0002.0000	128KB
	QSPIFLASH(Firmware)	H9000.0000	H91FF.FFFF	H0200.0000	32MB


------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "_Board.h"
#include "ARMLib/Protocolli/CanBus.h"
#include "ARMLib/Common/Crc.h"
#include "Font.h"
#include "stm32h7xx_ll_utils.h"


//====================================================================== DEFINES
typedef enum
{
	_BOARD_COUNTERTICK1MS_BEEPER,
	_BOARD_COUNTERTICK1MS_LED1VITABLINK,
	
	_BOARD_COUNTERTICK1MS_MAX
}_board_CounterTick1MS_e;

//=========================================================== PRIVATE PROTOTYPES
static int32_t CAN_SendPacket(canbus_Packet_t *ptrPacket);
static bool CAN_CanPushPacket(FDCAN_HandleTypeDef *hfdcan);

//============================================================= STATIC VARIABLES

static uint16_t ArrayCounterTick1MS[_BOARD_COUNTERTICK1MS_MAX];

static uint16_t ActualPosX=0;
static uint16_t ActualPosY=0;
static uint16_t ActualColor=(uint16_t)0xffffff;
static uint16_t ActualBcgColor=0;

static uint16_t *PtrVideoMem=(uint16_t*)(_BOARD_LCD_FRAMEBUFFERADDRESS);

//============================================================= GLOBAL VARIABLES
_board_Init_t _board_Init;
_board_Info_t _board_Info;

extern RTC_HandleTypeDef hrtc;


//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t _board_InitModule(void)
{
	bool errore=false;
	canbus_CfgInitObj_t canbusCfgInitObj;
	FDCAN_FilterTypeDef fDCanFilter;
	FDCAN_HandleTypeDef* ptrHandleFDCan;

	_BOARD_LED1_OFF();
	_BOARD_LED2_OFF();

	_board_USB_Power(_OFF_);
	_board_EEProm_SetCS(_OFF_);

	memset((void*)0xd0000000,0,0x100000);

	if(_board_Init.PtrHandleTIM_PwmLcd && _board_Init.PtrHandleTIM_PwmBuzzer &&
	   _board_Init.PtrHandleTIM_Joy[_BOARD_JOY_LEFT] && _board_Init.PtrHandleTIM_Joy[_BOARD_JOY_RIGHT])
	{
		HAL_TIM_Encoder_Start(_board_Init.PtrHandleTIM_Joy[_BOARD_JOY_LEFT], TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(_board_Init.PtrHandleTIM_Joy[_BOARD_JOY_RIGHT], TIM_CHANNEL_ALL);

		HAL_TIM_PWM_Start(_board_Init.PtrHandleTIM_PwmLcd, _board_Init.TIMChannel_PwmLcd);

		_board_LCD_Power(0);
	}
	else
		errore = true;

	memset(ArrayCounterTick1MS, 0, sizeof(ArrayCounterTick1MS));

	//______________________________________________________Inizializzazione CAN
	memset(_board_Info.StatoCanBus,0,sizeof(_board_Info.StatoCanBus));
	if(canbus_InitModule()==_STATUS_OK_)
	{
		if(_board_Init.PtrHandleFDCan[_BOARD_CANBUS_PRIMARIO])
		{
			ptrHandleFDCan = _board_Init.PtrHandleFDCan[_BOARD_CANBUS_PRIMARIO];

			// L'RXFIFO0 viene dedicato per i messaggi ad alta priorità nel range H000...H0FF
			memset(&fDCanFilter,0,sizeof(fDCanFilter));
			fDCanFilter.IdType 			= FDCAN_STANDARD_ID;
			fDCanFilter.FilterIndex 	= 0;
			fDCanFilter.FilterType 		= FDCAN_FILTER_RANGE;
			fDCanFilter.FilterID1 		= 0x0;
			fDCanFilter.FilterID2 		= 0x7FF;
			fDCanFilter.FilterConfig 	= FDCAN_FILTER_TO_RXFIFO0;

			// Configure global filter to reject all non-matching frames
			HAL_FDCAN_ConfigGlobalFilter(ptrHandleFDCan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_REJECT_REMOTE);
			if (HAL_FDCAN_ConfigFilter(ptrHandleFDCan, &fDCanFilter) == HAL_OK)
			{
				if (HAL_FDCAN_ActivateNotification(ptrHandleFDCan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
					errore = true;
			}
			else
				errore = true;

			memset(&canbusCfgInitObj,0,sizeof(canbusCfgInitObj));
			canbusCfgInitObj.NPacketBufferRXHP 		= 32;
			canbusCfgInitObj.NPacketBufferRXLP 		= 32;
			canbusCfgInitObj.NPacketBufferSwitch	= 0;	// No funzione switch
			canbusCfgInitObj.PtrHandleFDCan 		= ptrHandleFDCan;
			canbusCfgInitObj.PtrStructData8RX 		= _board_Init.PtrStructData8RX;
			canbusCfgInitObj.PtrFunSendCanPacket	= CAN_SendPacket;
			canbusCfgInitObj.PtrFunCanPushPacket	= CAN_CanPushPacket;

			_board_Info.PtrCanBusObj = canbus_CreateObj(&canbusCfgInitObj);
			if(_board_Info.PtrCanBusObj)
			{
				if(HAL_FDCAN_Start(ptrHandleFDCan) == HAL_OK)
				{
				}
				else
					errore = true;
			}
			else
				errore = true;
		}
	}
	else
		errore = true;

	//_______________________________________Init rilevazione VBAT e temperatura
	/*
	if(_board_Init.PtrHandleADCVBATTEMP)
	{
		if(HAL_ADCEx_Calibration_Start(_board_Init.PtrHandleADCVBATTEMP, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED)==HAL_OK)
		{
			if(HAL_ADC_Start(_board_Init.PtrHandleADCVBATTEMP)!= HAL_OK)
				errore= true;
		}
		else
			errore= true;
	}
	*/


	if(errore)
	{
		_board_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		_board_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

uint32_t NikNikULA;
uint32_t NikNikULB;
uint32_t NikNikULC;
uint32_t NikNikULARes;
uint32_t NikNikULBRes;
float NikNikULCRes;

/*______________________________________________________________________________
 Desc:  Manager del modulo da eseguire con cadenza precisa a 1ms
 Arg: - void
 Ret: - void
______________________________________________________________________________*/
void _board_Manager1MS(void)
{
	static bool OldStatusBeeper = false;
	static uint8_t CounterReset=0;
	uint8_t uCA;

	for(uCA = 0; uCA < _BOARD_COUNTERTICK1MS_MAX; uCA++)
	{
		if(ArrayCounterTick1MS[uCA])
			ArrayCounterTick1MS[uCA]--;
	}

	if(!ArrayCounterTick1MS[_BOARD_COUNTERTICK1MS_LED1VITABLINK])
	{
		ArrayCounterTick1MS[_BOARD_COUNTERTICK1MS_LED1VITABLINK] = 1000;
		_BOARD_LED1_EOR();
	}


	if(_board_Init.InitOk)
	{
		/*
		if(HAL_ADC_PollForConversion(_board_Init.PtrHandleADCVBATTEMP,100)==HAL_OK)
		{
			NikNikULC = HAL_ADC_GetValue(_board_Init.PtrHandleADCVBATTEMP);
			//NikNikULCRes = 357.558 - 0.187364*NikNikULC;

			NikNikULCRes = __HAL_ADC_CALC_TEMPERATURE(3300, NikNikULC, ADC_RESOLUTION_12B);
		}
		*/
		/*
		if(HAL_ADC_PollForConversion(_board_Init.PtrHandleADCVBATTEMP,100)==HAL_OK)
		{
			NikNikULA = HAL_ADC_GetValue(_board_Init.PtrHandleADCVBATTEMP);
			if(HAL_ADC_PollForConversion(_board_Init.PtrHandleADCVBATTEMP,100)==HAL_OK)
			{
				NikNikULB = HAL_ADC_GetValue(_board_Init.PtrHandleADCVBATTEMP);
				if(HAL_ADC_PollForConversion(_board_Init.PtrHandleADCVBATTEMP,100)==HAL_OK)
				{
					NikNikULC = HAL_ADC_GetValue(_board_Init.PtrHandleADCVBATTEMP);
				}
			}
		}

		NikNikULARes =  __HAL_ADC_CALC_VREFANALOG_VOLTAGE(NikNikULA, ADC_RESOLUTION_12B);
		NikNikULBRes = 4UL * __HAL_ADC_CALC_DATA_TO_VOLTAGE(NikNikULA, NikNikULB, ADC_RESOLUTION_12B);
		NikNikULCRes = __HAL_ADC_CALC_TEMPERATURE(NikNikULA, NikNikULB, ADC_RESOLUTION_12B);
//		NikNikULCRes = __HAL_ADC_CALC_TEMPERATURE_TYP_PARAMS(2000, 620, 30, NikNikULA, NikNikULB, ADC_RESOLUTION_12B);
  */

		if(_board_Init.InitOk)
		{
			if(ArrayCounterTick1MS[_BOARD_COUNTERTICK1MS_BEEPER])
			{
				if(!OldStatusBeeper)
					HAL_TIM_PWM_Start(_board_Init.PtrHandleTIM_PwmBuzzer,_board_Init.TIMChannel_PwmLcd);
				OldStatusBeeper = true;
			}
			else
			{
				if(OldStatusBeeper)
					HAL_TIM_PWM_Stop(_board_Init.PtrHandleTIM_PwmBuzzer,_board_Init.TIMChannel_PwmLcd);
				OldStatusBeeper = false;
			}
		}

		if(_BOARD_PULSANTE_GET())
		{
			CounterReset++;
			if(CounterReset>=100)
			{
				NVIC_SystemReset();
			}
		}
		else
			CounterReset = 0;
	}
}



//______________________________________________________________________________
// Desc:  Ritorna true se packet ricevuto
// Arg: - ptrPacket: storage del packet
// Ret: - true se packet ricevuto
//______________________________________________________________________________
void _board_CAN_Manager(void)
{
	canbus_ObjManager(_board_Info.PtrCanBusObj);
}

//______________________________________________________________________________
// Desc:  Esegue una richiesta di remote packet
// Arg: - id: id del packet da richiedeere in remote
// Ret: - void
//______________________________________________________________________________
int32_t _board_CAN_ReqRemoteId(uint16_t id)
{
	return canbus_ReqRemoteId(_board_Info.PtrCanBusObj,id);
}


/*______________________________________________________________________________
 Desc:  Pilota fisicamente il CS della eeprom
 Arg: - mSBeep: millisec di beep ON
 Ret: - void
______________________________________________________________________________*/
void _board_EEProm_SetCS(bool status)
{
	if(status)
		_BOARD_EEPROM_CS_ON();
	else
		_BOARD_EEPROM_CS_OFF();
}


/*______________________________________________________________________________
 Desc:  Setta i ms di beep
 Arg: - mSBeep: millisec di beep ON
 Ret: - void
______________________________________________________________________________*/
void _board_Beep_Set(uint16_t mSBeepON)
{
	ArrayCounterTick1MS[_BOARD_COUNTERTICK1MS_BEEPER] = mSBeepON;
}

/*______________________________________________________________________________
 Desc:  Setta la percentuale luminosità
 Arg: - percent: 0==spento ,100== a manetta
 Ret: - void
______________________________________________________________________________*/
void _board_LCD_Power(uint8_t percent)
{
	uint32_t uLA;

	if(percent > 100)
		percent = 100;

	if(percent)
	{
		_BOARD_LTDC_PIN31_1();
	}
	else
	{
		_BOARD_LTDC_PIN31_0();
	}

	uLA = _board_Init.PtrHandleTIM_PwmLcd->Instance->ARR;
	uLA *= percent;
	uLA /= 100;

	switch(_board_Init.TIMChannel_PwmLcd)
	{
		case TIM_CHANNEL_1:
			_board_Init.PtrHandleTIM_PwmLcd->Instance->CCR1 = uLA;
		break;
		case TIM_CHANNEL_2:
			_board_Init.PtrHandleTIM_PwmLcd->Instance->CCR2 = uLA;
		break;
		case TIM_CHANNEL_3:
			_board_Init.PtrHandleTIM_PwmLcd->Instance->CCR3 = uLA;
		break;
		case TIM_CHANNEL_4:
			_board_Init.PtrHandleTIM_PwmLcd->Instance->CCR4 = uLA;
		break;
	}
}

/*______________________________________________________________________________
 Desc:  Usato fuori dalla TouchGfx cancella lo schermo
 Arg: - void
 Ret: - void
______________________________________________________________________________*/
void _board_LCD_ClearDisp(void)
{
	memset((void*)_BOARD_LCD_FRAMEBUFFERADDRESS,0,_BOARD_LCD_FRAMEBUFFERSIZE);
}

void _board_LCD_GotoXY(uint16_t coordX,uint16_t coordY)
{
	ActualPosX = coordX;
	ActualPosY = coordY;
}

void _board_LCD_PutPixel(uint16_t coordX,uint16_t coordY,uint16_t color)
{
	uint16_t *ptrVideoMem=PtrVideoMem;

	if( (coordX<_BOARD_LCD_DIMX) && (coordY<_BOARD_LCD_DIMY) )
	{
		ptrVideoMem+=(coordY*_BOARD_LCD_DIMX)+coordX;
		*ptrVideoMem = color;
	}
}

void _board_LCD_SetColor(uint8_t r,uint8_t g,uint8_t b)
{
	uint16_t uSA;
	uSA = r;
	uSA<<=8;
	uSA &= 0xf100;	// 5bit
	ActualColor = uSA;
	uSA = g;
	uSA<<=3;
	uSA &= 0x07e0;	// 6bit
	ActualColor |= uSA;
	uSA = b;
	uSA>>=3;
	uSA &= 0x001f;	// 5bit
	ActualColor |= uSA;
}

void _board_LCD_SetBcgColor(uint8_t r,uint8_t g,uint8_t b)
{
	uint16_t uSA;
	uSA = r;
	uSA<<=8;
	uSA &= 0xf100;	// 5bit
	ActualBcgColor = uSA;
	uSA = g;
	uSA<<=3;
	uSA &= 0x07e0;	// 6bit
	ActualBcgColor |= uSA;
	uSA = b;
	uSA>>=3;
	uSA &= 0x001f;	// 5bit
	ActualBcgColor |= uSA;
}

void _board_LCD_Putch(char c)
{
	uint32_t x,y;
	uint8_t *ptrData;
	uint8_t rollata;
	uint8_t carattere;

	if(c>=0x20)
	{
		carattere = c-' ';

		ptrData = (uint8_t*)Font.chars[carattere].image->data;
		for(y=0;y<Font.chars[carattere].image->height;y++)
		{
			rollata=0x80;
			for(x=0;x<Font.chars[carattere].image->width;x++)
			{
				if(*ptrData&rollata)
					_board_LCD_PutPixel(ActualPosX+x,ActualPosY+y,ActualBcgColor);
				else
					_board_LCD_PutPixel(ActualPosX+x,ActualPosY+y,ActualColor);

				rollata>>=1;
				if(!rollata)
				{
					rollata=0x80;
					ptrData++;
				}
			}

			if(rollata!=0x80)	// Per evitare di incrementare quando la gimensione è giusta a multipli di 8
				ptrData++;
		}
		_board_LCD_GotoXY(ActualPosX+Font.chars[carattere].image->width,ActualPosY);
	}
	else if(c==0x0d)
	{
		_board_LCD_GotoXY(0,ActualPosY);
	}
	else if(c==0x0a)
	{
		_board_LCD_GotoXY(ActualPosX,ActualPosY+Font.chars[0].image->height);
	}

}

void _board_LCD_Puts(char* ptrStr)
{
	while(*ptrStr)
	{
		_board_LCD_Putch(*ptrStr);
		ptrStr++;
	}
}


//______________________________________________________________________________
// Desc:  Ritorna lo stato dell'encoder passato
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void _board_Encoder_GetStatus(_board_Joy_e numEncoder,bool *ptrPremuto,int16_t *ptrCounter)
{
	int16_t sA;

	switch(numEncoder)
	{
		default:
		break;

		case _BOARD_JOY_LEFT:
			sA = _board_Init.PtrHandleTIM_Joy[numEncoder]->Instance->CNT;
			_board_Info.PhyEncoder[numEncoder] = sA;
			sA>>=1;
			*ptrCounter = sA;

			if(_BOARD_PULSANTEJOYL_GET())
				*ptrPremuto = true;
			else
				*ptrPremuto = false;
		break;
		case _BOARD_JOY_CENTER:
			*ptrCounter = 0; //_board_Init.PtrHandleTIM_Joy[_BOARD_JOY_RIGHT]->Instance->CNT>>1;	// Fisicamente centrale è lo stesso del destro

			if(_BOARD_PULSANTEJOYC_GET())
				*ptrPremuto = true;
			else
				*ptrPremuto = false;
		break;
		case _BOARD_JOY_RIGHT:
			sA = _board_Init.PtrHandleTIM_Joy[numEncoder]->Instance->CNT;
			_board_Info.PhyEncoder[numEncoder] = sA;
			sA>>=1;
			*ptrCounter = sA;

			if(_BOARD_PULSANTEJOYR_GET())
				*ptrPremuto = true;
			else
				*ptrPremuto = false;
		break;
	}
}


//______________________________________________________________________________
// Desc:  Attiva o disattiva il power dell'USB
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void _board_USB_Power(bool power)
{
	if(power)
		_BOARD_USB_POWER_ON();
	else
		_BOARD_USB_POWER_OFF();


}

//______________________________________________________________________________
// Desc:  Legge il registro di backup MCU voluto
// Arg: - bckRegister: enum del tipo _board_BckRegister_e, in ogni caso 0..31
// Ret: - Valore del registro
//______________________________________________________________________________
uint32_t _board_BackupRegister_Read(_board_BckRegister_e bckRegister)
{
	return HAL_RTCEx_BKUPRead(&hrtc,bckRegister);
}

//______________________________________________________________________________
// Desc:  Scrive il registro di backup MCU voluto
// Arg: - bckRegister: enum del tipo _board_BckRegister_e, in ogni caso 0..31
// Ret: - Valore del registro
//______________________________________________________________________________
void _board_BackupRegister_Write(_board_BckRegister_e bckRegister,uint32_t value)
{
	HAL_RTCEx_BKUPWrite(&hrtc,bckRegister,value);
}

//______________________________________________________________________________
// Desc:  Ritorna il codice univoco scheda
// Arg: - Nessun argomento.
// Ret: - Codice univoco
//______________________________________________________________________________
uint32_t _board_GetCodiceUnivocoScheda(void)
{
	uint64_t valRet = 0;
	uint32_t uLA;

	uLA = LL_GetUID_Word0();
	valRet = crc_CalcCrc32((uint8_t*)&uLA,4,valRet);
	uLA = LL_GetUID_Word1();
	valRet = crc_CalcCrc32((uint8_t*)&uLA,4,valRet);
	uLA = LL_GetUID_Word2();
	valRet = crc_CalcCrc32((uint8_t*)&uLA,4,valRet);
	valRet = crc_CalcCrc32((uint8_t*)&_board_Init.CodiceEstesoScheda,4,valRet);

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Forza la modalità bootloader. NOTA: NON esegue il reset
 Arg: - Se a true forza la modalità bootloader per il prossimo riavvio
 Ret: - void
______________________________________________________________________________*/
void _board_ForceBootLoader(bool status)
{
	if(_board_Init.InitOk)
	{
		if(status)
			_board_BackupRegister_Write(_BOARD_BCKREGISTER_FORZABOOTLOADER, 0x11111111);
		else
			_board_BackupRegister_Write(_BOARD_BCKREGISTER_FORZABOOTLOADER, 0x00000000);
	}
}

/*______________________________________________________________________________
 Desc:  Ritorna lo stato di bootloader forzato
 Arg: - Nessun argomento.
 Ret: - true sse bootloader forzato
______________________________________________________________________________*/
bool _board_GetStatusForceBootLoader(void)
{
	bool valRet = false;

	if(_board_Init.InitOk)
	{
		if(_board_BackupRegister_Read(_BOARD_BCKREGISTER_FORZABOOTLOADER)==0x11111111)
			valRet = true;
		else
			valRet = false;
	}

	return valRet;
}

/*______________________________________________________________________________
 Desc:  Esegue un reboot scheda
 Arg: - Nessun argomento.
 Ret: - bool direzione
______________________________________________________________________________*/
void _board_ExecuteReboot(bool modalitaBootLoader)
{
	if(modalitaBootLoader)
		_board_ForceBootLoader(true);
	else
		_board_ForceBootLoader(false);

	NVIC_SystemReset();
}

//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicit� dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void _board_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS
/*
int __io_putchar(int ch)
{
	ITM_SendChar(ch);

	return ch;
}
*/

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
	  ITM_SendChar(*ptr++);
  }
  return len;
}


static bool CAN_CheckConnection(_board_FDCan_e whatCan)
{
	if(_board_Init.PtrHandleFDCan[whatCan]->Instance->TXFQS & FDCAN_TXFQS_TFQF)
	{
		if((_board_Init.PtrHandleFDCan[whatCan]->Instance->PSR & FDCAN_PSR_LEC)==0x03)
		{	// AckError
			_board_Info.StatoCanBus[whatCan].Connesso=false;
		}
	}
	else
		_board_Info.StatoCanBus[whatCan].Connesso=true;

	//Cerco di riconnettere se anomalie rilevate
	if(_board_Init.PtrHandleFDCan[whatCan]->Instance->CCCR & FDCAN_CCCR_INIT)
		_board_Init.PtrHandleFDCan[whatCan]->Instance->CCCR &= ~FDCAN_CCCR_INIT;

	return _board_Info.StatoCanBus[whatCan].Connesso;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	FDCAN_RxHeaderTypeDef rxHeader;
	uint8_t data[CANBUS_MAXDIMDATA];


	if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, data) == HAL_OK)
	{
		if(canbus_Init.InitOk)
		{
			canbus_PushRxMessage(_board_Info.PtrCanBusObj,&rxHeader,data,0);
			_board_Info.StatoCanBus[_BOARD_CANBUS_PRIMARIO].PacchettiRX++;
		}
	}
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

}


static int32_t CAN_SendPacket(canbus_Packet_t *ptrPacket)
{
	static FDCAN_TxHeaderTypeDef TxHeader=
	{
		.IdType 				= FDCAN_STANDARD_ID,
		.ErrorStateIndicator	= FDCAN_ESI_ACTIVE,
		.BitRateSwitch 			= FDCAN_BRS_OFF,
		.FDFormat 				= FDCAN_CLASSIC_CAN,
		.TxEventFifoControl 	= FDCAN_NO_TX_EVENTS,
		.MessageMarker 			= 0,
	};
	TxHeader.Identifier 	= ptrPacket->Id;
	switch(ptrPacket->DimData)
	{
		default:
		case 0:
			TxHeader.DataLength = FDCAN_DLC_BYTES_0;
		break;
		case 1:
			TxHeader.DataLength = FDCAN_DLC_BYTES_1;
		break;
		case 2:
			TxHeader.DataLength = FDCAN_DLC_BYTES_2;
		break;
		case 3:
			TxHeader.DataLength = FDCAN_DLC_BYTES_3;
		break;
		case 4:
			TxHeader.DataLength = FDCAN_DLC_BYTES_4;
		break;
		case 5:
			TxHeader.DataLength = FDCAN_DLC_BYTES_5;
		break;
		case 6:
			TxHeader.DataLength = FDCAN_DLC_BYTES_6;
		break;
		case 7:
			TxHeader.DataLength = FDCAN_DLC_BYTES_7;
		break;
		case 8:
			TxHeader.DataLength = FDCAN_DLC_BYTES_8;
		break;
	}
	if(ptrPacket->IsRemoteFrame)
		TxHeader.TxFrameType = FDCAN_REMOTE_FRAME;
	else
		TxHeader.TxFrameType = FDCAN_DATA_FRAME;

	if(CAN_CheckConnection(_BOARD_CANBUS_PRIMARIO))
	{
		if(HAL_FDCAN_AddMessageToTxFifoQ(_board_Init.PtrHandleFDCan[_BOARD_CANBUS_PRIMARIO], &TxHeader, ptrPacket->Data)==HAL_OK)
			_board_Info.StatoCanBus[_BOARD_CANBUS_PRIMARIO].PacchettiTX++;
		else
			_board_Info.StatoCanBus[_BOARD_CANBUS_PRIMARIO].PacketFifoFailInTX++;
	}

	return _STATUS_OK_;
}

// Verifica se è possbile trasmettere
static bool CAN_CanPushPacket(FDCAN_HandleTypeDef *hfdcan)
{
	CAN_CheckConnection(_BOARD_CANBUS_PRIMARIO);	// Tanto per sbloccare eventuali anomalie

	if(HAL_FDCAN_GetTxFifoFreeLevel(_board_Init.PtrHandleFDCan[_BOARD_CANBUS_PRIMARIO])>1)	// Considero l'eventuale trasmissione su tre can simultaneamente nel caso di gestione switch
		return true;
	else
		return false;
}
