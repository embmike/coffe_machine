/**
 * @file brewing_screenPresenter.hpp
 * @brief Presenter for the brewing screen.
 */

#ifndef BREWING_SCREENPRESENTER_HPP
#define BREWING_SCREENPRESENTER_HPP

#include "coffee_machine_simulation.hpp"
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class brewing_screenView;

class brewing_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
   /**
    * @brief Constructs the presenter for the brewing screen.
    * @param v Bound view instance.
    */
   brewing_screenPresenter(brewing_screenView& v);

   /**
    * The activate function is called automatically when this screen is "switched in"
    * (ie. made active). Initialization logic can be placed here.
    */
   virtual void activate();

   /**
    * The deactivate function is called automatically when this screen is "switched out"
    * (ie. made inactive). Teardown functionality can be placed here.
    */
   virtual void deactivate();

   virtual ~brewing_screenPresenter() {}

   /**
    * @brief Forwards updated session state to the view.
    * @param session Current brewing session state.
    */
   virtual void onBrewingSessionUpdated(const BrewingSession& session);

   /**
    * @brief Notifies the view that brewing finished.
    */
   virtual void onBrewingSessionCompleted();

private:
   brewing_screenPresenter();

   brewing_screenView& view; ///< Bound brewing screen view.
};

#endif // BREWING_SCREENPRESENTER_HPP
