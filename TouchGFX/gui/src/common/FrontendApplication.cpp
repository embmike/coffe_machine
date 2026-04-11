#include <gui/common/FrontendApplication.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/slection_screen_screen/slection_screenPresenter.hpp>
#include <gui/slection_screen_screen/slection_screenView.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>
#include <gui/brewing_screen_screen/brewing_screenView.hpp>
#include <touchgfx/transitions/NoTransition.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::gotoslection_screenScreenNoTransition()
{
    touchgfx::makeTransition<slection_screenView, slection_screenPresenter, touchgfx::NoTransition, Model>(
        &currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotobrewing_screenScreenNoTransition()
{
    touchgfx::makeTransition<brewing_screenView, brewing_screenPresenter, touchgfx::NoTransition, Model>(
        &currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
