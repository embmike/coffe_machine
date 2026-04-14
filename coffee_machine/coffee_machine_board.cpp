#include "coffee_machine_board.hpp"

extern "C" {
#include "app_config.h"
#include "fmc.h"
#include "gpio.h"
#include "ltdc.h"
#include "usart.h"
#if defined(APP_SYSTEM_TEST_ENABLED)
#include "usb_device.h"
#include "usbd_cdc_if.h"
extern USBD_HandleTypeDef hUsbDeviceFS;
#endif
}

#include <cstdarg>
#include <cstdio>

namespace
{
constexpr uint32_t kLcdWidth = APP_LCD_WIDTH; ///< Active panel width in pixels.
constexpr uint32_t kLcdHeight = APP_LCD_HEIGHT; ///< Active panel height in pixels.
constexpr uint32_t kFramebufferAddress = APP_LCD_FRAMEBUFFER_ADDR; ///< SDRAM framebuffer base address.

typedef struct __attribute__((packed))
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
} PixelRGB888;

__attribute__((section(".framebuffer"), aligned(32)))
static PixelRGB888 frame_buffer[kLcdWidth * kLcdHeight];

void displayReset()
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20U);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120U);
}

void displayBacklightOn()
{
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

void drawColorBars()
{
    static const PixelRGB888 colors[3] = {
        { 0x00U, 0x00U, 0xFFU },
        { 0x00U, 0xFFU, 0x00U },
        { 0xFFU, 0x00U, 0x00U }
    };
    const uint32_t bar_width = kLcdWidth / 3U;

    for (uint32_t y = 0; y < kLcdHeight; ++y)
    {
        for (uint32_t x = 0; x < kLcdWidth; ++x)
        {
            uint32_t bar = x / bar_width;
            if (bar > 2U)
            {
                bar = 2U;
            }
            frame_buffer[(y * kLcdWidth) + x] = colors[bar];
        }
    }
}

void flushFrameBuffer()
{
    SCB_CleanDCache();
    __DSB();
    __ISB();
}

HAL_StatusTypeDef sdramSelfTest()
{
    volatile uint32_t* const sdram = reinterpret_cast<uint32_t*>(kFramebufferAddress);
    uint32_t* const cache_buffer = reinterpret_cast<uint32_t*>(kFramebufferAddress);
    static const uint32_t patterns[] = {
        0x00000000U,
        0xFFFFFFFFU,
        0x55555555U,
        0xAAAAAAAAU,
        0x12345678U,
        0x89ABCDEFU
    };

    for (size_t index = 0; index < (sizeof(patterns) / sizeof(patterns[0])); ++index)
    {
        sdram[index] = patterns[index];
    }

    SCB_CleanDCache_by_Addr(cache_buffer, 32);
    SCB_InvalidateDCache_by_Addr(cache_buffer, 32);

    for (size_t index = 0; index < (sizeof(patterns) / sizeof(patterns[0])); ++index)
    {
        if (sdram[index] != patterns[index])
        {
            AppDebugLog("SDRAM mismatch @%p wrote=0x%08lX read=0x%08lX\r\n",
                &sdram[index],
                patterns[index],
                sdram[index]);
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}

void logLtdcState()
{
    AppDebugLog("LTDC SSCR=0x%08lX BPCR=0x%08lX AWCR=0x%08lX TWCR=0x%08lX GCR=0x%08lX\r\n",
        LTDC->SSCR,
        LTDC->BPCR,
        LTDC->AWCR,
        LTDC->TWCR,
        LTDC->GCR);
    AppDebugLog("LTDC L1 CFBAR=0x%08lX CFBLR=0x%08lX CFBLNR=0x%08lX CR=0x%08lX\r\n",
        LTDC_Layer1->CFBAR,
        LTDC_Layer1->CFBLR,
        LTDC_Layer1->CFBLNR,
        LTDC_Layer1->CR);
}
}

void CoffeeMachine_EarlyDiagInit()
{
    MX_USART3_UART_Init();
}

HAL_StatusTypeDef CoffeeMachine_DisplayBootstrap()
{
    HAL_GPIO_WritePin(LCD_DISPD7_GPIO_Port, LCD_DISPD7_Pin, GPIO_PIN_SET);
    AppDebugLog("coffee_machine display bootstrap\r\n");

    if (sdramSelfTest() != HAL_OK)
    {
        AppDebugLog("SDRAM self-test failed\r\n");
        return HAL_ERROR;
    }

    if (HAL_LTDC_SetAddress(&hltdc, kFramebufferAddress, 0) != HAL_OK)
    {
        AppDebugLog("LTDC layer0 address update failed\r\n");
        return HAL_ERROR;
    }

    drawColorBars();
    flushFrameBuffer();
    logLtdcState();
    displayReset();
    displayBacklightOn();

    AppDebugLog("SDRAM self-test passed\r\n");
    AppDebugLog("display enabled\r\n");
    return HAL_OK;
}

void CoffeeMachine_FatalBlinkLoop(uint16_t delay_ms)
{
    while (1)
    {
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
        HAL_Delay(delay_ms);
    }
}

extern "C" HAL_StatusTypeDef AppDebugLog(const char* format, ...)
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

    if (static_cast<size_t>(length) >= sizeof(buffer))
    {
        length = static_cast<int>(sizeof(buffer) - 1U);
    }

#if defined(APP_SYSTEM_TEST_ENABLED)
    if ((hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) && (hUsbDeviceFS.pClassData != nullptr))
    {
        const uint32_t deadline = HAL_GetTick() + 100U;
        while (HAL_GetTick() <= deadline)
        {
            const uint8_t result = CDC_Transmit_FS(reinterpret_cast<uint8_t*>(buffer), static_cast<uint16_t>(length));
            if (result == USBD_OK)
            {
                return HAL_OK;
            }

            if (result != USBD_BUSY)
            {
                break;
            }

            HAL_Delay(1U);
        }
    }
#endif

    return HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(buffer), static_cast<uint16_t>(length), 1000U);
}
