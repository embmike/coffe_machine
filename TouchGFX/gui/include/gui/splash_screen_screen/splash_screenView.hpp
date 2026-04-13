#ifndef SPLASH_SCREENVIEW_HPP
#define SPLASH_SCREENVIEW_HPP

#include <gui/splash_screen_screen/ISplash_View.hpp>
#include <gui_generated/splash_screen_screen/splash_screenViewBase.hpp>
#include <gui/splash_screen_screen/splash_screenPresenter.hpp>

class splash_screenView : public splash_screenViewBase, public ISplash_View
{
public:
   splash_screenView();
   virtual ~splash_screenView() {}
   virtual void setupScreen();
   virtual void tearDownScreen();
   virtual void handleTickEvent();
protected:
   static constexpr uint16_t SPLASH_TIMEOUT_TICKS = 300U;

   uint16_t tickCounter;
};

#endif // SPLASH_SCREENVIEW_HPP
