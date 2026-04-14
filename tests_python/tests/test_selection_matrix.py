"""
Full drink-selection coverage for the board-side coffee-machine system-test harness.
"""

from __future__ import annotations

import time

import pytest


@pytest.mark.system
def test_all_drinks_complete_the_selection_to_brewing_to_selection_flow(
    board,
    request: pytest.FixtureRequest,
) -> None:
    """
    Test goal:
    - verify the full user-facing selection flow for all supported drinks

    Expected behavior:
    - each drink starts from the selection state
    - each selection enters the brewing state
    - the reported active coffee matches the selected drink
    - after brewing time and hold time, the board returns to the selection state
    """
    drinks = (
        ("espresso", 35.0),
        ("cappuccino", 55.0),
        ("latte", 75.0),
        ("americano", 50.0),
    )
    pre_selection_delay_s = request.config.getoption("--pre-selection-delay")

    for drink, selection_timeout_s in drinks:
        time.sleep(pre_selection_delay_s)
        assert board.wait_until_state("selection", timeout_s=2.0)

        board.select_drink(drink)
        assert board.wait_until_state("brewing", timeout_s=2.0)

        session = board.get_session()
        assert session["active"] is True
        assert session["coffee"] == drink

        assert board.wait_until_state("selection", timeout_s=selection_timeout_s)
