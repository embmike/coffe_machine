#include <gui/slection_screen_screen/slection_screenView.hpp>
#include "main.h"

slection_screenView::slection_screenView()
    : espressoPressedCallback(this, &slection_screenView::onEspressoPressed),
      cappuccinoPressedCallback(this, &slection_screenView::onCappuccinoPressed),
      lattePressedCallback(this, &slection_screenView::onLattePressed),
      americanoPressedCallback(this, &slection_screenView::onAmericanoPressed)
{
}

void slection_screenView::setupScreen()
{
    slection_screenViewBase::setupScreen();
    sesc_espresso_button.setAction(espressoPressedCallback);
    sesc_cappucino_button.setAction(cappuccinoPressedCallback);
    sesc_latte_button.setAction(lattePressedCallback);
    sesc_americano_button.setAction(americanoPressedCallback);
}

void slection_screenView::tearDownScreen()
{
    slection_screenViewBase::tearDownScreen();
}

void slection_screenView::onEspressoPressed(const touchgfx::AbstractButtonContainer&)
{
    AppDebugLog("TouchGFX event: espresso selected\r\n");
}

void slection_screenView::onCappuccinoPressed(const touchgfx::AbstractButtonContainer&)
{
    AppDebugLog("TouchGFX event: cappuccino selected\r\n");
}

void slection_screenView::onLattePressed(const touchgfx::AbstractButtonContainer&)
{
    AppDebugLog("TouchGFX event: latte selected\r\n");
}

void slection_screenView::onAmericanoPressed(const touchgfx::AbstractButtonContainer&)
{
    AppDebugLog("TouchGFX event: americano selected\r\n");
}
