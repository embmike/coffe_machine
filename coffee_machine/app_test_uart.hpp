/**
 * @file app_test_uart.hpp
 * @brief UART command channel for Python-driven board-side system tests.
 */

#ifndef APP_TEST_UART_HPP
#define APP_TEST_UART_HPP

/**
 * @brief Processes pending UART bytes and dispatches completed test commands.
 */
void AppTest_UartProcess();

#endif // APP_TEST_UART_HPP
