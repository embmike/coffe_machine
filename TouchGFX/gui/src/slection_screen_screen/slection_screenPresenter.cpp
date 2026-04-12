#include <gui/slection_screen_screen/slection_screenView.hpp>
#include <gui/slection_screen_screen/slection_screenPresenter.hpp>

slection_screenPresenter::slection_screenPresenter(slection_screenView& v)
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
      model->startBrewing(type);
   }
}
