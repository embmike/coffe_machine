/**
 * @file brewing_screenView.hpp
 * @brief View logic for the brewing screen.
 */

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
    /**
     * @brief Constructs the brewing screen view.
     */
    brewing_screenView();
    virtual ~brewing_screenView() {}

    /**
     * @brief Initializes dynamic widgets and animation state.
     */
    virtual void setupScreen();

    /**
     * @brief Tears down the brewing screen.
     */
    virtual void tearDownScreen();

    /**
     * @brief Drives the manual steam animation on UI ticks.
     */
    virtual void handleTickEvent();

    /**
     * @brief Applies a new brewing session snapshot to the screen.
     * @param session Current brewing session state.
     */
    void updateSession(const BrewingSession& session);

    /**
     * @brief Returns to the selection screen after brewing completes.
     */
    void handleBrewingCompleted();
protected:
    /**
     * @brief Updates the dynamic text overlays for the current session.
     * @param session Current brewing session state.
     */
    void updateTexts(const BrewingSession& session);

    /**
     * @brief Updates the progress bar value.
     * @param percent Progress value in the range 0..100.
     */
    void updateProgress(uint8_t percent);

    /**
     * @brief Updates the hero image, pouring animation, and steam state.
     * @param session Current brewing session state.
     */
    void updateBrewingPhase(const BrewingSession& session);

    /**
     * @brief Updates the static hero bitmap for the selected drink.
     * @param type Selected drink type.
     */
    void updateHeroImage(CoffeeType type);

    /**
     * @brief Resets the manual steam animation to its first frame.
     */
    void resetSteamAnimation();

    /**
     * @brief Applies the desired steam animation state.
     * @param active True if the steam overlay should animate.
     * @param tickInterval Number of UI ticks between frame changes.
     */
    void applySteamState(bool active, uint16_t tickInterval);

    /**
     * @brief Advances the manual steam animation by one frame.
     */
    void advanceSteamFrame();

    touchgfx::TextAreaWithOneWildcard coffeeNameText;             ///< Dynamic overlay for the selected coffee name.
    touchgfx::TextAreaWithOneWildcard coffeeCharacterText;        ///< Dynamic overlay for the coffee subtitle.
    touchgfx::TextAreaWithOneWildcard countdownText;              ///< Dynamic overlay for the brewing countdown.
    touchgfx::Unicode::UnicodeChar coffeeNameBuffer[24];          ///< Wildcard buffer for the coffee name.
    touchgfx::Unicode::UnicodeChar coffeeCharacterBuffer[32];     ///< Wildcard buffer for the coffee subtitle.
    touchgfx::Unicode::UnicodeChar countdownBuffer[8];            ///< Wildcard buffer for the decimal countdown.
    uint8_t steamFrameIndex;                                      ///< Current frame index in the manual steam animation.
    int8_t steamFrameDirection;                                   ///< Ping-pong direction for the steam frame sequence.
    uint16_t steamTickCounter;                                    ///< Tick accumulator for the next steam frame change.
    uint16_t steamTickInterval;                                   ///< Number of UI ticks between steam frame changes.
    bool steamAnimationActive;                                    ///< True while the steam overlay is animated.
};

#endif // BREWING_SCREENVIEW_HPP
