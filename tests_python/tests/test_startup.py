"""
Startup-path tests for the board-side coffee-machine system-test harness.
"""

from __future__ import annotations

import pytest


@pytest.mark.system
def test_software_reboot_transitions_from_splash_to_selection(raw_board) -> None:
    """
    Test goal:
    - verify the observable startup path after a software-triggered board reboot

    Expected behavior:
    - a software reboot restarts the board
    - after reconnect, the board first reaches the splash state
    - after the splash timeout, the board reaches the selection state
    """
    raw_board.reboot_board(timeout_s=20.0)
    assert raw_board.wait_until_ready(timeout_s=10.0)
    assert raw_board.wait_until_state("splash", timeout_s=3.0, poll_interval_s=0.1)
    assert raw_board.wait_until_state("selection", timeout_s=10.0)
