/**
 * @file Model.hpp
 * @brief TouchGFX model layer for the coffee machine demonstrator.
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include "coffee_machine_simulation.hpp"

class ModelListener;

class Model
{
public:
    /**
     * @brief Constructs the model in its idle state.
     */
    Model();

    /**
     * @brief Binds the active screen listener to the model.
     * @param listener Active presenter-side listener.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    /**
     * @brief Starts a brewing session for the selected drink.
     * @param type Selected drink type.
     */
    void startBrewing(CoffeeType type);

    /**
     * @brief Advances the application model by one UI tick.
     */
    void tick();

    /**
     * @brief Returns the current brewing session snapshot.
     * @return Active brewing session state.
     */
    const BrewingSession& getBrewingSession() const;

    /**
     * @brief Indicates whether a brewing session is currently active.
     * @return True while brewing is active.
     */
    bool isBrewingActive() const;
protected:
    ModelListener* modelListener; ///< Active presenter callback interface.

private:
    /**
     * @brief Notifies the active presenter about an updated brewing session.
     */
    void notifyBrewingSessionUpdated();

    CoffeeMachineSimulation simulation_; ///< Brewing-domain state machine.
    uint32_t last_tick_ms_;              ///< Last tick timestamp used for delta calculation.
    uint32_t done_hold_ms_;              ///< Post-completion hold time before returning to selection.
    bool completion_notified_;           ///< Guards one-shot completion notification.
};

#endif // MODEL_HPP
