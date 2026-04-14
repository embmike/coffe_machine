#include <gui/common/FrontendApplication.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/slection_screen_screen/slection_screenPresenter.hpp>
#include <gui/slection_screen_screen/slection_screenView.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>
#include <gui/brewing_screen_screen/brewing_screenView.hpp>
#include "app_test_api.hpp"
#include <touchgfx/transitions/NoTransition.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap),
      selectionTransitionCallback(this, &FrontendApplication::gotoslection_screenScreenNoTransitionImpl),
      brewingTransitionCallback(this, &FrontendApplication::gotobrewing_screenScreenNoTransitionImpl)
{
    AppTest_Register_Application(this);
}

void FrontendApplication::gotoslection_screenScreenNoTransition()
{
    pendingScreenTransitionCallback = &selectionTransitionCallback;
}

void FrontendApplication::gotoslection_screenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<slection_screenView, slection_screenPresenter, touchgfx::NoTransition, Model>(
        &currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotobrewing_screenScreenNoTransition()
{
    pendingScreenTransitionCallback = &brewingTransitionCallback;
}

void FrontendApplication::gotobrewing_screenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<brewing_screenView, brewing_screenPresenter, touchgfx::NoTransition, Model>(
        &currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::Start_Brewing_For_Test(CoffeeType type)
{
    pending_coffee_type_ = type;
    pending_start_brewing_ = true;
}

void FrontendApplication::Reset_Demo_For_Test()
{
    pending_reset_demo_ = true;
}

void FrontendApplication::Run_Test_Actions_Now()
{
    Process_Test_Actions();
}

void FrontendApplication::handleTickEvent()
{
    Process_Test_Actions();
    model.tick();
    FrontendApplicationBase::handleTickEvent();
}

void FrontendApplication::Process_Test_Actions()
{
    if (pending_reset_demo_)
    {
        pending_reset_demo_ = false;
        pending_start_brewing_ = false;
        model.Reset_Demo();
        gotoslection_screenScreenNoTransition();
    }

    if (pending_start_brewing_)
    {
        pending_start_brewing_ = false;
        model.Start_Brewing(pending_coffee_type_);
        gotobrewing_screenScreenNoTransition();
    }
}
