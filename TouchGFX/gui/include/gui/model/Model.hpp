#ifndef MODEL_HPP
#define MODEL_HPP

#include "coffee_machine_simulation.hpp"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void startBrewing(CoffeeType type);
    void tick();

    const BrewingSession& getBrewingSession() const;
    bool isBrewingActive() const;
protected:
    ModelListener* modelListener;

private:
    void notifyBrewingSessionUpdated();

    CoffeeMachineSimulation simulation_;
    uint32_t last_tick_ms_;
    uint32_t done_hold_ms_;
    bool completion_notified_;
};

#endif // MODEL_HPP
