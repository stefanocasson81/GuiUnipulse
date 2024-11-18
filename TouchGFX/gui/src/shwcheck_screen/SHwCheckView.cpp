#include <gui/shwcheck_screen/SHwCheckView.hpp>

extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Viper/Common/ViperDef.h"
	#include "CORELib/Viper/UserInterface/ViperUI.h"
	#include "ARMLib/GUI/TGFX/TGFXCustom.h"
	#include "ARMLib/Common/SwTimer.h"
	#include "_Board.h"
	#include "ARMLib/Common/CustomString.h"
};

SHwCheckView::SHwCheckView()
{
	OldTickCounter = 0;
	OldTickCounterFast = 0;
	OldADCIo = 0xffff;
	OldADCVo = 0xffff;
	OldDACCnt = 0xffff;

	cShow8BitOutA1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[0]);
	cShow8BitOutA0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[1]);
	cShow8BitOutB1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[2]);
	cShow8BitOutB0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[3]);
	cShow8BitOutC1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[4]);
	cShow8BitOutC0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[5]);
	cShow8BitOutD1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[6]);
	cShow8BitOutD0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut0[7]);
	cShow8BitOutE1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[0]);
	cShow8BitOutE0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[1]);
	cShow8BitOutF1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[2]);
	cShow8BitOutF0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[3]);
	cShow8BitOutG1.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[4]);
	cShow8BitOutG0.SetPtrtData(&viperdef_GetTx_HwCheckPortOut1[5]);

	cShow8BitInA1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[0]);
	cShow8BitInA0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[1]);
	cShow8BitInB1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[2]);
	cShow8BitInB0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[3]);
	cShow8BitInC1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[4]);
	cShow8BitInC0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[5]);
	cShow8BitInD1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[6]);
	cShow8BitInD0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn0[7]);
	cShow8BitInE1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[0]);
	cShow8BitInE0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[1]);
	cShow8BitInF1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[2]);
	cShow8BitInF0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[3]);
	cShow8BitInG1.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[4]);
	cShow8BitInG0.SetPtrtData(&viperdef_GetTx_HwCheckPortIn1[5]);
}

void SHwCheckView::setupScreen()
{
    SHwCheckViewBase::setupScreen();
}

void SHwCheckView::tearDownScreen()
{
    SHwCheckViewBase::tearDownScreen();
}

void SHwCheckView::handleTickEvent()
{
	char tmpStr[20];
	uint16_t uSA;

	//________________________________________________________Valuta dove andare
	if(tgfxcustom_GetPressioneEncoder(TGFXCUSTOM_ENCODER_LEFT)==TGFXCUSTOM_PRESSIONEENCODER_RILASCIATOCORTO)
		application().VisualizzaSMenuPrincipale();

	cShow8BitOutA1.ManagerTickEvent();
	cShow8BitOutA0.ManagerTickEvent();
	cShow8BitOutB1.ManagerTickEvent();
	cShow8BitOutB0.ManagerTickEvent();
	cShow8BitOutC1.ManagerTickEvent();
	cShow8BitOutC0.ManagerTickEvent();
	cShow8BitOutD1.ManagerTickEvent();
	cShow8BitOutD0.ManagerTickEvent();
	cShow8BitOutE1.ManagerTickEvent();
	cShow8BitOutE0.ManagerTickEvent();
	cShow8BitOutF1.ManagerTickEvent();
	cShow8BitOutF0.ManagerTickEvent();
	cShow8BitOutG1.ManagerTickEvent();
	cShow8BitOutG0.ManagerTickEvent();

	cShow8BitInA1.ManagerTickEvent();
	cShow8BitInA0.ManagerTickEvent();
	cShow8BitInB1.ManagerTickEvent();
	cShow8BitInB0.ManagerTickEvent();
	cShow8BitInC1.ManagerTickEvent();
	cShow8BitInC0.ManagerTickEvent();
	cShow8BitInD1.ManagerTickEvent();
	cShow8BitInD0.ManagerTickEvent();
	cShow8BitInE1.ManagerTickEvent();
	cShow8BitInE0.ManagerTickEvent();
	cShow8BitInF1.ManagerTickEvent();
	cShow8BitInF0.ManagerTickEvent();
	cShow8BitInG1.ManagerTickEvent();
	cShow8BitInG0.ManagerTickEvent();

	memcpy(&uSA,&viperdef_GetTx_HwCheckAdc0[0],2);
	if(OldADCIo!=uSA)
	{
		OldADCIo = uSA;
		customstring_FormatFix(tmpStr, OldADCIo,5,0, ' ',',', false);
		Unicode::strncpy(tAADCIoBuffer,tmpStr, TAADCIO_SIZE-1);
		tAADCIo.invalidate();
	}

	memcpy(&uSA,&viperdef_GetTx_HwCheckAdc0[2],2);
	if(OldADCVo!=uSA)
	{
		OldADCVo = uSA;
		customstring_FormatFix(tmpStr, OldADCVo,5,0, ' ',',', false);
		Unicode::strncpy(tAADCVoBuffer,tmpStr, TAADCVO_SIZE-1);
		tAADCVo.invalidate();
	}

	memcpy(&uSA,&viperdef_GetTx_HwCheckDac[0],2);
	if(OldDACCnt!=uSA)
	{
		OldDACCnt = uSA;
		customstring_FormatFix(tmpStr, OldDACCnt,5,0, ' ',',', false);
		Unicode::strncpy(tADACCntBuffer,tmpStr, TADACCNT_SIZE-1);
		tADACCnt.invalidate();
	}

	if((swtimer_TickCounter-OldTickCounterFast)>100)
	{
		OldTickCounterFast = swtimer_TickCounter;

		_board_CAN_ReqRemoteId(0x100);	//niknik da togliere


		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKADC0);
		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKADC1);
		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKDAC);

		dGADCIo.addDataPoint((int)OldADCIo);
		dGADCVo.addDataPoint((int)OldADCVo);
		dGDACCnt.addDataPoint((int)OldDACCnt);
	}

	if((swtimer_TickCounter-OldTickCounter)>250)
	{
		OldTickCounter = swtimer_TickCounter;

		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT0);
		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTOUT1);
		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN0);
		_board_CAN_ReqRemoteId(VIPERDEF_BASECANID_GENTX+VIPERDEF_CANID_GEN_HWCHECKPORTIN1);
	}


	customstring_FormatFix(tmpStr, _board_Info.PhyEncoder[_BOARD_JOY_LEFT],6,0, ' ',',', true);
	Unicode::strncpy(tAEncSXBuffer,tmpStr, TAENCSX_SIZE-1);
	tAEncSX.invalidate();

	customstring_FormatFix(tmpStr, _board_Info.PhyEncoder[_BOARD_JOY_RIGHT],6,0, ' ',',', true);
	Unicode::strncpy(tAEncDXBuffer,tmpStr, TAENCDX_SIZE-1);
	tAEncDX.invalidate();
}

