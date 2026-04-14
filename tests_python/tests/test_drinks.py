"""
Drink-selection tests for the board-side coffee-machine system-test harness.
"""

from __future__ import annotations

import pytest


@pytest.mark.system
def test_espresso_selection_transitions_to_brewing_and_back_to_selection(board) -> None:
    """
    Test goal:
    - verify the complete state transition for Espresso

    Expected behavior:
    - the board starts in the selection state
    - Espresso selection moves the board to the brewing state
    - after brewing time and hold time, the board returns to the selection state
    """
    assert board.wait_until_state("selection", timeout_s=2.0)

    board.select_drink("espresso")
    assert board.wait_until_state("brewing", timeout_s=2.0)
    session = board.get_session()
    assert session["active"] is True
    assert session["coffee"] == "espresso"
    assert board.wait_until_state("selection", timeout_s=35.0)


@pytest.mark.system
def test_latte_selection_transitions_to_brewing_and_back_to_selection(board) -> None:
    """
    Test goal:
    - verify the complete state transition for Latte

    Expected behavior:
    - the board starts in the selection state
    - Latte selection moves the board to the brewing state
    - after brewing time and hold time, the board returns to the selection state
    """
    assert board.wait_until_state("selection", timeout_s=2.0)

    board.select_drink("latte")
    assert board.wait_until_state("brewing", timeout_s=2.0)
    session = board.get_session()
    assert session["active"] is True
    assert session["coffee"] == "latte"
    assert board.wait_until_state("selection", timeout_s=75.0)
