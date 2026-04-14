#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>
#include "app_test_api.hpp"

brewing_screenPresenter::brewing_screenPresenter(IBrewing_View& v)
   : view(v)
{

}

void brewing_screenPresenter::activate()
{
   AppTest_Set_State(AppTestState::Brewing);
   if (model != 0)
   {
      view.Update_Session(model->Get_Brewing_Session());
   }
}

void brewing_screenPresenter::deactivate()
{

}

void brewing_screenPresenter::onBrewingSessionUpdated(const BrewingSession& session)
{
   view.Update_Session(session);
}

void brewing_screenPresenter::onBrewingSessionCompleted()
{
   view.Handle_Brewing_Completed();
}
