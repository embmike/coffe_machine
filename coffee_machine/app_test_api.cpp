/**
 * @file app_test_api.cpp
 * @brief Semantic board-side test facade for Python-driven system tests.
 */

#include "app_test_api.hpp"

#if defined(UNIT_TEST)

namespace
{
constexpr char kProtocolVersion[] = "coffee_machine-systemtest-v1";

AppTestState g_state = AppTestState::Splash;
BrewingSession g_session = { CoffeeType::Espresso, 0U, 0U, 0U, 0U, BrewingPhase::Idle, SteamLevel::Off, false };
}

void AppTest_Register_Application(FrontendApplication* application)
{
    (void)application;
}

void AppTest_Register_Model(Model* model)
{
    (void)model;
}

void AppTest_Set_State(AppTestState state)
{
    g_state = state;
}

AppTestState AppTest_Get_State()
{
    return g_state;
}

void AppTest_Set_Session(const BrewingSession& session)
{
    g_session = session;
}

BrewingSession AppTest_Get_Session()
{
    return g_session;
}

const char* AppTest_Get_Version()
{
    return kProtocolVersion;
}

AppTestResult AppTest_Reset_Demo()
{
    return AppTestResult::NotReady;
}

AppTestResult AppTest_Select_Drink(CoffeeType type)
{
    (void)type;
    return AppTestResult::NotReady;
}

#else

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>

namespace
{
constexpr char kProtocolVersion[] = "coffee_machine-systemtest-v1";

FrontendApplication* g_application = nullptr;
Model* g_model = nullptr;
AppTestState g_state = AppTestState::Splash;
BrewingSession g_session = { CoffeeType::Espresso, 0U, 0U, 0U, 0U, BrewingPhase::Idle, SteamLevel::Off, false };
}

void AppTest_Register_Application(FrontendApplication* application)
{
    g_application = application;
}

void AppTest_Register_Model(Model* model)
{
    g_model = model;
}

void AppTest_Set_State(AppTestState state)
{
    g_state = state;
}

AppTestState AppTest_Get_State()
{
    return g_state;
}

void AppTest_Set_Session(const BrewingSession& session)
{
    g_session = session;
}

BrewingSession AppTest_Get_Session()
{
    return g_session;
}

const char* AppTest_Get_Version()
{
    return kProtocolVersion;
}

AppTestResult AppTest_Reset_Demo()
{
    if ((g_application == nullptr) || (g_model == nullptr))
    {
        return AppTestResult::NotReady;
    }

    g_application->Reset_Demo_For_Test();
    g_application->Run_Test_Actions_Now();
    AppTest_Set_State(AppTestState::Selection);
    return AppTestResult::Ok;
}

AppTestResult AppTest_Select_Drink(CoffeeType type)
{
    if ((g_application == nullptr) || (g_model == nullptr))
    {
        return AppTestResult::NotReady;
    }

    if (g_state != AppTestState::Selection)
    {
        return AppTestResult::InvalidState;
    }

    g_application->Start_Brewing_For_Test(type);
    g_application->Run_Test_Actions_Now();
    AppTest_Set_State(AppTestState::Brewing);
    return AppTestResult::Ok;
}

#endif
