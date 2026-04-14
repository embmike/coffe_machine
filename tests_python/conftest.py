"""
pytest configuration for board-side Python system tests.
"""

from __future__ import annotations

from collections.abc import Generator

import pytest

from harness.board_uart import BoardUartConfig, CoffeeMachineBoard


def pytest_addoption(parser: pytest.Parser) -> None:
    parser.addoption("--board-port", action="store", default="", help="Serial port of the coffee-machine board")
    parser.addoption("--board-baudrate", action="store", type=int, default=115200, help="UART baudrate")
    parser.addoption("--board-timeout", action="store", type=float, default=1.0, help="UART response timeout in seconds")
    parser.addoption("--debug-uart", action="store_true", help="Print raw UART transmit and receive lines")
    parser.addoption(
        "--pre-selection-delay",
        action="store",
        type=float,
        default=2.0,
        help="Additional wait time in seconds before each selection step in the full selection matrix test",
    )


def pytest_collection_modifyitems(items: list[pytest.Item]) -> None:
    order_map = {
        "test_startup.py": 0,
        "test_smoke.py": 1,
        "test_drinks.py": 2,
        "test_brewing_flow.py": 3,
        "test_selection_matrix.py": 4,
    }

    items.sort(key=lambda item: (order_map.get(item.fspath.basename, 99), str(item.fspath), item.name))


@pytest.fixture
def board(request: pytest.FixtureRequest) -> Generator[CoffeeMachineBoard, None, None]:
    port = request.config.getoption("--board-port")
    if not port:
        pytest.skip("No board port configured. Use --board-port COMx to run board-side system tests.")

    config = BoardUartConfig(
        port=port,
        baudrate=request.config.getoption("--board-baudrate"),
        timeout_s=request.config.getoption("--board-timeout"),
        debug_uart=request.config.getoption("--debug-uart"),
    )
    device = CoffeeMachineBoard(config)
    try:
        if not device.wait_until_ready(timeout_s=10.0):
            pytest.fail("Board-side system-test channel did not become ready within 10 seconds.")

        device.reset_demo()
        if not device.wait_until_state("selection", timeout_s=8.0):
            pytest.fail("Board did not reach the selection state after test reset.")

        yield device
    finally:
        device.close()


@pytest.fixture
def raw_board(request: pytest.FixtureRequest) -> Generator[CoffeeMachineBoard, None, None]:
    port = request.config.getoption("--board-port")
    if not port:
        pytest.skip("No board port configured. Use --board-port COMx to run board-side system tests.")

    config = BoardUartConfig(
        port=port,
        baudrate=request.config.getoption("--board-baudrate"),
        timeout_s=request.config.getoption("--board-timeout"),
        debug_uart=request.config.getoption("--debug-uart"),
    )
    device = CoffeeMachineBoard(config)
    try:
        if not device.wait_until_ready(timeout_s=10.0):
            pytest.fail("Board-side system-test channel did not become ready within 10 seconds.")

        yield device
    finally:
        device.close()
