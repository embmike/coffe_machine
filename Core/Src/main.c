/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include <stdarg.h>
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_WIDTH                          480U
#define LCD_HEIGHT                         272U
#define LCD_FRAMEBUFFER_ADDR               0xD0000000U
#define LCD_FRAMEBUFFER_SIZE_BYTES         (LCD_WIDTH * LCD_HEIGHT * 4U)

#define SDRAM_TIMEOUT                      0x1000U
#define SDRAM_REFRESH_COUNT                0x0603U
#define SDRAM_MODEREG_BURST_LENGTH_1       0x0000U
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL 0x0000U
#define SDRAM_MODEREG_CAS_LATENCY_3        0x0030U
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD 0x0000U
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE 0x0200U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
LTDC_HandleTypeDef hltdc;
UART_HandleTypeDef huart3;
SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
/* USER CODE BEGIN PFP */
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);
static void Display_Reset(void);
static void Display_Backlight_On(void);
static void FillFrameBuffer(uint32_t color);
static void DrawColorBars(void);
static void FlushFrameBuffer(void);
static HAL_StatusTypeDef UartLog(const char *format, ...);
static HAL_StatusTypeDef SDRAM_SelfTest(void);
static void LogLTDCState(void);
static void FatalBlinkLoop(uint16_t delay_ms);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* Place the framebuffer in external SDRAM so the LTDC can scan it directly. */
__attribute__((section(".framebuffer"), aligned(32)))
static uint32_t frame_buffer[LCD_WIDTH * LCD_HEIGHT];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

   /* USER CODE BEGIN 1 */

   /* USER CODE END 1 */

   /* MCU Configuration--------------------------------------------------------*/

   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();

   /* USER CODE BEGIN Init */

   /* USER CODE END Init */

   /* Configure the system clock */
   SystemClock_Config();

   /* USER CODE BEGIN SysInit */

   /* USER CODE END SysInit */

   /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_USART3_UART_Init();

  UartLog("\r\ncoffee_machine display bootstrap\r\n");
  UartLog("SystemCoreClock=%lu Hz\r\n", HAL_RCC_GetSysClockFreq());

  if (SDRAM_SelfTest() != HAL_OK)
  {
    UartLog("SDRAM self-test FAILED\r\n");
    FatalBlinkLoop(100U);
  }

  UartLog("SDRAM self-test passed\r\n");

  MX_LTDC_Init();
  Display_Reset();
  FillFrameBuffer(0xFF000000U);
  DrawColorBars();
  FlushFrameBuffer();
  Display_Backlight_On();
  LogLTDCState();
  UartLog("Display path enabled, framebuffer=%p\r\n", frame_buffer);

   /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
   while (1)
   {
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
   }
   /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
   /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
   /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI
                                   | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

   /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                              | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{
   /* USER CODE BEGIN LTDC_Init 0 */

   /* USER CODE END LTDC_Init 0 */
  LTDC_LayerCfgTypeDef layer_cfg = {0};

   /* USER CODE BEGIN LTDC_Init 1 */

   /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 42;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 554;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }

  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = LCD_WIDTH;
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = LCD_HEIGHT;
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layer_cfg.FBStartAdress = (uint32_t)frame_buffer;
  layer_cfg.ImageWidth = LCD_WIDTH;
  layer_cfg.ImageHeight = LCD_HEIGHT;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &layer_cfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
   /* USER CODE BEGIN LTDC_Init 2 */

   /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
   /* USER CODE BEGIN USART3_Init 0 */

   /* USER CODE END USART3_Init 0 */

   /* USER CODE BEGIN USART3_Init 1 */

   /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
   /* USER CODE BEGIN USART3_Init 2 */

   /* USER CODE END USART3_Init 2 */
}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

   /* USER CODE BEGIN FMC_Init 0 */

   /* USER CODE END FMC_Init 0 */

   FMC_SDRAM_TimingTypeDef sdram_timing = { 0 };

   /* USER CODE BEGIN FMC_Init 1 */

   /* USER CODE END FMC_Init 1 */

   /** Perform the SDRAM1 memory initialization sequence
   */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
   /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
   /* SdramTiming */
  sdram_timing.LoadToActiveDelay = 2;
  sdram_timing.ExitSelfRefreshDelay = 7;
  sdram_timing.SelfRefreshTime = 4;
  sdram_timing.RowCycleDelay = 7;
  sdram_timing.WriteRecoveryTime = 2;
  sdram_timing.RPDelay = 2;
  sdram_timing.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &sdram_timing) != HAL_OK)
  {
    Error_Handler();
  }

   /* USER CODE BEGIN FMC_Init 2 */
   SDRAM_Initialization_Sequence(&hsdram1);

   /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
   /* USER CODE BEGIN MX_GPIO_Init_1 */

   /* USER CODE END MX_GPIO_Init_1 */

   /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
   /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
   /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DISPD7_GPIO_Port, LCD_DISPD7_Pin, GPIO_PIN_SET);
   /*Configure GPIO pin : PH15 */
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

   /*Configure GPIO pin : LCD_DISPD7_Pin */
  GPIO_InitStruct.Pin = LCD_DISPD7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DISPD7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_INT_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : LCD_BL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_Pin;
  HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : LD1_Pin */
  GPIO_InitStruct.Pin = LD1_Pin;
  HAL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);
   /* USER CODE BEGIN MX_GPIO_Init_2 */

   /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
  FMC_SDRAM_CommandTypeDef command = {0};
  uint32_t mode_register = 0;

  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  if (HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(1);

  command.CommandMode = FMC_SDRAM_CMD_PALL;
  if (HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler();
  }

  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.AutoRefreshNumber = 8;
  if (HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler();
  }

  mode_register = SDRAM_MODEREG_BURST_LENGTH_1 |
                  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                  SDRAM_MODEREG_CAS_LATENCY_3 |
                  SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = mode_register;
  if (HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_SDRAM_ProgramRefreshRate(hsdram, SDRAM_REFRESH_COUNT) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Display_Reset(void)
{
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(120);
}

static void Display_Backlight_On(void)
{
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

static void FillFrameBuffer(uint32_t color)
{
  uint32_t pixel_count = LCD_WIDTH * LCD_HEIGHT;
  uint32_t index = 0;

  for (index = 0; index < pixel_count; ++index)
  {
    frame_buffer[index] = color;
  }
}

static void DrawColorBars(void)
{
  static const uint32_t colors[3] = {
    0xFFFF0000U,
    0xFF00FF00U,
    0xFF0000FFU
  };
  uint32_t bar_width = LCD_WIDTH / 3U;
  uint32_t x = 0;
  uint32_t y = 0;

  for (y = 0; y < LCD_HEIGHT; ++y)
  {
    for (x = 0; x < LCD_WIDTH; ++x)
    {
      uint32_t bar = x / bar_width;
      if (bar > 2U)
      {
        bar = 2U;
      }
      frame_buffer[(y * LCD_WIDTH) + x] = colors[bar];
    }
  }
}

static void FlushFrameBuffer(void)
{
  SCB_CleanDCache_by_Addr((uint32_t *)frame_buffer, (int32_t)LCD_FRAMEBUFFER_SIZE_BYTES);
}

static HAL_StatusTypeDef UartLog(const char *format, ...)
{
  char buffer[160];
  va_list args;
  int length = 0;

  va_start(args, format);
  length = vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  if (length < 0)
  {
    return HAL_ERROR;
  }

  if ((size_t)length >= sizeof(buffer))
  {
    length = (int)(sizeof(buffer) - 1U);
  }

  return HAL_UART_Transmit(&huart3, (uint8_t *)buffer, (uint16_t)length, 1000U);
}

static HAL_StatusTypeDef SDRAM_SelfTest(void)
{
  volatile uint32_t *const sdram = (uint32_t *)LCD_FRAMEBUFFER_ADDR;
  static const uint32_t patterns[] = {
    0x00000000U,
    0xFFFFFFFFU,
    0x55555555U,
    0xAAAAAAAAU,
    0x12345678U,
    0x89ABCDEFU
  };
  size_t index = 0;

  for (index = 0; index < (sizeof(patterns) / sizeof(patterns[0])); ++index)
  {
    sdram[index] = patterns[index];
  }

  SCB_CleanDCache_by_Addr((uint32_t *)sdram, 32);
  SCB_InvalidateDCache_by_Addr((uint32_t *)sdram, 32);

  for (index = 0; index < (sizeof(patterns) / sizeof(patterns[0])); ++index)
  {
    uint32_t read_back = sdram[index];
    if (read_back != patterns[index])
    {
      UartLog("SDRAM mismatch @%p wrote=0x%08lX read=0x%08lX\r\n",
              &sdram[index],
              patterns[index],
              read_back);
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

static void LogLTDCState(void)
{
  UartLog("LTDC SSCR=0x%08lX BPCR=0x%08lX AWCR=0x%08lX TWCR=0x%08lX GCR=0x%08lX\r\n",
          LTDC->SSCR,
          LTDC->BPCR,
          LTDC->AWCR,
          LTDC->TWCR,
          LTDC->GCR);
  UartLog("LTDC Layer1 CFBAR=0x%08lX CFBLR=0x%08lX CFBLNR=0x%08lX CR=0x%08lX\r\n",
          LTDC_Layer1->CFBAR,
          LTDC_Layer1->CFBLR,
          LTDC_Layer1->CFBLNR,
          LTDC_Layer1->CR);
}

static void FatalBlinkLoop(uint16_t delay_ms)
{
  while (1)
  {
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
    HAL_Delay(delay_ms);
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
   /* USER CODE BEGIN Error_Handler_Debug */
   /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
   /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
   /* USER CODE BEGIN 6 */
   /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  (void)file;
  (void)line;
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
