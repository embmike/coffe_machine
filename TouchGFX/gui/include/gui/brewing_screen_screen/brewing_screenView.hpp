#ifndef BREWING_SCREENVIEW_HPP
#define BREWING_SCREENVIEW_HPP

#include <gui_generated/brewing_screen_screen/brewing_screenViewBase.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>

class brewing_screenView : public brewing_screenViewBase
{
public:
    brewing_screenView();
    virtual ~brewing_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // BREWING_SCREENVIEW_HPP
