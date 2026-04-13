/**
 * @file ModelInterfaces.hpp
 * @brief Small handwritten interfaces used to unit-test model and presenter logic.
 */

#ifndef MODEL_INTERFACES_HPP
#define MODEL_INTERFACES_HPP

#include "coffee_machine_simulation.hpp"

#include <cstdint>

/**
 * @brief Abstraction for a tick time source used by the TouchGFX model.
 */
class ITick_Source
{
public:
    virtual ~ITick_Source() = default;

    /**
     * @brief Returns the current time base in milliseconds.
     * @return Current tick time in milliseconds.
     */
    virtual uint32_t Now_Ms() = 0;
};

/**
 * @brief Small model contract used by presenters and unit tests.
 */
class IModel
{
public:
    virtual ~IModel() = default;

    /**
     * @brief Starts a brewing session for the selected drink.
     * @param type Selected drink type.
     */
    virtual void Start_Brewing(CoffeeType type) = 0;

    /**
     * @brief Returns the current brewing session snapshot.
     * @return Active brewing session state.
     */
    virtual const BrewingSession& Get_Brewing_Session() const = 0;
};

#endif // MODEL_INTERFACES_HPP
