#ifndef CANBUS_H_INCLUDED
#define CANBUS_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Common/Data8Pack.h"


//====================================================================== DEFINES
#define CANBUS_MAXOBJ			2		// Attualmente supporatati due canali can, rivedere codice se aumenta

#define CANBUS_MAXDIMDATA		8		// Dimensione massima del pacchetto ma anche numero massimi di registri per ogni id CAN (8 per uint8_t,......2 per uint32_t...ecc)

typedef enum
{
	CANBUS_STATOCONNESSIONE_VOID,			// Stato iniziale
	CANBUS_STATOCONNESSIONE_CONNESSO,
	CANBUS_STATOCONNESSIONE_DISCONNESSO	// Avviene quando � gi� passato per CONESSO
}canbus_StatoConnessione_t;


#pragma pack(1)
typedef struct
{
	uint16_t Id;
	uint8_t Data[CANBUS_MAXDIMDATA];
	uint8_t DimData;
	bool IsRemoteFrame;
	uint8_t TagCanDiProvenienza;	// Sarebbe l0enum del can da zero utile per switch
}canbus_Packet_t;
#pragma pack()

typedef struct
{
	canbus_Packet_t *PtrListPacket;	//  Buffer pacchetti mallocali
	uint8_t IndiceWr;
	uint8_t IndiceRd;
	uint8_t NPacket;		// Numero pacchetti buffer
}canbus_BufferPacket_t;

typedef struct canbus_Obj_tt
{
	canbus_BufferPacket_t BufferPacketRXHP;		// Buffer packet in ricezione High Priority
	canbus_BufferPacket_t BufferPacketRXLP;		// Buffer packet in ricezione High Priority
	canbus_BufferPacket_t BufferPacketSwitch;	// Buffer packet in modalità switch (da girare su altri can)

	uint16_t *PtrBufferTXRemoteId;
	uint8_t IndiceWrBufferTXRemoteId;
	uint8_t IndiceRdBufferTXRemoteId;
	uint8_t NIdTXRemoteId;;

	#if defined(HAL_FDCAN_MODULE_ENABLED)
		FDCAN_HandleTypeDef* PtrHandleFDCan;
		FDCAN_RxHeaderTypeDef RxHeader;
		//FDCAN_TxHeaderTypeDef TxHeader;
		int32_t (*PtrFunSendCanPacket)(canbus_Packet_t *ptrPacket);	// Ptrfun alla fun spedizione fisica packet via CAN
		bool (*PtrFunCanPushPacket)(FDCAN_HandleTypeDef *hfdcan);	// Notifica tru se possibile trasmettere un packet
	#endif

	uint32_t TotNPacketTrasmessi;
	uint32_t TotNPacketRicevuti;

	const data8pack_Data_t *PtrStructData8RX;	// Ptr alla struttura 8pack di ricezione dal can
}canbus_Obj_t;

typedef struct
{
	uint8_t NPacketBufferRXHP;		// Numero pacchetti buffer RX in High Priority
	uint8_t NPacketBufferRXLP;		// Numero pacchetti buffer RX in Low Priority
	uint8_t NPacketBufferSwitch;	// Numero pacchetti buffer per funzionalità switch
	uint8_t NIdTXRemoteId;			// Numero richieste remote id

	const data8pack_Data_t *PtrStructData8RX;

	#if defined(HAL_FDCAN_MODULE_ENABLED)
		FDCAN_HandleTypeDef* PtrHandleFDCan;
		int32_t (*PtrFunSendCanPacket)(canbus_Packet_t *ptrPacket);	// Ptrfun alla fun spedizione fisica packet via CAN
		bool (*PtrFunCanPushPacket)(FDCAN_HandleTypeDef *hfdcan);	// Notifica tru se possibile trasmettere un packet
	#endif
}canbus_CfgInitObj_t;


typedef struct
{
	uint8_t Dummy;
}canbus_Info_t;

typedef struct
{
	bool InitOk;		// a true sse modulo correttamente inizializzato
}canbus_Init_t;



//============================================================= GLOBAL VARIABLES
extern canbus_Init_t canbus_Init;
extern canbus_Info_t canbus_Info;

//============================================================ GLOBAL PROTOTYPES
int32_t canbus_InitModule(void);
canbus_Obj_t* canbus_CreateObj(canbus_CfgInitObj_t *ptrCfgInitObj);
void canbus_ObjManager(canbus_Obj_t *ptrObj);
int32_t canbus_ReqRemoteId(canbus_Obj_t *ptrObj,uint16_t id);
void canbus_PushRxMessage(canbus_Obj_t *ptrObj,FDCAN_RxHeaderTypeDef *ptrRxHeader,uint8_t *ptrData,uint8_t tagCanDiProvenienza);
canbus_Packet_t* canbus_SwitchPop(canbus_Obj_t *ptrObj);



#endif /* CANBUS__H_INCLUDED */

