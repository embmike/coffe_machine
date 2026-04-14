# System Testing

## Goal

Describe the strategy for Python-based board-side system tests that verify user-visible application scenarios on real hardware.

This chapter is the planning and implementation guide for the next test layer after host-side unit tests.

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
- the proposed Python technology stack
- the recommended board-side test interface
- the proposed UART protocol style
- the Python harness structure
- the first planned system-test scenarios
- the rollout order

This chapter does not define:

- host-side C++ unit tests
- board bring-up diagnostics
- pixel-accurate display validation
- raw touch-controller validation

For those topics, continue with:

- [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
- [docs/04-drivers/touch-input.md](C:/st_apps/coffee_machine/docs/04-drivers/touch-input.md)
- [docs/04-drivers/uart-debug.md](C:/st_apps/coffee_machine/docs/04-drivers/uart-debug.md)

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
    -> UART transport
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

### 1. UART transport layer

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

- JSON Lines over UART

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

## Python Technology Stack

Recommended core stack:

- `pytest`
- `pyserial`

Recommended optional additions:

- `pytest-timeout`
- `pytest-html`

Why:

- `pytest` gives readable tests, fixtures, and good tooling
- `pyserial` is the standard UART transport on the PC side
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

That keeps the tests short and readable.

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

## Rollout Order

The recommended rollout order is:

1. define the board-side command set
2. define the visible state model
3. implement the UART parser and app test facade on the board
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
