/**
 * @file coffee_machine_board.hpp
 * @brief Board-level helpers for diagnostics, display bootstrap, and fatal handling.
 */

#ifndef COFFEE_MACHINE_BOARD_HPP
#define COFFEE_MACHINE_BOARD_HPP

#include <cstdint>

extern "C" {
#include "main.h"
}

/**
 * @brief Initializes the early diagnostic channel before the full peripheral setup.
 */
void CoffeeMachine_EarlyDiagInit();

/**
 * @brief Brings up the SDRAM-backed display path and emits diagnostic output.
 * @return HAL_OK on success, HAL_ERROR if a bring-up step fails.
 */
HAL_StatusTypeDef CoffeeMachine_DisplayBootstrap();

/**
 * @brief Blinks the fatal LED forever with the supplied delay.
 * @param delay_ms Blink delay in milliseconds.
 */
void CoffeeMachine_FatalBlinkLoop(uint16_t delay_ms);

#endif // COFFEE_MACHINE_BOARD_HPP
