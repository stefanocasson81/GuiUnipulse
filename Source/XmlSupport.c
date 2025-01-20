/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	09/05/23
 Descrizione:
 	 Modulo di supporto ai file xml di progetto

------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "XmlSupport.h"
#include "ARMLib/FileSystem/Xml.h"
#include "ARMLib/FileSystem/_DrvFileSystemUSB.h"
#include "Applicazione.h"
#include "ARMLib/Common/CustomString.h"
#include "Applicazione.h"
#include "CORELib/UserInterface/ViperUI.h"
#include "gui/custom/CommonDefine.h"

//====================================================================== DEFINES
typedef struct
{
	bool Permission;
	uint16_t CodiceSchedaRidotto;
	uint32_t CodiceUnivocoScheda;
	uint32_t CheckSum;
}xmlsupport_Permission_t;

//=========================================================== PRIVATE PROTOTYPES
static bool CheckPermissionImmediateCmd(xml_DataTag_t *ptrDataTag);
static void FunWrPermissionImmediateCmd(void* ptrVoidDataTag);


//============================================================= STATIC VARIABLES
// Data specifici per l'xml ImmediateCmd
static xmlsupport_Permission_t PermissionImmediateCmd;
static const xml_DataTag_t XmlDataTagImmediateCmd[]=
{
	{"Permission/CodiceSchedaRidotto",			&PermissionImmediateCmd.CodiceSchedaRidotto,							_TYPEDATA_UINT16_,	0,			0xffff,		FunWrPermissionImmediateCmd},
	{"Permission/CodiceUnivocoScheda",			&PermissionImmediateCmd.CodiceUnivocoScheda,							_TYPEDATA_UINT32_,	0,			0xffffffff,	FunWrPermissionImmediateCmd},
	{"CheckSumXml",								&PermissionImmediateCmd.CheckSum,										_TYPEDATA_UINT32_,	0,			0xffffffff,	_NULL_},	// Questa è una var di appoggio

	{"UI/CmdExecuteScreenShoot",				&applicazione_Info.ExecuteScreenShoot,									_TYPEDATA_BOOL_,	0,			1,			_NULL_},
	{"UI/ShowDebug",							&applicazione_Info.ShowDebug,											_TYPEDATA_BOOL_,	0,			1,			_NULL_},
	{"UI/Colore/Aziendale",						&viperui_ListStdColor[VIPERUI_STDCOLOR_AZIENDALE],						_TYPEDATA_UINT32_,	0,			0xffffff,	_NULL_},
	{"UI/Colore/GrigioScuro",					&viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOSCURO],					_TYPEDATA_UINT32_,	0,			0xffffff,	_NULL_},
	{"UI/Colore/GrigioMedio",					&viperui_ListStdColor[VIPERUI_STDCOLOR_GRIGIOMEDIO],					_TYPEDATA_UINT32_,	0,			0xffffff,	_NULL_},
	{"UI/Colore/Bianco",						&viperui_ListStdColor[VIPERUI_STDCOLOR_BIANCO],							_TYPEDATA_UINT32_,	0,			0xffffff,	_NULL_},

	{_NULL_},
};


