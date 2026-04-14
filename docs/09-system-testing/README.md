# System Testing

## Goal

Describe the strategy for Python-based board-side system tests that verify user-visible application scenarios on real hardware.

This chapter describes the implemented board-side system-test layer and its intended direction for the next steps.

## Current Status

The first implementation slice is now prepared in the code base:

- Python harness scaffold in `tests_python/...`
- documented command contract for the board-side test channel
- handwritten board-side test facade in `coffee_machine/app_test_api.*`
- handwritten USB-CDC command channel in `coffee_machine/app_test_channel.*`

Current implemented command set:

- `get_version`
- `get_state`
- `get_session`
- `reset_demo`
- `select_drink`
- `reboot_board`

Current intent of the current slice:

- keep the command set small
- keep the parser handwritten and understandable
- verify the end-to-end path before broadening the protocol

## Purpose

The purpose of this test layer is not to retest low-level hardware details.

It exists to verify the application behavior that a user cares about:

- the board reaches the interactive UI flow
- a drink can be selected
- brewing starts
- brewing progress advances
- brewing completes
- the UI returns to the selection screen

In short:

- unit tests verify isolated handwritten logic on the PC
- system tests verify end-to-end application scenarios on the board

## Scope

This chapter covers:

- the intended test level
- the Python technology stack
- the board-side test interface
- the command protocol style
- the Python harness structure
- the current system-test scenarios
- the rollout order

This chapter does not define:

- host-side C++ unit tests
- board bring-up diagnostics
- pixel-accurate display validation
- raw touch-controller validation

For those topics, continue with:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)
- [docs/04-drivers/touch-input.md](C:/st_apps/coffee_machine/docs/04-drivers/touch-input.md)
- [docs/04-drivers/uart-debug.md](C:/st_apps/coffee_machine/docs/04-drivers/uart-debug.md)
- [docs/04-drivers/usb-cdc-com.md](C:/st_apps/coffee_machine/docs/04-drivers/usb-cdc-com.md)

## Test Level

The intended test level is:

- black-box or gray-box application scenario testing on the real board

The key idea is:

- Python sends a semantic user action
- the board executes the normal application behavior
- Python reads back observable application state
- the test decides whether the behavior is correct

That means the tests should behave like a user-oriented observer, not like a low-level hardware exerciser.

## What Should Be Tested

The primary target is the current demonstrator flow described in:

- [README.md](C:/st_apps/coffee_machine/README.md)
- [docs/06-touchgfx/README.md](C:/st_apps/coffee_machine/docs/06-touchgfx/README.md)

Current user-visible flow:

1. splash screen
2. automatic transition to selection
3. user selects a drink
4. brewing screen becomes active
5. brewing progress and countdown advance
6. automatic return to selection

That flow is the core of the first system-test wave.

## What Should Not Be Tested First

The first Python-based system-test wave should not start by simulating:

- FT5336 raw touch data
- I2C transactions
- touch interrupts
- raw screen coordinates
- widget hit-testing internals

Why:

- those are lower-level integration concerns
- they increase complexity quickly
- they test different risks than the intended application scenarios

For this project, the better first step is:

- semantic application actions instead of raw hardware input emulation

## Recommended Technical Approach

The recommended technical path is:

```text
pytest
  -> pyserial
    -> USB CDC / COM transport
      -> board command parser
        -> app test facade
          -> application / model action
```

This keeps the system-test stack:

- small
- understandable
- automatable
- extendable later

## Board-Side Strategy

The board should expose a small handwritten test interface.

Recommended structure:

### 1. USB CDC / COM transport layer

Responsibilities:

- receive one command line
- send one response line
- handle framing and timeout behavior

### 2. Command parser

Responsibilities:

- parse test commands
- validate input
- dispatch to the application test facade
- serialize the response

### 3. Application test facade

Responsibilities:

- expose semantic user actions
- expose observable application state
- stay independent from raw touch hardware details

Recommended examples:

- `Reset_Demo()`
- `Select_Drink(CoffeeType type)`
- `Get_App_State()`
- `Get_Brewing_Session()`
- `Get_Version()`

### 4. Connection to the existing app flow

The test facade should talk to:

- the application layer
- the model
- or a small handwritten app-action service

It should not directly manipulate:

- generated widgets
- FT5336 raw touch paths
- LTDC details

## Why Semantic Actions Are Preferred

The user intent in this project is not:

- "touch X=120 and Y=180"

The user intent is:

- "select Espresso"

That leads to a cleaner test interface.

Preferred:

```json
{"cmd":"select_drink","drink":"espresso"}
```

Avoid at first:

```json
{"cmd":"inject_touch","x":120,"y":180}
```

The first form is:

- easier to implement
- easier to maintain
- closer to the use case
- less fragile when the UI layout changes

## Recommended Protocol

The recommended protocol is:

- JSON Lines over the USB-CDC command channel

That means:

- one JSON object per request line
- one JSON object per response line

Example request:

```json
{"cmd":"select_drink","drink":"espresso"}
```

Example response:

```json
{"status":"ok"}
```

Example state query:

