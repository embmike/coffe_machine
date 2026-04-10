/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
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
#include "quadspi.h"

/* USER CODE BEGIN 0 */
#include "stm32h750b_discovery_qspi.h"
/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

   /* USER CODE BEGIN QUADSPI_Init 0 */
   BSP_QSPI_Init_t qspi_initParams;

   /* USER CODE END QUADSPI_Init 0 */

   /* USER CODE BEGIN QUADSPI_Init 1 */

   /* USER CODE END QUADSPI_Init 1 */
   hqspi.Instance = QUADSPI;
   hqspi.Init.ClockPrescaler = 7;
   hqspi.Init.FifoThreshold = 4;
   hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
   hqspi.Init.FlashSize = 25;
   hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
   hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
   hqspi.Init.DualFlash = QSPI_DUALFLASH_ENABLE;
   if (HAL_QSPI_Init(&hqspi) != HAL_OK)
   {
      Error_Handler();
   }
   /* USER CODE BEGIN QUADSPI_Init 2 */
   qspi_initParams.InterfaceMode = MT25TL01G_QPI_MODE;
   qspi_initParams.TransferRate = MT25TL01G_DTR_TRANSFER;
   qspi_initParams.DualFlashMode = MT25TL01G_DUALFLASH_ENABLE;

   BSP_QSPI_DeInit(0);
   if (BSP_QSPI_Init(0, &qspi_initParams) != BSP_ERROR_NONE)
   {
      Error_Handler();
   }

   if (BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
   {
      Error_Handler();
   }
   /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

   GPIO_InitTypeDef GPIO_InitStruct = { 0 };
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
   if (qspiHandle->Instance == QUADSPI)
   {
      /* USER CODE BEGIN QUADSPI_MspInit 0 */

      /* USER CODE END QUADSPI_MspInit 0 */

      /** Initializes the peripherals clock
      */
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
      PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
      {
         Error_Handler();
      }

      /* QUADSPI clock enable */
      __HAL_RCC_QSPI_CLK_ENABLE();

      __HAL_RCC_GPIOG_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOE_CLK_ENABLE();
      __HAL_RCC_GPIOF_CLK_ENABLE();
      __HAL_RCC_GPIOH_CLK_ENABLE();
      /**QUADSPI GPIO Configuration
      PG9     ------> QUADSPI_BK2_IO2
      PC10     ------> QUADSPI_BK1_IO1
      PB6     ------> QUADSPI_BK1_NCS
      PC11     ------> QUADSPI_BK2_NCS
      PE2     ------> QUADSPI_BK1_IO2
      PG14     ------> QUADSPI_BK2_IO3
      PC9     ------> QUADSPI_BK1_IO0
      PF6     ------> QUADSPI_BK1_IO3
      PF10     ------> QUADSPI_CLK
      PH2     ------> QUADSPI_BK2_IO0
      PH3     ------> QUADSPI_BK2_IO1
      */
      GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_14;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
      HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_6;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_2;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
      HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_10;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
      HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
      HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

      /* USER CODE BEGIN QUADSPI_MspInit 1 */

      /* USER CODE END QUADSPI_MspInit 1 */
   }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

   if (qspiHandle->Instance == QUADSPI)
   {
      /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

      /* USER CODE END QUADSPI_MspDeInit 0 */
        /* Peripheral clock disable */
      __HAL_RCC_QSPI_CLK_DISABLE();

      /**QUADSPI GPIO Configuration
      PG9     ------> QUADSPI_BK2_IO2
      PC10     ------> QUADSPI_BK1_IO1
      PB6     ------> QUADSPI_BK1_NCS
      PC11     ------> QUADSPI_BK2_NCS
      PE2     ------> QUADSPI_BK1_IO2
      PG14     ------> QUADSPI_BK2_IO3
      PC9     ------> QUADSPI_BK1_IO0
      PF6     ------> QUADSPI_BK1_IO3
      PF10     ------> QUADSPI_CLK
      PH2     ------> QUADSPI_BK2_IO0
      PH3     ------> QUADSPI_BK2_IO1
      */
      HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9 | GPIO_PIN_14);

      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_9);

      HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

      HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);

      HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6 | GPIO_PIN_10);

      HAL_GPIO_DeInit(GPIOH, GPIO_PIN_2 | GPIO_PIN_3);

      /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

      /* USER CODE END QUADSPI_MspDeInit 1 */
   }
}

/* USER CODE BEGIN 1 */
HAL_StatusTypeDef QSPI_EnableMemoryMappedMode(void)
{
   if (BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
   {
      return HAL_ERROR;
   }

   __DSB();
   __ISB();
   SCB_InvalidateICache();
   SCB_InvalidateDCache();
   __DSB();
   __ISB();

   return HAL_OK;
}
/* USER CODE END 1 */
