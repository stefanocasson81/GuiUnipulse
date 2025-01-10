/*------------------------------------------------------------------------------
 Autore:	Nik
 Descrizione:
 	 Serie di funzioni utility di supporto veloce per TouchGFX

------------------------------------------------------------------------------*/


#include <ARMLib/TGFX/TGFXUtil.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/TypedText.hpp>

using namespace touchgfx;





tgfxguiutil_c::tgfxguiutil_c() {
	// TODO Auto-generated constructor stub

}

tgfxguiutil_c::~tgfxguiutil_c() {
	// TODO Auto-generated destructor stub
}



// Ritorna il colore standard per la primitiva passata
/*
tgfxguiutil_c::colortype GetStdColor(uint32_t coloreRGB,float opacita)
{
	return touchgfx::Color::getColorFrom24BitRGB(((coloreRGB>>16)&0xff)*opacita,(coloreRGB>>16)&0xff)*opacita,(coloreRGB>>16)&0xff)*opacita);
}
*/

/*
// Converte ptrSpecialTextOrg il testo UnicodeChar: potrebbe essere un TypedText oppure classico testo ascii mappato
// Ritorna il numero di caratteri copiati
uint16_t tgfxguiutil_c::SpecialTextStrNCpy(touchgfx::Unicode::UnicodeChar* ptrDest, void* ptrSpecialTextOrg, uint16_t maxchars)
{
	uint16_t valRet=0;

	if((uint32_t)ptrSpecialTextOrg)
	{
		if((uint32_t)ptrSpecialTextOrg<10000)
		{	// Se è testo TypedText (In pratica se non punta in ram o flash....)
			valRet = Unicode::strncpy(ptrDest,touchgfx::TypedText((uint32_t)ptrSpecialTextOrg).getText(), maxchars);
		}
		else
		{	// Se è testo ascii classico
			valRet = Unicode::strncpy(ptrDest,(const char*)ptrSpecialTextOrg, maxchars);
		}
	}

	return valRet;
}
*/
/*
uint16_t GuiUtil::GetIdFromImage(void* ptrImage)
{
	uint16_t indice;
	Bitmap::BitmapData* ptrListBitmap= (Bitmap::BitmapData*)BitmapDatabase::getInstance();

	if(ptrImage)
	{
		for(indice=0;indice<BitmapDatabase::getInstanceSize();indice++)
		{
			if(ptrListBitmap[indice].data==ptrImage)
				return indice;
		}
	}

	return 0;
}


void GuiUtil::GetUnicodeStrListBox(appdata_Info_t *ptrAppDataInfo,float valore,touchgfx::Unicode::UnicodeChar *ptrUnicodeDest,uint16_t maxChars)
{
	uint32_t vSearch=0;

	Unicode::UnicodeChar localUnicodeChar[201];
	uint16_t indiceLUC=0;
	uint16_t indiceWrTesto;
	bool anomalia=false;

	if(valore>=0 && ptrAppDataInfo->PtrAuxData)
		vSearch = valore;
	else
		anomalia = true;

	SpecialTextStrNCpy(localUnicodeChar,ptrAppDataInfo->PtrAuxData,sizeof(localUnicodeChar)-1);

	while(!anomalia)
	{
		if(vSearch==0)
		{
			// Siamo nella frase giusta
			indiceWrTesto=0;

			while((localUnicodeChar[indiceLUC] && localUnicodeChar[indiceLUC]!=0x000a) && indiceWrTesto<(maxChars-1))
			{
				ptrUnicodeDest[indiceWrTesto++] = localUnicodeChar[indiceLUC];
				indiceLUC++;
			}
			ptrUnicodeDest[indiceWrTesto]=0;	// Termino la stringa
			break;

		}
		else
		{
			while(localUnicodeChar[indiceLUC] && (localUnicodeChar[indiceLUC]!=0x000a))
			{
				indiceLUC++;
			}

			if(localUnicodeChar[indiceLUC]==0)
			{	// Anomalia e cmq in ogni caso numero frasi inferiore al valore
				anomalia = true;
				break;
			}
			else if(localUnicodeChar[indiceLUC]==0x000a)
			{
				indiceLUC++;	// Salto il 0x000a
				if(vSearch)
					vSearch--;
			}
		}
	}

	if(anomalia)
		Unicode::strncpy(ptrUnicodeDest,	"XxX!", maxChars );


}

// Ritorna il rapporto di moltiplicazione da applicare sia al width che height originale in base
// alle dimensioni massime passate. 0 NON è concesso
float GuiUtil::Riproporziona(uint16_t widthMax,uint16_t heightMax,int16_t widthOrg,int16_t heightOrg)
{
	float iRW,iRH;
	float valRet;

	if(widthOrg==0)
		widthOrg=1;
	if(heightOrg==0)
		heightOrg=1;

	iRW = widthMax;
	iRW /= widthOrg;

	iRH = heightMax;
	iRH /= heightOrg;

	if((widthMax!=0xffff) && (heightMax==0xffff))
	{
		valRet = iRW;
	}
	else if((heightMax!=0xffff) && (widthMax==0xffff))
	{
		valRet = iRH;
	}
	else
	{
		if(iRW>iRH)
			valRet = iRH;
		else
			valRet = iRW;
	}

	return valRet;
}

*/
