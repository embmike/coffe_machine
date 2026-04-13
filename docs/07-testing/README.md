# Testing

## Goal

Describe the unit-test strategy for the handwritten application logic, define the current test boundaries, and provide a practical entry path for GoogleTest and gMock in this repository.

This chapter is both:

- the test concept
- the current implementation guide for the host-side unit-test layer

## Audience And Entry Points

Different readers enter this chapter with different goals.

### Tester

Primary interest:

- how to run host-side unit tests
- how to use the Test Explorer
- when to use PC tests and when to use board tests

Main sections:

- `Visual Studio Click Path`
- `Typical Mistakes`
- `Validated Visual Studio View`

### Software developer

Primary interest:

- testable units
- seams and interfaces
- naming rules
- Doxygen conventions
- host-side CMake structure

Main sections:

- `Unit-Test Targets`
- `Implemented Test Seams`
- `Naming Rules For Test Code`
- `Doxygen Rules For Test Files`
- `CMake Direction`

### Test manager

Primary interest:

- which test layer exists today
- which source areas are already covered
- which parts remain board-side

Main sections:

- `Current Status`
- `Current Coverage Areas`
- `Test Strategy`
- `Rollout Status`

## Current Status

The first host-side unit-test wave is implemented and running.

Current verified areas:

- `coffee_machine_app`
- `countdown_formatter`
- `coffee_machine_simulation`
- `Model`
- `slection_screenPresenter`
- `brewing_screenPresenter`
- `splash_screenPresenter`

Current implemented seams:

- `IApp_Runtime`
- `ITick_Source`
- `IModel`
- `IBrewing_View`
- `ISelection_View`
- `ISplash_View`

So this chapter is no longer only a plan. It also describes the working host-side unit-test setup.

## Scope

This chapter covers:

- unit-test goals
- current test boundaries
- testable source files
- files intentionally excluded from unit tests
- implemented test seams
- naming rules for test code
- Doxygen rules for test files
- Visual Studio and Test Explorer usage
- current rollout status

This chapter does not define:

- board-level exploratory checks
- Python-driven PC-to-board application scenarios
- hardware validation
- system or integration tests on the target

Those topics belong to later validation work outside the C++ unit-test layer.

## Test Strategy

The project uses two complementary test layers:

1. host-side C++ unit tests
2. later PC-driven board scenario tests

The first layer is the focus of this chapter.

Unit tests are intended for:

- deterministic business logic
- presenter-to-model interaction logic
- presenter-to-view interaction logic
- small formatting helpers
- handwritten startup facades that can be isolated behind small interfaces

Later board-side scenario checks are intended for:

- real peripheral behavior
- touch interaction on hardware
- startup and flashing workflows
- target timing under realistic runtime conditions

## Architectural Fit

The handwritten GUI-side code follows the Model-View-Presenter pattern.

For testing, each layer has a different role:

- `Model`
  - owns UI-facing state progression and listener notifications
- `Presenter`
  - owns mediation between model and view
- `View`
  - owns TouchGFX widgets, rendering details, and screen transitions

Unit tests should primarily target:

- model logic
- presenter interaction logic
- domain logic outside TouchGFX
- small application facades outside TouchGFX startup glue

The concrete TouchGFX views themselves are not the primary unit-test target. They should stay thin and continue to own real widget behavior.

## Unit-Test Targets

The current handwritten source files are split into two groups.

### Group 1: Active unit-test targets

These files are part of the implemented host-side unit-test set:

- [coffee_machine/coffee_machine_app.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.hpp)
- [coffee_machine/coffee_machine_app.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.cpp)
- [coffee_machine/countdown_formatter.hpp](C:/st_apps/coffee_machine/coffee_machine/countdown_formatter.hpp)
- [coffee_machine/countdown_formatter.cpp](C:/st_apps/coffee_machine/coffee_machine/countdown_formatter.cpp)
- [coffee_machine/coffee_machine_simulation.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_simulation.hpp)
- [coffee_machine/coffee_machine_simulation.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_simulation.cpp)
- [TouchGFX/gui/include/gui/model/Model.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/model/Model.hpp)
- [TouchGFX/gui/src/model/Model.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/model/Model.cpp)
- [TouchGFX/gui/include/gui/slection_screen_screen/slection_screenPresenter.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/slection_screen_screen/slection_screenPresenter.hpp)
- [TouchGFX/gui/src/slection_screen_screen/slection_screenPresenter.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/slection_screen_screen/slection_screenPresenter.cpp)
- [TouchGFX/gui/include/gui/brewing_screen_screen/brewing_screenPresenter.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/brewing_screen_screen/brewing_screenPresenter.hpp)
- [TouchGFX/gui/src/brewing_screen_screen/brewing_screenPresenter.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/brewing_screen_screen/brewing_screenPresenter.cpp)
- [TouchGFX/gui/include/gui/splash_screen_screen/splash_screenPresenter.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/splash_screen_screen/splash_screenPresenter.hpp)
- [TouchGFX/gui/src/splash_screen_screen/splash_screenPresenter.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/splash_screen_screen/splash_screenPresenter.cpp)

