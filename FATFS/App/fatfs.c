/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */

/* USER CODE BEGIN Variables */
#include "ARMLib/Common/CustomRtc.h"

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	DWORD valRet;
	customrtc_Time_t customRtcTime;
	uint32_t uLA;

	customrtc_GetTime(&customRtcTime);

	if(customRtcTime.Anno<1980)
		customRtcTime.Anno = 1980;

	uLA = customRtcTime.Anno-1980;
	valRet = (uLA&0x7f)<<25;
	uLA = customRtcTime.Mese+1;
	valRet |= (uLA&0x0f)<<21;
	uLA = customRtcTime.GiornoDelMese;
	valRet |= (uLA&0x1f)<<16;
	uLA = customRtcTime.Ore;
	valRet |= (uLA&0x1f)<<11;
	uLA = customRtcTime.Minuti;
	valRet |= (uLA&0x3f)<<5;
	uLA = customRtcTime.Secondi;
	valRet |= (uLA&0x3f)>>1;

	return valRet;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
