# Testing

## Goal

Describe the unit-test strategy for the handwritten application logic, define the intended test boundaries, and provide a practical introduction path for GoogleTest and gMock in this repository.

This chapter is the planning and implementation guide for the unit-test layer.

## Scope

This chapter covers:

- unit-test goals
- test boundaries
- testable source files
- files intentionally excluded from unit tests
- required small refactorings
- naming rules for test code
- Doxygen rules for test files
- Visual Studio and Test Explorer integration goals
- phased introduction plan

This chapter does not define:

- board-level exploratory checks
- Python-driven PC-to-board application scenarios
- hardware validation
- system or integration tests on the target

Those topics belong to later validation work outside the C++ unit-test layer.

## Test Strategy

The project uses two complementary test layers:

1. host-based C++ unit tests
2. later PC-driven board scenario tests

The first layer is the focus of this chapter.

Unit tests are intended for:

- deterministic business logic
- presenter-to-model and presenter-to-view interaction logic
- small formatting helpers
- code that can run without HAL, TouchGFX runtime startup, or board hardware

Later board-side scenario checks are intended for:

- real peripheral behavior
- touch interaction on hardware
- startup and flashing workflows
- target timing under realistic runtime conditions

## Architectural Fit

The handwritten GUI-side code follows the Model-View-Presenter pattern.

That matters for testing because each layer has a different test role:

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

The real TouchGFX views themselves are not the primary unit-test target. They are better treated as framework-facing code with as little handwritten logic as possible.

## Unit-Test Targets

The current handwritten source files are split into three groups.

### Group 1: Immediate unit-test targets

These files provide the best value for the first test wave:

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

### Group 2: Deferred unit-test target

This file may be unit-tested later, but only after a small runtime abstraction is introduced:

- [coffee_machine/coffee_machine_app.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.hpp)
- [coffee_machine/coffee_machine_app.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.cpp)

### Group 3: Intentionally excluded from unit tests

This file is not part of the unit-test target set:

- [coffee_machine/coffee_machine_board.hpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_board.hpp)
- [coffee_machine/coffee_machine_board.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_board.cpp)

Reason:

- the board helper is itself used as an exploratory and bring-up-oriented runtime test path
- its direct dependencies are intentionally hardware-facing
- its value is higher in board validation than in host-side unit isolation

## Planned Test Coverage

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

- `startBrewing()` starts a session and notifies the listener
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

## Required Small Refactorings

The goal is not a large redesign. Only the smallest useful seams should be introduced.

### 1. Isolate the Model time source

Current issue:

- [TouchGFX/gui/src/model/Model.cpp](C:/st_apps/coffee_machine/TouchGFX/gui/src/model/Model.cpp) reads the current time internally through `HAL_GetTick()` or `std::chrono`

Planned change:

- introduce a handwritten tick-source abstraction
- inject it into `Model`

Expected benefit:

- deterministic tests for `tick()`
- no hidden dependency on wall-clock timing

Suggested interface:

```cpp
/**
 * @brief Abstraction for the model tick time source.
 */
class ITick_Source
{
public:
    virtual ~ITick_Source() = default;

    /**
     * @brief Returns the current tick time in milliseconds.
     * @return Current tick time.
     */
    virtual uint32_t Now_Ms() = 0;
};
```

### 2. Introduce tiny presenter-facing interfaces

Current issue:

- presenters are tightly coupled to concrete TouchGFX view classes

Planned change:

- introduce small view and model interfaces where interaction behavior should be unit-tested

Expected benefit:

- presenter tests become simple interaction tests
- no need to instantiate real TouchGFX screen classes in unit tests

Suggested examples:

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

```cpp
/**
 * @brief Model contract for the selection presenter.
 */
class ISelection_Model
{
public:
    virtual ~ISelection_Model() = default;
    virtual void Start_Brewing(CoffeeType type) = 0;
};
```

### 3. Defer coffee_machine_app until needed

Current issue:

- [coffee_machine/coffee_machine_app.cpp](C:/st_apps/coffee_machine/coffee_machine/coffee_machine_app.cpp) directly calls runtime functions such as `HAL_Delay()`, `MX_TouchGFX_Init()`, and `MX_TouchGFX_Process()`

