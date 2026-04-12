/**
 * @file coffee_machine_app.hpp
 * @brief Thin application facade for TouchGFX startup and processing.
 */

#ifndef COFFEE_MACHINE_APP_HPP
#define COFFEE_MACHINE_APP_HPP

/**
 * @brief Performs the application startup sequence after peripheral initialization.
 */
void CoffeeMachine_AppStart();

/**
 * @brief Processes one iteration of the application main loop.
 */
void CoffeeMachine_AppProcess();

#endif // COFFEE_MACHINE_APP_HPP
