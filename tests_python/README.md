# Python System Tests

This directory contains the upcoming board-side Python system-test harness and scenario tests.

Current goal:

- verify user-visible application scenarios on the real board
- communicate with the board over USB CDC / COM
- keep the first system-test wave small and robust

Recommended entry points:

- [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md)
- [environment.yml](C:/st_apps/coffee_machine/tests_python/environment.yml)
- [conftest.py](C:/st_apps/coffee_machine/tests_python/conftest.py)

Current status:

- Python harness scaffold created
- board-side USB-CDC system-test channel implemented on the board side
- first tests are prepared as the target shape of the system-test wave
- test functions document `Test goal` and `Expected behavior`

## Shell Usage

Run the smoke tests:

```powershell
python tests_python/run_pytest.py --port COM5 --suite smoke
```

Run the full current suite:

```powershell
python tests_python/run_pytest.py --port COM5 --suite all
```

Each launcher run writes a test report to `tests_python/reports/`:

- one JUnit XML report for tooling and archiving
- one HTML report for direct review when `pytest-html` is installed

An alternative report directory can be supplied explicitly:

```powershell
python tests_python/run_pytest.py --port COM5 --suite all --report-dir C:\temp\coffee_reports
```

Direct `pytest` execution is also supported:

```powershell
pytest tests_python/tests/test_smoke.py --board-port COM5
```