//============================================================= GLOBAL VARIABLES
xmlsupport_Init_t xmlsupport_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t xmlsupport_InitModule(void)
{
	bool errore=false;

	// Inserire qui le inizializzazioni del modulo
	// ...




	if(errore)
	{
		xmlsupport_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		xmlsupport_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

//______________________________________________________________________________
// Desc:  Leggi il file ImmediaCmd.xml
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
xml_Status_e xmlsupport_LeggiImmediateCmd(void)
{
	PermissionImmediateCmd.Permission = false;

	return xml_CaricaXml("ImmediateCmd.xml",&_drvfilesystemusb_Driver,(xml_DataTag_t*)XmlDataTagImmediateCmd,CheckPermissionImmediateCmd);
}

//______________________________________________________________________________
// Desc:  Salva il file xml passato con i datatag passati
// Arg: - ptrStrNomeFile: Nome del file
//		- ptrDrvCustomFS: Driver del file system
//		- ptrXmlDataTag: Tabella dei datatag
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
xml_Status_e xmlsupport_SalvaXmlActualCfg(void)
{
	xml_Status_e valRet = XML_STATUS_OK;
/*
	drvfilesystem_HandleFile_t handleFile;
	char tmpStr[100];
	char nomeFile[100];
	unsigned int uLA;

	customstring_FormatFix(tmpStr,applicazione_Info.CodiceSchedaRidotto,5,0,'0','.',false);
	strcpy(nomeFile,tmpStr);
	strcat(nomeFile,"_");
	sprintf(tmpStr,"%08X_Status.xml",(unsigned int)applicazione_Info.CodiceUnivocoScheda);
	strcat(nomeFile,tmpStr);

	if(xml_Init.InitOk)
	{
		if(_drvfilesystemusb_Driver.FOpen(&handleFile,nomeFile,DRVFILESYSTEM_FOPENTYPE_WRITE)==_STATUS_OK_)
		{
			// Il primo e il grande capo è sempre Root
			_drvfilesystemusb_Driver.FPrintf(&handleFile,"<Root>\r\n");

			customstring_FormatFix(tmpStr,applicazione_Info.CodiceSchedaRidotto,5,0,0,'.',false);
			_drvfilesystemusb_Driver.FPrintf(&handleFile,"\t<CodiceSchedaRidotto>%s</CodiceSchedaRidotto>\r\n",tmpStr);

			customstring_FormatFix(tmpStr,application_Info.CodiceUnivocoScheda,10,0,0,'.',false);
			_drvfilesystemusb_Driver.FPrintf(&handleFile,"\t<CodiceUnivocoScheda>%s</CodiceUnivocoScheda>\r\n",tmpStr);

			_drvfilesystemusb_Driver.FPrintf(&handleFile,"\t<CodiceUnivocoSchedaHex>%08X</CodiceUnivocoSchedaHex>\r\n",(unsigned int)application_Info.CodiceUnivocoScheda);

			uLA = ((bootloader_InfoHeader_t*)_BOARD_ADDRESSINFOHEADER)->DataCreazioneFile;
			sprintf(tmpStr,"V.%02d %02d/%02d/%04d",(int)((bootloader_InfoHeader_t*)_BOARD_ADDRESSINFOHEADER)->VersioneFirmware,((uLA>>24)&0xff),((uLA>>16)&0xff),((uLA)&0xffff));
			_drvfilesystemusb_Driver.FPrintf(&handleFile,"\t<VersioneFirmware>%s</VersioneFirmware>\r\n",tmpStr);

			_drvfilesystemusb_Driver.FPrintf(&handleFile,"</Root>");
			_drvfilesystemusb_Driver.FClose(&handleFile);
		}
		else
			valRet = XML_STATUS_FSERROR;
	}
*/
	return valRet;
}



//============================================================ PRIVATE FUNCTIONS
// Esegui un check di permission sulla possibilità di modifica Tag del file ImmediateCmd.xml
static bool CheckPermissionImmediateCmd(xml_DataTag_t *ptrDataTag)
{
	return PermissionImmediateCmd.Permission;
}

static void FunWrPermissionImmediateCmd(void* ptrVoidDataTag)
{
	xml_DataTag_t *ptrDataTag = (xml_DataTag_t*)ptrVoidDataTag;

	if(!strcmp(ptrDataTag->PtrStrTag,"Permission/CodiceSchedaRidotto"))
	{
		if((PermissionImmediateCmd.CodiceSchedaRidotto==applicazione_Info.CodiceSchedaRidotto) || (PermissionImmediateCmd.CodiceSchedaRidotto==0xffff))
			PermissionImmediateCmd.Permission = true;
	}
	if(!strcmp(ptrDataTag->PtrStrTag,"Permission/CodiceUnivocoScheda"))
	{
		if((PermissionImmediateCmd.CodiceUnivocoScheda==applicazione_Info.CodiceUnivocoScheda) || (PermissionImmediateCmd.CodiceUnivocoScheda==0xffffffff))
			PermissionImmediateCmd.Permission = true;
	}
}











