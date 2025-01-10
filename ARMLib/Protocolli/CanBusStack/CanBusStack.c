/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	19/09/23
 Descrizione:
 	 Gestore stack incapsulato via CANBus. Riferirisi al BusStack.doc per dettagli
 	 del protocollo.

 	 E' necessario l'SwTimer operativo e inizializzato.

 	 Il modulo usa un buffer locale per i pacchetti dedicati per permetterne la
 	 valutazione fuori irq tramite bustack_Manager(). La dimensione è passata da Init.
 	 Per il dimensionamento valutare la cadenza massima di arrivo via Irq e ogni
 	 quanto viene richiamanata la Manager. Es. per Can a 500KHz un pacchetto arriva
 	 ogni 500uSec, se il Manager viene richiamato ogni 5ms( preferibile il più veloce
 	 possibile in realtà...)il buffer al limite deve essere di 10 pacchetti, meglio il doppio secco...
 	 Si tenga presente che ogni pacchetto occupa 8 byte. In ogni caso in canbusstack_Info.NPacchettiRxPersi
 	 è possibile capire se il buffer è troppo limitato

 USO e ABUSO:
 	 - Inizializzazione modulo
 	 	 canbusstack_InitModule()

 	 - Looper
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Protocolli/CanBusStack/CanBusStack.h"
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/Common/SwTimer.h"
#include "ARMLib/Common/Data8Pack.h"
#include "String.h"
#include "stdlib.h"

//====================================================================== DEFINES
#define CANBUSSTACK_MSRITARDOPRESENTAZIONE 5000

#define APPLICATION_DIMBUFFERHPROTCAN 5000	// Dimensione del buffer di appoggio HProt sul canale CAN
typedef struct
{
	uint32_t IndiceWrTx;
	uint32_t IndiceRdTx;
	uint32_t IndiceWrRx;
	uint32_t IndiceRdRx;

	uint8_t *PtrBufferTx;
	uint8_t *PtrBufferRx;

	uint8_t AddressDiProvenienza;
	uint8_t AddressDiDestinazione;
}canbusstack_LocalRawData_t;

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

static swtimer_TimerHandler_t HTimerWakeOn;
static canbusstack_LocalRawData_t LocalRawData;


//============================================================= GLOBAL VARIABLES
canbusstack_Init_t canbusstack_Init=
{
	.GatewayMode			= false		,
	.CodiceUnivocoScheda	= 0			,
	.DimBufferRawData		= 100
};
canbusstack_Info_t canbusstack_Info;

//============================================================= GLOBAL FUNCTIONS




