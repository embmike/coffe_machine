#ifndef COFFEE_MACHINE_SIMULATION_HPP
#define COFFEE_MACHINE_SIMULATION_HPP

#include <cstdint>

enum class CoffeeType
{
    Espresso,
    Cappuccino,
    Latte,
    Americano
};

enum class BrewingPhase
{
    Idle,
    Start,
    Brewing,
    Finishing,
    Done
};

enum class SteamLevel
{
    Off,
    Low,
    Normal,
    Strong
};

struct CoffeeProfile
{
    CoffeeType type;
    const char* name;
    const char* character;
    uint32_t brew_time_ms;
};

struct BrewingSession
{
    CoffeeType coffee_type;
    uint32_t total_time_ms;
    uint32_t elapsed_time_ms;
    uint32_t remaining_time_ms;
    uint8_t progress_percent;
    BrewingPhase phase;
    SteamLevel steam_level;
    bool active;
};

class CoffeeMachineSimulation
{
public:
    CoffeeMachineSimulation();

    void start(CoffeeType type);
    void stop();
    void reset();
    void update(uint32_t delta_ms);

    bool isActive() const;
    bool isFinished() const;

    CoffeeType getCoffeeType() const;
    const CoffeeProfile& getProfile() const;
    BrewingPhase getPhase() const;
    SteamLevel getSteamLevel() const;

    uint32_t getRemainingTimeMs() const;
    uint8_t getProgressPercent() const;
    const char* getCoffeeName() const;
    const char* getCoffeeCharacter() const;

    bool shouldStartPouringAnimation() const;
    bool shouldStopPouringAnimation() const;

    const BrewingSession& getSession() const;

private:
    const CoffeeProfile* findProfile(CoffeeType type) const;
    void updateDerivedState();

    BrewingSession session_;
    const CoffeeProfile* active_profile_;
};

#endif // COFFEE_MACHINE_SIMULATION_HPP
