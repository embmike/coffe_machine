#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "app_config.h"

#ifdef SIMULATOR
#include <chrono>
#include <cstdio>
#else
#include "main.h"
#endif

namespace
{
uint32_t get_app_tick_ms()
{
#ifdef SIMULATOR
    using clock = std::chrono::steady_clock;
    static const auto start = clock::now();
    const auto now = clock::now();
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
#else
    return HAL_GetTick();
#endif
}

const char* coffeeTypeToName(CoffeeType type)
{
    switch (type)
    {
    case CoffeeType::Espresso:
        return "espresso";
    case CoffeeType::Cappuccino:
        return "cappuccino";
    case CoffeeType::Latte:
        return "latte";
    case CoffeeType::Americano:
        return "americano";
    default:
        return "unknown";
    }
}
}

Model::Model()
    : modelListener(0),
      simulation_(),
      last_tick_ms_(0U),
      done_hold_ms_(0U),
      completion_notified_(false)
{

}

void Model::startBrewing(CoffeeType type)
{
    simulation_.start(type);
    done_hold_ms_ = 0U;
    completion_notified_ = false;
#ifdef SIMULATOR
    std::printf("TouchGFX event: %s selected\r\n", coffeeTypeToName(type));
#else
    AppDebugLog("TouchGFX event: %s selected\r\n", coffeeTypeToName(type));
#endif
    notifyBrewingSessionUpdated();
}

void Model::tick()
{
    const uint32_t now = get_app_tick_ms();

    if (last_tick_ms_ == 0U)
    {
        last_tick_ms_ = now;
        return;
    }

    const uint32_t delta_ms = now - last_tick_ms_;
    last_tick_ms_ = now;

    if (!simulation_.isActive())
    {
        return;
    }

    simulation_.update(delta_ms);
    notifyBrewingSessionUpdated();

    if (simulation_.isFinished())
    {
        done_hold_ms_ += delta_ms;
        if ((done_hold_ms_ >= APP_BREWING_DONE_HOLD_MS) && !completion_notified_)
        {
            completion_notified_ = true;
            if (modelListener != 0)
            {
                modelListener->onBrewingSessionCompleted();
            }
            simulation_.stop();
        }
    }
}

const BrewingSession& Model::getBrewingSession() const
{
    return simulation_.getSession();
}

bool Model::isBrewingActive() const
{
    return simulation_.isActive();
}

void Model::notifyBrewingSessionUpdated()
{
    if (modelListener != 0)
    {
        modelListener->onBrewingSessionUpdated(simulation_.getSession());
    }
}
