#ifndef BREWING_SCREENVIEW_HPP
#define BREWING_SCREENVIEW_HPP

#include "coffee_machine_simulation.hpp"
#include <gui_generated/brewing_screen_screen/brewing_screenViewBase.hpp>
#include <gui/brewing_screen_screen/brewing_screenPresenter.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class brewing_screenView : public brewing_screenViewBase
{
public:
    brewing_screenView();
    virtual ~brewing_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    void updateSession(const BrewingSession& session);
    void handleBrewingCompleted();
protected:
    void updateTexts(const BrewingSession& session);
    void updateProgress(uint8_t percent);
    void updateBrewingPhase(const BrewingSession& session);
    void updateHeroImage(CoffeeType type);
    void resetSteamAnimation();
    void applySteamState(bool active, uint16_t tickInterval);
    void advanceSteamFrame();

    touchgfx::TextAreaWithOneWildcard coffeeNameText;
    touchgfx::TextAreaWithOneWildcard coffeeCharacterText;
    touchgfx::TextAreaWithOneWildcard countdownText;
    touchgfx::Unicode::UnicodeChar coffeeNameBuffer[24];
    touchgfx::Unicode::UnicodeChar coffeeCharacterBuffer[32];
    touchgfx::Unicode::UnicodeChar countdownBuffer[8];
    uint8_t steamFrameIndex;
    int8_t steamFrameDirection;
    uint16_t steamTickCounter;
    uint16_t steamTickInterval;
    bool steamAnimationActive;
};

#endif // BREWING_SCREENVIEW_HPP
