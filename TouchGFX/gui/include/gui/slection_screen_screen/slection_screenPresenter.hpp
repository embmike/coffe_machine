#ifndef SLECTION_SCREENPRESENTER_HPP
#define SLECTION_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class slection_screenView;

class slection_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    slection_screenPresenter(slection_screenView& v);

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

    virtual ~slection_screenPresenter() {}

private:
    slection_screenPresenter();

    slection_screenView& view;
};

#endif // SLECTION_SCREENPRESENTER_HPP
