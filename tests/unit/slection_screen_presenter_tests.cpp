/**
 * @file slection_screen_presenter_tests.cpp
 * @brief Unit tests for the selection presenter.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gui/model/ModelInterfaces.hpp>
#include <gui/slection_screen_screen/ISelection_View.hpp>
#include <gui/slection_screen_screen/slection_screenPresenter.hpp>

namespace
{
class Dummy_Selection_View : public ISelection_View
{
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

/**
 * @brief Test goal: The selected coffee type shall be forwarded to the model.
 *
 * Expected behavior:
 * - Start_Brewing is called once with Espresso
 */
TEST(SlectionScreenPresenterTests, Start_Brewing_Forwards_Type_To_Model)
{
    Dummy_Selection_View view;
    Mock_Model model;
    slection_screenPresenter presenter(view);

    presenter.bind(&model);

    EXPECT_CALL(model, Start_Brewing(CoffeeType::Espresso)).Times(Exactly(1));

    presenter.startBrewing(CoffeeType::Espresso);
}

/**
 * @brief Test goal: Calling start without a bound model shall do nothing.
 *
 * Expected behavior:
 * - the presenter returns without dereferencing a model pointer
 */
TEST(SlectionScreenPresenterTests, Start_Brewing_Does_Nothing_Without_Model)
{
    Dummy_Selection_View view;
    slection_screenPresenter presenter(view);

    presenter.startBrewing(CoffeeType::Latte);

    SUCCEED();
}