```json
{"cmd":"get_state"}
```

Example response:

```json
{"status":"ok","state":"brewing"}
```

For the current implementation slice, the board parser is intentionally small and handwritten.
It is designed for the canonical JSON emitted by the Python harness, not as a general-purpose JSON parser.

## Why JSON Lines Fit This Project

This project already has meaningful application state that should be observable:

- splash
- selection
- brewing
- active coffee
- brewing phase
- progress
- remaining time

JSON fits that very well because it is:

- machine-readable
- extensible
- easy to parse from Python
- easy to log in test reports

## Minimal First Command Set

The first version of the board-side API should stay small.

Recommended commands:

### Actions

- `reset_demo`
- `select_drink`

### Queries

- `get_state`
- `get_session`
- `get_version`

Recommended examples:

```json
{"cmd":"reset_demo"}
{"cmd":"get_state"}
{"cmd":"select_drink","drink":"espresso"}
{"cmd":"get_session"}
```

## Concrete First API Contract

The first implementation should use the following concrete command names and payloads.

### `reset_demo`

Request:

```json
{"cmd":"reset_demo"}
```

Success response:

```json
{"status":"ok"}
```

Meaning:

- return the demonstrator to a defined start state for testing

### `get_state`

Request:

```json
{"cmd":"get_state"}
```

Success response:

```json
{"status":"ok","state":"selection"}
```

Allowed first-version states:

- `splash`
- `selection`
- `brewing`

### `select_drink`

Request:

```json
{"cmd":"select_drink","drink":"espresso"}
```

Allowed first-version drink values:

- `espresso`
- `cappuccino`
- `latte`
- `americano`

Success response:

```json
{"status":"ok"}
```

Typical semantic error:

```json
{"status":"error","reason":"invalid_state_for_selection"}
```

### `get_session`

Request:

```json
{"cmd":"get_session"}
```

Success response:

```json
{
  "status":"ok",
  "active":true,
  "coffee":"espresso",
  "phase":"start",
  "progress":0,
  "remaining_ms":25000
}
```

Recommended first session fields:

- `active`
- `coffee`
- `phase`
- `progress`
- `remaining_ms`

Recommended first phase values:

- `idle`
- `start`
- `brewing`
- `finishing`
- `done`

### `get_version`

Request:

```json
{"cmd":"get_version"}
```

Success response:

```json
{"status":"ok","version":"coffee_machine-systemtest-v1"}
```

Purpose:

- prove that the board speaks the expected test protocol version

## Recommended Response Shape

Each board response should clearly signal success or failure.

### Success

```json
{"status":"ok","state":"selection"}
```

### Semantic error

```json
{"status":"error","reason":"invalid_state_for_selection"}
```

### Timeout

- no response within the Python-side timeout window

That allows the Python tests to cleanly separate:

- correct behavior
- incorrect behavior
- communication failure

## Recommended Observable State

The board should make a small application state model visible to the tests.

Recommended top-level app states:

- `splash`
- `selection`
- `brewing`

Recommended session fields:

- `active`
- `coffee`
- `phase`
- `progress`
- `remaining_ms`

Those fields map naturally to the current demonstrator and brewing-domain model described in:

- [docs/06-touchgfx/README.md](C:/st_apps/coffee_machine/docs/06-touchgfx/README.md)
- [coffee_machine/coffee_machine_simulation.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_simulation.hpp)

## Current Suggested Implementation Boundary

The first board-side implementation should introduce a small handwritten seam such as:

- `coffee_machine/app_test_api.hpp`
- `coffee_machine/app_test_api.cpp`
- `coffee_machine/app_test_channel.h`
- `coffee_machine/app_test_channel.cpp`

Suggested responsibilities:

- `app_test_api`
  - semantic actions
  - visible app state
  - brewing-session snapshot
- `app_test_channel`
  - USB-CDC line buffering
  - JSON request parsing
  - JSON response serialization

That keeps the implementation aligned with the existing handwritten ownership in:

- [coffee_machine](C:/st_apps/coffee_machine/coffee_machine)
- [TouchGFX/gui/src/model/Model.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/model/Model.cpp)
- [TouchGFX/gui/src/common/FrontendApplication.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/common/FrontendApplication.cpp)

## Python Technology Stack

Recommended core stack:

- `pytest`
- `pyserial`

Recommended optional additions:

- `pytest-timeout`
- `pytest-html`

Why:

- `pytest` gives readable tests, fixtures, and good tooling
- `pyserial` is the standard serial transport on the PC side
- `pytest-timeout` helps with hangs
- `pytest-html` provides human-readable reports

## Recommended Environment Approach

Because you are preparing Anaconda, the practical recommendation is:

- create a dedicated environment for the board-side system tests

That environment should stay focused on:

- Python
- `pytest`
- `pyserial`
- optional reporting helpers

Avoid mixing it with unrelated packages if possible.

## Proposed Python Repository Structure

Recommended structure:

```text
tests_python/
  harness/
    board_uart.py
    protocol.py
  tests/
    test_smoke.py
    test_drinks.py
    test_brewing_flow.py
  conftest.py
  requirements.txt
  environment.yml
```

