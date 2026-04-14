/**
 * @file app_test_uart.cpp
 * @brief UART command channel for Python-driven board-side system tests.
 */

#include "app_test_uart.hpp"

#include "app_test_api.hpp"

extern "C" {
#include "main.h"
#include "usart.h"
}

#include <cstdio>
#include <cstring>

namespace
{
constexpr size_t kRxBufferSize = 128U;
char g_rx_buffer[kRxBufferSize];
size_t g_rx_length = 0U;

const char* to_state_name(AppTestState state)
{
    switch (state)
    {
    case AppTestState::Splash:
        return "splash";
    case AppTestState::Selection:
        return "selection";
    case AppTestState::Brewing:
        return "brewing";
    default:
        return "splash";
    }
}

const char* to_coffee_name(CoffeeType type)
{
    switch (type)
    {
    case CoffeeType::Espresso:
        return "espresso";
    case CoffeeType::Cappuccino:
        return "cappuccino";
    case CoffeeType::Latte:
        return "latte";
    case CoffeeType::Americano:
        return "americano";
    default:
        return "espresso";
    }
}

const char* to_phase_name(BrewingPhase phase)
{
    switch (phase)
    {
    case BrewingPhase::Idle:
        return "idle";
    case BrewingPhase::Start:
        return "start";
    case BrewingPhase::Brewing:
        return "brewing";
    case BrewingPhase::Finishing:
        return "finishing";
    case BrewingPhase::Done:
        return "done";
    default:
        return "idle";
    }
}

const char* to_result_reason(AppTestResult result)
{
    switch (result)
    {
    case AppTestResult::NotReady:
        return "not_ready";
    case AppTestResult::InvalidState:
        return "invalid_state_for_selection";
    case AppTestResult::InvalidArgument:
        return "invalid_argument";
    case AppTestResult::Ok:
    default:
        return "unknown";
    }
}

void send_json_line(const char* json_line)
{
    AppDebugLog("[app_test] tx %s", json_line);
    const size_t length = std::strlen(json_line);
    (void)HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(const_cast<char*>(json_line)),
        static_cast<uint16_t>(length), 100U);
}

void send_status_ok()
{
    send_json_line("{\"status\":\"ok\"}\r\n");
}

void send_status_error(const char* reason)
{
    char buffer[96];
    const int length = std::snprintf(buffer, sizeof(buffer),
        "{\"status\":\"error\",\"reason\":\"%s\"}\r\n", reason);

    if (length > 0)
    {
        send_json_line(buffer);
    }
}

const char* find_key_value(const char* json_line, const char* key)
{
    char pattern[32];
    const int pattern_length = std::snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    if ((pattern_length <= 0) || (static_cast<size_t>(pattern_length) >= sizeof(pattern)))
    {
        return nullptr;
    }

    const char* key_position = std::strstr(json_line, pattern);
    if (key_position == nullptr)
    {
        return nullptr;
    }

    const char* colon = std::strchr(key_position + pattern_length, ':');
    if (colon == nullptr)
    {
        return nullptr;
    }

    const char* value = colon + 1;
    while ((*value == ' ') || (*value == '\t'))
    {
        ++value;
    }

    return value;
}

bool extract_string_field(const char* json_line, const char* key, char* value, size_t value_size)
{
    const char* raw_value = find_key_value(json_line, key);
    if ((raw_value == nullptr) || (*raw_value != '"') || (value_size == 0U))
    {
        return false;
    }

    ++raw_value;
    size_t index = 0U;
    while ((*raw_value != '\0') && (*raw_value != '"'))
    {
        if (index + 1U >= value_size)
        {
            return false;
        }

        value[index] = *raw_value;
        ++index;
        ++raw_value;
    }

    if (*raw_value != '"')
    {
        return false;
    }

    value[index] = '\0';
    return true;
}

bool try_parse_drink(const char* drink_name, CoffeeType& type)
{
    if (std::strcmp(drink_name, "espresso") == 0)
    {
        type = CoffeeType::Espresso;
        return true;
    }

    if (std::strcmp(drink_name, "cappuccino") == 0)
    {
        type = CoffeeType::Cappuccino;
        return true;
    }

    if (std::strcmp(drink_name, "latte") == 0)
    {
        type = CoffeeType::Latte;
        return true;
    }

    if (std::strcmp(drink_name, "americano") == 0)
    {
        type = CoffeeType::Americano;
        return true;
    }

    return false;
}

