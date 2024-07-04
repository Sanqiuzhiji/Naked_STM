/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INA22_Pin GPIO_PIN_2
#define INA22_GPIO_Port GPIOE
#define INB11_Pin GPIO_PIN_3
#define INB11_GPIO_Port GPIOE
#define INB12_Pin GPIO_PIN_4
#define INB12_GPIO_Port GPIOE
#define PWM2_Pin GPIO_PIN_5
#define PWM2_GPIO_Port GPIOE
#define PWM3_Pin GPIO_PIN_6
#define PWM3_GPIO_Port GPIOE
#define B1_Encoding1_Pin GPIO_PIN_0
#define B1_Encoding1_GPIO_Port GPIOA
#define B1_Encoding2_Pin GPIO_PIN_1
#define B1_Encoding2_GPIO_Port GPIOA
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOA
#define NRF_SCK_Pin GPIO_PIN_5
#define NRF_SCK_GPIO_Port GPIOA
#define NRF_MISO_Pin GPIO_PIN_6
#define NRF_MISO_GPIO_Port GPIOA
#define NRF_MOSI_Pin GPIO_PIN_7
#define NRF_MOSI_GPIO_Port GPIOA
#define NRF_CE_Pin GPIO_PIN_0
#define NRF_CE_GPIO_Port GPIOB
#define NRF_IQR_Pin GPIO_PIN_1
#define NRF_IQR_GPIO_Port GPIOB
#define A2_Encoding2_Pin GPIO_PIN_9
#define A2_Encoding2_GPIO_Port GPIOE
#define A2_Encoding1_Pin GPIO_PIN_11
#define A2_Encoding1_GPIO_Port GPIOE
#define INA11_Pin GPIO_PIN_8
#define INA11_GPIO_Port GPIOD
#define INA12_Pin GPIO_PIN_9
#define INA12_GPIO_Port GPIOD
#define INB21_Pin GPIO_PIN_10
#define INB21_GPIO_Port GPIOD
#define INB22_Pin GPIO_PIN_11
#define INB22_GPIO_Port GPIOD
#define PWM1_Pin GPIO_PIN_12
#define PWM1_GPIO_Port GPIOD
#define PWM4_Pin GPIO_PIN_13
#define PWM4_GPIO_Port GPIOD
#define B2_Encoding2_Pin GPIO_PIN_15
#define B2_Encoding2_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOD
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOD
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOD
#define SW4_Pin GPIO_PIN_4
#define SW4_GPIO_Port GPIOD
#define SW5_Pin GPIO_PIN_5
#define SW5_GPIO_Port GPIOD
#define B2_Encoding1_Pin GPIO_PIN_3
#define B2_Encoding1_GPIO_Port GPIOB
#define A1_Encoding1_Pin GPIO_PIN_4
#define A1_Encoding1_GPIO_Port GPIOB
#define A1_Encoding2_Pin GPIO_PIN_5
#define A1_Encoding2_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define INA21_Pin GPIO_PIN_1
#define INA21_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
