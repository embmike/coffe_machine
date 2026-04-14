/**
 * @file brewing_screen_presenter_tests.cpp
 * @brief Unit tests for the brewing presenter.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "app_test_api.hpp"
#include <gui/brewing_screen_screen/IBrewing_View.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>
#include <gui/model/ModelInterfaces.hpp>

namespace
{
class Mock_Brewing_View : public IBrewing_View
{
public:
    MOCK_METHOD(void, Update_Session, (const BrewingSession& session), (override));
    MOCK_METHOD(void, Handle_Brewing_Completed, (), (override));
};

class Mock_Model : public IModel
{
public:
    MOCK_METHOD(void, Start_Brewing, (CoffeeType type), (override));
    MOCK_METHOD(const BrewingSession&, Get_Brewing_Session, (), (const, override));
};
}

using ::testing::_;
using ::testing::Exactly;
using ::testing::Ref;
using ::testing::ReturnRef;

/**
 * @brief Test goal: Activating the presenter shall push the current session to the view.
 *
 * Expected behavior:
 * - the presenter requests the current session from the model
 * - the presenter forwards that session to the view
 * - the observable app-test state becomes brewing
 */
TEST(BrewingScreenPresenterTests, Activate_Updates_View_With_Current_Session)
{
    Mock_Brewing_View view;
    Mock_Model model;
    brewing_screenPresenter presenter(view);
    const BrewingSession session = {
        CoffeeType::Espresso, 25000U, 1000U, 24000U, 4U, BrewingPhase::Start, SteamLevel::Low, true
    };

    presenter.bind(&model);

    EXPECT_CALL(model, Get_Brewing_Session()).Times(Exactly(1)).WillOnce(ReturnRef(session));
    EXPECT_CALL(view, Update_Session(Ref(session))).Times(Exactly(1));

    AppTest_Set_State(AppTestState::Selection);
    presenter.activate();

    EXPECT_EQ(AppTest_Get_State(), AppTestState::Brewing);
}

/**
 * @brief Test goal: Session updates shall be forwarded to the bound view.
 *
 * Expected behavior:
 * - Update_Session is called once with the forwarded session
 */
TEST(BrewingScreenPresenterTests, On_Brewing_Session_Updated_Forwards_Session_To_View)
{
    Mock_Brewing_View view;
    brewing_screenPresenter presenter(view);
    const BrewingSession session = {
        CoffeeType::Latte, 65000U, 13000U, 52000U, 20U, BrewingPhase::Brewing, SteamLevel::Normal, true
    };

    EXPECT_CALL(view, Update_Session(Ref(session))).Times(Exactly(1));

    presenter.onBrewingSessionUpdated(session);
}

/**
 * @brief Test goal: Completion shall be forwarded to the view.
 *
 * Expected behavior:
 * - Handle_Brewing_Completed is called once
 */
TEST(BrewingScreenPresenterTests, On_Brewing_Session_Completed_Forwards_To_View)
{
    Mock_Brewing_View view;
    brewing_screenPresenter presenter(view);

    EXPECT_CALL(view, Handle_Brewing_Completed()).Times(Exactly(1));

    presenter.onBrewingSessionCompleted();
}
