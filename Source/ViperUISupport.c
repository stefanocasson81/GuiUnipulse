/*------------------------------------------------------------------------------
 Autore:	Nik
 DataVer:	04/07/23
 Descrizione:
	Modulo di support all'interfaccia grafica ViperUI.
	Ad esempio vengono compilate le varie stringhe di visualizzazione a seconda
	del tipo di dato.
	Il modulo è senza inizializzazione fuori dalla norma.

------------------------------------------------------------------------------*/




//===================================================================== INCLUDES
#include <ViperUISupport.h>


//====================================================================== DEFINES

//=========================================================== PRIVATE PROTOTYPES

//============================================================= STATIC VARIABLES

//============================================================= GLOBAL VARIABLES

//============================================================= GLOBAL FUNCTIONS


//______________________________________________________________________________
// Desc:  Converte in testo formattato il ptrData passato
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void viperuisupport_GetTxtPtrData(data_Data_t *ptrData,char* ptrStr,uint16_t maxLen)
{
	int32_t value = data_GetValData((data_Data_t *)ptrData);


	if(ptrData == &viperui_StrutturaDati[VIPERUI_DATA_MIG_TEMPI])
	{
		switch((viperdef_Tempi_e)value)
		{
			default:
				snprintf(ptrStr,maxLen,"XxX");
			break;

			case VIPERDEF_TEMPI_2T:
				snprintf(ptrStr,maxLen,"2T");
			break;
			case VIPERDEF_TEMPI_4T:
				snprintf(ptrStr,maxLen,"4T");
			break;
			case VIPERDEF_TEMPI_2TPLUS:
				snprintf(ptrStr,maxLen,"2T Plus");
			break;
			case VIPERDEF_TEMPI_4TPLUS:
				snprintf(ptrStr,maxLen,"4T Plus");
			break;
		}
	}
	else if(ptrData == &viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOFILO])
	{
		switch((viperdef_TipoFilo_e)value)
		{
			default:
				snprintf(ptrStr,maxLen,"XxX");
			break;

			case VIPERDEF_TIPOFILO_MANUAL:
				snprintf(ptrStr,maxLen,"MANUAL");
			break;
			case VIPERDEF_TIPOFILO_FE:
				snprintf(ptrStr,maxLen,"Fe");
			break;
			case VIPERDEF_TIPOFILO_FEMETAL:
				snprintf(ptrStr,maxLen,"FeMetal");
			break;
			case VIPERDEF_TIPOFILO_FERUTIL:
				snprintf(ptrStr,maxLen,"FeRutil");
			break;
			case VIPERDEF_TIPOFILO_CRNI308:
				snprintf(ptrStr,maxLen,"CrNi-308");
			break;
			case VIPERDEF_TIPOFILO_CRNI316:
				snprintf(ptrStr,maxLen,"CrNi-316");
			break;
			case VIPERDEF_TIPOFILO_CRNI2209:
				snprintf(ptrStr,maxLen,"CrNi-2209");
			break;
			case VIPERDEF_TIPOFILO_ALSI4043:
				snprintf(ptrStr,maxLen,"Al-4043");
			break;
			case VIPERDEF_TIPOFILO_AL4047:
				snprintf(ptrStr,maxLen,"Al-4047");
			break;
			case VIPERDEF_TIPOFILO_AL4145:
				snprintf(ptrStr,maxLen,"Al-4145");
			break;
			case VIPERDEF_TIPOFILO_AL4557:
				snprintf(ptrStr,maxLen,"Al-4557");
			break;
			case VIPERDEF_TIPOFILO_ALMG5356:
				snprintf(ptrStr,maxLen,"Al-5356");
			break;
			case VIPERDEF_TIPOFILO_AL5554:
				snprintf(ptrStr,maxLen,"Al-5554");
			break;
			case VIPERDEF_TIPOFILO_CUSI3CRNI:
				snprintf(ptrStr,maxLen,"CuSi3CrNi");
			break;
			case VIPERDEF_TIPOFILO_CUSI3ZN:
				snprintf(ptrStr,maxLen,"CuSi3Zn");
			break;
			case VIPERDEF_TIPOFILO_CUAL8CRNI:
				snprintf(ptrStr,maxLen,"CuAl8CrNi");
			break;
			case VIPERDEF_TIPOFILO_CUAL8ZN:
				snprintf(ptrStr,maxLen,"CuAl8Zn");
			break;
			case VIPERDEF_TIPOFILO_ER80:
				snprintf(ptrStr,maxLen,"Er80");
			break;
			case VIPERDEF_TIPOFILO_ER120:
				snprintf(ptrStr,maxLen,"Er120");
			break;
			case VIPERDEF_TIPOFILO_NICR3:
				snprintf(ptrStr,maxLen,"NiCr3");
			break;
		}
	}
	else if(ptrData == &viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOGAS])
	{
		switch((viperdef_TipoGas_e)value)
		{
			default:
				snprintf(ptrStr,maxLen,"XxX");
			break;

			case VIPERDEF_TIPOGAS_NOGAS:
				snprintf(ptrStr,maxLen,"No GAS");
			break;
			case VIPERDEF_TIPOGAS_I1_ARGON:
				snprintf(ptrStr,maxLen,"I1 Argon");
			break;
			case VIPERDEF_TIPOGAS_M12_MIX9802:
				snprintf(ptrStr,maxLen,"M12 Mix9802");
			break;
			case VIPERDEF_TIPOGAS_M20_MIX9208:
				snprintf(ptrStr,maxLen,"M20 Mix9208");
			break;
			case VIPERDEF_TIPOGAS_M21_MIX8020:
				snprintf(ptrStr,maxLen,"M21 Mix8020");
			break;
			case VIPERDEF_TIPOGAS_C1_CO2:
				snprintf(ptrStr,maxLen,"C1 CO2");
			break;
			case VIPERDEF_TIPOGAS_C2_MIX9802:
				snprintf(ptrStr,maxLen,"C2 Mix9802");
			break;
		}
	}
	else if(ptrData == &viperui_StrutturaDati[VIPERUI_DATA_BASE_DIAMETROFILO])
	{
		switch((viperdef_DiametroFilo_e)value)
		{
			default:
				snprintf(ptrStr,maxLen,"XxX mm");
			break;
			case VIPERDEF_DIAMETROFILO_06:
				snprintf(ptrStr,maxLen,"0.6 mm");
			break;
			case VIPERDEF_DIAMETROFILO_08:
				snprintf(ptrStr,maxLen,"0.8 mm");
			break;
			case VIPERDEF_DIAMETROFILO_09:
				snprintf(ptrStr,maxLen,"0.9 mm");
			break;
			case VIPERDEF_DIAMETROFILO_10:
				snprintf(ptrStr,maxLen,"1.0 mm");
			break;
			case VIPERDEF_DIAMETROFILO_12:
				snprintf(ptrStr,maxLen,"1.2 mm");
			break;
			case VIPERDEF_DIAMETROFILO_14:
				snprintf(ptrStr,maxLen,"1.4 mm");
			break;
			case VIPERDEF_DIAMETROFILO_16:
				snprintf(ptrStr,maxLen,"1.6 mm");
			break;
		}
	}
	else if(ptrData == &viperui_StrutturaDati[VIPERUI_DATA_TIG_DTENSCUTOFF])
	{
		if(value == data_GetValMin(&viperui_StrutturaDati[VIPERUI_DATA_TIG_DTENSCUTOFF]))
			snprintf(ptrStr,maxLen,"AUTO");
		else if(value == data_GetValMax(&viperui_StrutturaDati[VIPERUI_DATA_TIG_DTENSCUTOFF]))
			snprintf(ptrStr,maxLen,"OFF");
		else
			customstring_FormatFix(ptrStr, value, ptrData->NumInteri, ptrData->NumDecimali, ' ',',', false);
	}
	else
	{
		if(data_GetValMin(ptrData)<0)
			customstring_FormatFix(ptrStr, value, ptrData->NumInteri, ptrData->NumDecimali, ' ',',', true);
		else
			customstring_FormatFix(ptrStr, value, ptrData->NumInteri, ptrData->NumDecimali, ' ',',', false);
	}
}