/*______________________________________________________________________________ 
 Desc:  Init del modulo.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t canbusstack_InitModule(void)
{
	uint32_t errori=0;

	// SwTimer necessario
	if(swtimer_Init.InitOk)
	{
		HTimerWakeOn = swtimer_Create();
		if(HTimerWakeOn<0)
			errori++;
	}
	else
		errori++;

	if(!data8pack_Init.InitOk)
		errori++;

	// Necessario codice univoco scheda
	if(!canbusstack_Init.CodiceUnivocoScheda)
		errori++;

	// Necessario dimensionare buffer pacchetti in ricezione
	memset(&LocalRawData,0,sizeof(LocalRawData));
	if(canbusstack_Init.DimBufferRawData)
	{
		LocalRawData.PtrBufferRx = _MALLOC_(canbusstack_Init.DimBufferRawData);
		LocalRawData.PtrBufferTx = _MALLOC_(canbusstack_Init.DimBufferRawData);

		if((LocalRawData.PtrBufferRx==_NULL_) || (LocalRawData.PtrBufferRx==_NULL_))
		{
			_FREE_(LocalRawData.PtrBufferRx);
			_FREE_(LocalRawData.PtrBufferTx);
			errori++;
		}
	}
	else
		errori++;

	memset(canbusstack_Info.InfoBoard,0,sizeof(canbusstack_Info.InfoBoard));
	if(canbusstack_Init.GatewayMode)
		canbusstack_Info.InfoBoard[1].CodiceUnivocoScheda = canbusstack_Init.CodiceUnivocoScheda;

	canbusstack_Info.AddressBoard = 0;

	if(errori)
	{
		canbusstack_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		canbusstack_Init.InitOk = true;
		return _STATUS_OK_;
	}
}





typedef enum
{
	CANBUSSTACK_ASSEGNAZIONEADDRESS_SL_VOID,

	CANBUSSTACK_ASSEGNAZIONEADDRESS_SL_SENDGATEWAYADDRESS,
	CANBUSSTACK_ASSEGNAZIONEADDRESS_SL_SENDNOTIFICACRC32SWVER,
	CANBUSSTACK_ASSEGNAZIONEADDRESS_SL_SENDNOTIFICADATASW

}canbusstack_AssegnazioneAddress_SL_t;


/*______________________________________________________________________________
 Desc:  Manager del modulo da eseguire anche con cadenza non stabile ma decentemente
  	  	  veloce. Gestisce la definizione degli address delle periferiche in CANBUS
  	  	  e la tasmissione del RawData.
 Arg: - Nessun argomento.
 Ret: - id CAN del pacchetto da spedire. Sono distinti per id:
 	 	 CANBUSSTACK_IDCAN_BUSSTACKBASE...CANBUSSTACK_IDCAN_BUSSTACKENDZONE leggere su canbusstack_Info.RawDataToSend
 	 	 CANBUSSTACK_IDCAN_BUSSTACKEVENT leggere sucanbusstack_Info.EventToSend
______________________________________________________________________________*/
uint16_t canbusstack_Manager(void)
{
	uint16_t valRet = 0;
	uint8_t *ptrData;
	uint32_t uLA;
	uint32_t dimData;
	uint32_t indiceRd;

	if(canbusstack_Init.InitOk)
	{
		if(canbusstack_Info.AddressBoard )
		{
			indiceRd = LocalRawData.IndiceRdTx;
			// Se RawData da spedire
			if(indiceRd!=LocalRawData.IndiceWrTx)
			{
				if(LocalRawData.AddressDiDestinazione<CANBUSSTACK_MAXNUMADDRESS)	// Un minimo di controllo
				{
					// Se è effettivamente possibile trasmettere su questo id...
					if(data8pack_IsIdImmediateTXFreeToSend(CANBUSSTACK_IDCAN_BUSSTACKBASE+LocalRawData.AddressDiDestinazione))
					{
						dimData = 0;
						canbusstack_Info.RawDataToSend[LocalRawData.AddressDiDestinazione].AddressOrg = canbusstack_Info.AddressBoard;
						while(indiceRd!=LocalRawData.IndiceWrTx)
						{
							indiceRd++;
							if(indiceRd>=canbusstack_Init.DimBufferRawData)
								indiceRd=0;

							canbusstack_Info.RawDataToSend[LocalRawData.AddressDiDestinazione].Data[dimData] = LocalRawData.PtrBufferTx[indiceRd];

							dimData++;
							if(dimData>=7)
							{	// Massimo 7 byte per volta
								break;
							}
						}

						canbusstack_Info.RawDataToSend[LocalRawData.AddressDiDestinazione].DimData = dimData;
						valRet = CANBUSSTACK_IDCAN_BUSSTACKBASE+LocalRawData.AddressDiDestinazione;
						LocalRawData.IndiceRdTx = indiceRd;
					}
				}
			}
		}
		else
		{	// Se da comunicare la presentazione del gateway
			if((!swtimer_Test(HTimerWakeOn)) && data8pack_IsIdImmediateTXFreeToSend(CANBUSSTACK_IDCAN_BUSSTACKEVENT))
			{
				memset(&canbusstack_Info.EventToSend,0,sizeof(canbusstack_Info.EventToSend));

				if(canbusstack_Init.GatewayMode)
				{
					// Nel caso del gateway si presenta all'accensione e poi eventualmente ogni volta che si collega un NUOVO slave
					canbusstack_Info.EventToSend.TipoEvento = CANBUSSTACK_TIPOEVENTO_GATEWAYWAKEON;
					canbusstack_Info.AddressBoard = 1;
				}
				else
				{
					// nel caso di slave si presenta ogni CANBUSSTACK_MSRITARDOPRESENTAZIONE finchè non gli viene assegnato un address
					canbusstack_Info.EventToSend.TipoEvento = CANBUSSTACK_TIPOEVENTO_SLAVEWAKEON;
					swtimer_Set(HTimerWakeOn,CANBUSSTACK_MSRITARDOPRESENTAZIONE);
				}

				memcpy(canbusstack_Info.EventToSend.Data,&canbusstack_Init.CodiceUnivocoScheda,4);

				valRet = CANBUSSTACK_IDCAN_BUSSTACKEVENT;
			}
		}

		// Se non devo trasmettere qualcosa valuto se devo spedire settaggi di address
		if(canbusstack_Init.GatewayMode)
		{
			if((!valRet) && data8pack_IsIdImmediateTXFreeToSend(CANBUSSTACK_IDCAN_BUSSTACKEVENT))
			{
				for(uLA=1;uLA<CANBUSSTACK_MAXNUMADDRESS;uLA++)	// Il gateway (1) prende una strada diversa
				{
					if(canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda)
					{
						if(canbusstack_Info.InfoBoard[uLA].NotificaAddress)
						{
							canbusstack_Info.InfoBoard[uLA].NotificaAddress = false;

							memset(&canbusstack_Info.EventToSend,0,sizeof(canbusstack_Info.EventToSend));

							canbusstack_Info.EventToSend.TipoEvento = CANBUSSTACK_TIPOEVENTO_GATEWAYTOSLAVESETADDRESS;
							memcpy(&canbusstack_Info.EventToSend.Data[0],&canbusstack_Info.InfoBoard[uLA],4);	// Occhio qui devo mandare il CUS dello slave di destinazione
							canbusstack_Info.EventToSend.Data[4] = uLA;
							valRet = CANBUSSTACK_IDCAN_BUSSTACKEVENT;
							break;
						}
					}
					else
						break;	// fine della lista raggiunto
				}
			}
		}

		if(valRet == CANBUSSTACK_IDCAN_BUSSTACKEVENT)
		{	// Se devo trasmettere un pacchetto calcolo la checksum
			// Calcolo la checksum
			ptrData = (uint8_t*)&canbusstack_Info.EventToSend;
			for(uLA=0;uLA<7;uLA++)
			{
				ptrData[7]^=ptrData[uLA];
			}
		}
	}

	if(valRet)
		data8pack_ForceImmediateTX(valRet);

	return valRet;
}


