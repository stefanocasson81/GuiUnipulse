/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Gestore gui custom style. Gestisce la lettura encoder e tasti.
	Notifica i vari eventi delle pressioni tasti ed encoder.
	Il modulo necessita del SWTimer inizializzato prima dell'init.
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/TGFX/TGFXCustom.h"
#include "ARMLib/Common/SWTimer.h"
#include "String.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES
static int16_t OldCounter[TGFXCUSTOM_ENCODER_MAX];
static uint16_t MsDecounterRefresh=0;


//============================================================= GLOBAL VARIABLES
tgfxcustom_Init_t tgfxcustom_Init;

tgfxcustom_EncoderStatus_t tgfxcustom_EncoderStatus[TGFXCUSTOM_ENCODER_MAX];

//============================================================= GLOBAL FUNCTIONS

/*______________________________________________________________________________ 
 Desc:  Init del modulo.
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t tgfxcustom_InitModule(void)
{
	bool errore = false;
	uint8_t numEncoder;
	bool premuto;

	if(tgfxcustom_Init.PtrFunBeep && tgfxcustom_Init.PtrFunGetStatusEncoder)
	{
		memset(tgfxcustom_EncoderStatus,0,sizeof(tgfxcustom_EncoderStatus));
		memset(OldCounter,0,sizeof(OldCounter));

		for(numEncoder=0;numEncoder<TGFXCUSTOM_ENCODER_MAX;numEncoder++)
			tgfxcustom_Init.PtrFunGetStatusEncoder(numEncoder,&premuto,&OldCounter[numEncoder]);
	}
	else
	   errore = true;

	if(!swtimer_Init.InitOk) // Necessaio SwTimer inizializzato...
		errore = true;

	if(!errore)
	{
		tgfxcustom_Init.InitOk = true;
		return _STATUS_OK_;
	}
	else
		return _STATUS_FAIL_;
}


//______________________________________________________________________________
// Desc:  Reset eventi degli encoder
// Arg: - numEncoder: numero encoder, sse TGFXCUSTOM_ENCODER_MAX restta tutti gli encoders
// Ret: - void
//______________________________________________________________________________
void tgfxcustom_ResetEvent(tgfxcustom_Encoder_e numEncoder)
{
	switch(numEncoder)
	{
		case TGFXCUSTOM_ENCODER_LEFT:
		case TGFXCUSTOM_ENCODER_CENTER:
		case TGFXCUSTOM_ENCODER_RIGHT:
			tgfxcustom_EncoderStatus[numEncoder].OffsetEncoder = 0;
			tgfxcustom_EncoderStatus[numEncoder].MsTempoPressionePrecedente = 0;
		break;

		case TGFXCUSTOM_ENCODER_MAX:
			for(numEncoder=0;numEncoder<TGFXCUSTOM_ENCODER_MAX;numEncoder++)
			{
				tgfxcustom_EncoderStatus[numEncoder].OffsetEncoder = 0;
				tgfxcustom_EncoderStatus[numEncoder].MsTempoPressionePrecedente = 0;
			}
		break;
	}
}


//______________________________________________________________________________
// Desc:  Manager da eseguire ogni 1ms
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void tgfxcustom_Manager1MS(void)
{
	uint8_t numEncoder;
	uint32_t actualMsActivity = swtimer_TickCounter;
	bool premuto;
	int16_t counter;
	int32_t lA;

	if(tgfxcustom_Init.InitOk)
	{
		if(MsDecounterRefresh)
			MsDecounterRefresh--;

		for(numEncoder=0;numEncoder<TGFXCUSTOM_ENCODER_MAX;numEncoder++)
		{
			tgfxcustom_Init.PtrFunGetStatusEncoder(numEncoder,&premuto,&counter);

			if(premuto)
			{
				if(!tgfxcustom_EncoderStatus[numEncoder].Premuto)
				{	// Fronte di salita
					tgfxcustom_Init.PtrFunBeep(30);
					tgfxcustom_EncoderStatus[numEncoder].MsAttivityStartPress = actualMsActivity;
				}

				tgfxcustom_EncoderStatus[numEncoder].Premuto = true;
				tgfxcustom_EncoderStatus[numEncoder].MsTempoPressione = actualMsActivity-tgfxcustom_EncoderStatus[numEncoder].MsAttivityStartPress;

				if(tgfxcustom_EncoderStatus[numEncoder].MsTempoPressione>=30)
					tgfxcustom_EncoderStatus[numEncoder].PressioneConfermata = true;
			}
			else
			{
				if(tgfxcustom_EncoderStatus[numEncoder].Premuto)
				{	// Fronte di discesa
					tgfxcustom_EncoderStatus[numEncoder].MsTempoPressionePrecedente = tgfxcustom_EncoderStatus[numEncoder].MsTempoPressione;
					tgfxcustom_EncoderStatus[numEncoder].MsTempoPressione = 0;
				}

				tgfxcustom_EncoderStatus[numEncoder].Premuto = false;
				tgfxcustom_EncoderStatus[numEncoder].PressioneConfermata = false;
			}

			if(counter!=OldCounter[numEncoder])
			{
				lA = counter;
				lA -= OldCounter[numEncoder];

				tgfxcustom_EncoderStatus[numEncoder].OffsetEncoder += lA;
				OldCounter[numEncoder] = counter;
			}
		}
	}
}

//______________________________________________________________________________
// Desc:  Ritorna true sse tempo refresh video passato, utile per evitare super
//        refresh video inutili
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
bool tgfxcustom_GetIfRefresh(uint16_t msRefresh)
{
	if(!msRefresh)
		MsDecounterRefresh = 0;

	if(MsDecounterRefresh)
		return false;
	else
	{
		MsDecounterRefresh = msRefresh;
		return true;
	}
}

//______________________________________________________________________________
// Desc:  Ritorna l'avanzamento encoder rispetto l'ultima volta
// Arg: - encoder: numero dell'encoder
// Ret: - il valore dell'offset rispetto all'ultima chiamata
//______________________________________________________________________________
int16_t tgfxcustom_GetOffsetEncoder(tgfxcustom_Encoder_e encoder)
{
	int16_t valRet = tgfxcustom_EncoderStatus[encoder].OffsetEncoder;

	tgfxcustom_EncoderStatus[encoder].OffsetEncoder = 0;

	return valRet;
}

//______________________________________________________________________________
// Desc:  Ritorna il numero di pressioni dell'encodere rispetto l'ultima volta
// Arg: - encoder: numero dell'encoder
// Ret: - il valore dell'offset rispetto all'ultima chiamata
//______________________________________________________________________________
tgfxcustom_PressioneEncoder_e tgfxcustom_GetPressioneEncoder(tgfxcustom_Encoder_e encoder)
{
	tgfxcustom_PressioneEncoder_e valRet = TGFXCUSTOM_PRESSIONEENCODER_NONPREMUTO;

	if(tgfxcustom_EncoderStatus[encoder].Premuto)
	{
		if(tgfxcustom_EncoderStatus[encoder].MsTempoPressione>3000)
			valRet = TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGHISSIMO;
		else if(tgfxcustom_EncoderStatus[encoder].MsTempoPressione>1500)
			valRet = TGFXCUSTOM_PRESSIONEENCODER_PREMUTOLUNGO;
		else
			valRet = TGFXCUSTOM_PRESSIONEENCODER_PREMUTO;
	}
	else
	{
		if(tgfxcustom_EncoderStatus[encoder].MsTempoPressionePrecedente && (tgfxcustom_EncoderStatus[encoder].MsTempoPressionePrecedente<800))
		{
			tgfxcustom_EncoderStatus[encoder].MsTempoPressionePrecedente = 0;
			valRet = TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO;
		}
	}

	return valRet;
}


#pragma pack(push,1)
	typedef struct {
		uint16_t Type;              // Magic identifier: 0x4d42 "BM"
		uint32_t Size;              // File size in bytes
		uint16_t Reserved1;         // Not used
		uint16_t Reserved2;         // Not used
		uint32_t Offset;            // Offset to image data in bytes from beginning of file
		uint32_t DibHeaderSize;   	// DIB Header size in bytes
		int32_t  WidthPx;          	// Width of the image
		int32_t  HeightPx;         	// Height of image
		uint16_t NumPlanes;        	// Number of color planes
		uint16_t BitsPerPixel;    	// Bits per pixel
		uint32_t Compression;       // Compression type
		uint32_t ImageSizeBytes;  	// Image size in bytes
		int32_t  XResolutionPpm;  	// Pixels per meter
		int32_t  YResolutionPpm;  	// Pixels per meter
		uint32_t NumColors;        	// Number of colors
		uint32_t ImportantColors;  	// Important colors
	}tgfxcustom_HeaderBmp_t;
#pragma pack(pop)  // restore the previous pack setting


/*______________________________________________________________________________
 Desc:  Salva in BMP il framebuffer LCD passato. Il file viene sovrascritto se
 	 	già esistente. Il formato supportato è RG565
 Arg: - Nessun argomento.
 Ret: - _STATUS_OK_ o _STATUS_FAIL_
______________________________________________________________________________*/
int32_t tgfxcustom_SaveFrameBuffer(void* ptrFrameBuffer,uint16_t dimPixelX,uint16_t dimPixelY,drvfilesystem_Driver_t *ptrCustomFsDriver,char* ptrStrNomeFile)
{
	int32_t valRet = _STATUS_FAIL_;
	drvfilesystem_HandleFile_t usbHandleFile;
	tgfxcustom_HeaderBmp_t bmpHeader;
	bool errore=false;
	uint32_t uLA;
	uint32_t coordX,coordY;
	uint16_t *localPtr16FrameBuffer;
	uint16_t uSA;
	uint8_t *ptrBuffer8Linea;

	if(ptrCustomFsDriver->FOpen(&usbHandleFile,ptrStrNomeFile,DRVFILESYSTEM_FOPENTYPE_WRITE )==_STATUS_OK_)
	{
	    bmpHeader.Type=0x4d42;

		uLA = dimPixelX*dimPixelY*3;
		// Allineo a 4 byte
		if(uLA%4)
			uLA+=4-(uLA%4);

		bmpHeader.Size=sizeof(tgfxcustom_HeaderBmp_t)+uLA;	// Dimensione del file
		bmpHeader.ImageSizeBytes = uLA;	// Dimensione in byte dell'immagine bitmap senza le 3 sezioni iniziali

		bmpHeader.BitsPerPixel=24;	// Numero di bit (colore) per pixel


	    bmpHeader.Reserved1=0;
	    bmpHeader.Reserved2=0;;
	    bmpHeader.Offset = sizeof(tgfxcustom_HeaderBmp_t);	// Scostamento (offset) in bytes dove inizia l'immagine bitmap vera e propria
	    bmpHeader.DibHeaderSize	= 40;		// lunghezza in byte di questo header.
	    bmpHeader.WidthPx	= dimPixelX;	// Larghezza in pixel del bitmap
	    bmpHeader.HeightPx	= dimPixelY;	// Altezza in pixel del bitmap
	    bmpHeader.NumPlanes	= 1;			// umero di piani - vale sempre 1
	    bmpHeader.Compression	= 0;		// Nessuna compressione
	    bmpHeader.XResolutionPpm = 2835;	// Risoluzione orizzontale in px al mt
	    bmpHeader.YResolutionPpm = 2835;	// Risoluzione verticale in px al mt
	    bmpHeader.NumColors = 0;			// Numero di colori da considerare nella palette (0 indica tutti)
	    bmpHeader.ImportantColors= 0;		// zero indica che tutti i colori sono importanti

	    ptrCustomFsDriver->FWrite(&usbHandleFile,&bmpHeader,sizeof(bmpHeader));

		ptrBuffer8Linea = _MALLOC_(dimPixelX*3);

		if(ptrBuffer8Linea)
		{
			localPtr16FrameBuffer = ptrFrameBuffer;
			localPtr16FrameBuffer += dimPixelX*dimPixelY;
			coordY = dimPixelY;
			while(coordY)
			{
				coordY--;
				// Giro la linea
				coordX=dimPixelX;
				while(coordX)
				{
					coordX--;
					localPtr16FrameBuffer--;

					uSA = (*localPtr16FrameBuffer);
					ptrBuffer8Linea[(3*coordX)+2]= (uSA>>8)&0x00f8;	//5bit
					ptrBuffer8Linea[(3*coordX)+1]= (uSA>>3)&0xfc;  	//6bit
					ptrBuffer8Linea[(3*coordX)]= (uSA<<3)&0xf8;	//5bit
				}

				ptrCustomFsDriver->FWrite(&usbHandleFile,ptrBuffer8Linea,dimPixelX*3);
			}
			_FREE_(ptrBuffer8Linea);
		}
		else
			errore = true;

		ptrCustomFsDriver->FClose(&usbHandleFile);

		if(!errore)
			valRet = _STATUS_OK_;
	}

	return valRet;
}

//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicità dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void tgfxcustom_ZZZ(void)
{

}

//============================================================ PRIVATE FUNCTIONS

