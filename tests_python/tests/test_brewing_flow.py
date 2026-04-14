"""
Brewing-flow tests for the board-side coffee-machine system-test harness.
"""

from __future__ import annotations

import time

import pytest


@pytest.mark.system
def test_espresso_brewing_progress_advances(board) -> None:
    """
    Test goal:
    - verify that an active brewing session changes over time

    Expected behavior:
    - progress stays equal or increases
    - remaining time stays equal or decreases
    """
    board.select_drink("espresso")
    assert board.wait_until_state("brewing", timeout_s=2.0)
    first = board.get_session()
    time.sleep(1.0)
    second = board.get_session()

    assert second["progress"] >= first["progress"]
    assert second["remaining_ms"] <= first["remaining_ms"]
