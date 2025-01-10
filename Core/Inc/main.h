/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCODERDX_A_Pin GPIO_PIN_0
#define ENCODERDX_A_GPIO_Port GPIOA
#define ENCODERDX_B_Pin GPIO_PIN_1
#define ENCODERDX_B_GPIO_Port GPIOA
#define PWM_CICALINO_Pin GPIO_PIN_2
#define PWM_CICALINO_GPIO_Port GPIOA
#define ENCODERCC_P_Pin GPIO_PIN_2
#define ENCODERCC_P_GPIO_Port GPIOB
#define ENCODERSX_P_Pin GPIO_PIN_11
#define ENCODERSX_P_GPIO_Port GPIOD
#define ENCODERSX_A_Pin GPIO_PIN_12
#define ENCODERSX_A_GPIO_Port GPIOD
#define ENCODERDX_P_Pin GPIO_PIN_2
#define ENCODERDX_P_GPIO_Port GPIOG
#define LTDC_PIN31_Pin GPIO_PIN_3
#define LTDC_PIN31_GPIO_Port GPIOG
#define PWM_ILLUMINAZIONELCD_Pin GPIO_PIN_8
#define PWM_ILLUMINAZIONELCD_GPIO_Port GPIOC
#define LTDC_PIN35_Pin GPIO_PIN_12
#define LTDC_PIN35_GPIO_Port GPIOC
#define PULSRETRO_Pin GPIO_PIN_4
#define PULSRETRO_GPIO_Port GPIOD
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOD
#define LED_2_Pin GPIO_PIN_6
#define LED_2_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_10
#define SPI1_CS_GPIO_Port GPIOG
#define USB_PWN_ON_Pin GPIO_PIN_12
#define USB_PWN_ON_GPIO_Port GPIOG
#define ENCODERSX_B_Pin GPIO_PIN_7
#define ENCODERSX_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
