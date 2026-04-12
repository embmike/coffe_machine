#include <gui/brewing_screen_screen/brewing_screenView.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>

brewing_screenPresenter::brewing_screenPresenter(brewing_screenView& v)
   : view(v)
{

}

void brewing_screenPresenter::activate()
{
   if (model != 0)
   {
      view.updateSession(model->getBrewingSession());
   }
}

void brewing_screenPresenter::deactivate()
{

}

void brewing_screenPresenter::onBrewingSessionUpdated(const BrewingSession& session)
{
   view.updateSession(session);
}

void brewing_screenPresenter::onBrewingSessionCompleted()
{
   view.handleBrewingCompleted();
}
