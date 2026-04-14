/**
 * @file coffee_machine_app.hpp
 * @brief Thin application facade for TouchGFX startup and processing.
 */

#ifndef COFFEE_MACHINE_APP_HPP
#define COFFEE_MACHINE_APP_HPP

#include <cstdint>

/**
 * @brief Runtime contract used to unit-test the handwritten application facade.
 */
class IApp_Runtime
{
public:
    virtual ~IApp_Runtime() = default;

    /**
     * @brief Writes a diagnostic message.
     * @param message Null-terminated message string.
     */
    virtual void Log(const char* message) = 0;

    /**
     * @brief Waits for the specified time.
     * @param delay_ms Delay in milliseconds.
     */
    virtual void Delay_Ms(uint32_t delay_ms) = 0;

    /**
     * @brief Starts TouchGFX.
     */
    virtual void TouchGfx_Init() = 0;

    /**
     * @brief Processes one TouchGFX loop iteration.
     */
    virtual void TouchGfx_Process() = 0;
};

/**
 * @brief Performs the application startup sequence after peripheral initialization.
 */
void CoffeeMachine_AppStart();

/**
 * @brief Performs the application startup sequence with an injected runtime adapter.
 * @param runtime Runtime abstraction used by the startup path.
 */
void CoffeeMachine_AppStart(IApp_Runtime& runtime);

/**
 * @brief Processes one iteration of the application main loop.
 */
void CoffeeMachine_AppProcess();

/**
 * @brief Processes one iteration of the application main loop with an injected runtime adapter.
 * @param runtime Runtime abstraction used by the process path.
 */
void CoffeeMachine_AppProcess(IApp_Runtime& runtime);

#endif // COFFEE_MACHINE_APP_HPP