### Group 2: Intentionally excluded from unit tests

This file is intentionally not part of the host-side unit-test scope:

- [coffee_machine/coffee_machine_board.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_board.hpp)
- [coffee_machine/coffee_machine_board.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_board.cpp)

Reason:

- the board helper is itself used as an exploratory and bring-up-oriented runtime test path
- its direct dependencies are intentionally hardware-facing
- its value is higher in board validation than in host-side unit isolation

## Current Coverage Areas

### coffee_machine_app

Purpose:

- verify the handwritten startup and process facade around TouchGFX

Typical checks:

- startup logs the expected path for the active build configuration
- startup initializes TouchGFX exactly once
- process forwards exactly one TouchGFX loop iteration

Recommended tools:

- `GoogleTest`
- `gMock`

### countdown_formatter

Purpose:

- verify countdown formatting rules

Typical checks:

- `0 ms` formats as `0`
- sub-second values truncate to `0`
- full seconds format as expected
- null buffer input returns safely
- zero-size buffer input returns safely

Recommended tool:

- `GoogleTest`

### coffee_machine_simulation

Purpose:

- verify the demonstrator brewing-domain rules

Typical checks:

- each `CoffeeType` resolves the expected profile
- `start()` initializes the correct session state
- `update()` advances elapsed time and remaining time
- progress stays in the valid range
- phase transitions happen at the expected thresholds
- `stop()` and `reset()` return to idle state
- coffee-name and log-name helpers return the expected values

Recommended tool:

- `GoogleTest`

### Model

Purpose:

- verify UI-facing state progression and listener notifications

Typical checks:

- `Start_Brewing()` starts a session and notifies the listener
- first tick establishes the time base only
- active ticks advance the simulation
- completion notification happens after the done-hold interval
- completion notification is emitted only once

Recommended tools:

- `GoogleTest`
- `gMock`

### selection presenter

Purpose:

- verify forwarding of user intent from presenter to model

Typical checks:

- selected drink type is forwarded to the model
- no call happens if the presenter is not bound to a model

Recommended tools:

- `GoogleTest`
- `gMock`

### brewing presenter

Purpose:

- verify forwarding of model state to the view

Typical checks:

- `activate()` forwards the current session
- session updates are forwarded to the view
- completion is forwarded to the view

Recommended tools:

- `GoogleTest`
- `gMock`

### splash presenter

Purpose:

- keep a small smoke-test around an intentionally simple presenter

Typical checks:

- `activate()` has no unintended side effect
- `deactivate()` has no unintended side effect

Recommended tool:

- `GoogleTest`

## Test Boundaries

The unit-test layer should stay cleanly separated from:

- HAL APIs
- direct peripheral handles
- TouchGFX runtime startup
- display assets
- generated screen base classes
- board timing that depends on the target

The practical rule is:

- if a dependency talks directly to hardware or framework startup, isolate it behind a tiny handwritten interface or keep it out of the unit-test scope

## Implemented Test Seams

The goal was not a large redesign. Only the smallest useful seams were introduced.

### 1. Model time source seam

Current implementation:

- [TouchGFX/gui/include/gui/model/ModelInterfaces.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/model/ModelInterfaces.hpp)
- [TouchGFX/gui/include/gui/model/Model.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/model/Model.hpp)
- [TouchGFX/gui/src/model/Model.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/model/Model.cpp)

Practical shape:

- `ITick_Source` isolates time acquisition
- `Model` accepts it through `Set_Tick_Source(...)`
- tests can drive `tick()` deterministically without HAL or wall-clock timing

Suggested interface:

```cpp
/**
 * @brief Abstraction for the model tick time source.
 */
class ITick_Source
{
public:
    virtual ~ITick_Source() = default;
    virtual uint32_t Now_Ms() = 0;
};
```

### 2. Presenter-facing seams

Current implementation:

- [TouchGFX/gui/include/gui/model/ModelInterfaces.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/model/ModelInterfaces.hpp)
- [TouchGFX/gui/include/gui/brewing_screen_screen/IBrewing_View.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/brewing_screen_screen/IBrewing_View.hpp)
- [TouchGFX/gui/include/gui/slection_screen_screen/ISelection_View.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/slection_screen_screen/ISelection_View.hpp)
- [TouchGFX/gui/include/gui/splash_screen_screen/ISplash_View.hpp](C:/st_apps/coffee_machine/TouchGFX/gui/include/gui/splash_screen_screen/ISplash_View.hpp)

