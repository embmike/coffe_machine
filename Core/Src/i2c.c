/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c4;

/* I2C4 init function */
HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *phi2c, uint32_t timing)
{
   I2C_HandleTypeDef *i2cHandle = phi2c;

   /* USER CODE BEGIN I2C4_Init 0 */
   if (i2cHandle == NULL)
   {
      return HAL_ERROR;
   }

   /* USER CODE END I2C4_Init 0 */

   /* USER CODE BEGIN I2C4_Init 1 */

   /* USER CODE END I2C4_Init 1 */
   i2cHandle->Instance = I2C4;
   i2cHandle->Init.Timing = timing;
   i2cHandle->Init.OwnAddress1 = 0;
   i2cHandle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   i2cHandle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   i2cHandle->Init.OwnAddress2 = 0;
   i2cHandle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
   i2cHandle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   i2cHandle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
   if (HAL_I2C_Init(i2cHandle) != HAL_OK)
   {
      return HAL_ERROR;
   }

   /** Configure Analogue filter
   */
   if (HAL_I2CEx_ConfigAnalogFilter(i2cHandle, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
   {
      return HAL_ERROR;
   }

   /** Configure Digital filter
   */
   if (HAL_I2CEx_ConfigDigitalFilter(i2cHandle, 0) != HAL_OK)
   {
      return HAL_ERROR;
   }
   /* USER CODE BEGIN I2C4_Init 2 */

   hi2c4 = *i2cHandle;

   /* USER CODE END I2C4_Init 2 */

   return HAL_OK;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

   GPIO_InitTypeDef GPIO_InitStruct = { 0 };
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
   if (i2cHandle->Instance == I2C4)
   {
      /* USER CODE BEGIN I2C4_MspInit 0 */

      /* USER CODE END I2C4_MspInit 0 */

      /** Initializes the peripherals clock
      */
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
      PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
      {
         Error_Handler();
      }

      __HAL_RCC_GPIOD_CLK_ENABLE();
      /**I2C4 GPIO Configuration
      PD12     ------> I2C4_SCL
      PD13     ------> I2C4_SDA
      */
      GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

      /* I2C4 clock enable */
      __HAL_RCC_I2C4_CLK_ENABLE();
      /* USER CODE BEGIN I2C4_MspInit 1 */

      /* USER CODE END I2C4_MspInit 1 */
   }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

   if (i2cHandle->Instance == I2C4)
   {
      /* USER CODE BEGIN I2C4_MspDeInit 0 */

      /* USER CODE END I2C4_MspDeInit 0 */
        /* Peripheral clock disable */
      __HAL_RCC_I2C4_CLK_DISABLE();

      /**I2C4 GPIO Configuration
      PD12     ------> I2C4_SCL
      PD13     ------> I2C4_SDA
      */
      HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);

      HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);

      /* USER CODE BEGIN I2C4_MspDeInit 1 */

      /* USER CODE END I2C4_MspDeInit 1 */
   }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