//______________________________________________________________________________
// Desc: Pushata del packet ricevuto dal bus nel range id per il rawdata.
// Arg: - ptrStructData: E' il riferimento al data8pack_Data_t del can
// Ret: - void
//______________________________________________________________________________
int8_t canbusstack_PushReceivedRawDataFromCAN(void *ptrStructData)
{
	uint32_t uLA;
	uint32_t realDim;
	data8pack_Data_t *ptrData8Pack = (data8pack_Data_t*)ptrStructData;
	canbusstack_RawData_t *ptrRawData;
	uint8_t indice;

	ptrRawData = (canbusstack_RawData_t*)ptrData8Pack->DataEsploso->PtrData;

	if(canbusstack_Info.AddressBoard)
	{
		if(ptrData8Pack->Id==(CANBUSSTACK_IDCAN_BUSSTACKBASE+canbusstack_Info.AddressBoard))
		{
			// Il primo byte è l'origine e dimensione
			LocalRawData.AddressDiProvenienza = ptrRawData->AddressOrg;
			realDim = ptrRawData->DimData;
			uLA = LocalRawData.IndiceWrRx;
			indice = 0;
			while(realDim--)
			{
				uLA++;
				if(uLA>=canbusstack_Init.DimBufferRawData)
					uLA=0;

				LocalRawData.PtrBufferRx[uLA] = ptrRawData->Data[indice++];
			}
			LocalRawData.IndiceWrRx = uLA;
		}
	}

	return 0;
}


