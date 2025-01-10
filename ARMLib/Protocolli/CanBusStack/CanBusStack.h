#ifndef CANBUSSTACK_H_INCLUDED
#define CANBUSSTACK_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES


typedef enum
{
	CANBUSSTACK_TIPOEVENTO_GATEWAYWAKEON				,
	CANBUSSTACK_TIPOEVENTO_SLAVEWAKEON					,
	CANBUSSTACK_TIPOEVENTO_GATEWAYTOSLAVESETADDRESS		,
	CANBUSSTACK_TIPOEVENTO_GATEWAYREFRESH				,
}canbusstack_TipoEvento_e;


#define CANBUSSTACK_IDCAN_BUSSTACKBASE				0x500	// Id can di base per il busstack, seguono 16 id continui
#define CANBUSSTACK_IDCAN_BUSSTACKENDZONE			0x50f	// Id can di fine competenza busstack
#define CANBUSSTACK_IDCAN_BUSSTACKEVENT				0x510	// Id can evento StackBus

#define CANBUSSTACK_ADDRESSROUTER	1	// Address del router
#define CANBUSSTACK_MAXNUMADDRESS 16	// 1..15 0==broadcast

#pragma pack(push,1)
typedef struct
{
	canbusstack_TipoEvento_e TipoEvento;
	uint8_t Data[6];
	uint8_t Chk;
}canbusstack_Event_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	struct
	{
		uint8_t DimData:4;
		uint8_t AddressOrg:4;
	};
	uint8_t Data[7];
}canbusstack_RawData_t;
#pragma pack(pop)


typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato

	bool GatewayMode;	// A true per abilitare funzionalità gateway

	uint32_t DimBufferRawData;	// Buffer di appoggio dati raw

	uint32_t CodiceUnivocoScheda;
}canbusstack_Init_t;





//_____________________________Strutture e var specifico per AssegnazioneAddress
//_____________________________Strutture e var specifico per AssegnazioneAddress
//_____________________________Strutture e var specifico per AssegnazioneAddress
typedef struct
{
	uint8_t AddressBoard;

	struct
	{
		uint32_t CodiceUnivocoScheda;	// Codice univoco scheda
		int32_t NumEventWakeOn;	// Se -1 significa che deve essere comunicato il nuovo address
		bool NotificaAddress;
	}InfoBoard[CANBUSSTACK_MAXNUMADDRESS];

	uint8_t IdCANPacketToSend;	// Id CAN di trasmissione pacchetto composto dalla Manager
	uint32_t AggiornamentiInfoBoard;	// Incrementato ad ogni aggiornamento della struttura InfoBoard

	canbusstack_RawData_t RawDataToSend[CANBUSSTACK_MAXNUMADDRESS];
	canbusstack_Event_t EventToSend;
}canbusstack_Info_t;


//============================================================= GLOBAL VARIABLES
extern canbusstack_Init_t canbusstack_Init;
extern canbusstack_Info_t canbusstack_Info;


//============================================================ GLOBAL PROTOTYPES
int32_t canbusstack_InitModule(void);
uint16_t canbusstack_Manager(void);
int8_t canbusstack_PushReceivedRawDataFromCAN(void *ptrStructData);
int8_t canbusstack_PushEventFromCAN(void *ptrStructData);
void canbusstack_EventManager(canbusstack_Event_t *ptrEvent);

int32_t canbusstack_RawDataGetChar(void);
int32_t canbusstack_RawDataSendData(uint8_t *ptrData,uint32_t dimData);
void canbusstack_RawDataFlushRX(void);
void canbusstack_RawDataSetAddressDest(uint8_t address);
uint8_t canbusstack_RawDataGetAddressOrg(void);
bool canbusstack_RawDataIsEmptyTX(void);




#endif /* CANBUSSTACK_H_INCLUDED */

