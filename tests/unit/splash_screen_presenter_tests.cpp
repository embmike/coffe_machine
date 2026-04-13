/**
 * @file splash_screen_presenter_tests.cpp
 * @brief Unit tests for the splash presenter.
 */

#include <gtest/gtest.h>

#include <gui/splash_screen_screen/ISplash_View.hpp>
#include <gui/splash_screen_screen/splash_screenPresenter.hpp>

namespace
{
class Dummy_Splash_View : public ISplash_View
{
};
}

/**
 * @brief Test goal: Activating the splash presenter shall not cause side effects.
 *
 * Expected behavior:
 * - activate returns without error
 */
TEST(SplashScreenPresenterTests, Activate_Has_No_Side_Effect)
{
    Dummy_Splash_View view;
    splash_screenPresenter presenter(view);

    presenter.activate();

    SUCCEED();
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
