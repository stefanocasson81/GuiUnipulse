
#include <gui/supgradefirmware_screen/SUpgradeFirmwareView.hpp>




extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "ARMLib/FileSystem/_DrvFileSystemUSB.h"
	#include "ARMLib/Protocolli/HProt/HProtFU.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "ARMLib/Common/FWUpgrade.h"
};


SUpgradeFirmwareView::SUpgradeFirmwareView()
{
}

void SUpgradeFirmwareView::setupScreen()
{
	memset(Abbinamento,0,sizeof(Abbinamento));
	Abbinamento[0].Tag = 2;
	Abbinamento[1].Tag = 3;
	Abbinamento[2].Tag = 4;
	Abbinamento[3].Tag = 5;

	cUpgradeFirmware1.SetPtrAbbinamento(&Abbinamento[0]);
	cUpgradeFirmware2.SetPtrAbbinamento(&Abbinamento[1]);
	cUpgradeFirmware3.SetPtrAbbinamento(&Abbinamento[2]);
	cUpgradeFirmware4.SetPtrAbbinamento(&Abbinamento[3]);

	OldPeripheralsStatus = DRVFILESYSTEM_PERIPHERALSSTATUS_NOTCONFIGURED;	// Tanto per forzare una ricerca

	AggiornaFirmware = false;

	Selezionato = 0;

	rB1.setSelected(true);
	rB2.setSelected(false);
	rB3.setSelected(false);
	rB4.setSelected(false);

	SUpgradeFirmwareViewBase::setupScreen();
}

void SUpgradeFirmwareView::tearDownScreen()
{
    SUpgradeFirmwareViewBase::tearDownScreen();
}

