/**
 * @file model_tests.cpp
 * @brief Unit tests for the handwritten TouchGFX model logic.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

namespace
{
class Fake_Tick_Source : public ITick_Source
{
public:
    /**
     * @brief Sets the next tick time returned by the fake.
     * @param now_ms Tick time in milliseconds.
     */
    void Set_Now_Ms(uint32_t now_ms)
    {
        now_ms_ = now_ms;
    }

    uint32_t Now_Ms() override
    {
        return now_ms_;
    }

private:
    uint32_t now_ms_ = 0U;
};

class Mock_Model_Listener : public ModelListener
{
public:
    MOCK_METHOD(void, onBrewingSessionUpdated, (const BrewingSession& session), (override));
    MOCK_METHOD(void, onBrewingSessionCompleted, (), (override));
};
}

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;

/**
 * @brief Test goal: Starting brewing shall notify the active listener.
 *
 * Expected behavior:
 * - one updated-session notification is emitted
 */
TEST(ModelTests, Start_Brewing_Notifies_Updated_Session)
{
    Model model;
    Mock_Model_Listener listener;

    model.bind(&listener);

    EXPECT_CALL(listener, onBrewingSessionUpdated(_)).Times(Exactly(1));
    EXPECT_CALL(listener, onBrewingSessionCompleted()).Times(Exactly(0));

    model.Start_Brewing(CoffeeType::Espresso);
}

/**
 * @brief Test goal: The first tick call shall establish the model time base only.
 *
 * Expected behavior:
 * - no update notification is emitted
 * - no completion notification is emitted
 */
TEST(ModelTests, Tick_Establishes_Time_Base_On_First_Call)
{
    Model model;
    Fake_Tick_Source tick_source;
    Mock_Model_Listener listener;

    model.Set_Tick_Source(&tick_source);
    model.bind(&listener);

    tick_source.Set_Now_Ms(100U);

    EXPECT_CALL(listener, onBrewingSessionUpdated(_)).Times(Exactly(0));
    EXPECT_CALL(listener, onBrewingSessionCompleted()).Times(Exactly(0));

    model.tick();
}

/**
 * @brief Test goal: Active brewing shall advance on later tick calls.
 *
 * Expected behavior:
 * - the listener receives update notifications
 * - the session progress increases above zero
 */
TEST(ModelTests, Tick_Updates_Active_Session)
{
    Model model;
    Fake_Tick_Source tick_source;
    Mock_Model_Listener listener;

    model.Set_Tick_Source(&tick_source);
    model.bind(&listener);

    tick_source.Set_Now_Ms(100U);
    model.tick();

    EXPECT_CALL(listener, onBrewingSessionUpdated(_)).Times(AtLeast(2));
    EXPECT_CALL(listener, onBrewingSessionCompleted()).Times(Exactly(0));

    model.Start_Brewing(CoffeeType::Espresso);
    tick_source.Set_Now_Ms(1100U);
    model.tick();

    EXPECT_GT(model.Get_Brewing_Session().progress_percent, 0U);
}

/**
 * @brief Test goal: Completion shall be notified after the done-hold interval.
 *
 * Expected behavior:
 * - one completion notification is emitted
 * - the model returns to an inactive state afterwards
 */
TEST(ModelTests, Tick_Notifies_Completion_After_Done_Hold)
{
    Model model;
    Fake_Tick_Source tick_source;
    Mock_Model_Listener listener;

    model.Set_Tick_Source(&tick_source);
    model.bind(&listener);

    tick_source.Set_Now_Ms(100U);
    model.tick();

    EXPECT_CALL(listener, onBrewingSessionUpdated(_)).Times(AtLeast(2));
    EXPECT_CALL(listener, onBrewingSessionCompleted()).Times(Exactly(1));

    model.Start_Brewing(CoffeeType::Espresso);

    tick_source.Set_Now_Ms(25100U);
    model.tick();

    tick_source.Set_Now_Ms(27100U);
    model.tick();

    EXPECT_FALSE(model.isBrewingActive());
}

/**
 * @brief Test goal: Completion shall only be reported once.
 *
 * Expected behavior:
 * - only one completion notification is emitted even if tick continues
 */
TEST(ModelTests, Tick_Notifies_Completion_Only_Once)
{
    Model model;
    Fake_Tick_Source tick_source;
    Mock_Model_Listener listener;

    model.Set_Tick_Source(&tick_source);
    model.bind(&listener);

    tick_source.Set_Now_Ms(100U);
    model.tick();

    EXPECT_CALL(listener, onBrewingSessionUpdated(_)).Times(AtLeast(2));
    EXPECT_CALL(listener, onBrewingSessionCompleted()).Times(Exactly(1));

    model.Start_Brewing(CoffeeType::Espresso);

    tick_source.Set_Now_Ms(25100U);
    model.tick();

    tick_source.Set_Now_Ms(27100U);
    model.tick();

    tick_source.Set_Now_Ms(29100U);
    model.tick();
}
