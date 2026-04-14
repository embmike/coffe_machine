#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
   FrontendApplication(Model& m, FrontendHeap& heap);
   virtual ~FrontendApplication() {}

   void gotoslection_screenScreenNoTransition();
   void gotobrewing_screenScreenNoTransition();
   void Start_Brewing_For_Test(CoffeeType type);
   void Reset_Demo_For_Test();
   void Run_Test_Actions_Now();

   virtual void handleTickEvent() override;
private:
   void gotoslection_screenScreenNoTransitionImpl();
   void gotobrewing_screenScreenNoTransitionImpl();
   void Process_Test_Actions();

   touchgfx::Callback<FrontendApplication> selectionTransitionCallback;
   touchgfx::Callback<FrontendApplication> brewingTransitionCallback;
   bool pending_reset_demo_ = false;
   bool pending_start_brewing_ = false;
   CoffeeType pending_coffee_type_ = CoffeeType::Espresso;
};

#endif // FRONTENDAPPLICATION_HPP
