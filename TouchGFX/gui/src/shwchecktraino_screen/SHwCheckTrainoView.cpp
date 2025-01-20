#include <gui/shwchecktraino_screen/SHwCheckTrainoView.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Common/ViperDef.h"
	#include "CORELib/UserInterface/ViperUI.h"
	#include "ARMLib/TGFX/TGFXCustom.h"
	#include "ARMLib/Common/SwTimer.h"
	#include "_Board.h"
	#include "ARMLib/Common/CustomString.h"
};

SHwCheckTrainoView::SHwCheckTrainoView()
{
	OldTickCounter = 0;
	OldDVBus = 0xffff;
	OldDVEvGas = 0xffff;
	OldMAEvGas = 0xffff;
	OldDVMot = 0xffff;
	OldMAMot = 0xffff;
	OldMMVelMot = 0xffff;

	memset(&OldTrainoInfoStatus,0xff,sizeof(OldTrainoInfoStatus));
}

void SHwCheckTrainoView::setupScreen()
{
    SHwCheckTrainoViewBase::setupScreen();
}

void SHwCheckTrainoView::tearDownScreen()
{
    SHwCheckTrainoViewBase::tearDownScreen();
}

void SHwCheckTrainoView::handleTickEvent()
{
	char tmpStr[20];
	uint16_t uSA;

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(ENCODER_LEFT)==ENCODER_PRESSION_RELEASED)
		application().VisualizzaSMenuPrincipale();

	if((swtimer_TickCounter-OldTickCounter)>100)
	{
		OldTickCounter = swtimer_TickCounter;

		_board_CAN_ReqRemoteId(VIPERDEF_CANID_TRAINOTX_HWSTATUS);
		_board_CAN_ReqRemoteId(VIPERDEF_CANID_TRAINOTX_HWSTATUS2);

		uSA = viperdef_Pack8TrainoTX_HWStatus.DVBus;
		dGVBus.addDataPoint((int)uSA);
		if(OldDVBus!=uSA)
		{
			OldDVBus=uSA;
			customstring_FormatFix(tmpStr, uSA,3,1, ' ',',', false);
			Unicode::strncpy(tAVBusBuffer,tmpStr, TAVBUS_SIZE-1);
			tAVBus.invalidate();
		}

		uSA = viperdef_Pack8TrainoTX_HWStatus.DVEvGas;
		dGVEvGas.addDataPoint((int)uSA);
		if(OldDVEvGas!=uSA)
		{
			OldDVEvGas=uSA;
			customstring_FormatFix(tmpStr, uSA,3,1, ' ',',', false);
			Unicode::strncpy(tAVEvGasBuffer,tmpStr, TAVEVGAS_SIZE-1);
			tAVEvGas.invalidate();
		}

		uSA = viperdef_Pack8TrainoTX_HWStatus.MAEvGas;
		dGAEvGas.addDataPoint((int)uSA);
		if(OldMAEvGas!=uSA)
		{
			OldMAEvGas=uSA;
			customstring_FormatFix(tmpStr, uSA,4,0, ' ',',', false);
			Unicode::strncpy(tAAEvGasBuffer,tmpStr, TAAEVGAS_SIZE-1);
			tAAEvGas.invalidate();
		}

		uSA = viperdef_Pack8TrainoTX_HWStatus.MMinVelMot/100;
		dGMMVelMot.addDataPoint((int)uSA);
		if(OldMAEvGas!=uSA)
		{
			OldMMVelMot=uSA;
			customstring_FormatFix(tmpStr, uSA,2,1, ' ',',', false);
			Unicode::strncpy(tAMMVelMotBuffer,tmpStr, TAMMVELMOT_SIZE-1);
			tAMMVelMot.invalidate();
		}

		uSA = viperdef_Pack8TrainoTX_HWStatus2.DVMot;
		dGVMot.addDataPoint((int)uSA);
		if(OldDVMot!=uSA)
		{
			OldDVMot=uSA;
			customstring_FormatFix(tmpStr, uSA,3,1, ' ',',', false);
			Unicode::strncpy(tAVMotBuffer,tmpStr, TAVMOT_SIZE-1);
			tAVMot.invalidate();
		}

		uSA = viperdef_Pack8TrainoTX_HWStatus2.MAMot;
		dGAMot.addDataPoint((int)uSA);
		if(OldMAMot!=uSA)
		{
			OldMAMot=uSA;
			customstring_FormatFix(tmpStr, uSA,4,0, ' ',',', false);
			Unicode::strncpy(tAAMotBuffer,tmpStr, TAAMOT_SIZE-1);
			tAAMot.invalidate();
		}

		customstring_FormatFix(tmpStr, viperdef_Pack8TrainoTX_HWStatus2.AdcCadPot1,5,0, ' ',',', false);
		Unicode::strncpy(tACadPot1Buffer,tmpStr, TACADPOT1_SIZE-1);
		tACadPot1.invalidate();

		customstring_FormatFix(tmpStr, viperdef_Pack8TrainoTX_HWStatus2.AdcCadPot2,5,0, ' ',',', false);
		Unicode::strncpy(tACadPot2Buffer,tmpStr, TACADPOT2_SIZE-1);
		tACadPot2.invalidate();
	}

	if(memcmp(&OldTrainoInfoStatus,&viperdef_Pack8TrainoTX_InfoStatus,sizeof(OldTrainoInfoStatus)))
	{
		memcpy(&OldTrainoInfoStatus,&viperdef_Pack8TrainoTX_InfoStatus,sizeof(OldTrainoInfoStatus));

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.PulsanteTorcia1)
			Unicode::strncpy(tAPT1Buffer,"Press", TAPT1_SIZE-1);
		else
			Unicode::strncpy(tAPT1Buffer," ", TAPT1_SIZE-1);
		tAPT1.invalidate();

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.PulsanteTorcia2)
			Unicode::strncpy(tAPT2Buffer,"Press", TAPT2_SIZE-1);
		else
			Unicode::strncpy(tAPT2Buffer," ", TAPT2_SIZE-1);
		tAPT2.invalidate();

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.TrainoInMovimento)
			Unicode::strncpy(tAMotMoveBuffer,"Moving", TAMOTMOVE_SIZE-1);
		else
			Unicode::strncpy(tAMotMoveBuffer," ", TAMOTMOVE_SIZE-1);
		tAMotMove.invalidate();

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.PulsanteAvanzaFilo)
			Unicode::strncpy(tAPAvFiloBuffer,"Press", TAPAVFILO_SIZE-1);
		else
			Unicode::strncpy(tAPAvFiloBuffer," ", TAPAVFILO_SIZE-1);
		tAPAvFilo.invalidate();

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.PulsanteTestGas)
			Unicode::strncpy(tAPGasBuffer,"Press", TAPGAS_SIZE-1);
		else
			Unicode::strncpy(tAPGasBuffer," ", TAPGAS_SIZE-1);
		tAPGas.invalidate();

		if(viperdef_Pack8TrainoTX_InfoStatus.Info.EvGasPilotata)
			Unicode::strncpy(tACmdEvGasBuffer,"Open", TACMDEVGAS_SIZE-1);
		else
			Unicode::strncpy(tACmdEvGasBuffer," ", TACMDEVGAS_SIZE-1);
		tACmdEvGas.invalidate();


		if(viperdef_Pack8TrainoTX_InfoStatus.Info.CadOn)
			Unicode::strncpy(tACadOnBuffer,"ON", TACADON_SIZE-1);
		else
			Unicode::strncpy(tACadOnBuffer," ", TACADON_SIZE-1);
		tACadOn.invalidate();
		if(viperdef_Pack8TrainoTX_InfoStatus.Info.CadUp)
			Unicode::strncpy(tACadUpBuffer,"ON", TACADUP_SIZE-1);
		else
			Unicode::strncpy(tACadUpBuffer," ", TACADUP_SIZE-1);
		tACadUp.invalidate();
		if(viperdef_Pack8TrainoTX_InfoStatus.Info.CadDown)
			Unicode::strncpy(tACadDownBuffer,"ON", TACADDOWN_SIZE-1);
		else
			Unicode::strncpy(tACadDownBuffer," ", TACADDOWN_SIZE-1);
		tACadDown.invalidate();
		if(viperdef_Pack8TrainoTX_InfoStatus.Info.CadLeft)
			Unicode::strncpy(tACadLeftBuffer,"ON", TACADLEFT_SIZE-1);
		else
			Unicode::strncpy(tACadLeftBuffer," ", TACADLEFT_SIZE-1);
		tACadLeft.invalidate();
		if(viperdef_Pack8TrainoTX_InfoStatus.Info.CadRight)
			Unicode::strncpy(tACadRightBuffer,"ON", TACADRIGHT_SIZE-1);
		else
			Unicode::strncpy(tACadRightBuffer," ", TACADRIGHT_SIZE-1);
		tACadRight.invalidate();
	}
}
