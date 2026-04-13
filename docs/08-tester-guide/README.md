# Tester Guide

## Goal

Give testers, developers, and test managers a clear practical path through the available test workflows in this repository.

This chapter explains:

- which runtime or test world to use for which task
- how to run host-side unit tests in Visual Studio
- how to connect the Test Explorer to the correct configuration
- how the tester workflow differs from normal embedded development work

## Audience Map

Different readers usually need different entry points.

### Application user

Primary interest:

- what the demonstrator does
- what the visible application flow looks like

Recommended reading:

- [README.md](C:/st_apps/coffee_machine/README.md)

### Software developer

Primary interest:

- architecture
- build and flash workflows
- debugging
- TouchGFX ownership
- unit-test implementation details

Recommended reading:

1. [docs/01-architecture/README.md](C:/st_apps/coffee_machine/docs/01-architecture/README.md)
2. [docs/02-build-and-flash/README.md](C:/st_apps/coffee_machine/docs/02-build-and-flash/README.md)
3. [docs/03-debugging/README.md](C:/st_apps/coffee_machine/docs/03-debugging/README.md)
4. [docs/06-touchgfx/README.md](C:/st_apps/coffee_machine/docs/06-touchgfx/README.md)
5. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)

### Tester

Primary interest:

- how to run host-side unit tests
- how to use the Test Explorer
- when to use PC tests and when to use board tests

Recommended reading:

1. this chapter
2. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
3. [README.md](C:/st_apps/coffee_machine/README.md)

### Test manager

Primary interest:

- which test layers exist
- which parts are covered by host-side unit tests
- which parts are still validated on the target

Recommended reading:

1. [README.md](C:/st_apps/coffee_machine/README.md)
2. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
3. this chapter

## The Three Working Worlds

This repository currently has three practical work contexts.

### 1. `extmem_bootloader`

Use this when the work is about:

- external memory initialization
- bootloader startup
- jump-to-application behavior
- early startup failures before the app owns execution

This is an embedded target and belongs to board-side work.

### 2. `coffee_machine`

Use this when the work is about:

- the application on the board
- LTDC and framebuffer behavior
- touch integration
- UART diagnostics
- TouchGFX behavior on the target

This is also an embedded target and belongs to board-side work.

### 3. `host-unittest-debug`

Use this when the work is about:

- host-side unit tests
- GoogleTest / gMock
- Test Explorer
- logic that is intentionally isolated from HAL and target hardware

This is not a board target. It is the PC-side unit-test world.

Recommended practical form:

- open the folder [tests](C:/st_apps/coffee_machine/tests) as its own Visual Studio CMake workspace
- use the preset from [tests/CMakePresets.json](C:/st_apps/coffee_machine/tests/CMakePresets.json)

## Simple Decision Rule

If the question is:

- "Do I need the board?"
  - yes -> use `coffee_machine` or `extmem_bootloader`
  - no -> use `host-unittest-debug`

That one rule avoids most confusion.

## Practical Meaning Of The Unit-Test World

The host-side unit-test path is intentionally separate from the embedded VisualGDB path.

Why:

- `coffee_machine` and `extmem_bootloader` are embedded targets
- `coffee_machine_unittest` is a host target
- the Test Explorer should see a PC-side test executable, not an ARM-target build

This separation avoids issues such as:

- `googletest` being built with the embedded toolchain
- `pthread`-related configuration problems in a cross-compiling setup
- tests appearing in the wrong project world

## Current Host-Test Configuration

The host-side unit-test configuration is defined through:

- [tests/CMakeLists.txt](C:/st_apps/coffee_machine/tests/CMakeLists.txt)
- [tests/CMakePresets.json](C:/st_apps/coffee_machine/tests/CMakePresets.json)

The current preset is:

- `host-unittest-debug`

That preset intentionally builds:

- `coffee_machine_unittest`

and intentionally does not build:

- `coffee_machine`
- `extmem_bootloader`

## Visual Studio Click Path

The following path is the recommended manual workflow in Visual Studio.

### A. Open the unit-test workspace in Visual Studio

1. Start Visual Studio 2022.
2. Open the folder:
   - `C:/st_apps/coffee_machine/tests`

Expected result:

- Visual Studio loads only the host-side test workspace.
- the project tree stays small and focused on unit tests.

### B. Select the host-side test preset

1. Locate the CMake preset selector in the Visual Studio toolbar.
2. Open the preset drop-down.
3. Select:
   - `Build Host Unittest Debug`

Expected result:

- Visual Studio configures the project for the host-side unit-test world.

Important note:

- do not use the normal embedded VisualGDB configuration for Test Explorer work
- do not use the VisualGDB project properties of `coffee_machine` for host-side tests

Practical note:

- the internal preset name is `host-unittest-debug`
- Visual Studio can show the display name `Build Host Unittest Debug` in the toolbar instead
- that is the correct selection

### C. Wait for CMake configure to finish

1. After selecting the preset, wait until CMake configuration completes.
2. Check the Output window or CMake status area if needed.

Expected result:

- the host build directory is generated
- the target `coffee_machine_unittest` becomes available
- GoogleTest and gMock appear only as helper dependencies inside the test workspace

### D. Build the unit-test target

For click-based users, the simplest path is:

1. In the Visual Studio menu, click:
   - `Build`
   - `Build All`

Alternative:

1. Open the Project Explorer or CMake target view.
2. Find:
   - `coffee_machine_unittest`
3. Build that target.

Expected result:

- the host-side test executable is built successfully

### E. Open the Test Explorer

