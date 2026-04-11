#include <gui/splash_screen_screen/splash_screenView.hpp>

splash_screenView::splash_screenView()
    : tickCounter(0U)
{
}

void splash_screenView::setupScreen()
{
    splash_screenViewBase::setupScreen();
    tickCounter = 0U;
}

void splash_screenView::tearDownScreen()
{
    splash_screenViewBase::tearDownScreen();
}

void splash_screenView::handleTickEvent()
{
    if (tickCounter < SPLASH_TIMEOUT_TICKS)
    {
        ++tickCounter;
    }

    if (tickCounter == SPLASH_TIMEOUT_TICKS)
    {
        ++tickCounter;
        application().gotoslection_screenScreenNoTransition();
    }
}
