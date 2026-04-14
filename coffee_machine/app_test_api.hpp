/**
 * @file app_test_api.hpp
 * @brief Semantic board-side test facade for Python-driven system tests.
 */

#ifndef APP_TEST_API_HPP
#define APP_TEST_API_HPP

#include "coffee_machine_simulation.hpp"

class FrontendApplication;
class Model;

/**
 * @brief Visible high-level application states for board-side system tests.
 */
enum class AppTestState
{
    Splash,
    Selection,
    Brewing
};

/**
 * @brief Result codes for semantic test-facade actions.
 */
enum class AppTestResult
{
    Ok,
    NotReady,
    InvalidState,
    InvalidArgument
};

/**
 * @brief Registers the live TouchGFX frontend application instance.
 * @param application Active frontend application.
 */
void AppTest_Register_Application(FrontendApplication* application);

/**
 * @brief Registers the live TouchGFX model instance.
 * @param model Active TouchGFX model.
 */
void AppTest_Register_Model(Model* model);

/**
 * @brief Updates the visible high-level app state.
 * @param state Current visible app state.
 */
void AppTest_Set_State(AppTestState state);

/**
 * @brief Returns the visible high-level app state.
 * @return Current visible app state.
 */
AppTestState AppTest_Get_State();

/**
 * @brief Updates the observable brewing-session snapshot.
 * @param session Current brewing-session snapshot.
 */
void AppTest_Set_Session(const BrewingSession& session);

/**
 * @brief Returns the current observable brewing-session snapshot.
 * @return Copy of the current brewing-session snapshot.
 */
BrewingSession AppTest_Get_Session();

/**
 * @brief Returns the supported board-side test protocol version.
 * @return Null-terminated protocol version string.
 */
const char* AppTest_Get_Version();

/**
 * @brief Resets the demonstrator to a defined interactive test state.
 * @return Result code describing the outcome.
 */
AppTestResult AppTest_Reset_Demo();

/**
 * @brief Starts brewing for the selected drink through the normal app path.
 * @param type Selected drink type.
 * @return Result code describing the outcome.
 */
AppTestResult AppTest_Select_Drink(CoffeeType type);

#endif // APP_TEST_API_HPP