void SUpgradeFirmwareView::handleTickEvent()
{
	static bool PrimoGiro = true;
	fwupgrade_InfoFileCodificato_t infoFileCodificato;
	bool invalida = false;
	uint8_t address;
	char tmpStr[100];
	char* ptrStrFile;
	uint8_t uCA;
	uint32_t codiceSchedaRidotto;
	int16_t sA;

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
		application().VisualizzaSMenuAdvance();	// Torna indietro

	if(tgfxcustom_GetPressioneEncoder(ENCODER_RIGHT)==ENCODER_PRESSION_RELEASED)
		AggiornaFirmware = true;

	uCA = 0;
	sA = tgfxcustom_GetOffsetEncoder(ENCODER_LEFT);
	if(sA>0)
	{
		Selezionato++;
		if(Selezionato>3)
			Selezionato = 3;

		uCA=1;
	}
	else if(sA<0)
	{
		if(Selezionato)
			Selezionato--;

		uCA=1;
	}
	if(uCA)
	{
		switch(Selezionato)
		{
			case 0:
				rB1.setSelected(true);
				rB2.setSelected(false);
				rB3.setSelected(false);
				rB4.setSelected(false);
			break;
			case 1:
				rB1.setSelected(false);
				rB2.setSelected(true);
				rB3.setSelected(false);
				rB4.setSelected(false);
			break;
			case 2:
				rB1.setSelected(false);
				rB2.setSelected(false);
				rB3.setSelected(true);
				rB4.setSelected(false);
			break;
			case 3:
				rB1.setSelected(false);
				rB2.setSelected(false);
				rB3.setSelected(false);
				rB4.setSelected(true);
			break;
		}
	}

	if(OldPeripheralsStatus!=_drvfilesystemusb_Driver.Info.PeripheralsStatus)
	{
		OldPeripheralsStatus=_drvfilesystemusb_Driver.Info.PeripheralsStatus;

		AbbinamentiTrovati = 0;
		invalida = true;

		switch(OldPeripheralsStatus)
		{
			case DRVFILESYSTEM_PERIPHERALSSTATUS_READY:
				memset(Abbinamento,0,sizeof(Abbinamento));
				Abbinamento[0].Tag = 2;
				Abbinamento[1].Tag = 3;
				Abbinamento[2].Tag = 4;
				Abbinamento[3].Tag = 5;

				for(address=2;address<HPROTFU_MAXNUMADDRESS;address++)
				{
					if(hprotfu_Info.InfoBoard[address].Presente)
					{
						// Compongo il nome del file dal codice univoco scheda prendendo gli ultimi due byte e convertendoli in decimale
						snprintf(tmpStr,sizeof(tmpStr)-1,"%05d_*.helvi",(int)hprotfu_Info.InfoBoard[address].CodiceSchedaEsteso%100000);

						ptrStrFile = _drvfilesystemusb_Driver.FFind(true,_NULL_,tmpStr);

						while(ptrStrFile)
						{
							// Il primo beccato lo uso
							fwupgrade_CheckFile(ptrStrFile,&infoFileCodificato,false);
							switch(infoFileCodificato.TipoFile)
							{
								case FWUPGRADE_TIPOFILE_HEX:
								case FWUPGRADE_TIPOFILE_BIN:
									// Estraggo il codice scheda ridotto dal nome file
									codiceSchedaRidotto = (ptrStrFile[0]-0x30)*10000;
									codiceSchedaRidotto += (ptrStrFile[1]-0x30)*1000;
									codiceSchedaRidotto += (ptrStrFile[2]-0x30)*100;
									codiceSchedaRidotto += (ptrStrFile[3]-0x30)*10;
									codiceSchedaRidotto += (ptrStrFile[4]-0x30);

									// Assegno le info alla giusta scheda
									for(uCA=0;uCA<SUPGRADEFIRMWARE_MAXSHOWABBINAMENTO;uCA++)
									{
										if((hprotfu_Info.InfoBoard[2+uCA].Presente) && ((hprotfu_Info.InfoBoard[2+uCA].CodiceSchedaEsteso%100000)==codiceSchedaRidotto))
										{
											Abbinamento[uCA].Crc32File    				= infoFileCodificato.Crc32Originale;
											Abbinamento[uCA].VersioneFile 				= infoFileCodificato.VersioneFirmware;
											Abbinamento[uCA].DataFile 	  				= infoFileCodificato.DataCreazione;
											strncpy(Abbinamento[uCA].NomeFile,ptrStrFile,sizeof(Abbinamento[uCA].NomeFile)-1);
											AbbinamentiTrovati++;	// Niente break, potrebbe essercik più di una scheda con lo stesso codice ridotto
										}
									}
								break;
								default:
								break;
							}

							if(AbbinamentiTrovati>=SUPGRADEFIRMWARE_MAXSHOWABBINAMENTO)
								break;

							ptrStrFile = _drvfilesystemusb_Driver.FFind(false,_NULL_,tmpStr);	// Continua la ricerca
						}
					}
					else
						break;
				}
			break;	// Fine case DRVCUSTOMFS_PERIPHERALSSTATUS_READY:
			case DRVFILESYSTEM_PERIPHERALSSTATUS_DISCONNECT:
				memset(Abbinamento,0,sizeof(Abbinamento));
				Abbinamento[0].Tag = 2;
				Abbinamento[1].Tag = 3;
				Abbinamento[2].Tag = 4;
				Abbinamento[3].Tag = 5;
			break;
			default:
			break;
		}
	}

	if(invalida || PrimoGiro)
	{
		cUpgradeFirmware1.Refresh();
		cUpgradeFirmware2.Refresh();
		cUpgradeFirmware3.Refresh();
		cUpgradeFirmware4.Refresh();
	}


	if(AggiornaFirmware)
	{
		AggiornaFirmware = false;
		if(hprotfu_InfoManager.AggiornaFirmware.StatusLoop==HPROTFU_SLAGGIORNAFIRMWARE_VOID)
		{
			if( hprotfu_Info.InfoBoard[2+Selezionato].Presente && (!hprotfu_InfoManager.AggiornaFirmware.FlgStart))
				//hprotfu_CoreDestroy(HPROTFU_CMD_START,hprotfu_Info.InfoBoard[2+Selezionato].CodiceUnivocoScheda,hprotfu_Info.InfoBoard[2+Selezionato].CodiceSchedaEsteso);
				hprotfu_AggiornaFirmware(HPROTFU_CMD_START,hprotfu_Info.InfoBoard[2+Selezionato].CodiceUnivocoScheda,hprotfu_Info.InfoBoard[2+Selezionato].CodiceSchedaEsteso,Abbinamento[Selezionato].NomeFile);
		}
	}

	if(_drvfilesystemusb_Driver.Info.PeripheralsStatus == DRVFILESYSTEM_PERIPHERALSSTATUS_READY)
	{
		switch(fwupgrade_Info.Status)
		{
			default:
			case FWUPGRADE_STATUS_USBNONRILEVATA:				// Usb NON rilevata
			case FWUPGRADE_STATUS_FILENONRILEVATO:				// Usb rilevata ma file NON rilevato
			case FWUPGRADE_STATUS_FILENONVALIDO:				// File NON valido
			case FWUPGRADE_STATUS_FILEINVERIFICA:				// Verifica del file in corso
				snprintf(tmpStr,sizeof(tmpStr)-1,"Left Joy to start");
			break;
			break;
			case FWUPGRADE_STATUS_FIRMWAREINAGGIORNAMENTO:		// Firmware in aggiornamento
				snprintf(tmpStr,sizeof(tmpStr)-1,"Updating: %s (%3d.%01d)",hprotfu_InfoManager.AggiornaFirmware.PtrStrNomeFile,fwupgrade_Info.DecPercentualeEsecuzione/10,fwupgrade_Info.DecPercentualeEsecuzione%10);
			break;
			case FWUPGRADE_STATUS_FIRMWAREAGGIORNATO:			// Firmware aggiornato
				snprintf(tmpStr,sizeof(tmpStr)-1,"Updated");
			break;
			case FWUPGRADE_STATUS_FIRMWAREERRFLASH:				// Firmware non aggiornato per errore generico in mcuflash
				snprintf(tmpStr,sizeof(tmpStr)-1,"ERROR FLASH!!!");
			break;
			case FWUPGRADE_STATUS_ERROREGENERICO:				// Altro errore generico
				snprintf(tmpStr,sizeof(tmpStr)-1,"GENERIC ERROR!!!");
			break;
			case FWUPGRADE_STATUS_AGGIORNAMENTONONNECESSARIO:	// Aggiornamento non necessario
				snprintf(tmpStr,sizeof(tmpStr)-1,"Update required");
			break;
		}
	}
	else
		snprintf(tmpStr,sizeof(tmpStr)-1,"NO USB");

	Unicode::strncpy(tAInfoBuffer,tmpStr, TAINFO_SIZE-1);
	tAInfo.invalidate();

	PrimoGiro = false;
}