/*______________________________________________________________________________
 Desc:  Pushata di un evento arrivato nell'id CANBUSSTACK_IDCAN_BUSSTACKEVENT del CAN.
 	 	La valutazione è immediata e quindi non serve bufferizzazione
 Arg: - ptrData: Dati arrivati dal CAN
 	  - dimData: Dimensione del Data in arrivo
 	  - id: id del CAN arrivato messo più che altre per compatiblità con il passato
 Ret: - void
______________________________________________________________________________*/
int8_t canbusstack_PushEventFromCAN(void *ptrStructData)
{
	uint8_t chk;
	uint32_t indice;
	uint32_t uLA;
	uint32_t codiceUnivocoScheda;
	data8pack_Data_t *ptrData8Pack = (data8pack_Data_t*)ptrStructData;
	canbusstack_Event_t *ptrEvent;

	ptrEvent = (canbusstack_Event_t*)ptrData8Pack->DataEsploso->PtrData;

	if(ptrData8Pack->DataEsploso[0].Size==8)
	{
		// Un minimo di controlllo sulla checksum per filtrare pacchetti che non centrano
		chk=ptrEvent->TipoEvento;
		for(indice=0;indice<sizeof(ptrEvent->Data) ;indice++)
			chk ^= ptrEvent->Data[indice];

		if(chk == ptrEvent->Chk)
		{	// test di sicurezza passato, procedo con la valutazione
			switch(ptrEvent->TipoEvento)
			{
				case CANBUSSTACK_TIPOEVENTO_GATEWAYWAKEON:
					if(canbusstack_Init.GatewayMode)
					{	// Per il gateway non dovrebbe mai arrivare perchè significherebbe che c'è un altro gateway in linea e sarebbero cazzi..

					}
					else
					{	// Per gli slave significa resetta la tabella address
						memset(canbusstack_Info.InfoBoard,0,sizeof(canbusstack_Info.InfoBoard));

						memcpy(&codiceUnivocoScheda,ptrEvent->Data,4);
						canbusstack_Info.InfoBoard[1].CodiceUnivocoScheda = codiceUnivocoScheda;	// Segno il gateway intanto

						canbusstack_Info.AddressBoard = 0;	// Cosi riparte la presentazione di questo slave
						canbusstack_Info.AggiornamentiInfoBoard++;
					}
				break;
				case CANBUSSTACK_TIPOEVENTO_SLAVEWAKEON:
					memcpy(&codiceUnivocoScheda,ptrEvent->Data,4);

					if(canbusstack_Init.GatewayMode)
					{	// Per il gateway significa comunicare l'address del nuovo arrivato
						for(uLA=2;uLA<CANBUSSTACK_MAXNUMADDRESS;uLA++)
						{
							if(canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda)
							{
								if(canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda==codiceUnivocoScheda)
								{	// E' già mappato, tengo lo stesso address
									canbusstack_Info.InfoBoard[uLA].NumEventWakeOn++;
									canbusstack_Info.AggiornamentiInfoBoard++;
									break;
								}
							}
							else
							{	// Trovato un buco libero,lo piazzo qui
								canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda = codiceUnivocoScheda;
								canbusstack_Info.InfoBoard[uLA].NumEventWakeOn++;
								canbusstack_Info.AggiornamentiInfoBoard++;

								break;
							}
						}

						// Rinotifico a tutti gli slave gli indirizzi così ogniuno si ricrea la tabella interna degli indirizzi
						for(uLA=1;uLA<CANBUSSTACK_MAXNUMADDRESS;uLA++)	// Qui aggiungo anche il gateway
						{
							if(canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda)
								canbusstack_Info.InfoBoard[uLA].NotificaAddress = true;
							else
								break;	// Arivato in fondo alla lista
						}
					}
					else
					{	// Per lo slave interessa nulla

					}
				break;
				case CANBUSSTACK_TIPOEVENTO_GATEWAYTOSLAVESETADDRESS:
					memcpy(&codiceUnivocoScheda,ptrEvent->Data,4);
					if(canbusstack_Init.GatewayMode)
					{	// Per il gateway non dovrebbe mai arrivare perchè significherebbe che c'è un altro gateway in linea..
					}
					else
					{	// Per lo slave interessato si asssegna l'address, per gli altri se lo mappano
						uLA = ptrEvent->Data[4];	// Recupero l'address
						if(uLA>=CANBUSSTACK_MAXNUMADDRESS)
						{	// Address non valido
							uLA = 0;
						}
						else
						{
							if(canbusstack_Init.CodiceUnivocoScheda == codiceUnivocoScheda)
								canbusstack_Info.AddressBoard = (uint8_t)uLA;
						}

						canbusstack_Info.InfoBoard[uLA].CodiceUnivocoScheda = codiceUnivocoScheda;
						canbusstack_Info.AggiornamentiInfoBoard++;
					}
				break;

				case CANBUSSTACK_TIPOEVENTO_GATEWAYREFRESH:	// E' solo un refresh del gateway
					memcpy(&codiceUnivocoScheda,ptrEvent->Data,4);
					if(canbusstack_Init.GatewayMode)
					{	// Per il gateway non dovrebbe mai arrivare perchè significherebbe che c'è un altro gateway in linea..
					}
					else
					{	// Per lo slave interessato si asssegna l'address, per gli altri se lo mappano
						canbusstack_Info.InfoBoard[1].CodiceUnivocoScheda = codiceUnivocoScheda;
					}

				break;
			}

		}
	}

	return 0;
}

