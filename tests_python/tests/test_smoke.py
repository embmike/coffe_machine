"""
Smoke tests for the board-side coffee-machine system-test harness.
"""

from __future__ import annotations

import pytest


@pytest.mark.system
@pytest.mark.smoke
def test_board_reports_protocol_version(board) -> None:
    """
    Test goal:
    - verify that the board exposes a non-empty system-test protocol version

    Expected behavior:
    - the board responds to the version query
    - the returned version string is not empty
    """
    version = board.get_version()
    assert version != ""


@pytest.mark.system
@pytest.mark.smoke
def test_board_reaches_selection_as_initial_test_state(board) -> None:
    """
    Test goal:
    - verify that the board reaches the interactive selection state as the defined test start state

    Expected behavior:
    - the board reaches the selection state within the timeout window
    """
    assert board.wait_until_state("selection", timeout_s=8.0)
