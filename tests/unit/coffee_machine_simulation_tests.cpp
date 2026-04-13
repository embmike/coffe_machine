/**
 * @file coffee_machine_simulation_tests.cpp
 * @brief Unit tests for the brewing-domain logic in CoffeeMachineSimulation.
 */

#include <gtest/gtest.h>

#include "coffee_machine_simulation.hpp"

/**
 * @brief Test goal: Espresso shall resolve to a valid profile.
 *
 * Expected behavior:
 * - the profile pointer is not null
 * - the profile fields match the Espresso definition
 */
TEST(CoffeeMachineSimulationTests, Find_Profile_Returns_Espresso_Profile)
{
    const CoffeeProfile* profile = CoffeeMachine_FindProfile(CoffeeType::Espresso);

    ASSERT_NE(nullptr, profile);
    EXPECT_EQ(CoffeeType::Espresso, profile->type);
    EXPECT_STREQ("Espresso", profile->name);
    EXPECT_STREQ("strong & intense", profile->character);
    EXPECT_EQ(25000U, profile->brew_time_ms);
}

/**
 * @brief Test goal: Starting Espresso shall initialize the expected session state.
 *
 * Expected behavior:
 * - the simulation becomes active
 * - the phase is Start
 * - the progress is zero
 * - the steam level is Low
 */
TEST(CoffeeMachineSimulationTests, Start_Initializes_Session_For_Espresso)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);

    EXPECT_TRUE(simulation.isActive());
    EXPECT_EQ(CoffeeType::Espresso, simulation.getCoffeeType());
    EXPECT_EQ(BrewingPhase::Start, simulation.getPhase());
    EXPECT_EQ(SteamLevel::Low, simulation.getSteamLevel());
    EXPECT_EQ(0U, simulation.getProgressPercent());
    EXPECT_EQ(25000U, simulation.getRemainingTimeMs());
}

/**
 * @brief Test goal: Updating an active session shall advance the progress.
 *
 * Expected behavior:
 * - elapsed work changes the remaining time
 * - progress increases above zero
 */
TEST(CoffeeMachineSimulationTests, Update_Advances_Progress_For_Active_Session)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);
    simulation.update(1000U);

    EXPECT_LT(simulation.getRemainingTimeMs(), 25000U);
    EXPECT_GT(simulation.getProgressPercent(), 0U);
}

/**
 * @brief Test goal: Brewing phase shall be entered after twenty percent progress.
 *
 * Expected behavior:
 * - the phase becomes Brewing
 * - the steam level becomes Normal
 */
TEST(CoffeeMachineSimulationTests, Update_Sets_Phase_To_Brewing_After_Twenty_Percent)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);
    simulation.update(5000U);

    EXPECT_EQ(BrewingPhase::Brewing, simulation.getPhase());
    EXPECT_EQ(SteamLevel::Normal, simulation.getSteamLevel());
}

/**
 * @brief Test goal: Finishing phase shall be entered after eighty percent progress.
 *
 * Expected behavior:
 * - the phase becomes Finishing
 * - the steam level becomes Strong
 */
TEST(CoffeeMachineSimulationTests, Update_Sets_Phase_To_Finishing_After_Eighty_Percent)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);
    simulation.update(20000U);

    EXPECT_EQ(BrewingPhase::Finishing, simulation.getPhase());
    EXPECT_EQ(SteamLevel::Strong, simulation.getSteamLevel());
}

/**
 * @brief Test goal: Done phase shall be entered at one hundred percent progress.
 *
 * Expected behavior:
 * - the phase becomes Done
 * - the progress is capped at one hundred
 * - the simulation reports finished
 */
TEST(CoffeeMachineSimulationTests, Update_Sets_Phase_To_Done_At_Hundred_Percent)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);
    simulation.update(25000U);

    EXPECT_EQ(BrewingPhase::Done, simulation.getPhase());
    EXPECT_EQ(100U, simulation.getProgressPercent());
    EXPECT_TRUE(simulation.isFinished());
}

/**
 * @brief Test goal: Stopping a session shall reset the state to idle.
 *
 * Expected behavior:
 * - the simulation becomes inactive
 * - the phase becomes Idle
 * - the progress returns to zero
 */
TEST(CoffeeMachineSimulationTests, Stop_Resets_Session_To_Idle)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);
    simulation.stop();

    EXPECT_FALSE(simulation.isActive());
    EXPECT_EQ(BrewingPhase::Idle, simulation.getPhase());
    EXPECT_EQ(0U, simulation.getProgressPercent());
    EXPECT_EQ(0U, simulation.getRemainingTimeMs());
}

/**
 * @brief Test goal: Reset shall clear an active session.
 *
 * Expected behavior:
 * - the simulation becomes inactive
 * - the session returns to the idle baseline
 */
TEST(CoffeeMachineSimulationTests, Reset_Clears_Active_State)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Latte);
    simulation.reset();

    EXPECT_FALSE(simulation.isActive());
    EXPECT_EQ(BrewingPhase::Idle, simulation.getPhase());
    EXPECT_EQ(SteamLevel::Off, simulation.getSteamLevel());
    EXPECT_EQ(0U, simulation.getProgressPercent());
}

/**
 * @brief Test goal: Log names shall match the expected lowercase labels.
 *
 * Expected behavior:
 * - Espresso resolves to "espresso"
 * - Americano resolves to "americano"
 */
TEST(CoffeeMachineSimulationTests, Get_Coffee_Log_Name_Returns_Expected_Label)
{
    EXPECT_STREQ("espresso", CoffeeMachine_GetCoffeeLogName(CoffeeType::Espresso));
    EXPECT_STREQ("americano", CoffeeMachine_GetCoffeeLogName(CoffeeType::Americano));
}