//______________________________________________________________________________
// Desc:  Ritorna un byte dal buffer di appoggio rawdata in simil seriale
// Arg: - void
// Ret: - -1 se nulla ricevuto, il byte altrimenti
//______________________________________________________________________________
int32_t canbusstack_RawDataGetChar(void)
{
	uint32_t uLA;
	int32_t valRet = -1;

	//NOTA: Lasciare il giro di copia su uLA per non creare semaforica
	uLA = LocalRawData.IndiceRdRx;
	if(uLA != LocalRawData.IndiceWrRx)
	{
		uLA++;
		if(uLA>=canbusstack_Init.DimBufferRawData)
			uLA=0;

		valRet = LocalRawData.PtrBufferRx[uLA];

		LocalRawData.IndiceRdRx = uLA;
	}

	return valRet;
}
//______________________________________________________________________________
// Desc:  Spedisce un buffer sul buffer di appoggio rawdata in simil seriale
// Arg: - ptrData: ptr al buffer da spedire
//		- dimData: dimensione dei dati da spedire
// Ret: - _STATUS_OK_ sempre
//______________________________________________________________________________
int32_t canbusstack_RawDataSendData(uint8_t *ptrData,uint32_t dimData)
{
	uint32_t uLA = LocalRawData.IndiceWrTx;
	while(dimData--)
	{
		uLA++;
		if(uLA>=canbusstack_Init.DimBufferRawData)
			uLA=0;


		while(uLA==LocalRawData.IndiceRdTx);
		/*
		// Sgancia tempo se non è possibile cacciare fuori dati. Il CAN potrebbe essere in ritardo....
		while(uLA==LocalRawData.IndiceRdTx)
			osDelay(1);
*/
		LocalRawData.PtrBufferTx[uLA] = *ptrData;
		ptrData++;

		LocalRawData.IndiceWrTx = uLA;
	}

	return _STATUS_OK_;
}
//______________________________________________________________________________
// Desc:  Flusha il buffer di ricezione
// Arg: - void
// Ret: - void
//______________________________________________________________________________
void canbusstack_RawDataFlushRX(void)
{
	if(LocalRawData.IndiceRdRx != LocalRawData.IndiceWrRx)
		LocalRawData.IndiceRdRx = LocalRawData.IndiceWrRx;
}
//______________________________________________________________________________
// Desc:  Ritorna true sse buffer di trasmissione vuoto (tasmesso tutto il trasmissibile)
// Arg: - void
// Ret: - true sse buffer Tx vuoto
//______________________________________________________________________________
bool canbusstack_RawDataIsEmptyTX(void)
{
	if(LocalRawData.IndiceWrTx==LocalRawData.IndiceRdTx)
		return true;
	else
		return false;
}

//______________________________________________________________________________
// Desc:  Desfinisce l'address di destinazione del rawdata
// Arg: - addreess: 0..15
// Ret: - void
//______________________________________________________________________________
void canbusstack_RawDataSetAddressDest(uint8_t address)
{
	if(address<CANBUSSTACK_MAXNUMADDRESS)
		LocalRawData.AddressDiDestinazione = address;
}


//______________________________________________________________________________
// Desc:  Ritorn al'addre di provenienze del rawdata
// Arg: - void
// Ret: - addreess: 0..15
//______________________________________________________________________________
uint8_t canbusstack_RawDataGetAddressOrg(void)
{
	return LocalRawData.AddressDiProvenienza;
}

//============================================================ PRIVATE FUNCTIONS



