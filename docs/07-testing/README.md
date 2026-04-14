# Testing

## Goal

Provide the common entry point for all testing topics in this repository.

This chapter answers three questions:

- why the project uses more than one test layer
- what each test layer is responsible for
- where a reader should continue next

## Structure

The testing documentation is intentionally split into three parts:

1. this chapter: testing introduction and overall test strategy
2. [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md): host-side unit tests
3. [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md): board-side system tests

That split keeps the roles clear:

- this chapter explains the test landscape
- the unit-test chapter explains isolated PC-side verification
- the system-test chapter explains user-flow verification on the real board

## Purpose

The project uses testing for two different but complementary goals:

- verify handwritten logic early and deterministically on the PC
- verify observable user-facing behavior later on the real board

This matters because the project itself spans two different worlds:

- host-side software design and logic
- embedded runtime behavior on the STM32H750B-DK

One test layer alone would not describe both worlds well.

## Test Strategy

The repository currently uses two active test layers.

### Layer 1: Unit tests

Host-side C++ unit tests are used for:

- formatting helpers
- simulation logic
- presenter logic
- model logic with seams
- handwritten application helpers that can be isolated cleanly

Key properties:

- run on the PC
- no HAL dependency
- no TouchGFX runtime dependency
- deterministic and fast

Continue here:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)

### Layer 2: System tests

Board-side Python system tests are used for:

- startup path checks
- observable state transitions
- drink-selection flow checks
- brewing progress and return-to-selection flow
- end-to-end user-visible scenarios on real hardware

Key properties:

- run against the real board
- communicate through the optional USB-CDC system-test channel
- verify application behavior from the outside

Continue here:

- [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md)

## Boundaries

The intended boundaries are:

- unit tests:
  isolate logic from HAL, TouchGFX runtime startup, and board dependencies
- system tests:
  use the real firmware and verify observable behavior instead of low-level peripheral internals

This means:

- unit tests should not try to validate display timing, USB reconnect timing, or board boot behavior
- system tests should not try to replace small deterministic logic tests that are easier on the PC

## Roles

### Software developer

Typical questions:

- what should be unit-tested first?
- how do I keep new code testable?
- where is the current unit-test setup documented?

Primary chapter:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)

### Tester

Typical questions:

- what is the difference between unit tests and system tests here?
- which test layer should I use for which task?
- how do I run the board-side checks and read the reports?

Primary chapters:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)
- [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md)

## Recommended Reading Order

For a new reader, the most useful order is:

1. [README.md](C:/st_apps/coffee_machine/README.md)
2. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
3. [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)
4. [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md)

## Summary

The project now documents testing in one logical staircase:

- overview and strategy
- unit tests
- system tests

That gives developers and testers one common entrance and then two clearly separated follow-up paths.
