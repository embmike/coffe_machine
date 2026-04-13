#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "app_config.h"

#if defined(SIMULATOR) || defined(UNIT_TEST)
#include <chrono>
#include <cstdio>
#endif

#if !defined(SIMULATOR) && !defined(UNIT_TEST)
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
#elif defined(UNIT_TEST)
    return 0U;
#else
    return HAL_GetTick();
#endif
}

void log_selected_coffee(CoffeeType type)
{
#ifdef SIMULATOR
    std::printf("TouchGFX event: %s selected\r\n", CoffeeMachine_GetCoffeeLogName(type));
#elif defined(UNIT_TEST)
    (void)type;
#else
    AppDebugLog("TouchGFX event: %s selected\r\n", CoffeeMachine_GetCoffeeLogName(type));
#endif
}

}

Model::Model()
    : modelListener(0),
      simulation_(),
      tick_source_(0),
      last_tick_ms_(0U),
      done_hold_ms_(0U),
      completion_notified_(false)
{

}

void Model::Set_Tick_Source(ITick_Source* tick_source)
{
    tick_source_ = tick_source;
}

void Model::Start_Brewing(CoffeeType type)
{
    simulation_.start(type);
    done_hold_ms_ = 0U;
    completion_notified_ = false;
    log_selected_coffee(type);
    notifyBrewingSessionUpdated();
}

void Model::startBrewing(CoffeeType type)
{
    Start_Brewing(type);
}

void Model::tick()
{
    const uint32_t now = (tick_source_ != 0) ? tick_source_->Now_Ms() : get_app_tick_ms();

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

const BrewingSession& Model::Get_Brewing_Session() const
{
    return simulation_.getSession();
}

const BrewingSession& Model::getBrewingSession() const
{
    return Get_Brewing_Session();
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
