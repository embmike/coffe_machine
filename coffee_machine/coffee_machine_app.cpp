/**
 * @file coffee_machine_app.cpp
 * @brief Thin application facade for TouchGFX startup and processing.
 */

#include "coffee_machine_app.hpp"

extern "C" {
#include "app_config.h"
#include "app_touchgfx.h"
#include "main.h"
}

namespace
{
constexpr uint32_t kTestPatternHoldMs = APP_TEST_PATTERN_HOLD_MS; ///< Bring-up test pattern dwell time.
}

void CoffeeMachine_AppStart()
{
#if APP_SHOW_BRINGUP_TEST_PATTERN
    AppDebugLog("holding test pattern for %lu ms\r\n", kTestPatternHoldMs);
    HAL_Delay(kTestPatternHoldMs);
#else
    AppDebugLog("skipping test pattern hold, starting TouchGFX\r\n");
#endif

    MX_TouchGFX_Init();
    AppDebugLog("touchgfx initialized\r\n");
}

void CoffeeMachine_AppProcess()
{
    MX_TouchGFX_Process();
}
