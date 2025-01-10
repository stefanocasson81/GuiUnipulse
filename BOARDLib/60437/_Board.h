#ifndef _BOARD_H_INCLUDED
#define _BOARD_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Protocolli/CanBus.h"


//====================================================================== DEFINES
#define _BOARD_QSPI_FLASHSIZE			(0x1000000*2)	// 16MB*2 == 32MB
#define _BOARD_QSPI_SECTORSIZE			(0x10000*2)		// 64KB*2 == 128KB
#define _BOARD_QSPI_PAGESIZE			(0x100*2)		// 256*2  == 512
#define _BOARD_QSPI_ADDRESS				0x90000000

#define _BOARD_ADDRESSBOOTLOADER		0x08000000
#define _BOARD_MAXDIMBOOTLOADER			0x20000		// 128KB

#define _BOARD_ADDRESSINFOHEADER		_BOARD_QSPI_ADDRESS
#define _BOARD_DIMINFOHEADER			0x200		// 512B

#define _BOARD_ADDRESSFIRMWARE 			(_BOARD_ADDRESSINFOHEADER+_BOARD_DIMINFOHEADER)
#define _BOARD_MAXDIMFIRMWARE			(_BOARD_QSPI_FLASHSIZE-_BOARD_DIMINFOHEADER)

#define _BOARD_ADDRESSSDRAM				0xd0000000
#define _BOARD_DIMSDRAM					0x800000	// 8MB

#define _BOARD_LCD_DIMX					480
#define _BOARD_LCD_DIMY					272
#define _BOARD_LCD_FRAMEBUFFERADDRESS	0xd0000000
#define _BOARD_LCD_FRAMEBUFFERSIZE		_BOARD_LCD_DIMX*2*_BOARD_LCD_DIMY


#define _BOARD_LED1_ON()				_PORTSETOUT_(GPIOD,_B5_)
#define _BOARD_LED1_OFF()				_PORTRESETOUT_(GPIOD,_B5_)
#define _BOARD_LED1_EOR()				_PORTEOROUT_(GPIOD,_B5_)
#define _BOARD_LED2_ON()				_PORTSETOUT_(GPIOD,_B6_)
#define _BOARD_LED2_OFF()				_PORTRESETOUT_(GPIOD,_B6_)
#define _BOARD_LED2_EOR()				_PORTEOROUT_(GPIOD,_B6_)

#define _BOARD_PULSANTE_GET()			(_PORTGETIN_(GPIOD,_B4_)^_B4_)

#define _BOARD_PULSANTEJOYL_GET()		(_PORTGETIN_(GPIOG,_B2_)^_B2_)
#define _BOARD_PULSANTEJOYC_GET()		(_PORTGETIN_(GPIOB,_B2_)^_B2_)
#define _BOARD_PULSANTEJOYR_GET()		(_PORTGETIN_(GPIOD,_B11_)^_B11_)

#define _BOARD_LTDC_PIN31_1()			_PORTSETOUT_(GPIOG,_B3_)
#define _BOARD_LTDC_PIN31_0()			_PORTRESETOUT_(GPIOG,_B3_)
#define _BOARD_LTDC_PIN35_1()			_PORTSETOUT_(GPIOC,_B12_)
#define _BOARD_LTDC_PIN35_0()			_PORTRESETOUT_(GPIOC,_B12_)

#define _BOARD_USB_POWER_ON()			_PORTRESETOUT_(GPIOG,_B12_)
#define _BOARD_USB_POWER_OFF()			_PORTSETOUT_(GPIOG,_B12_)

#define _BOARD_EEPROM_CS_ON()			_PORTRESETOUT_(GPIOG,_B10_)
#define _BOARD_EEPROM_CS_OFF()			_PORTSETOUT_(GPIOG,_B10_)

#define _BOARD_DEBUGOUT4_ON()			_PORTSETOUT_(GPIOF,_B6_)
#define _BOARD_DEBUGOUT4_OFF()			_PORTRESETOUT_(GPIOF,_B6_)
#define _BOARD_DEBUGOUT4_EOR()			_PORTEOROUT_(GPIOF,_B6_)
#define _BOARD_DEBUGOUT6_ON()			_PORTSETOUT_(GPIOC,_B10_)
#define _BOARD_DEBUGOUT6_OFF()			_PORTRESETOUT_(GPIOC,_B10_)
#define _BOARD_DEBUGOUT6_EOR()			_PORTEOROUT_(GPIOC,_B10_)


