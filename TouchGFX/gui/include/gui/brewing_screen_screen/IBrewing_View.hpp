/**
 * @file IBrewing_View.hpp
 * @brief Small brewing-view contract for presenter unit tests.
 */

#ifndef IBREWING_VIEW_HPP
#define IBREWING_VIEW_HPP

#include "coffee_machine_simulation.hpp"

/**
 * @brief View contract used by the brewing presenter.
 */
class IBrewing_View
{
public:
    virtual ~IBrewing_View() = default;

    /**
     * @brief Applies the current brewing session to the view.
     * @param session Current brewing session.
     */
    virtual void Update_Session(const BrewingSession& session) = 0;

    /**
     * @brief Handles the brewing-completed event in the view.
     */
    virtual void Handle_Brewing_Completed() = 0;
};

#endif // IBREWING_VIEW_HPP
