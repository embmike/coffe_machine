"""
UART harness for the coffee-machine board-side system tests.
"""

from __future__ import annotations

import json
import time
from dataclasses import dataclass
from typing import Any

import serial

from harness.protocol import (
    CMD_GET_SESSION,
    CMD_GET_STATE,
    CMD_GET_VERSION,
    CMD_REBOOT_BOARD,
    CMD_RESET_DEMO,
    CMD_SELECT_DRINK,
    BoardState,
    DrinkName,
)


class BoardProtocolError(RuntimeError):
    """Raised when the board returns malformed or semantic error data."""


class BoardTimeoutError(TimeoutError):
    """Raised when the board does not respond in time."""


@dataclass(slots=True)
class BoardUartConfig:
    port: str
    baudrate: int = 115200
    timeout_s: float = 1.0
    startup_wait_s: float = 1.5
    debug_uart: bool = False


class CoffeeMachineBoard:
    """Small UART wrapper for semantic board-side system tests."""

    def __init__(self, config: BoardUartConfig) -> None:
        self._config = config
        self._serial = self._open_serial()

    def _open_serial(self) -> serial.Serial:
        device = serial.Serial(
            port=self._config.port,
            baudrate=self._config.baudrate,
            timeout=self._config.timeout_s,
        )
        time.sleep(self._config.startup_wait_s)
        device.reset_input_buffer()
        device.reset_output_buffer()
        return device

    def _reopen_serial_until_ready(self, timeout_s: float) -> bool:
        deadline = time.monotonic() + timeout_s
        self.close()

        while time.monotonic() < deadline:
            try:
                self._serial = self._open_serial()
                return True
            except (serial.SerialException, PermissionError, OSError):
                time.sleep(0.2)

        return False

    def close(self) -> None:
        """Close the UART port."""
        if self._serial.is_open:
            self._serial.close()

    def _trace(self, direction: str, message: str) -> None:
        """Print optional UART trace output."""
        if not self._config.debug_uart:
            return

        print(f"[uart {time.monotonic():.3f}] {direction}: {message}", flush=True)

    def request(self, payload: dict[str, Any]) -> dict[str, Any]:
        """Send one JSON request and return one JSON response."""
        line = json.dumps(payload, separators=(",", ":")) + "\n"
        self._trace("tx", line.rstrip())
        try:
            self._serial.write(line.encode("utf-8"))
            self._serial.flush()
        except serial.SerialException as exc:
            raise BoardTimeoutError(f"Serial write failed for command: {payload!r}") from exc

        deadline = time.monotonic() + self._config.timeout_s
        response: dict[str, Any] | None = None

        while time.monotonic() < deadline:
            try:
                response_line = self._serial.readline().decode("utf-8", errors="replace").strip()
            except serial.SerialException as exc:
                raise BoardTimeoutError(f"Serial read failed for command: {payload!r}") from exc
            if not response_line:
                continue

            self._trace("rx", response_line)

            if not response_line.startswith("{"):
                continue

            try:
                response = json.loads(response_line)
                break
            except json.JSONDecodeError:
                continue

        if response is None:
            raise BoardTimeoutError(f"No JSON response from board for command: {payload!r}")

        if response.get("status") == "error":
            raise BoardProtocolError(f"Board returned semantic error: {response}")

        if response.get("status") != "ok":
            raise BoardProtocolError(f"Board returned unexpected status: {response}")

        return response

    def reset_demo(self) -> None:
        self.request({"cmd": CMD_RESET_DEMO})

    def get_state(self) -> BoardState:
        response = self.request({"cmd": CMD_GET_STATE})
        return response["state"]

    def select_drink(self, drink: DrinkName) -> None:
        self.request({"cmd": CMD_SELECT_DRINK, "drink": drink})

    def get_session(self) -> dict[str, Any]:
        return self.request({"cmd": CMD_GET_SESSION})

    def get_version(self) -> str:
        response = self.request({"cmd": CMD_GET_VERSION})
        return response["version"]

    def reboot_board(self, timeout_s: float = 20.0) -> None:
        line = json.dumps({"cmd": CMD_REBOOT_BOARD}, separators=(",", ":")) + "\n"
        self._trace("tx", line.rstrip())
        self._serial.write(line.encode("utf-8"))
        self._serial.flush()

        response_deadline = time.monotonic() + self._config.timeout_s
        while time.monotonic() < response_deadline:
            try:
                response_line = self._serial.readline().decode("utf-8", errors="replace").strip()
            except serial.SerialException:
                break

            if not response_line:
                continue

            self._trace("rx", response_line)
            if response_line.startswith("{"):
                try:
                    response = json.loads(response_line)
                except json.JSONDecodeError:
                    continue

                if response.get("status") == "ok":
                    break

        self.close()

        if self._reopen_serial_until_ready(timeout_s):
            return

        raise BoardTimeoutError(
            f"Board did not reconnect on port {self._config.port} within {timeout_s} seconds after reboot."
        )

    def wait_until_ready(self, timeout_s: float) -> bool:
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            try:
                return self.get_version() != ""
            except BoardTimeoutError:
                self._trace("info", "board not ready yet")
                self._reopen_serial_until_ready(timeout_s=1.0)
                time.sleep(0.2)
            except BoardProtocolError:
                self._trace("info", "board not ready yet")
                time.sleep(0.2)
        return False

    def wait_until_state(self, expected: BoardState, timeout_s: float, poll_interval_s: float = 0.2) -> bool:
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            try:
                if self.get_state() == expected:
                    return True
            except BoardTimeoutError:
                self._reopen_serial_until_ready(timeout_s=1.0)
            except BoardProtocolError:
                pass
            time.sleep(poll_interval_s)
        return False
