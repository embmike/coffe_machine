#ifndef BREWING_SCREENPRESENTER_HPP
#define BREWING_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class brewing_screenView;

class brewing_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
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

private:
    brewing_screenPresenter();

    brewing_screenView& view;
};

#endif // BREWING_SCREENPRESENTER_HPP