void handle_get_version()
{
    char buffer[96];
    const int length = std::snprintf(buffer, sizeof(buffer),
        "{\"status\":\"ok\",\"version\":\"%s\"}\r\n", AppTest_Get_Version());

    if (length > 0)
    {
        send_json_line(buffer);
    }
}

void handle_get_state()
{
    char buffer[96];
    const int length = std::snprintf(buffer, sizeof(buffer),
        "{\"status\":\"ok\",\"state\":\"%s\"}\r\n", to_state_name(AppTest_Get_State()));

    if (length > 0)
    {
        send_json_line(buffer);
    }
}

void handle_get_session()
{
    const BrewingSession session = AppTest_Get_Session();
    char buffer[192];
    const int length = std::snprintf(buffer, sizeof(buffer),
        "{\"status\":\"ok\",\"active\":%s,\"coffee\":\"%s\",\"phase\":\"%s\",\"progress\":%u,\"remaining_ms\":%lu}\r\n",
        session.active ? "true" : "false",
        to_coffee_name(session.coffee_type),
        to_phase_name(session.phase),
        static_cast<unsigned int>(session.progress_percent),
        static_cast<unsigned long>(session.remaining_time_ms));

    if (length > 0)
    {
        send_json_line(buffer);
    }
}

void handle_select_drink(const char* json_line)
{
    char drink_name[24];
    CoffeeType drink_type = CoffeeType::Espresso;
    AppDebugLog("[app_test] select_drink raw=%s\r\n", json_line);

    if (!extract_string_field(json_line, "drink", drink_name, sizeof(drink_name)))
    {
        send_status_error("missing_drink");
        return;
    }

    if (!try_parse_drink(drink_name, drink_type))
    {
        send_status_error("invalid_drink");
        return;
    }

    const AppTestResult result = AppTest_Select_Drink(drink_type);
    AppDebugLog("[app_test] select_drink parsed=%s result=%d\r\n", drink_name, static_cast<int>(result));
    if (result != AppTestResult::Ok)
    {
        send_status_error(to_result_reason(result));
        return;
    }

    send_status_ok();
}

void dispatch_command(const char* json_line)
{
    char command[32];
    AppDebugLog("[app_test] rx %s\r\n", json_line);

    if (!extract_string_field(json_line, "cmd", command, sizeof(command)))
    {
        send_status_error("missing_cmd");
        return;
    }

    AppDebugLog("[app_test] cmd=%s\r\n", command);

    if (std::strcmp(command, "get_version") == 0)
    {
        handle_get_version();
        return;
    }

    if (std::strcmp(command, "get_state") == 0)
    {
        handle_get_state();
        return;
    }

    if (std::strcmp(command, "get_session") == 0)
    {
        handle_get_session();
        return;
    }

    if (std::strcmp(command, "reset_demo") == 0)
    {
        const AppTestResult result = AppTest_Reset_Demo();
        if (result != AppTestResult::Ok)
        {
            send_status_error(to_result_reason(result));
            return;
        }

        send_status_ok();
        return;
    }

    if (std::strcmp(command, "select_drink") == 0)
    {
        handle_select_drink(json_line);
        return;
    }

    send_status_error("unknown_cmd");
}

void consume_rx_byte(char rx_byte)
{
    if (rx_byte == '\r')
    {
        return;
    }

    if (rx_byte == '\n')
    {
        if (g_rx_length == 0U)
        {
            return;
        }

        g_rx_buffer[g_rx_length] = '\0';
        dispatch_command(g_rx_buffer);
        g_rx_length = 0U;
        return;
    }

    if (g_rx_length + 1U >= kRxBufferSize)
    {
        g_rx_length = 0U;
        send_status_error("command_too_long");
        return;
    }

    g_rx_buffer[g_rx_length] = rx_byte;
    ++g_rx_length;
}
}

void AppTest_UartProcess()
{
    uint8_t rx_byte = 0U;

    while (HAL_UART_Receive(&huart3, &rx_byte, 1U, 0U) == HAL_OK)
    {
        consume_rx_byte(static_cast<char>(rx_byte));
    }
}
