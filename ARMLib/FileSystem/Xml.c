/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
	Gestore file di tipo xml.
------------------------------------------------------------------------------*/



//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"
#include "ARMLib/FileSystem/Xml.h"
#include "ARMLib/Common/CustomString.h"
#include "ARMLib/Common/Crc.h"
#include "ARMLib/FileSystem/DrvFileSystem.h"

//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES
static int32_t AddTagToPath(char *ptrStrTagPath,char* ptrStrTag,uint32_t maxNumChar);
static void RemoveLastTagToPath(char *ptrStrTagPath);

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES
xml_Init_t xml_Init;

//============================================================= GLOBAL FUNCTIONS

//______________________________________________________________________________
// Desc:  Init del modulo.
// Arg: - Nessun argomento.
// Ret: - _STATUS_OK_ o _STATUS_FAIL_
//______________________________________________________________________________
int32_t xml_InitModule(void)
{
	bool errore=false;

	// Inserire qui le inizializzazioni del modulo
	// ...

	if(errore)
	{
		xml_Init.InitOk = false;
		return _STATUS_FAIL_;
	}
	else
	{
		xml_Init.InitOk = true;
		return _STATUS_OK_;
	}
}

typedef enum
{
	XML_SLR_SEARCH_TAGTESTA				,
	XML_SLR_SEARCH_TAG					,

	XML_SLR_SEARCH_DATATAG				,

	XML_SLR_SEARCH_COMMENTSTART1M		,
	XML_SLR_SEARCH_COMMENTSTART2M		,
	XML_SLR_SEARCH_COMMENT				,
	XML_SLR_SEARCH_COMMENTEND2M			,
	XML_SLR_SEARCH_COMMENTCAPEND		,
}xml_SLR_e;

