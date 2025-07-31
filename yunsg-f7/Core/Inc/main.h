/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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
#define PLASMA1_RELAY_Pin GPIO_PIN_2
#define PLASMA1_RELAY_GPIO_Port GPIOE
#define MOTOR_DIR_1_Pin GPIO_PIN_3
#define MOTOR_DIR_1_GPIO_Port GPIOE
#define MOTOR_DIR_2_Pin GPIO_PIN_4
#define MOTOR_DIR_2_GPIO_Port GPIOE
#define PTC1_Pin GPIO_PIN_5
#define PTC1_GPIO_Port GPIOE
#define PTC2_Pin GPIO_PIN_6
#define PTC2_GPIO_Port GPIOE
#define ULTRAVIOLET_SENSOR_Pin GPIO_PIN_1
#define ULTRAVIOLET_SENSOR_GPIO_Port GPIOC
#define MOTOR_STEP_1_Pin GPIO_PIN_0
#define MOTOR_STEP_1_GPIO_Port GPIOA
#define MOTOR_STEP_2_Pin GPIO_PIN_1
#define MOTOR_STEP_2_GPIO_Port GPIOA
#define RC522_CS_Pin GPIO_PIN_4
#define RC522_CS_GPIO_Port GPIOA
#define RC522_SCK_Pin GPIO_PIN_5
#define RC522_SCK_GPIO_Port GPIOA
#define RC522_MISO_Pin GPIO_PIN_6
#define RC522_MISO_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define RC522_RST_Pin GPIO_PIN_2
#define RC522_RST_GPIO_Port GPIOB
#define ETC2_Pin GPIO_PIN_14
#define ETC2_GPIO_Port GPIOB
#define ETC1_Pin GPIO_PIN_15
#define ETC1_GPIO_Port GPIOB
#define HC_SR04_Trig1_Pin GPIO_PIN_11
#define HC_SR04_Trig1_GPIO_Port GPIOD
#define HC_SR04_Trig2_Pin GPIO_PIN_3
#define HC_SR04_Trig2_GPIO_Port GPIOG
#define PLASMA2_RELAY_Pin GPIO_PIN_3
#define PLASMA2_RELAY_GPIO_Port GPIOI
#define RC522_MOSI_Pin GPIO_PIN_7
#define RC522_MOSI_GPIO_Port GPIOD
#define MOTOR_EN_2_Pin GPIO_PIN_9
#define MOTOR_EN_2_GPIO_Port GPIOG
#define MOTOR_EN_1_Pin GPIO_PIN_10
#define MOTOR_EN_1_GPIO_Port GPIOG
#define FAN_RELAY_Pin GPIO_PIN_12
#define FAN_RELAY_GPIO_Port GPIOG
#define LCD_BL_Pin GPIO_PIN_5
#define LCD_BL_GPIO_Port GPIOB
#define PCA9685_SCL_Pin GPIO_PIN_6
#define PCA9685_SCL_GPIO_Port GPIOB
#define PCA9685_SDA_Pin GPIO_PIN_7
#define PCA9685_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
