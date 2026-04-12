/**
 * @file coffee_machine_simulation.hpp
 * @brief Brewing-domain types and the simple demonstrator state machine.
 */

#ifndef COFFEE_MACHINE_SIMULATION_HPP
#define COFFEE_MACHINE_SIMULATION_HPP

#include <cstdint>

/**
 * @brief Supported drinks in the demonstrator.
 */
enum class CoffeeType
{
    Espresso,
    Cappuccino,
    Latte,
    Americano
};

/**
 * @brief High-level phases of one brewing session.
 */
enum class BrewingPhase
{
    Idle,
    Start,
    Brewing,
    Finishing,
    Done
};

/**
 * @brief Visual steam intensity used by the brewing screen.
 */
enum class SteamLevel
{
    Off,
    Low,
    Normal,
    Strong
};

/**
 * @brief Static metadata for one supported drink.
 */
struct CoffeeProfile
{
    CoffeeType type;
    const char* name;       ///< Display name used by the GUI.
    const char* character;  ///< Short descriptive subtitle for the GUI.
    uint32_t brew_time_ms;  ///< Nominal brewing duration in milliseconds.
};

/**
 * @brief Returns the profile metadata for a drink type.
 * @param type The selected drink.
 * @return Pointer to the matching profile, or nullptr if the type is unknown.
 */
const CoffeeProfile* CoffeeMachine_FindProfile(CoffeeType type);

/**
 * @brief Returns the localized display name for a drink.
 * @param type The selected drink.
 * @return Null-terminated display string.
 */
const char* CoffeeMachine_GetCoffeeName(CoffeeType type);

/**
 * @brief Returns the descriptive subtitle for a drink.
 * @param type The selected drink.
 * @return Null-terminated description string.
 */
const char* CoffeeMachine_GetCoffeeCharacter(CoffeeType type);

/**
 * @brief Returns a lowercase log label for a drink.
 * @param type The selected drink.
 * @return Null-terminated log string.
 */
const char* CoffeeMachine_GetCoffeeLogName(CoffeeType type);

/**
 * @brief Runtime snapshot of the active brewing session.
 */
struct BrewingSession
{
    CoffeeType coffee_type;       ///< Active drink type.
    uint32_t total_time_ms;       ///< Total brewing duration in milliseconds.
    uint32_t elapsed_time_ms;     ///< Already simulated time in milliseconds.
    uint32_t remaining_time_ms;   ///< Remaining brewing time in milliseconds.
    uint8_t progress_percent;     ///< Progress value in the range 0..100.
    BrewingPhase phase;           ///< High-level brewing phase.
    SteamLevel steam_level;       ///< Steam intensity for the UI animation.
    bool active;                  ///< True while the brewing session is active.
};

/**
 * @brief Small demonstrator state machine for the brewing workflow.
 */
class CoffeeMachineSimulation
{
public:
    /**
     * @brief Constructs an idle simulation instance.
     */
    CoffeeMachineSimulation();

    /**
     * @brief Starts a new brewing session for the selected drink.
     * @param type The selected drink.
     */
    void start(CoffeeType type);

    /**
     * @brief Stops the active session and returns to the idle state.
     */
    void stop();

    /**
     * @brief Resets all runtime state to the idle state.
     */
    void reset();

    /**
     * @brief Advances the simulation state by one time slice.
     * @param delta_ms Elapsed time in milliseconds since the last update.
     */
    void update(uint32_t delta_ms);

    /**
     * @brief Indicates whether a brewing session is active.
     * @return True while brewing is active.
     */
    bool isActive() const;

    /**
     * @brief Indicates whether the current session reached the done phase.
     * @return True when brewing has finished.
     */
    bool isFinished() const;

    /**
     * @brief Returns the currently selected drink type.
     * @return Active coffee type.
     */
    CoffeeType getCoffeeType() const;

    /**
     * @brief Returns the active profile metadata.
     * @return Active profile or the default fallback profile.
     */
    const CoffeeProfile& getProfile() const;

    /**
     * @brief Returns the current brewing phase.
     * @return Current phase.
     */
    BrewingPhase getPhase() const;

    /**
     * @brief Returns the current steam intensity.
     * @return Steam level for UI animation.
     */
    SteamLevel getSteamLevel() const;

    /**
     * @brief Returns the remaining brewing time.
     * @return Remaining time in milliseconds.
     */
    uint32_t getRemainingTimeMs() const;

    /**
     * @brief Returns the current progress percentage.
     * @return Progress in the range 0..100.
     */
    uint8_t getProgressPercent() const;

    /**
     * @brief Returns the GUI display name of the active drink.
     * @return Null-terminated display name.
     */
    const char* getCoffeeName() const;

    /**
     * @brief Returns the GUI subtitle of the active drink.
     * @return Null-terminated description string.
     */
    const char* getCoffeeCharacter() const;

    /**
     * @brief Indicates whether the pouring animation should be visible.
     * @return True while the brewing animation should run.
     */
    bool shouldStartPouringAnimation() const;

    /**
     * @brief Indicates whether the pouring animation should stop.
     * @return True when the animation should no longer be visible.
     */
    bool shouldStopPouringAnimation() const;

    /**
     * @brief Returns the complete runtime session snapshot.
     * @return Const reference to the session state.
     */
    const BrewingSession& getSession() const;

private:
    /**
     * @brief Resolves the static profile metadata for a drink type.
     * @param type The selected drink.
     * @return Matching profile pointer or nullptr.
     */
    const CoffeeProfile* findProfile(CoffeeType type) const;

    /**
     * @brief Recomputes derived runtime values after a state update.
     */
    void updateDerivedState();

    BrewingSession session_;              ///< Mutable runtime state of the current brewing session.
    const CoffeeProfile* active_profile_; ///< Metadata of the currently selected drink profile.
};

#endif // COFFEE_MACHINE_SIMULATION_HPP