typedef enum
{
	_BOARD_JOY_LEFT		,
	_BOARD_JOY_CENTER	,
	_BOARD_JOY_RIGHT	,

	_BOARD_JOY_MAX
}_board_Joy_e;

typedef enum
{
	_BOARD_CANBUS_PRIMARIO,
	_BOARD_CANBUS_SECONDARIO,

	_BOARD_CANBUS_MAX
}_board_FDCan_e;

typedef enum
{
	_BOARD_BCKREGISTER_CHECKVALIDITABCKREG,	// Se != 0xaa556699 >> Validità registri di backup MCU saltati
	_BOARD_BCKREGISTER_CHECKVALIDITARTC,	// Se != 0xaa556699 >> Validità RTC saltata
	_BOARD_BCKREGISTER_FORZABOOTLOADER,		// Se 0x11111111 aggiornamento da can richiesto sul bootloader
}_board_BckRegister_e;

typedef struct
{
	FDCAN_HandleTypeDef* PtrHandleFDCan[_BOARD_CANBUS_MAX];

//	ADC_HandleTypeDef *PtrHandleADCVBATTEMP;					// Ptr all'adc in comune per VBAT e Temperatura

	TIM_HandleTypeDef *PtrHandleTIM_PwmLcd; 				// Ptr al TIMx per pwm lcd
	uint32_t TIMChannel_PwmLcd;

	TIM_HandleTypeDef *PtrHandleTIM_PwmBuzzer; 				// Ptr al TIMx per pwm buzzer
	uint32_t TIMChannel_PwmBuzzer;

	TIM_HandleTypeDef *PtrHandleTIM_Joy[_BOARD_JOY_MAX];	// Ptr al TIMX per quadratura Encoder Joy SX, CENTER e DX

	const data8pack_Data_t *PtrStructData8RX;				// PTr alla struttura 8pack di ricezione dal can

	uint32_t CodiceEstesoScheda;

	bool InitOk;		// a true sse modulo correttamente inizializzato
}_board_Init_t;

typedef struct
{
	int16_t PhyEncoder[_BOARD_JOY_MAX];

	struct
	{
		bool Connesso;
		uint32_t PacketFifoFailInTX;	// Pacchetti non trasmessi per buffer can pieno
		uint32_t PacchettiRX;
		uint32_t PacchettiTX;
	}StatoCanBus[_BOARD_CANBUS_MAX];

	canbus_Obj_t *PtrCanBusObj;	// Obj del can bus fisico
}_board_Info_t;



//============================================================= GLOBAL VARIABLES
extern _board_Init_t _board_Init;
extern _board_Info_t _board_Info;
extern canbus_Obj_t *_board_PtrCanBusObj[_BOARD_CANBUS_MAX];	// Obj del can bus fisico


//============================================================ GLOBAL PROTOTYPES
int32_t _board_InitModule(void);
void _board_Manager1MS(void);

void _board_LCD_Power(uint8_t percent);
void _board_LCD_ClearDisp(void);
void _board_LCD_GotoXY(uint16_t coordX,uint16_t coordY);
void _board_LCD_PutPixel(uint16_t coordX,uint16_t coordY,uint16_t color);
void _board_LCD_SetColor(uint8_t r,uint8_t g,uint8_t b);
void _board_LCD_SetBcgColor(uint8_t r,uint8_t g,uint8_t b);
void _board_LCD_Putch(char c);
void _board_LCD_Puts(char* ptrStr);


void _board_Encoder_GetStatus(_board_Joy_e numEncoder,bool *ptrPremuto,int16_t *ptrCounter);

void _board_Beep_Set(uint16_t mSBeepON);

void _board_USB_Power(bool power);

void _board_EEProm_SetCS(bool status);

void _board_CAN_Manager(void);
int32_t _board_CAN_ReqRemoteId(uint16_t id);

uint32_t _board_BackupRegister_Read(_board_BckRegister_e bckRegister);
void _board_BackupRegister_Write(_board_BckRegister_e bckRegister,uint32_t value);

uint32_t _board_GetCodiceUnivocoScheda(void);

void _board_ForceBootLoader(bool status);
bool _board_GetStatusForceBootLoader(void);
void _board_ExecuteReboot(bool modalitaBootLoader);


#endif /* _BOARD_H_INCLUDED */