## Recommended Python Harness Design

The Python side should not scatter raw serial calls across test files.

Instead, use a small board harness class.

Recommended responsibilities:

- open and close the serial port
- send one request
- receive one response
- parse JSON
- raise clear errors on timeout or invalid data
- expose helper methods such as:
  - `reset_demo()`
  - `get_state()`
  - `select_drink()`
  - `get_session()`
  - `wait_until_state()`

The harness should also tolerate non-JSON startup log lines on the same UART and keep reading until the next valid JSON response is received or the timeout expires.

That keeps the tests short and readable.

## Documentation Rules For Python System Tests

The Python-based system tests should follow the same documentation idea as the C++ unit tests:

- each test should state its test goal
- each test should state the expected behavior
- naming should stay readable and explicit

Recommended practical form:

- one short module docstring at the top of the file
- one short docstring directly inside each test function

Recommended example:

```python
def test_select_espresso_starts_brewing(board) -> None:
    """
    Test goal:
    - verify that selecting Espresso starts the brewing flow

    Expected behavior:
    - the board enters the brewing state
    - the active coffee is espresso
    """
```

This keeps the Python system tests aligned with the documentation style already used for:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)

## Result Model For Each Test

Each test should evaluate one of three outcomes:

### 1. Correct response

- board replies
- reply is parseable
- reply matches the expected behavior

Result:

- pass

### 2. Wrong response

- board replies
- but the state or payload does not match the expected scenario

Result:

- fail

### 3. No response or timeout

- board does not answer in time
- or the response is incomplete / invalid

Result:

- fail

This gives the test layer a clear and stable evaluation model.

## First Recommended Scenarios

The first system-test wave should stay focused on a few high-value flows.

### 1. Board reaches selection after startup

Goal:

- verify that the UI reaches the interactive state after the splash screen

### 2. Selecting Espresso starts brewing

Goal:

- verify that a semantic user action starts the expected brewing flow

### 3. Brewing progress advances

Goal:

- verify that the brewing session is alive and changing over time

### 4. Brewing returns to selection

Goal:

- verify that the current user flow finishes cleanly

### 5. Invalid command returns a structured error

Goal:

- verify that the test interface itself is robust enough for automation

## Example BDD-Style Scenario

The tests do not need a BDD framework at first, but the intended scenario shape is already clear:

```gherkin
Feature: Brewing coffee

  Scenario: User selects espresso
    Given the board is in selection state
    When the user selects "espresso"
    Then brewing should start
    And the active coffee should be "espresso"
    And the board should return to selection after brewing
```

That is the right behavioral level for this project.

## Why BDD Tooling Should Wait

At the start, the real risk is not missing BDD syntax.

The real risks are:

- unstable UART communication
- unclear board command API
- missing timeouts
- weak state observability

So the recommended order is:

1. stable UART protocol
2. stable Python harness
3. stable first scenarios
4. only later, if useful, `pytest-bdd`

## Reporting

Recommended reporting path:

### Human-readable report

- `pytest-html`

### CI-friendly report

- JUnit XML from `pytest --junitxml=...`

That gives:

- easy local inspection
- future CI compatibility

## Execution Path

Python-based board-side system tests are intentionally executed through the shell path only.

### Shell

Recommended smoke-test entry point:

```powershell
python tests_python/run_pytest.py --port COM5 --suite smoke
```

Recommended full current suite:

```powershell
python tests_python/run_pytest.py --port COM5 --suite all
```

Each launcher run writes a timestamped test report set to `tests_python/reports/`:

- one JUnit XML report for archival or tooling
- one HTML report for manual review when `pytest-html` is installed

An alternative report directory can be provided with `--report-dir`.

Direct `pytest` use remains valid:

```powershell
pytest tests_python/tests/test_smoke.py --board-port COM5
```

This keeps the tester workflow simple:

- one launcher script
- one visible command line
- one place to collect logs and reports

Recommended first use:

- flash the current firmware to the board
- connect the USB cable to `CN13`
- confirm that the board appears as a COM port on the host
- close other tools that may still hold the same COM port
- run `python tests_python/run_pytest.py --port COM5 --suite smoke`
- only then continue with the broader system-test suite

## Rollout Order

The recommended rollout order is:

1. define the board-side command set
2. define the visible state model
3. implement the command parser and app test facade on the board
4. create the Python environment
5. implement the Python harness
6. add the first smoke test
7. add the first brewing-flow tests
8. add reporting

## Safe Boundaries

Preferred places for the upcoming work:

- handwritten board-side helper code in `coffee_machine/...`
- Python harness and tests in `tests_python/...`
- documentation in this chapter

Avoid for the first system-test wave:

- generated TouchGFX files
- low-level touch-controller emulation
- broad refactoring of the UI architecture

## Satisfaction Check

This chapter is in a good place if a reader can answer these questions:

- what exactly should the Python system tests verify?
- why should semantic user actions be preferred over raw touch injection?
- what should the board-side UART API look like?
- which Python tools should be used first?
- which first scenarios should be implemented?

That is the bar this chapter is meant to meet.
