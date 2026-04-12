#include "coffee_machine_simulation.hpp"

namespace
{
constexpr CoffeeProfile kCoffeeProfiles[] = {
    { CoffeeType::Espresso, "Espresso", "strong & intense", 25000U },
    { CoffeeType::Cappuccino, "Cappuccino", "creamy & smooth", 45000U },
    { CoffeeType::Latte, "Latte", "mild & silky", 65000U },
    { CoffeeType::Americano, "Americano", "clean & aromatic", 40000U }
};
}

CoffeeMachineSimulation::CoffeeMachineSimulation()
    : session_{ CoffeeType::Espresso, 0U, 0U, 0U, 0U, BrewingPhase::Idle, SteamLevel::Off, false },
      active_profile_(nullptr)
{
}

void CoffeeMachineSimulation::start(CoffeeType type)
{
    active_profile_ = findProfile(type);
    if (active_profile_ == nullptr)
    {
        reset();
        return;
    }

    session_.coffee_type = type;
    session_.total_time_ms = active_profile_->brew_time_ms;
    session_.elapsed_time_ms = 0U;
    session_.remaining_time_ms = active_profile_->brew_time_ms;
    session_.progress_percent = 0U;
    session_.phase = BrewingPhase::Start;
    session_.steam_level = SteamLevel::Low;
    session_.active = true;
}

void CoffeeMachineSimulation::stop()
{
    reset();
}

void CoffeeMachineSimulation::reset()
{
    session_.elapsed_time_ms = 0U;
    session_.remaining_time_ms = 0U;
    session_.total_time_ms = 0U;
    session_.progress_percent = 0U;
    session_.phase = BrewingPhase::Idle;
    session_.steam_level = SteamLevel::Off;
    session_.active = false;
    active_profile_ = nullptr;
}

void CoffeeMachineSimulation::update(uint32_t delta_ms)
{
    if (!session_.active || active_profile_ == nullptr)
    {
        return;
    }

    const uint32_t next_elapsed = session_.elapsed_time_ms + delta_ms;
    session_.elapsed_time_ms = (next_elapsed > session_.total_time_ms) ? session_.total_time_ms : next_elapsed;
    updateDerivedState();
}

bool CoffeeMachineSimulation::isActive() const
{
    return session_.active;
}

bool CoffeeMachineSimulation::isFinished() const
{
    return session_.phase == BrewingPhase::Done;
}

CoffeeType CoffeeMachineSimulation::getCoffeeType() const
{
    return session_.coffee_type;
}

const CoffeeProfile& CoffeeMachineSimulation::getProfile() const
{
    return (active_profile_ != nullptr) ? *active_profile_ : kCoffeeProfiles[0];
}

BrewingPhase CoffeeMachineSimulation::getPhase() const
{
    return session_.phase;
}

SteamLevel CoffeeMachineSimulation::getSteamLevel() const
{
    return session_.steam_level;
}

uint32_t CoffeeMachineSimulation::getRemainingTimeMs() const
{
    return session_.remaining_time_ms;
}

uint8_t CoffeeMachineSimulation::getProgressPercent() const
{
    return session_.progress_percent;
}

const char* CoffeeMachineSimulation::getCoffeeName() const
{
    return getProfile().name;
}

const char* CoffeeMachineSimulation::getCoffeeCharacter() const
{
    return getProfile().character;
}

bool CoffeeMachineSimulation::shouldStartPouringAnimation() const
{
    return session_.phase == BrewingPhase::Brewing;
}

bool CoffeeMachineSimulation::shouldStopPouringAnimation() const
{
    return session_.phase == BrewingPhase::Finishing || session_.phase == BrewingPhase::Done || session_.phase == BrewingPhase::Idle;
}

const BrewingSession& CoffeeMachineSimulation::getSession() const
{
    return session_;
}

const CoffeeProfile* CoffeeMachineSimulation::findProfile(CoffeeType type) const
{
    for (const CoffeeProfile& profile : kCoffeeProfiles)
    {
        if (profile.type == type)
        {
            return &profile;
        }
    }

    return nullptr;
}

void CoffeeMachineSimulation::updateDerivedState()
{
    if (session_.total_time_ms == 0U)
    {
        session_.remaining_time_ms = 0U;
        session_.progress_percent = 0U;
        session_.phase = BrewingPhase::Idle;
        session_.steam_level = SteamLevel::Off;
        session_.active = false;
        return;
    }

    session_.remaining_time_ms = session_.total_time_ms - session_.elapsed_time_ms;
    session_.progress_percent = static_cast<uint8_t>((session_.elapsed_time_ms * 100U) / session_.total_time_ms);
    if (session_.progress_percent > 100U)
    {
        session_.progress_percent = 100U;
    }

    if (session_.progress_percent >= 100U)
    {
        session_.phase = BrewingPhase::Done;
        session_.steam_level = SteamLevel::Low;
    }
    else if (session_.progress_percent >= 80U)
    {
        session_.phase = BrewingPhase::Finishing;
        session_.steam_level = SteamLevel::Strong;
    }
    else if (session_.progress_percent >= 20U)
    {
        session_.phase = BrewingPhase::Brewing;
        session_.steam_level = SteamLevel::Normal;
    }
    else
    {
        session_.phase = BrewingPhase::Start;
        session_.steam_level = SteamLevel::Low;
    }
}
