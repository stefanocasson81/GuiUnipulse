#include <gui/containers/CAllarmi.hpp>

extern "C"
{
	#include "CORELib/Viper/UserInterface/ViperUI.h"
	#include "CORELib/Viper/Common/ViperDef.h"
//	#include "ARMLib/Common/CustomString.h"
//	#include "ARMLib/Common/SwTimer.h"
};

CAllarmi::CAllarmi()
{
	AnomalieTrainoShow.TrainoAnomalie=0xff;
	AnomalieBoardShow.GenAnomalieBoard=0xff;
	AnomaliePowerShow.GenAnomaliePower=0xff;
}

void CAllarmi::initialize()
{
    CAllarmiBase::initialize();
}

// Ritorna true se deve essre rimesso in testa alla visualizzazione
bool CAllarmi::ManagerTickEvent(void)
{
	bool valRet=false;
	char tmpStr[20];

	if(viperdef_Pack8GenTx_InfoStatus.StatoMacchina==VIPERDEF_STATOMACCHINA_ALLARME)
	{
		if(!isVisible())
		{
			valRet = true;
			setVisible(true);
			invalidate();
		}
	}
	else
	{
		if(isVisible())
		{
			setVisible(false);
			invalidate();
		}
	}

	if(isVisible())
	{
		if(AnomalieTrainoShow.TrainoAnomalie!=viperdef_Pack8GenTx_InfoStatus.TrainoAnomalie.TrainoAnomalie)
		{
			AnomalieTrainoShow.TrainoAnomalie=viperdef_Pack8GenTx_InfoStatus.TrainoAnomalie.TrainoAnomalie;

			sprintf(tmpStr,"%8X",viperdef_Pack8GenTx_InfoStatus.TrainoAnomalie.TrainoAnomalie);
			Unicode::strncpy(tATrainoBuffer,tmpStr, TATRAINO_SIZE);
			tATraino.invalidate();
		}

		if(AnomalieBoardShow.GenAnomalieBoard!=viperdef_Pack8GenTx_InfoStatus.GenAnomalieBoard.GenAnomalieBoard)
		{
			AnomalieBoardShow=viperdef_Pack8GenTx_InfoStatus.GenAnomalieBoard;

			sprintf(tmpStr,"%8X",viperdef_Pack8GenTx_InfoStatus.GenAnomalieBoard.GenAnomalieBoard);
			Unicode::strncpy(tABoardBuffer,tmpStr, TABOARD_SIZE);
			tATraino.invalidate();
		}

		if(AnomaliePowerShow.GenAnomaliePower!=viperdef_Pack8GenTx_InfoStatus.GenAnomaliePower.GenAnomaliePower)
		{
			AnomaliePowerShow=viperdef_Pack8GenTx_InfoStatus.GenAnomaliePower;

			sprintf(tmpStr,"%8X",viperdef_Pack8GenTx_InfoStatus.GenAnomaliePower.GenAnomaliePower);
			Unicode::strncpy(tAPowerBuffer,tmpStr, TAPOWER_SIZE);
			tATraino.invalidate();
		}
	}

	return valRet;
}
