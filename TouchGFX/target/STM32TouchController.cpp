/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.26.1. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>

extern "C"
{
#include "app_config.h"
#include "main.h"
#include "stm32h750b_discovery_ts.h"
}

namespace
{
bool g_touchInitialized = false;
bool g_touchInitLogged = false;
bool g_touchWasDown = false;
int32_t g_lastLoggedX = -1;
int32_t g_lastLoggedY = -1;
}

void STM32TouchController::init()
{
    TS_Init_t touchScreenConfig;

    touchScreenConfig.Width = APP_LCD_WIDTH;
    touchScreenConfig.Height = APP_LCD_HEIGHT;
    touchScreenConfig.Orientation = TS_SWAP_XY;
    touchScreenConfig.Accuracy = APP_TOUCH_ACCURACY;

    g_touchInitialized = (BSP_TS_Init(0, &touchScreenConfig) == BSP_ERROR_NONE);

    if (!g_touchInitLogged)
    {
        g_touchInitLogged = true;
        AppDebugLog("touch init: %s\r\n", g_touchInitialized ? "ok" : "failed");
    }
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    TS_State_t state = { 0 };

    if (!g_touchInitialized)
    {
        return false;
    }

    if (BSP_TS_GetState(0, &state) == BSP_ERROR_NONE && state.TouchDetected != 0U)
    {
        x = (int32_t)state.TouchX;
        y = (int32_t)state.TouchY;

        if (!g_touchWasDown ||
            (x < (g_lastLoggedX - 8)) || (x > (g_lastLoggedX + 8)) ||
            (y < (g_lastLoggedY - 8)) || (y > (g_lastLoggedY + 8)))
        {
            AppDebugLog("touch sample: x=%ld y=%ld\r\n", x, y);
            g_lastLoggedX = x;
            g_lastLoggedY = y;
        }

        g_touchWasDown = true;
        return true;
    }

    g_touchWasDown = false;

    return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
