#include "countdown_formatter.hpp"

#include <cstdio>

void format_countdown(uint32_t remaining_ms, char* buffer, size_t buffer_size)
{
    if (buffer == nullptr || buffer_size == 0U)
    {
        return;
    }

    const uint32_t total_seconds = remaining_ms / 1000U;
    std::snprintf(buffer, buffer_size, "%lu",
        static_cast<unsigned long>(total_seconds));
}
