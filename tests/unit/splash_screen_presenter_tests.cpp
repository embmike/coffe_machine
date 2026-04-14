/**
 * @file splash_screen_presenter_tests.cpp
 * @brief Unit tests for the splash presenter.
 */

#include <gtest/gtest.h>

#include "app_test_api.hpp"
#include <gui/splash_screen_screen/ISplash_View.hpp>
#include <gui/splash_screen_screen/splash_screenPresenter.hpp>

namespace
{
class Dummy_Splash_View : public ISplash_View
{
};
}

/**
 * @brief Test goal: Activating the splash presenter shall expose the splash state.
 *
 * Expected behavior:
 * - the observable app-test state becomes splash
 */
TEST(SplashScreenPresenterTests, Activate_Sets_App_Test_State_To_Splash)
{
    Dummy_Splash_View view;
    splash_screenPresenter presenter(view);

    AppTest_Set_State(AppTestState::Selection);
    presenter.activate();

    EXPECT_EQ(AppTest_Get_State(), AppTestState::Splash);
}

/**
 * @brief Test goal: Deactivating the splash presenter shall not cause side effects.
 *
 * Expected behavior:
 * - deactivate returns without error
 */
TEST(SplashScreenPresenterTests, Deactivate_Has_No_Side_Effect)
{
    Dummy_Splash_View view;
    splash_screenPresenter presenter(view);

    presenter.deactivate();

    SUCCEED();
}
