#include <gui/brewing_screen_screen/brewing_screenView.hpp>

#include "app_config.h"
#include "countdown_formatter.hpp"
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Unicode.hpp>

namespace
{
touchgfx::BitmapId getHeroBitmapId(CoffeeType)
{
    return BITMAP_POURING_FRAME_10_180_120_ID;
}

touchgfx::BitmapId getPouringBitmapId(const BrewingSession& session)
{
    static const touchgfx::BitmapId kPouringFrames[] = {
        BITMAP_POURING_FRAME_01_180_120_ID,
        BITMAP_POURING_FRAME_02_180_120_ID,
        BITMAP_POURING_FRAME_03_180_120_ID,
        BITMAP_POURING_FRAME_04_180_120_ID,
        BITMAP_POURING_FRAME_05_180_120_ID,
        BITMAP_POURING_FRAME_06_180_120_ID,
        BITMAP_POURING_FRAME_07_180_120_ID,
        BITMAP_POURING_FRAME_08_180_120_ID,
        BITMAP_POURING_FRAME_09_180_120_ID,
        BITMAP_POURING_FRAME_10_180_120_ID
    };

    if (session.total_time_ms == 0U)
    {
        return kPouringFrames[0];
    }

    const uint32_t elapsed = session.total_time_ms - session.remaining_time_ms;
    uint32_t frame_index = (elapsed * 10U) / session.total_time_ms;
    if (frame_index >= 10U)
    {
        frame_index = 9U;
    }

    return kPouringFrames[frame_index];
}

touchgfx::BitmapId getSteamBitmapId(uint8_t frameIndex)
{
    static const touchgfx::BitmapId kSteamFrames[] = {
        BITMAP_STEAM_OVERLAY_01_180_120_ID,
        BITMAP_STEAM_OVERLAY_02_180_120_ID,
        BITMAP_STEAM_OVERLAY_03_180_120_ID,
        BITMAP_STEAM_OVERLAY_04_180_120_ID
    };

    if (frameIndex >= 4U)
    {
        frameIndex = 0U;
    }

    return kSteamFrames[frameIndex];
}
}

brewing_screenView::brewing_screenView()
    : steamFrameIndex(0U),
      steamFrameDirection(1),
      steamTickCounter(0U),
      steamTickInterval(8U),
      steamAnimationActive(false)
{
    coffeeNameBuffer[0] = 0;
    coffeeCharacterBuffer[0] = 0;
    countdownBuffer[0] = 0;
}

void brewing_screenView::setupScreen()
{
    brewing_screenViewBase::setupScreen();

    brsc_caffee_name_text.setVisible(false);
    brsc_coffee_character_text.setVisible(false);
    brsc_brewing_count_down.setVisible(false);

    coffeeNameText.setPosition(156, 16, 180, 24);
    coffeeNameText.setColor(touchgfx::Color::getColorFromRGB(232, 226, 215));
    coffeeNameText.setTypedText(touchgfx::TypedText(T_BRSC_BREWING_DYNAMIC_COFFEE_NAME));
    coffeeNameText.setWildcard(coffeeNameBuffer);
    add(coffeeNameText);

    coffeeCharacterText.setPosition(156, 48, 180, 12);
    coffeeCharacterText.setColor(touchgfx::Color::getColorFromRGB(232, 226, 215));
    coffeeCharacterText.setTypedText(touchgfx::TypedText(T_BRSC_BREWING_DYNAMIC_COFFEE_CHARACTER));
    coffeeCharacterText.setWildcard(coffeeCharacterBuffer);
    add(coffeeCharacterText);

    countdownText.setPosition(229, 236, 48, 24);
    countdownText.setColor(touchgfx::Color::getColorFromRGB(232, 226, 215));
    countdownText.setTypedText(touchgfx::TypedText(T_BRSC_BREWING_DYNAMIC_COUNTDOWN));
    countdownText.setWildcard(countdownBuffer);
    add(countdownText);

    brsc_caffee_steam.stopAnimation();
    brsc_caffee_steam.setVisible(false);
    brsc_caffee_steam.setBitmap(touchgfx::Bitmap(getSteamBitmapId(0U)));

    brsc_brewing_progess_bar.setProgressIndicatorPosition(0, 0, 300, 10);
    brsc_brewing_progess_bar.setValue(0);
    brsc_brewing_animated_image.stopAnimation();
    brsc_brewing_animated_image.setBitmap(touchgfx::Bitmap(BITMAP_POURING_FRAME_01_180_120_ID));
}

void brewing_screenView::tearDownScreen()
{
    brewing_screenViewBase::tearDownScreen();
}

void brewing_screenView::handleTickEvent()
{
    if (!steamAnimationActive)
    {
        return;
    }

    ++steamTickCounter;
    if (steamTickCounter < steamTickInterval)
    {
        return;
    }

    steamTickCounter = 0U;
    advanceSteamFrame();
}

