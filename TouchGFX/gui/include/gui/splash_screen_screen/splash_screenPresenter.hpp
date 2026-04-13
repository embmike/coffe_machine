#ifndef SPLASH_SCREENPRESENTER_HPP
#define SPLASH_SCREENPRESENTER_HPP

#include <gui/splash_screen_screen/ISplash_View.hpp>
#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class splash_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    splash_screenPresenter(ISplash_View& v);

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

    virtual ~splash_screenPresenter() {}

private:
    splash_screenPresenter();

    ISplash_View& view;
};

#endif // SPLASH_SCREENPRESENTER_HPP
