#ifndef COUNTDOWN_FORMATTER_HPP
#define COUNTDOWN_FORMATTER_HPP

#include <cstddef>
#include <cstdint>

void format_countdown(uint32_t remaining_ms, char* buffer, size_t buffer_size);

#endif // COUNTDOWN_FORMATTER_HPP
