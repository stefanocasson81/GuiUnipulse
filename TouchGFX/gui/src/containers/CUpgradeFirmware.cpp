#include <gui/containers/CUpgradeFirmware.hpp>


extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "ARMLib/Protocolli/HProt/HProtFU.h"
};

CUpgradeFirmware::CUpgradeFirmware()
{
	PtrAbbinamento = _NULL_;
}

void CUpgradeFirmware::initialize()
{
    CUpgradeFirmwareBase::initialize();
}

void CUpgradeFirmware::SetPtrAbbinamento(fwupgrade_Abbinamento_t *ptrAbbinamento)
{
	PtrAbbinamento = ptrAbbinamento;
}

void CUpgradeFirmware::Refresh(void)
{
	char tmpStr[100];
	char tmpStrAux[20];
	int ret;
	uint32_t uLA;
	char stage;

	if(PtrAbbinamento)
	{
		if(hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].Presente)
		{
			if(PtrAbbinamento->NomeFile[0])
			{
				// Giro demmerda per evitare warning snprintf con %s
				ret = snprintf(tmpStr,sizeof(tmpStr)-1,"[File]: %s V%d %02d/%02d/%04d",PtrAbbinamento->NomeFile,PtrAbbinamento->VersioneFile,(int)(PtrAbbinamento->DataFile&0xff),(int)(PtrAbbinamento->DataFile>>8)&0xff,(int)(PtrAbbinamento->DataFile>>16));
				if(ret<0)
					tmpStr[0]='X';
				Unicode::strncpy(tASopraBuffer,tmpStr, TASOPRA_SIZE-1);
				tASopra.setVisible(true);
			}
			else
				tASopra.setVisible(false);

			uLA=hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].FirmwareData;
			sprintf(tmpStrAux,"%02d\\%02d\\%04d",(int)(uLA&0xff),(int)(uLA>>8)&0xff,(int)(uLA>>16));

			stage = (char)(hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].FirmwareVer>>8);
			if(stage==0)
				stage = ' ';

			ret = snprintf(tmpStr,sizeof(tmpStr)-1,"[Board(%d)]:%d_%c%02d  %s  Crc:%08X CUS:%08X",(int)(hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].Address),
																							(int)(hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].CodiceSchedaEsteso%100000),
																							stage,
																							hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].FirmwareVer&0xff,
																							tmpStrAux,
																							(unsigned int)hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].FirmwareCrc32,
																							(unsigned int)hprotfu_Info.InfoBoard[PtrAbbinamento->Tag].CodiceUnivocoScheda);
			if(ret<0)
				tmpStr[0]='X';
			Unicode::strncpy(tASottoBuffer,tmpStr, TASOTTO_SIZE-1);
			tASotto.setVisible(true);
		}
		else
		{
			tASopra.setVisible(false);
			tASotto.setVisible(false);
		}

		tASopra.invalidate();
		tASotto.invalidate();
	}
}
