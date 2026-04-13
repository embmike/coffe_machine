#include <gui/slection_screen_screen/slection_screenPresenter.hpp>

slection_screenPresenter::slection_screenPresenter(ISelection_View& v)
   : view(v)
{

}

void slection_screenPresenter::activate()
{

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
