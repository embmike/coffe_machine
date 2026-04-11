#ifndef SLECTION_SCREENVIEW_HPP
#define SLECTION_SCREENVIEW_HPP

#include <gui_generated/slection_screen_screen/slection_screenViewBase.hpp>
#include <gui/slection_screen_screen/slection_screenPresenter.hpp>
#include <touchgfx/Callback.hpp>

class slection_screenView : public slection_screenViewBase
{
public:
   slection_screenView();
   virtual ~slection_screenView() {}
   virtual void setupScreen();
   virtual void tearDownScreen();
protected:
   void onEspressoPressed(const touchgfx::AbstractButtonContainer& button);
   void onCappuccinoPressed(const touchgfx::AbstractButtonContainer& button);
   void onLattePressed(const touchgfx::AbstractButtonContainer& button);
   void onAmericanoPressed(const touchgfx::AbstractButtonContainer& button);

   touchgfx::Callback<slection_screenView, const touchgfx::AbstractButtonContainer&> espressoPressedCallback;
   touchgfx::Callback<slection_screenView, const touchgfx::AbstractButtonContainer&> cappuccinoPressedCallback;
   touchgfx::Callback<slection_screenView, const touchgfx::AbstractButtonContainer&> lattePressedCallback;
   touchgfx::Callback<slection_screenView, const touchgfx::AbstractButtonContainer&> americanoPressedCallback;
};

#endif // SLECTION_SCREENVIEW_HPP
