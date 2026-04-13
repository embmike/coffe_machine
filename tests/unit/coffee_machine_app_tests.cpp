/**
 * @file coffee_machine_app_tests.cpp
 * @brief Unit tests for the handwritten TouchGFX application facade.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "coffee_machine_app.hpp"

namespace
{
class Mock_App_Runtime : public IApp_Runtime
{
public:
    MOCK_METHOD(void, Log, (const char* message), (override));
    MOCK_METHOD(void, Delay_Ms, (uint32_t delay_ms), (override));
    MOCK_METHOD(void, TouchGfx_Init, (), (override));
    MOCK_METHOD(void, TouchGfx_Process, (), (override));
};
}

using ::testing::Exactly;
using ::testing::StrEq;

/**
 * @brief Test goal: Application start shall initialize TouchGFX and log the startup path.
 *
 * Expected behavior:
 * - the skip-hold message is logged for the current configuration
 * - TouchGfx_Init is called once
 * - the initialized message is logged afterwards
 */
TEST(CoffeeMachineAppTests, App_Start_Initializes_TouchGfx_And_Logs_Start_Path)
{
    Mock_App_Runtime runtime;

    {
        ::testing::InSequence sequence;
        EXPECT_CALL(runtime, Log(StrEq("skipping test pattern hold, starting TouchGFX\r\n"))).Times(Exactly(1));
        EXPECT_CALL(runtime, TouchGfx_Init()).Times(Exactly(1));
        EXPECT_CALL(runtime, Log(StrEq("touchgfx initialized\r\n"))).Times(Exactly(1));
    }
    EXPECT_CALL(runtime, Delay_Ms(::testing::_)).Times(Exactly(0));

    CoffeeMachine_AppStart(runtime);
}

/**
 * @brief Test goal: Application process shall forward one loop iteration to TouchGFX.
 *
 * Expected behavior:
 * - TouchGfx_Process is called exactly once
 */
TEST(CoffeeMachineAppTests, App_Process_Forwards_One_TouchGfx_Iteration)
{
    Mock_App_Runtime runtime;

    EXPECT_CALL(runtime, TouchGfx_Process()).Times(Exactly(1));

    CoffeeMachine_AppProcess(runtime);
}