//______________________________________________________________________________
// Desc:  Converte in testo formattato l'enum Data passato
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void viperuisupport_GetTxtEnumData(viperui_Data_e dataEnum,char* ptrStr,uint16_t maxLen)
{
	viperuisupport_GetTxtPtrData((data_Data_t *)(&viperui_StrutturaDati[dataEnum]),ptrStr,maxLen);
}

//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicità dell'editor
// Arg: - ptrObjTabParametri: Ptr all'obj di gestione da compilare
//		  ptrStructParametri: Ptr alla struttura parametri da gestire
// Ret: - void
//______________________________________________________________________________
void viperuisupport_ObjTabParametri_Create(viperuisupport_ObjTabParametri_t *ptrObjTabParametri,const viperuisupport_StructParametri_t *ptrStructParametri)
{
	viperuisupport_ObjTabParametri_t *ptrLocalObjTabParametri = (viperuisupport_ObjTabParametri_t*)ptrObjTabParametri;

	memset(ptrObjTabParametri,0,sizeof(viperuisupport_ObjTabParametri_t));

	ptrLocalObjTabParametri->IndiceParametroSelezionato = 0;
	ptrLocalObjTabParametri->NumeroParametri = 0;
	ptrLocalObjTabParametri->PtrStructParametri = ptrStructParametri;

	while(ptrStructParametri->PtrTestoParametro || ptrStructParametri->EnumTypedText)
	{
		ptrLocalObjTabParametri->NumeroParametri++;
		ptrStructParametri++;
	}
}

//______________________________________________________________________________
// Desc:  Obj create della tabella parametri da editare
// Arg: - ptrObjTabParametri: Ptr all'obj di gestione da compilare
//		  ptrStructParametri: Ptr alla struttura parametri da gestire
// Ret: - void
//______________________________________________________________________________
void viperuisupport_ObjTabParametri_SelezioneOffset(viperuisupport_ObjTabParametri_t *ptrObjTabParametri,int16_t offsetSel)
{
	int16_t sA=ptrObjTabParametri->IndiceParametroSelezionato;

	sA+=offsetSel;
	if(sA<0)
		sA = 0;
	else if(sA>=ptrObjTabParametri->NumeroParametri)
		sA = ptrObjTabParametri->NumeroParametri-1;

	ptrObjTabParametri->IndiceParametroSelezionato = (uint8_t)sA;
}

//______________________________________________________________________________
// Desc:  Usato come ultima funzione delle globali per semplicità dell'editor
// Arg: - Nessun argomento.
// Ret: - void
//______________________________________________________________________________
void viperuisupport_ZZZ(void)
{
	
}

//============================================================ PRIVATE FUNCTIONS

