/**
 * @file countdown_formatter.hpp
 * @brief Formatting helper for brewing countdown values.
 */

#ifndef COUNTDOWN_FORMATTER_HPP
#define COUNTDOWN_FORMATTER_HPP

#include <cstddef>
#include <cstdint>

/**
 * @brief Formats the remaining brewing time for display.
 * @param remaining_ms Remaining time in milliseconds.
 * @param buffer Output buffer for the decimal countdown string.
 * @param buffer_size Size of @p buffer in bytes.
 */
void format_countdown(uint32_t remaining_ms, char* buffer, size_t buffer_size);

#endif // COUNTDOWN_FORMATTER_HPP
