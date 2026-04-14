"""
Small launcher for board-side Python system tests.
"""

from __future__ import annotations

import argparse
import importlib.util
import subprocess
import sys
from datetime import datetime
from pathlib import Path


def build_pytest_command(args: argparse.Namespace) -> list[str]:
    """
    Build the pytest command line for the requested test suite.
    """
    root_dir = Path(__file__).resolve().parent
    suite_map = {
        "smoke": root_dir / "tests" / "test_smoke.py",
        "all": root_dir / "tests",
    }
    reports_dir = Path(args.report_dir).resolve()
    reports_dir.mkdir(parents=True, exist_ok=True)

    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    report_prefix = f"{args.suite}_{timestamp}"
    html_report = reports_dir / f"{report_prefix}.html"
    xml_report = reports_dir / f"{report_prefix}.xml"

    command = [
        sys.executable,
        "-m",
        "pytest",
        str(suite_map[args.suite]),
        "--board-port",
        args.port,
        "--board-baudrate",
        str(args.baudrate),
        "--board-timeout",
        str(args.timeout),
        "--pre-selection-delay",
        str(args.pre_selection_delay),
        f"--junitxml={xml_report}",
    ]

    if importlib.util.find_spec("pytest_html") is not None:
        command.extend(
            [
                f"--html={html_report}",
                "--self-contained-html",
            ]
        )

    if args.debug_uart:
        command.append("--debug-uart")

    command.extend(args.pytest_args)
    return command


def parse_args() -> argparse.Namespace:
    """
    Parse command-line arguments for the launcher.
    """
    parser = argparse.ArgumentParser(
        description="Run board-side coffee-machine system tests.",
    )
    parser.add_argument(
        "--port",
        default="",
        help="Board serial port, for example COM5.",
    )
    parser.add_argument(
        "--baudrate",
        type=int,
        default=115200,
        help="UART baudrate used by the board-side test channel.",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=1.0,
        help="UART response timeout in seconds.",
    )
    parser.add_argument(
        "--debug-uart",
        action="store_true",
        help="Print raw UART transmit and receive lines.",
    )
    parser.add_argument(
        "--pre-selection-delay",
        type=float,
        default=2.0,
        help="Additional wait time in seconds before each selection step in the full selection matrix test.",
    )
    parser.add_argument(
        "--suite",
        choices=("smoke", "all"),
        default="smoke",
        help="Named test suite to execute.",
    )
    parser.add_argument(
        "--report-dir",
        default=str(Path(__file__).resolve().parent / "reports"),
        help="Directory where HTML and XML test reports are written.",
    )
    parser.add_argument(
        "pytest_args",
        nargs=argparse.REMAINDER,
        help="Additional pytest arguments appended after the launcher options.",
    )

    args = parser.parse_args()
    if not args.port:
        parser.error("missing --port, for example --port COM5")

    if args.pytest_args and args.pytest_args[0] == "--":
        args.pytest_args = args.pytest_args[1:]

    return args


def main() -> int:
    """
    Run pytest and return the pytest exit code.
    """
    args = parse_args()
    command = build_pytest_command(args)
    reports_dir = Path(args.report_dir).resolve()
    html_enabled = importlib.util.find_spec("pytest_html") is not None

    print("Running:", " ".join(command))
    if not html_enabled:
        print("Note: pytest-html is not installed. HTML report is skipped; JUnit XML is still generated.")
    completed = subprocess.run(command, check=False)
    print(f"Reports written to: {reports_dir}")
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())
