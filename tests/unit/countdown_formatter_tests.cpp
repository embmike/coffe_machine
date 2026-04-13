/**
 * @file countdown_formatter_tests.cpp
 * @brief Unit tests for the brewing countdown formatting helper.
 */

#include <gtest/gtest.h>

#include "countdown_formatter.hpp"

/**
 * @brief Test goal: Zero milliseconds shall be formatted as zero seconds.
 *
 * Expected behavior:
 * - the output buffer contains the string "0"
 */
TEST(CountdownFormatterTests, Formats_Zero_Milliseconds_As_Zero)
{
    char buffer[8] = {};

    format_countdown(0U, buffer, sizeof(buffer));

    EXPECT_STREQ("0", buffer);
}

/**
 * @brief Test goal: Sub-second values shall be truncated to zero seconds.
 *
 * Expected behavior:
 * - the output buffer contains the string "0"
 */
TEST(CountdownFormatterTests, Truncates_Sub_Second_Value_To_Zero)
{
    char buffer[8] = {};

    format_countdown(999U, buffer, sizeof(buffer));

    EXPECT_STREQ("0", buffer);
}

/**
 * @brief Test goal: Full seconds shall be formatted as their decimal value.
 *
 * Expected behavior:
 * - the output buffer contains the string "1" for 1000 ms
 */
TEST(CountdownFormatterTests, Formats_One_Second_As_One)
{
    char buffer[8] = {};

    format_countdown(1000U, buffer, sizeof(buffer));

    EXPECT_STREQ("1", buffer);
}

/**
 * @brief Test goal: A null output buffer shall be handled safely.
 *
 * Expected behavior:
 * - the function returns without dereferencing the buffer pointer
 */
TEST(CountdownFormatterTests, Returns_Safely_For_Null_Buffer)
{
    format_countdown(1000U, nullptr, 8U);

    SUCCEED();
}

/**
 * @brief Test goal: A zero-sized output buffer shall be handled safely.
 *
 * Expected behavior:
 * - the function returns without writing to the buffer
 */
TEST(CountdownFormatterTests, Returns_Safely_For_Zero_Buffer_Size)
{
    char buffer[8] = { 'x', '\0' };

    format_countdown(1000U, buffer, 0U);

    EXPECT_EQ('x', buffer[0]);
}
