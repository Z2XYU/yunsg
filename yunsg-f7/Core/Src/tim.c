/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim4;

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 216-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**TIM4 GPIO Configuration
    PD12     ------> TIM4_CH1
    PD13     ------> TIM4_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /**TIM4 GPIO Configuration
    PD12     ------> TIM4_CH1
    PD13     ------> TIM4_CH2
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12|GPIO_PIN_13);

    /* TIM4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#include "hc_sr04.h"
#include "user_TaskInit.h"
#include "stdio.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  HC_SR04_Sensor_t *sensor = &hc_sr04_sensor[hc_sr04_selected_id];
  // 控制操作每次是单次的 直接判断这个id
  if (htim->Instance == sensor->tim.htim->Instance)
  {
    if (sensor->cap_data.is_first_capture == 0)
    {
      // 记录上升沿
      sensor->cap_data.rising_edge_val = HAL_TIM_ReadCapturedValue(sensor->tim.htim, sensor->tim.tim_channel);
      // 切换输入捕获为下降沿
      __HAL_TIM_SET_CAPTUREPOLARITY(sensor->tim.htim, sensor->tim.tim_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
      // 将首次捕获的值反转
      sensor->cap_data.is_first_capture = 1;
    }
    else
    {
      // 记录下降沿
      sensor->cap_data.falling_edge_val = HAL_TIM_ReadCapturedValue(sensor->tim.htim, sensor->tim.tim_channel);
      // 关中断
      HAL_TIM_IC_Stop_IT(sensor->tim.htim, sensor->tim.tim_channel);
      // 将输入捕获的检测 重新设置为上升沿
      __HAL_TIM_SET_CAPTUREPOLARITY(sensor->tim.htim, sensor->tim.tim_channel, TIM_INPUTCHANNELPOLARITY_RISING);
      sensor->cap_data.is_first_capture = 0;

      // 计算脉冲宽度(考虑计数器溢出情况)
      if (sensor->cap_data.falling_edge_val >= sensor->cap_data.rising_edge_val)
      {
        sensor->cap_data.echo_time_us = (sensor->cap_data.falling_edge_val - sensor->cap_data.rising_edge_val);
      }
      else
      {
        sensor->cap_data.echo_time_us = (0xFFFF - sensor->cap_data.rising_edge_val + sensor->cap_data.falling_edge_val);
      }

      // 距离计算
      if (sensor->cap_data.echo_time_us > 100 && sensor->cap_data.echo_time_us < 30000) // 约1.7cm~5m
      {
        /*遗留bug 1us 往返距离除以2 其实不是真实值 ，不需要除以2才是真是结果 ，不知道为什么*/
        sensor->cap_data.distance_cm = (sensor->cap_data.echo_time_us * 0.0343f);
      }
      else
      {
        sensor->cap_data.distance_cm = -1; // 标记无效
      }

#if DEBUG_HCSR04
      printf("Rise: %lu Fall: %lu\n", sensor->cap_data.rising_edge_val, sensor->cap_data.falling_edge_val);
      printf("Echo time: %lu us\n", sensor->cap_data.echo_time_us);
      printf("Distance: %d cm\n", (int)sensor->cap_data.distance_cm);

#endif

      osSemaphoreRelease(distReadySemaphoreHandle);
    }
  }
}

/* USER CODE END 1 */