Planned change:

- no immediate action
- only introduce a small runtime adapter if unit-test value becomes important

Expected benefit:

- the first test wave stays focused and small

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

## Planned Test Tree

The recommended host-test structure is:

```text
tests/
  CMakeLists.txt
  unit/
    countdown_formatter_tests.cpp
    coffee_machine_simulation_tests.cpp
    model_tests.cpp
    slection_screen_presenter_tests.cpp
    brewing_screen_presenter_tests.cpp
    splash_screen_presenter_tests.cpp
  doubles/
    fake_tick_source.hpp
    mock_model_listener.hpp
    mock_selection_model.hpp
    mock_brewing_view.hpp
```

This is a working structure, not a rigid rule. The important part is:

- production code stays in its current folders
- tests live in a separate host-side tree
- fakes and mocks are easy to find

## CMake Direction

The recommended next step is a dedicated host-side CMake target for unit tests.

Intended target:

- `coffee_machine_unit_tests`

Main expectations:

- builds as a normal host executable on Windows
- runs in Visual Studio
- is visible in Test Explorer
- does not require STM32 startup files
- links only the handwritten source files under test

Expected ingredients:

- `GoogleTest`
- `gMock`
- `gtest_discover_tests(...)`

The unit-test target should stay separate from:

- the main STM32 application target
- the bootloader target
- flashing targets

## Visual Studio Goal

The intended day-to-day workflow is:

1. build the host-side test target in Visual Studio
2. run tests in Test Explorer
3. keep failures local and fast
4. use board-side Python checks later for end-to-end scenarios

That split keeps:

- unit-test feedback fast
- hardware checks realistic
- failure analysis simpler

## Introduction Plan

The recommended rollout order is:

1. Create the `tests/` tree and host-side `CMakeLists.txt`
2. Add `countdown_formatter` tests
3. Add `coffee_machine_simulation` tests
4. Introduce `ITick_Source` and add `Model` tests
5. Introduce small presenter-facing interfaces
6. Add presenter tests
7. Re-evaluate `coffee_machine_app`

This order gives:

- fast visible progress
- low architectural churn
- useful tests early

## First Suggested Test Set

### countdown_formatter

- `Formats_Zero_Milliseconds_As_Zero`
- `Truncates_Sub_Second_Value_To_Zero`
- `Formats_One_Second_As_One`
- `Returns_Safely_For_Null_Buffer`
- `Returns_Safely_For_Zero_Buffer_Size`

### coffee_machine_simulation

- `Find_Profile_Returns_Espresso_Profile`
- `Start_Initializes_Session_For_Espresso`
- `Update_Advances_Progress_For_Active_Session`
- `Update_Sets_Phase_To_Brewing_After_Twenty_Percent`
- `Update_Sets_Phase_To_Finishing_After_Eighty_Percent`
- `Update_Sets_Phase_To_Done_At_Hundred_Percent`
- `Stop_Resets_Session_To_Idle`
- `Reset_Clears_Active_State`
- `Get_Coffee_Log_Name_Returns_Expected_Label`

### Model

- `Start_Brewing_Notifies_Updated_Session`
- `Tick_Establishes_Time_Base_On_First_Call`
- `Tick_Updates_Active_Session`
- `Tick_Notifies_Completion_After_Done_Hold`
- `Tick_Notifies_Completion_Only_Once`

### selection presenter

- `Start_Brewing_Forwards_Type_To_Model`
- `Start_Brewing_Does_Nothing_Without_Model`

### brewing presenter

- `Activate_Updates_View_With_Current_Session`
- `On_Brewing_Session_Updated_Forwards_Session_To_View`
- `On_Brewing_Session_Completed_Forwards_To_View`

### splash presenter

- `Activate_Has_No_Side_Effect`
- `Deactivate_Has_No_Side_Effect`

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

- which files belong in the first unit-test wave?
- which file is intentionally excluded?
- where do model and presenter tests fit into MVP?
- which small refactorings are expected?
- how should test names be written?
- how should test files and test goals be documented?
- what is the recommended rollout order?

That is the bar this chapter is meant to meet.