//______________________________________________________________________________
// Desc:  Carica il file xml passato con i datatag passati
// Arg: - ptrStrNomeFile: Nome del file
//		- ptrDrvCustomFS: Driver del file system
//		- ptrXmlDataTag: Tabella dei datatag
// Ret: - enum del tipo xml_BitStatus_e
//______________________________________________________________________________
xml_Status_e xml_CaricaXml(char* ptrStrNomeFile,drvfilesystem_Driver_t *ptrDrvCustomFS, xml_DataTag_t *ptrXmlDataTag,bool(*ptrFunCheckPermission)(xml_DataTag_t *ptrDataTag))
{
	xml_Status_e valRet = XML_STATUS_OK;
	char strTagPathInCorso[150];
	char tmpStrTag[100];
	char tmpStrDataTag[100];
	uint32_t indiceDataTag=0;
	drvfilesystem_HandleFile_t handleFile;
	xml_SLR_e statusLoop = XML_SLR_SEARCH_TAGTESTA;
	uint32_t uLA;
	uint32_t livelloTagAttuale=0;
	char carattere;
	bool aggiungiTag=true;
	xml_DataTag_t *ptrTmpXmlDataTag;
	double dA;
	float fA;
	int64_t lLA;
	int32_t lA;
	bool verificaFile=true;
	bool resultCheckPermission;
	uint32_t checkSum=0;
	uint32_t wordCheckSumAnalize=0;
	uint32_t nByteLetti=0;
	bool stopCalcoloCheckSum=false;
	uint32_t checksumOrgXml=0;
	bool tagPermission=false;

	// NOTA: il file viene parsato una prima volta per verificare che sia correttamente formattato e una seconda volta per effettivamente eseguire i comandi/impostazioni

	memset(strTagPathInCorso,0,sizeof(strTagPathInCorso));
	if(xml_Init.InitOk)
	{
		if(ptrDrvCustomFS->FOpen(&handleFile,ptrStrNomeFile,DRVFILESYSTEM_FOPENTYPE_READ)==_STATUS_OK_)
		{
			while(valRet==XML_STATUS_OK)
			{
				lA = ptrDrvCustomFS->FRead(&handleFile,&carattere,1);
				if(lA==_STATUS_FAIL_)
					valRet = XML_STATUS_FSERROR;
				else if(lA==0)
				{	// Arrivati alla fine del file, alcune verifiche
					if(livelloTagAttuale || strlen(strTagPathInCorso))
					{	// Se i Tag sono sbilanciati qualcosa nel file non va
						valRet = XML_STATUS_TAGSBILANCIATI;
					}

					if(valRet==XML_STATUS_OK)
					{
						if(verificaFile)
						{	// OK. file grammaticalmente valido procedi con la valutazione vera e propria se checksum valida
							verificaFile = false;

							if(checksumOrgXml==checkSum)
							{	// Se checksum valida procede con la valutazione vera e propria
								ptrDrvCustomFS->FSeek(&handleFile,0,DRVFILESYSTEM_SEEK_SET);	// Riparto dall'inizio e riprocedo con la vera valutazione del file
								statusLoop = XML_SLR_SEARCH_TAGTESTA;
								memset(strTagPathInCorso,0,sizeof(strTagPathInCorso));
								livelloTagAttuale=0;
							}
							else
							{
								valRet = XML_STATUS_CHECKSUMERRATA;	// CheckSum errata, file manomesso
							}
						}
						else
							break;	// Fine lettura file
					}
				}

				nByteLetti++;
				if(!stopCalcoloCheckSum)
				{
					wordCheckSumAnalize^=(uint8_t)carattere;
					wordCheckSumAnalize<<=8;
					wordCheckSumAnalize|=(uint8_t)carattere;
					if(!(nByteLetti%4))
					{
						wordCheckSumAnalize^=0x48454C56;	// "HELV"
						checkSum = crc_CalcCrc32((uint8_t*)&wordCheckSumAnalize, 4, checkSum);
						wordCheckSumAnalize = 0;
					}
				}

				switch(statusLoop)
				{
					case XML_SLR_SEARCH_TAGTESTA:
						switch(carattere)
						{
							case '<':
								memset(tmpStrTag,0,sizeof(tmpStrTag));
								aggiungiTag = true;
								statusLoop++;
							break;
							default:
							break;
						}
					break;
					case XML_SLR_SEARCH_TAG:
						switch(carattere)
						{
							case '!':
								statusLoop = XML_SLR_SEARCH_COMMENTSTART1M;
							break;
							case '/':
								aggiungiTag = false;
							break;
							case '>':
								if(aggiungiTag)
								{

									livelloTagAttuale++;
									if(livelloTagAttuale>1)
									{	// Non agiungo il Root...
										AddTagToPath(strTagPathInCorso,tmpStrTag,sizeof(strTagPathInCorso)-1);
									}
									memset(tmpStrDataTag,0,sizeof(tmpStrDataTag));
									indiceDataTag=0;

									if(!strcmp(strTagPathInCorso,"CheckSumXml"))
									{
										stopCalcoloCheckSum = true;
									}

									statusLoop=XML_SLR_SEARCH_DATATAG;
								}
								else
								{
									if(livelloTagAttuale)
									{
										livelloTagAttuale--;

										tagPermission = false;
										if((strstr(strTagPathInCorso,"Permission/")) || (!strcmp(strTagPathInCorso,"CheckSumXml")))
											tagPermission = true;

										// Se tag parente è Permission o CheckSumXml è sempre valutabile
										if(strlen(tmpStrDataTag) && ((!verificaFile) || tagPermission))
										{	// Se siamo in verificaFile NON valuto il Tag
											// Analizzo i dati del tag in corso
											ptrTmpXmlDataTag = ptrXmlDataTag;
											while(ptrTmpXmlDataTag->PtrStrTag)
											{
												if(!strcmp(ptrTmpXmlDataTag->PtrStrTag,strTagPathInCorso))
												{	// Se Path conforme, beccato
													resultCheckPermission = true;
													if(ptrFunCheckPermission && !tagPermission)
														resultCheckPermission = ptrFunCheckPermission(ptrTmpXmlDataTag);

													if(resultCheckPermission)
													{
														switch(ptrTmpXmlDataTag->TypeData)
														{
															case _TYPEDATA_PTRSTR_:
																strncpy(ptrTmpXmlDataTag->PtrData,tmpStrDataTag,ptrTmpXmlDataTag->ValMax);
															break;
															case _TYPEDATA_FLOAT_:
																customstring_AtoX(tmpStrDataTag,ptrTmpXmlDataTag->PtrData,ptrTmpXmlDataTag->TypeData);
																fA = *(float*)ptrTmpXmlDataTag->PtrData;
																if(fA>ptrTmpXmlDataTag->ValMax)
																	fA=ptrTmpXmlDataTag->ValMax;
																else if(fA<ptrTmpXmlDataTag->ValMin)
																	fA=ptrTmpXmlDataTag->ValMin;
																*(float*)ptrTmpXmlDataTag->PtrData = fA;
															break;
															case _TYPEDATA_DOUBLE_:
																customstring_AtoX(tmpStrDataTag,ptrTmpXmlDataTag->PtrData,ptrTmpXmlDataTag->TypeData);
																dA = *(double*)ptrTmpXmlDataTag->PtrData;
																if(dA>ptrTmpXmlDataTag->ValMax)
																	dA=ptrTmpXmlDataTag->ValMax;
																else if(dA<ptrTmpXmlDataTag->ValMin)
																	dA=ptrTmpXmlDataTag->ValMin;
																*(float*)ptrTmpXmlDataTag->PtrData = dA;
															break;
															default:
																customstring_AtoX(tmpStrDataTag,ptrTmpXmlDataTag->PtrData,ptrTmpXmlDataTag->TypeData);
																lLA = customstring_GetInt64t(ptrTmpXmlDataTag->PtrData,ptrTmpXmlDataTag->TypeData);
																if(lLA>ptrTmpXmlDataTag->ValMax)
																	lLA=ptrTmpXmlDataTag->ValMax;
																else if(lLA<ptrTmpXmlDataTag->ValMin)
																	lLA=ptrTmpXmlDataTag->ValMin;
																customstring_SetValTypeInt(ptrTmpXmlDataTag->PtrData,ptrTmpXmlDataTag->TypeData,lLA);

																if(verificaFile)
																{
																	// Questa è una eccezione, estrapolo la checksum scritta sul file
																	if(!strcmp(ptrTmpXmlDataTag->PtrStrTag,"CheckSumXml"))
																		checksumOrgXml = lLA;
																}
															break;
														}

														if(ptrTmpXmlDataTag->PtrFunPostWr)
															ptrTmpXmlDataTag->PtrFunPostWr(ptrTmpXmlDataTag);
													}
												}
												ptrTmpXmlDataTag++;
											}	// fine while(ptrTmpXmlDataTag->PtrStrTag)
										}	// fine if(strlen(tmpStrDataTag))
										memset(tmpStrDataTag,0,sizeof(tmpStrDataTag));

										RemoveLastTagToPath(strTagPathInCorso);
										statusLoop=XML_SLR_SEARCH_TAGTESTA;
									}
									else
										valRet = XML_STATUS_TAGSBILANCIATI;	// I Tag sono sbilanciati il file non è valido
								}
							break;
							default:
								uLA = strlen(tmpStrTag);
								if((uLA+2)<=sizeof(tmpStrTag))
									tmpStrTag[uLA++] = carattere;	// Non serve terminarlo è già stato fatto all'inizio con il memset
								else
									valRet = XML_STATUS_ERRGENERICO;
							break;

						}
					break;

					case XML_SLR_SEARCH_DATATAG:
						switch(carattere)
						{
							case '<':	// Fine data tag raggiunto
								memset(tmpStrTag,0,sizeof(tmpStrTag));
								aggiungiTag = true;
								statusLoop=XML_SLR_SEARCH_TAG;
							break;
							case '\r':
							case '\n':
							case '\t':
							break;
							default:
								tmpStrDataTag[indiceDataTag++] = carattere;
								if(indiceDataTag>=(sizeof(tmpStrDataTag)-1))
									valRet = XML_STATUS_ERRGENERICO;
							break;
						}
					break;

					case XML_SLR_SEARCH_COMMENTSTART1M:	// Primo - della testa commento
						if(carattere=='-')
							statusLoop++;
						else
							valRet = XML_STATUS_GRAMMATICAERRATA;
					break;
					case XML_SLR_SEARCH_COMMENTSTART2M:	// Secondo - della testa commento
						if(carattere=='-')
							statusLoop++;
						else
							valRet = XML_STATUS_GRAMMATICAERRATA;
					break;
					case XML_SLR_SEARCH_COMMENT:	// Alla ricerca del primo - coda commento
						if(carattere=='-')
							statusLoop++;
					break;
					case XML_SLR_SEARCH_COMMENTEND2M:
						if(carattere=='-')
							statusLoop++;
						else
							statusLoop = XML_SLR_SEARCH_COMMENT;
					break;
					case XML_SLR_SEARCH_COMMENTCAPEND:
						if(carattere=='>')
							statusLoop = XML_SLR_SEARCH_TAGTESTA;
						else
							statusLoop = XML_SLR_SEARCH_COMMENT;
					break;
				}	// Fine switch(statusLoop)
			}
		}	// fine while(!errore)
		ptrDrvCustomFS->FClose(&handleFile);
	}
	else
	{
		valRet = XML_STATUS_FSERROR;
	}

	return valRet;
}

//============================================================ PRIVATE FUNCTIONS
// Compone il path del tipo xxxx/ttttt/zzzz.....
static int32_t AddTagToPath(char *ptrStrTagPath,char* ptrStrTag,uint32_t maxNumChar)
{
	uint32_t sizePath = strlen(ptrStrTagPath);
	if((strlen(ptrStrTag)+sizePath+1)<=maxNumChar)
	{
		if(sizePath)
			strcat(ptrStrTagPath,"/");

		strcat(ptrStrTagPath,ptrStrTag);
		return _STATUS_OK_;
	}
	else
		return _STATUS_FAIL_;
}

static void RemoveLastTagToPath(char *ptrStrTagPath)
{
	uint32_t uLA = strlen(ptrStrTagPath);

	while(uLA)
	{
		uLA--;
		if(ptrStrTagPath[uLA]=='/')
		{
			ptrStrTagPath[uLA]=0;
			break;
		}
		else
			ptrStrTagPath[uLA]=0;
	}
}
