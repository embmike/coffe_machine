#include <gui/splash_screen_screen/splash_screenPresenter.hpp>
#include "app_test_api.hpp"

splash_screenPresenter::splash_screenPresenter(ISplash_View& v)
   : view(v)
{

}

void splash_screenPresenter::activate()
{
   AppTest_Set_State(AppTestState::Splash);
}

void splash_screenPresenter::deactivate()
{

}
