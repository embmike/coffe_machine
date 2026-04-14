/**
 * @file coffee_machine_app.cpp
 * @brief Thin application facade for TouchGFX startup and processing.
 */

#include "coffee_machine_app.hpp"

extern "C" {
#include "app_config.h"
#if !defined(UNIT_TEST)
#include "main.h"

void MX_TouchGFX_Init(void);
void MX_TouchGFX_Process(void);
#endif
}

namespace
{
constexpr uint32_t kTestPatternHoldMs = APP_TEST_PATTERN_HOLD_MS; ///< Bring-up test pattern dwell time.

class Embedded_App_Runtime : public IApp_Runtime
{
public:
    void Log(const char* message) override
    {
#if defined(UNIT_TEST)
        (void)message;
#else
        AppDebugLog("%s", message);
#endif
    }

    void Delay_Ms(uint32_t delay_ms) override
    {
#if defined(UNIT_TEST)
        (void)delay_ms;
#else
        HAL_Delay(delay_ms);
#endif
    }

    void TouchGfx_Init() override
    {
#if !defined(UNIT_TEST)
        MX_TouchGFX_Init();
#endif
    }

    void TouchGfx_Process() override
    {
#if !defined(UNIT_TEST)
        MX_TouchGFX_Process();
#endif
    }
};
}

void CoffeeMachine_AppStart()
{
    Embedded_App_Runtime runtime;
    CoffeeMachine_AppStart(runtime);
}

void CoffeeMachine_AppStart(IApp_Runtime& runtime)
{
#if APP_SHOW_BRINGUP_TEST_PATTERN
    runtime.Log("holding test pattern for configured delay\r\n");
    runtime.Delay_Ms(kTestPatternHoldMs);
#else
    runtime.Log("skipping test pattern hold, starting TouchGFX\r\n");
#endif

    runtime.TouchGfx_Init();
    runtime.Log("touchgfx initialized\r\n");
}

void CoffeeMachine_AppProcess()
{
    Embedded_App_Runtime runtime;
    CoffeeMachine_AppProcess(runtime);
}

void CoffeeMachine_AppProcess(IApp_Runtime& runtime)
{
    runtime.TouchGfx_Process();
}
