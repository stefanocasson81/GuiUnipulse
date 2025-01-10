/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
 	 Gestione comunicazione via CanBus.
 	 Questo modulo inizializza il can e mette a disposizione le primitive per la
 	 trasmissione e ricezione di pacchetti.
 	 Logica di trasmissione in base all'id ad 11bit H000...H7FF
 	 	 - H000...H0FF	High Priority di Tx e Rx
 	 	 - H100...H7FF  Low Priority di Tx e Rx
 
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Protocolli/CanBus.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES
static bool PopPacket(canbus_Obj_t *ptrObj,canbus_Packet_t *ptrPacket);

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
canbus_Init_t canbus_Init;
canbus_Info_t canbus_Info;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t canbus_InitModule(void)
{
	bool errore=false;

	if(errore)
	{
		canbus_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		canbus_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

//______________________________________________________________________________
// Desc:  Crea l'oggetto CANBUS
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
canbus_Obj_t* canbus_CreateObj(canbus_CfgInitObj_t *ptrCfgInitObj)
{
	canbus_Obj_t object;
	canbus_Obj_t *valRet = _NULL_;
	bool errore = false;

	if(canbus_Init.InitOk && ptrCfgInitObj)
	{
		if(ptrCfgInitObj->PtrHandleFDCan)
		{
			memset(&object,0,sizeof(canbus_Obj_t));
			object.PtrHandleFDCan = ptrCfgInitObj->PtrHandleFDCan;

			if(ptrCfgInitObj->NPacketBufferRXHP==0)	// Garantisco un minimo di buffer
				ptrCfgInitObj->NPacketBufferRXHP = 16;
			if(ptrCfgInitObj->NPacketBufferRXLP==0)	// Garantisco un minimo di buffer
				ptrCfgInitObj->NPacketBufferRXLP = 16;
			if(ptrCfgInitObj->NIdTXRemoteId==0)	// Garantisco un minimo di buffer
				ptrCfgInitObj->NIdTXRemoteId = 16;

			object.BufferPacketRXHP.PtrListPacket = _MALLOC_(sizeof(canbus_Packet_t)*ptrCfgInitObj->NPacketBufferRXHP);
			object.BufferPacketRXHP.NPacket = ptrCfgInitObj->NPacketBufferRXHP;

			object.BufferPacketRXLP.PtrListPacket = _MALLOC_(sizeof(canbus_Packet_t)*ptrCfgInitObj->NPacketBufferRXLP);
			object.BufferPacketRXLP.NPacket = ptrCfgInitObj->NPacketBufferRXLP;

			if(ptrCfgInitObj->NPacketBufferSwitch)
				object.BufferPacketSwitch.PtrListPacket = _MALLOC_(sizeof(canbus_Packet_t)*ptrCfgInitObj->NPacketBufferSwitch);
			object.BufferPacketSwitch.NPacket = ptrCfgInitObj->NPacketBufferSwitch;

			object.PtrStructData8RX = ptrCfgInitObj->PtrStructData8RX;
			object.NIdTXRemoteId = ptrCfgInitObj->NIdTXRemoteId;

			object.PtrBufferTXRemoteId = _MALLOC_(sizeof(uint16_t)*ptrCfgInitObj->NIdTXRemoteId);

			object.PtrFunSendCanPacket = ptrCfgInitObj->PtrFunSendCanPacket;
			object.PtrFunCanPushPacket = ptrCfgInitObj->PtrFunCanPushPacket;

			if( object.BufferPacketRXHP.PtrListPacket &&
				object.BufferPacketRXLP.PtrListPacket &&
				object.PtrFunSendCanPacket &&
				object.PtrFunCanPushPacket)
			{
				valRet = _MALLOC_(sizeof(canbus_Obj_t));
				if(valRet)
					memcpy(valRet,&object,sizeof(canbus_Obj_t));
				else
					errore = true;
			}
			else
				errore = true;
		}
	}

	if(errore)
		valRet = _NULL_;

	return valRet;
}


//______________________________________________________________________________
// Desc: Manager dell'obj passato, si occupata della valutazione di trasmissione
//       e/o ricezione
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void canbus_ObjManager(canbus_Obj_t *ptrObj)
{
	data8pack_Data_t *ptrData8PackData;
	canbus_Packet_t packet;
	data8pack_Data_t *ptrData8RX;
	uint16_t id;
	int8_t sCA;
	uint8_t indiceRd;

	if(ptrObj->PtrFunCanPushPacket(ptrObj->PtrHandleFDCan))
	{	// Se possibile trasmettere qualcosa in CAN, vede se c'è da spedire qualcosa
		ptrData8PackData = (data8pack_Data_t*)data8pack_PopData();
		if(ptrData8PackData)
		{
			packet.Id = ptrData8PackData->Id;
			sCA = -1;
			if(ptrData8PackData->PtrFunExeceute)
				sCA=ptrData8PackData->PtrFunExeceute(ptrData8PackData);

			if(sCA==-1)
			{
				packet.DimData = data8pack_ZipData(packet.Data,ptrData8PackData);
			}
			else
			{
				packet.DimData = (uint8_t)sCA;
				memcpy(packet.Data,ptrData8PackData->DataEsploso[0].PtrData,packet.DimData);
			}

			packet.IsRemoteFrame = false;

			if(ptrObj->PtrFunSendCanPacket(&packet)==_STATUS_OK_)
				ptrObj->TotNPacketTrasmessi++;
		}
		else
		{
			indiceRd = ptrObj->IndiceRdBufferTXRemoteId;

			if(indiceRd!=ptrObj->IndiceWrBufferTXRemoteId)
			{	// Qualcosa da trasmettere
				indiceRd++;
				if(indiceRd>=ptrObj->NIdTXRemoteId)
					indiceRd=0;
				packet.Id = ptrObj->PtrBufferTXRemoteId[indiceRd];
				packet.DimData = 0;
				packet.IsRemoteFrame = true;

				if(ptrObj->PtrFunSendCanPacket(&packet)==_STATUS_OK_)
					ptrObj->TotNPacketTrasmessi++;

				ptrObj->IndiceRdBufferTXRemoteId = indiceRd;
			}
		}
	}

	//sca ritorna true se ricevuto pacchettto
	if(PopPacket(ptrObj,&packet))
	{	// Se ricezione avvenuta la valuta
		if(packet.IsRemoteFrame)
			data8pack_ForceTX(packet.Id);
		else
		{
			ptrData8RX = (data8pack_Data_t *)ptrObj->PtrStructData8RX;
			id = packet.Id;
			while(ptrData8RX->Id)
			{
				if(id == ptrData8RX->Id)
				{ 	// Pacchetto riconosciuto
					data8pack_UnzipData(ptrData8RX, packet.Data);
					if(ptrData8RX->PtrFunExeceute)
					{
						ptrData8RX->PtrFunExeceute(ptrData8RX);
					}
					break;
				}

				ptrData8RX++;
			}
		}
	}

}

//______________________________________________________________________________
// Desc:  Esegue una richiesta di remote packet
// Arg: - id: id del packet da richiedeere in remote
// Ret: - void
//______________________________________________________________________________
int32_t canbus_ReqRemoteId(canbus_Obj_t *ptrObj,uint16_t id)
{
	int32_t valRet = _STATUS_FAIL_;
	uint8_t indiceWr = ptrObj->IndiceWrBufferTXRemoteId;

	indiceWr++;
	if(indiceWr>=ptrObj->NIdTXRemoteId)
		indiceWr = 0;

	if(indiceWr!=ptrObj->IndiceRdBufferTXRemoteId)
	{
		ptrObj->PtrBufferTXRemoteId[indiceWr]=id;
		ptrObj->IndiceWrBufferTXRemoteId = indiceWr;
		valRet = _STATUS_OK_;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Pusha un nuovo messaggio ricevuto via CAN
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void canbus_PushRxMessage(canbus_Obj_t *ptrObj,FDCAN_RxHeaderTypeDef *ptrRxHeader,uint8_t *ptrData,uint8_t tagCanDiProvenienza)
{
	uint8_t indiceWr;
	uint8_t indiceWrSwitch;
	canbus_Packet_t *ptrPacket;
	canbus_BufferPacket_t *ptrBufferPacket;

	if(ptrRxHeader->Identifier<0x100)	// High priority
		ptrBufferPacket = &ptrObj->BufferPacketRXHP;
	else
		ptrBufferPacket = &ptrObj->BufferPacketRXLP;

	indiceWr = ptrBufferPacket->IndiceWr;
	indiceWr++;
	if(indiceWr>=ptrBufferPacket->NPacket)
		indiceWr = 0;

	ptrPacket = &ptrBufferPacket->PtrListPacket[indiceWr];
	ptrPacket->Id = ptrRxHeader->Identifier;
	ptrPacket->TagCanDiProvenienza = tagCanDiProvenienza;

	switch(ptrRxHeader->DataLength)
	{
		default:
		case FDCAN_DLC_BYTES_0:
			ptrPacket->DimData = 0;
		break;
		case FDCAN_DLC_BYTES_1:
			ptrPacket->DimData = 1;
		break;
		case FDCAN_DLC_BYTES_2:
			ptrPacket->DimData = 2;
		break;
		case FDCAN_DLC_BYTES_3:
			ptrPacket->DimData = 3;
		break;
		case FDCAN_DLC_BYTES_4:
			ptrPacket->DimData = 4;
		break;
		case FDCAN_DLC_BYTES_5:
			ptrPacket->DimData = 5;
		break;
		case FDCAN_DLC_BYTES_6:
			ptrPacket->DimData = 6;
		break;
		case FDCAN_DLC_BYTES_7:
			ptrPacket->DimData = 7;
		break;
		case FDCAN_DLC_BYTES_8:
			ptrPacket->DimData = 8;
		break;
	}

	if(ptrPacket->DimData>CANBUS_MAXDIMDATA)	// Un minimo di protezione
		ptrPacket->DimData = CANBUS_MAXDIMDATA;

	if(ptrRxHeader->RxFrameType == FDCAN_REMOTE_FRAME)
		ptrPacket->IsRemoteFrame = true;
	else
		ptrPacket->IsRemoteFrame = false;

	memcpy(ptrPacket->Data,ptrData,ptrPacket->DimData);

	//__________________________________________________Gestione modalità switch
	if(ptrObj->BufferPacketSwitch.NPacket)
	{
		indiceWrSwitch = ptrObj->BufferPacketSwitch.IndiceWr;
		indiceWrSwitch++;
		if(indiceWrSwitch>=ptrObj->BufferPacketSwitch.NPacket)
			indiceWrSwitch = 0;

		memcpy(&ptrObj->BufferPacketSwitch.PtrListPacket[indiceWrSwitch],ptrPacket,sizeof(canbus_Packet_t));

		ptrObj->BufferPacketSwitch.IndiceWr = indiceWrSwitch;
	}

	ptrBufferPacket->IndiceWr = indiceWr;	// Tenere come ultima riga

	ptrObj->TotNPacketRicevuti++;
}


//______________________________________________________________________________
// Desc:  Ritorna evetuale packet da switchare sugli altri CAN
// Arg: - prObj: l'obj di gestione canbus
// Ret: - _NULL_ se nulla da switchare, altrimenti il packet da trattare
//______________________________________________________________________________
canbus_Packet_t* canbus_SwitchPop(canbus_Obj_t *ptrObj)
{
	canbus_Packet_t *valRet = _NULL_;
	uint8_t indiceRd;

	if(ptrObj->BufferPacketSwitch.NPacket)
	{
		if(ptrObj->PtrFunCanPushPacket(ptrObj->PtrHandleFDCan))
		{
			indiceRd = ptrObj->BufferPacketSwitch.IndiceRd;
			if(ptrObj->BufferPacketSwitch.IndiceWr!=indiceRd)
			{
				indiceRd++;
				if(indiceRd>=ptrObj->BufferPacketSwitch.NPacket)
					indiceRd=0;

				valRet = &ptrObj->BufferPacketSwitch.PtrListPacket[indiceRd];

				ptrObj->BufferPacketSwitch.IndiceRd = indiceRd;
			}
		}
	}

	return valRet;
}


//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicit� dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void canbus_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS

//______________________________________________________________________________
// Desc:  Ritorna true se packet ricevuto
// Arg: - ptrPacket: storage del packet
// Ret: - true se packet ricevuto
//______________________________________________________________________________
static bool PopPacket(canbus_Obj_t *ptrObj,canbus_Packet_t *ptrPacket)
{
	bool valRet = false;
//#if defined(HAL_FDCAN_MODULE_ENABLED)
	canbus_BufferPacket_t *ptrBufferPacket;
	uint8_t indiceRd;
	uint8_t giroBuffer=2;	// Il primo giro cerca sul buffer alta prioritò, casomai passa al meno prioritario

	if(ptrObj && ptrPacket)
	{
		// Prioritario il buffer in High Priority, poi il Low Priority
		ptrBufferPacket = &ptrObj->BufferPacketRXHP;
		while(giroBuffer)
		{
			indiceRd = ptrBufferPacket->IndiceRd;
			if(ptrBufferPacket->IndiceWr!=indiceRd)
			{
				indiceRd++;
				if(indiceRd>=ptrBufferPacket->NPacket)
					indiceRd=0;

				memcpy(ptrPacket,&ptrBufferPacket->PtrListPacket[indiceRd],sizeof(canbus_Packet_t));
				ptrBufferPacket->IndiceRd = indiceRd;

				valRet = true;
				break;
			}

			ptrBufferPacket = &ptrObj->BufferPacketRXLP;
			giroBuffer--;
		}
	}
//#endif

	return valRet;
}
