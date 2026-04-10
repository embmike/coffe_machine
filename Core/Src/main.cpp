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
extern "C" {
   /* USER CODE END Header */
   /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma2d.h"
#include "ltdc.h"
#include "memorymap.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "app_touchgfx.h"

   /* Private includes ----------------------------------------------------------*/
   /* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "stm32h750b_discovery_qspi.h"
#include "stm32h750b_discovery_sdram.h"
} // extern "C"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_WIDTH                    480U
#define LCD_HEIGHT                   272U
#define LCD_FRAMEBUFFER_ADDR         0xD0000000U
#define LCD_FRAMEBUFFER_SIZE_BYTES   (LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t))
#define TEST_PATTERN_HOLD_MS         20000U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__attribute__((section(".framebuffer"), aligned(32)))
static uint16_t frame_buffer[LCD_WIDTH * LCD_HEIGHT];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
static void EarlyDiagInit(void);
static void Display_Reset(void);
static void Display_Backlight_On(void);
static void DrawColorBars(void);
static void FlushFrameBuffer(void);
extern "C" void DebugProbe_AppEntry(void);
extern "C" void DebugProbe_PreTouchGFXInit(void);
extern "C" void DebugProbe_PostTouchGFXInit(void);
extern "C" void DebugProbe_MainLoop(void);
static HAL_StatusTypeDef UartLog(const char *format, ...);
static HAL_StatusTypeDef SDRAM_SelfTest(void);
static void LogLTDCState(void);
static void FatalBlinkLoop(uint16_t delay_ms);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern "C" __attribute__((noinline)) void DebugProbe_AppEntry(void)
{
   __NOP();
}

extern "C" __attribute__((noinline)) void DebugProbe_PreTouchGFXInit(void)
{
   __NOP();
}

extern "C" __attribute__((noinline)) void DebugProbe_PostTouchGFXInit(void)
{
   __NOP();
}

extern "C" __attribute__((noinline)) void DebugProbe_MainLoop(void)
{
   __NOP();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

   /* USER CODE BEGIN 1 */

   /* USER CODE END 1 */

   /* MPU Configuration--------------------------------------------------------*/
   MPU_Config();

   /* Enable the CPU Cache */

   /* Enable I-Cache---------------------------------------------------------*/
   SCB_EnableICache();

   /* Enable D-Cache---------------------------------------------------------*/
   SCB_EnableDCache();

   /* MCU Configuration--------------------------------------------------------*/

   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();

   /* USER CODE BEGIN Init */

   /* USER CODE END Init */

   /* Configure the system clock */
   SystemClock_Config();

   /* USER CODE BEGIN SysInit */
   DebugProbe_AppEntry();
   EarlyDiagInit();
   UartLog("boot: clock configured, entering peripheral init\r\n");

   /* USER CODE END SysInit */

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_FMC_Init();
   MX_LTDC_Init();
   MX_USART3_UART_Init();
   MX_DMA2D_Init();
   MX_CRC_Init();
   /* USER CODE BEGIN 2 */
   /* QSPI is already configured for XIP by extmem_boot before jumping here. */
   HAL_GPIO_WritePin(LCD_DISPD7_GPIO_Port, LCD_DISPD7_Pin, GPIO_PIN_SET);

   UartLog("coffee_machine display bootstrap\r\n");

   if (SDRAM_SelfTest() != HAL_OK)
   {
      UartLog("SDRAM self-test failed\r\n");
      FatalBlinkLoop(100U);
   }

   if (HAL_LTDC_SetAddress(&hltdc, LCD_FRAMEBUFFER_ADDR, 0) != HAL_OK)
   {
      UartLog("LTDC layer0 address update failed\r\n");
      FatalBlinkLoop(200U);
   }

   DrawColorBars();
   FlushFrameBuffer();
   LogLTDCState();
   Display_Reset();
   Display_Backlight_On();

   UartLog("SDRAM self-test passed\r\n");
   UartLog("display enabled\r\n");
   UartLog("holding test pattern for %lu ms\r\n", TEST_PATTERN_HOLD_MS);
   HAL_Delay(TEST_PATTERN_HOLD_MS);

   DebugProbe_PreTouchGFXInit();
   //MX_TouchGFX_Init();
   DebugProbe_PostTouchGFXInit();
   UartLog("touchgfx initialized\r\n");

   /* USER CODE END 2 */

   /* Infinite loop */
   /* USER CODE BEGIN WHILE */
   while (1)
   {
      /* USER CODE END WHILE */

      DebugProbe_MainLoop();
      //MX_TouchGFX_Process();
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
   RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
   RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

   /** Supply configuration update enable
   */
   HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

   /** Configure the main internal regulator output voltage
   */
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

   while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

   /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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

/* USER CODE BEGIN 4 */
static void EarlyDiagInit(void)
{
   MX_USART3_UART_Init();
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

static void DrawColorBars(void)
{
   static const uint16_t colors[3] = {
      0xF800U,
      0x07E0U,
      0x001FU
   };
   uint32_t x = 0;
   uint32_t y = 0;
   const uint32_t bar_width = LCD_WIDTH / 3U;

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
   SCB_CleanDCache();
   __DSB();
   __ISB();
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
      if (sdram[index] != patterns[index])
      {
         UartLog("SDRAM mismatch @%p wrote=0x%08lX read=0x%08lX\r\n",
            &sdram[index],
            patterns[index],
            sdram[index]);
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
   UartLog("LTDC L1 CFBAR=0x%08lX CFBLR=0x%08lX CFBLNR=0x%08lX CR=0x%08lX\r\n",
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

 /* MPU Configuration */

void MPU_Config(void)
{
   MPU_Region_InitTypeDef MPU_InitStruct = { 0 };

   /* Disables the MPU */
   HAL_MPU_Disable();

   /** Initializes and configures the Region and the memory to be protected
   */
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER0;
   MPU_InitStruct.BaseAddress = 0x24000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
   MPU_InitStruct.SubRegionDisable = 0x0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /** Initializes and configures the Region and the memory to be protected
   */
   MPU_InitStruct.Number = MPU_REGION_NUMBER1;
   MPU_InitStruct.BaseAddress = 0x90000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
   MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /** Initializes and configures the Region and the memory to be protected
   */
   MPU_InitStruct.Number = MPU_REGION_NUMBER2;
   MPU_InitStruct.BaseAddress = 0x90000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /** Initializes and configures the Region and the memory to be protected
   */
   MPU_InitStruct.Number = MPU_REGION_NUMBER3;
   MPU_InitStruct.BaseAddress = 0xD0000000;
   MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
   MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /** Initializes and configures the Region and the memory to be protected
   */
   MPU_InitStruct.Number = MPU_REGION_NUMBER4;
   MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);
   /* Enables the MPU */
   HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
   /* USER CODE BEGIN Error_Handler_Debug */
      /* User can add his own implementation to report the HAL error return state */
   __disable_irq();
   UartLog("Error_Handler reached\r\n");
   while (1)
   {
      HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
      HAL_Delay(150U);
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
       /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