1. In Visual Studio, open:
   - `Test`
   - `Test Explorer`

2. Wait briefly for test discovery.

Expected result:

- GoogleTest test cases from `coffee_machine_unittest` are discovered
- the test tree appears under:
  - `coffee_machine_unittest`
  - `<empty namespace>`

If nothing appears yet:

1. Build `coffee_machine_unittest` again.
2. In Test Explorer, click refresh or run discovery again.
3. Verify that `Build Host Unittest Debug` is still the selected toolbar entry.
4. If needed, close Visual Studio and reopen the folder [tests](C:/st_apps/coffee_machine/tests).

### F. Run the tests

1. In Test Explorer, click:
   - `Run All`

or

2. Select a subset of tests and run only those.

Expected result:

- the tests execute on the PC
- no board is required

Observed current result:

- `28` tests were discovered
- `28` tests passed

## Practical Tester Workflow

For a tester, the normal host-side path is:

1. Open Visual Studio
2. Open the folder [tests](C:/st_apps/coffee_machine/tests)
3. Leave `Build Host Unittest Debug` selected in the toolbar
4. Click `Build -> Build All`
5. Open Test Explorer
6. Click `Run All`
7. Review failures

That is the everyday path for the current unit-test layer.

## Practical Developer Workflow

For a developer, the decision is:

### If the change is logic-only

Use:

- `host-unittest-debug`

Typical examples:

- `coffee_machine_simulation`
- `Model`
- presenters
- `countdown_formatter`
- `coffee_machine_app`

### If the change is target- or hardware-related

Use:

- `coffee_machine`
- or `extmem_bootloader`

Typical examples:

- LTDC
- SDRAM
- QSPI
- touch controller
- bootloader hand-off

## Practical Test Manager View

The current testing split is:

- host-side unit tests for isolated handwritten logic
- later board-side scenarios for realistic application and hardware behavior

That means:

- fast logic regression checks can run on the PC
- board-dependent behavior still belongs to target-side validation

## Command-Line Equivalent

The Visual Studio path has a matching shell path.

Current verified commands:

```powershell
cmake -S tests -B build/host-unittest-standalone -G "Visual Studio 17 2022"
cmake --build build/host-unittest-standalone --config Debug --target coffee_machine_unittest
ctest -C Debug --test-dir build/host-unittest-standalone --output-on-failure
```

Use this path when:

- Visual Studio test discovery is unclear
- a tester wants an explicit shell-based confirmation
- CI or scripted local execution is desired later

## Typical Mistakes

### 1. Using the embedded target world for unit tests

Symptom:

- `googletest` errors inside an embedded or cross-compiling setup
- VisualGDB tries to inspect `coffee_machine_unittest.exe` like an ELF file
- memory-utilization messages appear for a Windows `.exe`

Fix:

- open [tests](C:/st_apps/coffee_machine/tests) as the unit-test workspace
- switch to `Build Host Unittest Debug`

### 2. Expecting the board for host-side tests

Symptom:

- confusion about flashing, debugging probes, or target startup

Fix:

- host-side unit tests do not need the board

### 3. Building the wrong target

Symptom:

- Test Explorer stays empty

Fix:

- use `Build -> Build All` in the [tests](C:/st_apps/coffee_machine/tests) workspace
- or build `coffee_machine_unittest`, not `coffee_machine`

### 4. Mixing the workflows mentally

Symptom:

- uncertainty about whether to use `flash_app`, `flash_bootloader`, or Test Explorer

Fix:

- flash targets belong to embedded work
- Test Explorer belongs to `host-unittest-debug`

## Why This Layout Is Cleaner

If the whole repository is opened through the embedded main project, the IDE can mix:

- embedded targets
- host test targets
- third-party GoogleTest helper targets
- VisualGDB-specific views

That is technically possible, but it is visually noisy and easy to misunderstand.

The separate [tests](C:/st_apps/coffee_machine/tests) workspace keeps the tester view smaller:

- one practical preset
- one practical main target
- one Test Explorer task
- no board-flash actions in the same work tree

## Validated Visual Studio View

The currently validated click path produced the following visible result in Test Explorer:

- test container `coffee_machine_unittest`
- node `<empty namespace>`
- discovered groups:
  - `BrewingScreenPresenterTests`
  - `CoffeeMachineAppTests`
  - `CoffeeMachineSimulationTests`
  - `CountdownFormatterTests`
  - `ModelTests`
  - `SlectionScreenPresenterTests`
  - `SplashScreenPresenterTests`
- total result:
  - `28` tests
  - `28` passed

## Recommended Reading Order By Role

### For testers

1. this chapter
2. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
3. [README.md](C:/st_apps/coffee_machine/README.md)

### For developers

1. [docs/01-architecture/README.md](C:/st_apps/coffee_machine/docs/01-architecture/README.md)
2. [docs/03-debugging/README.md](C:/st_apps/coffee_machine/docs/03-debugging/README.md)
3. [docs/06-touchgfx/README.md](C:/st_apps/coffee_machine/docs/06-touchgfx/README.md)
4. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
5. this chapter

### For test managers

1. [README.md](C:/st_apps/coffee_machine/README.md)
2. [docs/07-testing/README.md](C:/st_apps/coffee_machine/docs/07-testing/README.md)
3. this chapter

## Satisfaction Check

This chapter is in a good place if a reader can answer these questions:

- when do I use `coffee_machine`?
- when do I use `extmem_bootloader`?
- when do I use `host-unittest-debug`?
- which target do I build for Test Explorer?
- why is the host-side test world separate from the embedded world?

That is the bar this chapter is meant to meet.
