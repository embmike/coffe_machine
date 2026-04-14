"""
Protocol definitions for the UART-based coffee-machine system tests.
"""

from __future__ import annotations

from typing import Final, Literal, TypedDict


BoardState = Literal["splash", "selection", "brewing"]
DrinkName = Literal["espresso", "cappuccino", "latte", "americano"]
SessionPhase = Literal["idle", "start", "brewing", "finishing", "done"]

CMD_RESET_DEMO: Final[str] = "reset_demo"
CMD_GET_STATE: Final[str] = "get_state"
CMD_SELECT_DRINK: Final[str] = "select_drink"
CMD_GET_SESSION: Final[str] = "get_session"
CMD_GET_VERSION: Final[str] = "get_version"
CMD_REBOOT_BOARD: Final[str] = "reboot_board"


class StatusResponse(TypedDict):
    status: str


class StateResponse(StatusResponse):
    state: BoardState


class SessionResponse(StatusResponse):
    active: bool
    coffee: DrinkName
    phase: SessionPhase
    progress: int
    remaining_ms: int


class VersionResponse(StatusResponse):
    version: str
