# Build and Flash

## Goal

Explain build configurations, flash targets, and developer workflows.

## Scope Boundary

This chapter describes the embedded build and flash world.

It covers:

- `coffee_machine`
- `extmem_bootloader`
- `flash_app`
- `flash_bootloader`
- `flash_system`

It does not cover the host-side unit-test world.

For that path, use:

- [docs/08-unit-testing/README.md](C:/st_apps/coffee_machine/docs/08-unit-testing/README.md)

## Build Configurations

### Debug

Use `Debug` for normal development work.

Characteristics:

- best source-level debugging experience
- debug symbols available
- application breakpoints are expected to work
- preferred configuration for driver bring-up, bootloader work, and UI integration work

Typical usage:

- debug the bootloader
- debug the boot path from bootloader to application
- debug the application itself
- verify UART diagnostics and test-pattern bring-up

### Release

Use `Release` for runnable software images and performance-oriented tests.

Characteristics:

- optimized build
- source-level debugging is limited by optimization
- breakpoints may still work, but line-by-line behavior is less predictable

Typical usage:

- create a realistic runtime image
- validate full-system behavior
- prepare software intended to run without active debugging

## Flash Targets

### `flash_bootloader`

Programs only the internal bootloader image into internal flash.

Use this when:

- bootloader sources changed
- the bootloader must be restored
- bootloader-specific debugging is planned

What it writes:

- internal flash only

What it does not write:

- the external QSPI application image

### `flash_app`

Programs only the application image into the external QSPI flash.

Use this when:

- only application sources changed
- bootloader is already known-good and already programmed
- application debugging is planned

What it writes:

- external QSPI application image only

What it does not write:

- internal bootloader

### `flash_system`

Programs the complete software stack used by the board at runtime.

Use this when:

- bringing up a fresh board
- synchronizing bootloader and application together
- validating a full software state
- preparing a release candidate image

What it writes:

- internal bootloader
- external QSPI application image

## Typical Workflows

## Host-side unit tests are a separate path

Host-side unit tests are intentionally not part of the embedded flash workflow.

Important practical rule:

- do not use `flash_app`, `flash_bootloader`, or `flash_system` for host-side unit tests

The host-side unit-test path uses:

- the separate Visual Studio CMake workspace at [tests](C:/st_apps/coffee_machine/tests)
- preset `host-unittest-debug`
- target `coffee_machine_unittest`
- Visual Studio Test Explorer or `ctest`

That path belongs to the PC-side test world and does not require the board.

### Update and debug the bootloader

Recommended configuration: `Debug`

Steps:

1. In Visual Studio, select the `Debug` build configuration.
2. Build the project.
3. Run `flash_bootloader`.
4. Start debugging [extmem_bootloader](C:/st_apps/coffee_machine/build/VisualGDB/Debug/extmem_bootloader) from the IDE.
5. Set breakpoints in bootloader source files through the IDE.

Use this workflow for:

- QSPI bring-up
- SDRAM bring-up
- jump-to-application behavior
- early exception handling in the bootloader

### Update and debug the application

Recommended configuration: `Debug`

Steps:

1. In Visual Studio, select the `Debug` build configuration.
2. Build the project.
3. Run `flash_app`.
4. Start debugging [coffee_machine](C:/st_apps/coffee_machine/build/VisualGDB/Debug/coffee_machine) from the IDE.
5. Set application breakpoints in [main.cpp](C:/st_apps/coffee_machine/Core/Src/main.cpp) or other application files through the IDE.

Runtime behavior:

- the board still starts through the bootloader
- the bootloader initializes external memory
- the bootloader jumps to the application
- the debugger then reaches application breakpoints

Use this workflow for:

- application bring-up
- LTDC / framebuffer debugging
- UART diagnostic flow
- TouchGFX integration work
- target-side validation after unit-test work on the PC

### Program the complete system

Recommended configuration:

- `Debug` for a fully debuggable complete image
- `Release` for a production-style runtime image

Steps:

1. In Visual Studio, select the intended build configuration.
2. Build the project.
3. Run `flash_system`.
4. Either run the board standalone or start a matching debug session.

Use this workflow for:

- synchronizing bootloader and application after wider changes
- validating a complete software state
- preparing software for integration tests

## Step-by-step Notes

### Build and flash the application for application debugging

1. In the Visual Studio toolbar, choose `Debug`.
2. Build the solution or build the required target.
3. In the Project Explorer, right-click `flash_app`.
4. Click `Build Target(s)`.
5. Wait until STM32CubeProgrammer finishes programming the external flash.
6. Start debugging [coffee_machine](C:/st_apps/coffee_machine/build/VisualGDB/Debug/coffee_machine).

### Build and flash the bootloader for bootloader debugging

1. In the Visual Studio toolbar, choose `Debug`.
2. Build the solution or build the required target.
3. In the Project Explorer, right-click `flash_bootloader`.
4. Click `Build Target(s)`.
5. Start debugging [extmem_bootloader](C:/st_apps/coffee_machine/build/VisualGDB/Debug/extmem_bootloader).

### Build and flash the complete system

1. In the Visual Studio toolbar, choose `Debug` or `Release`.
2. Build the solution.
3. In the Project Explorer, right-click `flash_system`.
4. Click `Build Target(s)`.
5. Run the board standalone or start debugging with the matching debug path.

### GDB terminal input

Normal developer workflows should not require manual GDB commands.

Current expectation:

- `flash_bootloader`: no manual GDB input
- `flash_app`: no manual GDB input
- `flash_system`: no manual GDB input
- normal bootloader debugging: no manual GDB input
- normal boot-to-app debugging: requires a short manual GDB symbol-loading sequence

At the current project maturity, Boot-to-App debug is the main exception. The validated sequence is documented in:

- [Debugging](C:/st_apps/coffee_machine/docs/03-debugging/README.md)

Manual GDB work should still be treated as a controlled workflow step, not as ad-hoc trial-and-error.

## Satisfaction Check

This chapter is in a good place if a reader can answer these questions:

- which parts belong to the embedded build and flash world?
- which flash target writes what?
- when should `flash_system` be used instead of `flash_app`?
- why are host-side unit tests not described as flash workflows here?

That is the bar this chapter is meant to meet.