void brewing_screenView::updateSession(const BrewingSession& session)
{
    updateHeroImage(session.coffee_type);
    updateTexts(session);
    updateProgress(session.progress_percent);
    updateBrewingPhase(session);
}

void brewing_screenView::Update_Session(const BrewingSession& session)
{
    updateSession(session);
}

void brewing_screenView::handleBrewingCompleted()
{
    application().gotoslection_screenScreenNoTransition();
}

void brewing_screenView::Handle_Brewing_Completed()
{
    handleBrewingCompleted();
}

void brewing_screenView::updateTexts(const BrewingSession& session)
{
    char countdown_ascii[8] = { 0 };

    touchgfx::Unicode::strncpy(coffeeNameBuffer,
        CoffeeMachine_GetCoffeeName(session.coffee_type),
        static_cast<uint16_t>(sizeof(coffeeNameBuffer) / sizeof(coffeeNameBuffer[0])));
    touchgfx::Unicode::strncpy(coffeeCharacterBuffer,
        CoffeeMachine_GetCoffeeCharacter(session.coffee_type),
        static_cast<uint16_t>(sizeof(coffeeCharacterBuffer) / sizeof(coffeeCharacterBuffer[0])));

    format_countdown(session.remaining_time_ms, countdown_ascii, sizeof(countdown_ascii));
    touchgfx::Unicode::strncpy(countdownBuffer,
        countdown_ascii,
        static_cast<uint16_t>(sizeof(countdownBuffer) / sizeof(countdownBuffer[0])));

    coffeeNameText.invalidate();
    coffeeCharacterText.invalidate();
    countdownText.invalidate();
}

void brewing_screenView::updateProgress(uint8_t percent)
{
    brsc_brewing_progess_bar.setValue(percent);
    brsc_brewing_progess_bar.invalidate();
}

void brewing_screenView::resetSteamAnimation()
{
    steamFrameIndex = 0U;
    steamFrameDirection = 1;
    steamTickCounter = 0U;
    brsc_caffee_steam.setBitmap(touchgfx::Bitmap(getSteamBitmapId(steamFrameIndex)));
    brsc_caffee_steam.invalidate();
}

void brewing_screenView::applySteamState(bool active, uint16_t tickInterval)
{
    if (!active)
    {
        steamAnimationActive = false;
        brsc_caffee_steam.setVisible(false);
        resetSteamAnimation();
        return;
    }

    brsc_caffee_steam.setVisible(true);

    if (!steamAnimationActive)
    {
        resetSteamAnimation();
    }

    steamAnimationActive = true;
    steamTickInterval = tickInterval;
}

void brewing_screenView::advanceSteamFrame()
{
    if (steamFrameIndex == 0U)
    {
        steamFrameDirection = 1;
    }
    else if (steamFrameIndex == 3U)
    {
        steamFrameDirection = -1;
    }

    steamFrameIndex = static_cast<uint8_t>(static_cast<int16_t>(steamFrameIndex) + steamFrameDirection);
    brsc_caffee_steam.setBitmap(touchgfx::Bitmap(getSteamBitmapId(steamFrameIndex)));
    brsc_caffee_steam.invalidate();
}

void brewing_screenView::updateBrewingPhase(const BrewingSession& session)
{
    switch (session.phase)
    {
    case BrewingPhase::Start:
        brsc_brewing_animated_image.setVisible(true);
        brsc_brewing_animated_image.stopAnimation();
        brsc_brewing_animated_image.setBitmap(touchgfx::Bitmap(getPouringBitmapId(session)));
        break;
    case BrewingPhase::Brewing:
        brsc_brewing_animated_image.setVisible(true);
        brsc_brewing_animated_image.stopAnimation();
        brsc_brewing_animated_image.setBitmap(touchgfx::Bitmap(getPouringBitmapId(session)));
        break;
    case BrewingPhase::Finishing:
        brsc_brewing_animated_image.setVisible(true);
        brsc_brewing_animated_image.stopAnimation();
        brsc_brewing_animated_image.setBitmap(touchgfx::Bitmap(getPouringBitmapId(session)));
        break;
    case BrewingPhase::Done:
    case BrewingPhase::Idle:
    default:
        brsc_brewing_animated_image.stopAnimation();
        brsc_brewing_animated_image.setVisible(false);
        break;
    }
    brsc_brewing_animated_image.invalidate();

    switch (session.steam_level)
    {
    case SteamLevel::Off:
        applySteamState(false, 8U);
        break;
    case SteamLevel::Low:
        applySteamState(true, APP_STEAM_TICK_INTERVAL_LOW);
        break;
    case SteamLevel::Normal:
        applySteamState(true, APP_STEAM_TICK_INTERVAL_NORMAL);
        break;
    case SteamLevel::Strong:
        applySteamState(true, APP_STEAM_TICK_INTERVAL_STRONG);
        break;
    }
}

void brewing_screenView::updateHeroImage(CoffeeType type)
{
    brsc_brewing_hero_image.setBitmap(touchgfx::Bitmap(getHeroBitmapId(type)));
    brsc_brewing_hero_image.invalidate();
}