Practical shape:

- `IModel` gives presenters a narrow model contract
- `IBrewing_View`, `ISelection_View`, and `ISplash_View` isolate presenter interaction from concrete screen classes
- real view classes still implement the real UI behavior

Suggested example:

```cpp
/**
 * @brief View contract for the brewing presenter.
 */
class IBrewing_View
{
public:
    virtual ~IBrewing_View() = default;
    virtual void Update_Session(const BrewingSession& session) = 0;
    virtual void Handle_Brewing_Completed() = 0;
};
```

### 3. Application runtime seam

Current implementation:

- [coffee_machine/coffee_machine_app.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.hpp)
- [coffee_machine/coffee_machine_app.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.cpp)

Practical shape:

- `IApp_Runtime` isolates logging, delay, and TouchGFX startup/process calls
- the existing production-facing free functions still exist
- overloads with injected runtime adapters enable direct host-side tests

Suggested example:

```cpp
/**
 * @brief Runtime contract for the handwritten TouchGFX application facade.
 */
class IApp_Runtime
{
public:
    virtual ~IApp_Runtime() = default;
    virtual void Log(const char* message) = 0;
    virtual void Delay_Ms(uint32_t delay_ms) = 0;
    virtual void TouchGfx_Init() = 0;
    virtual void TouchGfx_Process() = 0;
};
```

## Naming Rules For Test Code

Test code should use names that stay readable in Visual Studio, in diffs, and during code review.

Required naming style:

- separate logical words with underscores
- keep each visible word group short
- prefer names that can be read aloud without visual strain

Examples:

- `Start_Initializes_Session_For_Espresso`
- `Tick_Notifies_Completion_After_Done_Hold`
- `Activate_Updates_View_With_Current_Session`

Avoid harder-to-scan names such as:

- `StartInitializesSessionForEspresso`
- `TickNotifiesCompletionAfterDoneHold`

This rule applies to:

- test case names
- helper types in test code
- fake and mock class names
- small support functions added for testability

## Doxygen Rules For Test Files

The handwritten production code already uses Doxygen comments. The test code should continue that style.

### File header

Each test source file should begin with a Doxygen file header.

Example:

```cpp
/**
 * @file coffee_machine_simulation_tests.cpp
 * @brief Unit tests for the brewing-domain logic in CoffeeMachineSimulation.
 */
```

### Test goal comment

Each test should document:

- the test goal
- the expected behavior

Example:

```cpp
/**
 * @brief Test goal: Starting an Espresso session initializes the session
 *        with the expected start values.
 *
 * Expected behavior:
 * - the simulation becomes active
 * - the phase is Start
 * - the progress is zero
 * - the steam level is Low
 */
TEST(CoffeeMachineSimulationTests, Start_Initializes_Session_For_Espresso)
{
    CoffeeMachineSimulation simulation;

    simulation.start(CoffeeType::Espresso);

    EXPECT_TRUE(simulation.isActive());
    EXPECT_EQ(BrewingPhase::Start, simulation.getPhase());
    EXPECT_EQ(0U, simulation.getProgressPercent());
    EXPECT_EQ(SteamLevel::Low, simulation.getSteamLevel());
}
```

### Comment style

Practical rules:

- describe behavior, not implementation trivia
- state why the test exists
- keep comments short enough to stay readable
- use English in the repository documentation and test code to match the existing project language

## Current Test Tree

The current host-test structure is:

```text
tests/
  CMakeLists.txt
  unit/
    coffee_machine_app_tests.cpp
    countdown_formatter_tests.cpp
    coffee_machine_simulation_tests.cpp
    model_tests.cpp
    slection_screen_presenter_tests.cpp
    brewing_screen_presenter_tests.cpp
    splash_screen_presenter_tests.cpp
```

This is a working structure, not a rigid rule. The important part is:

- production code stays in its current folders
- tests live in a separate host-side tree
- test doubles are easy to find, even if they currently live inside the test translation units

## CMake Direction

The repository now contains a dedicated host-side CMake target for unit tests.

Current target:

- `coffee_machine_unittest`

Main properties:

- builds as a normal host executable on Windows
- runs in Visual Studio
- is discoverable through `gtest_discover_tests(...)`
- does not require STM32 startup files
- links only the handwritten source files under test

Current ingredients:

- `GoogleTest`
- `gMock`
- `gtest_discover_tests(...)`

The unit-test target stays separate from:

- the main STM32 application target
- the bootloader target
- flashing targets

The top-level CMake entry point controls this through:

