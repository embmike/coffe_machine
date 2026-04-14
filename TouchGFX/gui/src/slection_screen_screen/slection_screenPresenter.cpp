#include <gui/slection_screen_screen/slection_screenPresenter.hpp>
#include "app_test_api.hpp"

slection_screenPresenter::slection_screenPresenter(ISelection_View& v)
   : view(v)
{

}

void slection_screenPresenter::activate()
{
   AppTest_Set_State(AppTestState::Selection);
}

void slection_screenPresenter::deactivate()
{

}

void slection_screenPresenter::startBrewing(CoffeeType type)
{
   if (model != 0)
   {
      model->Start_Brewing(type);
   }
}