- [CMakeLists.txt](C:/st_apps/coffee_machine/CMakeLists.txt)
- option `ENABLE_UNIT_TESTS`

For day-to-day Visual Studio work, the recommended entry point is the dedicated test workspace:

- [tests/CMakeLists.txt](C:/st_apps/coffee_machine/tests/CMakeLists.txt)
- [tests/CMakePresets.json](C:/st_apps/coffee_machine/tests/CMakePresets.json)

## Short Workflow

The day-to-day workflow is:

1. open [tests](C:/st_apps/coffee_machine/tests) as its own Visual Studio CMake workspace
2. build `coffee_machine_unittest`
3. run the tests in Test Explorer
4. use later board-side Python checks for end-to-end scenarios

Shell equivalent:

```powershell
cmake -S tests -B build/host-unittest-standalone -G "Visual Studio 17 2022"
cmake --build build/host-unittest-standalone --config Debug --target coffee_machine_unittest
ctest -C Debug --test-dir build/host-unittest-standalone --output-on-failure
```

This split keeps:

- unit-test feedback fast
- hardware checks realistic
- failure analysis simpler

## Visual Studio Click Path

The following path is the recommended manual workflow in Visual Studio for click-based users.

### A. Open the unit-test workspace in Visual Studio

1. Start Visual Studio 2022.
2. Open the folder:
   - `C:/st_apps/coffee_machine/tests`

Expected result:

- Visual Studio loads only the host-side test workspace
- the project tree stays small and focused on unit tests

### B. Select the host-side test preset

1. Locate the CMake preset selector in the Visual Studio toolbar.
2. Open the preset drop-down.
3. Select:
   - `Build Host Unittest Debug`

Practical note:

- the internal preset name is `host-unittest-debug`
- Visual Studio can show the display name `Build Host Unittest Debug` in the toolbar instead
- that is the correct selection

### C. Build the unit-test target

Simplest path:

1. In the Visual Studio menu, click:
   - `Build`
   - `Build All`

Alternative path:

1. Open the Project Explorer or CMake target view.
2. Find:
   - `coffee_machine_unittest`
3. Build that target.

Expected result:

- the host-side test executable is built successfully

### D. Open the Test Explorer

1. In Visual Studio, open:
   - `Test`
   - `Test Explorer`
2. Wait briefly for test discovery.

Expected result:

- GoogleTest test cases from `coffee_machine_unittest` are discovered
- the test tree appears under:
  - `coffee_machine_unittest`
  - `<empty namespace>`

### E. Run the tests

1. In Test Explorer, click:
   - `Run All`

Expected result:

- the tests execute on the PC
- no board is required

## Rollout Status

The original rollout order has progressed as follows:

1. `tests/` tree and host-side `CMakeLists.txt`: done
2. `countdown_formatter` tests: done
3. `coffee_machine_simulation` tests: done
4. `ITick_Source` and `Model` tests: done
5. presenter-facing interfaces: done
6. presenter tests: done
7. `IApp_Runtime` and `coffee_machine_app` tests: done

Current next sensible expansion areas:

- additional edge-case coverage in `coffee_machine_app`
- finer-grained presenter tests if new handwritten behavior is added
- extraction of shared test doubles only if the test tree grows enough to justify it

## Current Test Set

The current test tree already includes:

- `CoffeeMachineAppTests`
- `CountdownFormatterTests`
- `CoffeeMachineSimulationTests`
- `ModelTests`
- `SlectionScreenPresenterTests`
- `BrewingScreenPresenterTests`
- `SplashScreenPresenterTests`

Representative test cases:

- `App_Start_Initializes_TouchGfx_And_Logs_Start_Path`
- `App_Process_Forwards_One_TouchGfx_Iteration`
- `Start_Initializes_Session_For_Espresso`
- `Tick_Notifies_Completion_After_Done_Hold`
- `Activate_Updates_View_With_Current_Session`
- `Start_Brewing_Forwards_Type_To_Model`

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

## Safe Edit Boundaries

The preferred places for unit-test-related handwritten work are:

- `tests/...`
- handwritten files in `coffee_machine/...`
- handwritten files in `TouchGFX/gui/include/gui/...`
- handwritten files in `TouchGFX/gui/src/...`

Avoid placing unit-test logic in:

- `TouchGFX/generated/...`
- CubeMX-generated startup code
- board-side helper code used for exploratory hardware checks

## Satisfaction Check

This chapter is in a good place if a developer can answer these questions from it:

- which files are currently covered by host-side unit tests?
- which file is intentionally excluded?
- where do model and presenter tests fit into MVP?
- which small test seams exist today?
- how should test names be written?
- how should test files and test goals be documented?
- how is the host-side test tree configured and run?

That is the bar this chapter is meant to meet.
