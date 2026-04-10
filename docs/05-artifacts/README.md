# Artifacts

## Goal

Describe which outputs are intended for developers and which are internal helper artifacts.

## Projects

### `coffee_machine`

Main application project.

Purpose:

- build and debug the XIP application
- contain the application source code, drivers, UI, and runtime logic

Typical developer use:

- set application breakpoints in the IDE
- start application-oriented debug sessions
- produce the external application image

Main output:

- [coffee_machine](C:/st_apps/coffee_machine/build/VisualGDB/Debug/coffee_machine)

### `extmem_bootloader`

Bootloader project.

Purpose:

- initialize external memories before the application starts
- prepare the XIP runtime path
- jump from internal flash to the external application

Typical developer use:

- debug QSPI bring-up
- debug SDRAM bring-up
- debug the jump to the application

Main output:

- [extmem_bootloader](C:/st_apps/coffee_machine/build/VisualGDB/Debug/extmem_bootloader)

## Flash Targets

### `flash_app`

Developer-facing flash target.

Purpose:

- program only the application image into external QSPI flash

Use this when:

- application code changed
- bootloader is already valid on the board
- application debugging is planned

Writes:

- external application image only

### `flash_bootloader`

Developer-facing flash target.

Purpose:

- program only the bootloader into internal flash

Use this when:

- bootloader code changed
- bootloader debugging is planned
- the board boot path must be repaired or restored

Writes:

- internal bootloader only

### `flash_system`

Developer-facing flash target.

Purpose:

- program the complete software stack used by the board at runtime

Use this when:

- the board should be synchronized to a known complete software state
- both bootloader and application changed
- a full-system image is needed for test or release work

Writes:

- internal bootloader
- external application image

## Internal Artifacts

### `artifact_app_hex`

Internal helper target that generates the application HEX file.

Generated file:

- [app_target.hex](C:/st_apps/coffee_machine/build/VisualGDB/Debug/app_target.hex)

Developer note:

- usually not started directly in normal workflows
- used as a dependency of `flash_app`

### `artifact_boot_hex`

Internal helper target that generates the bootloader HEX file.

Generated file:

- [boot.hex](C:/st_apps/coffee_machine/build/VisualGDB/Debug/boot.hex)

Developer note:

- usually not started directly in normal workflows
- used as a dependency of `flash_bootloader` and `flash_system`

### `artifact_boot_bin`

Internal helper target that generates the bootloader BIN file.

Generated file:

- [boot.bin](C:/st_apps/coffee_machine/build/VisualGDB/Debug/boot.bin)

Developer note:

- mainly useful as a packaging or exchange artifact
- not the normal day-to-day developer entry point

### `artifact_system_hex`

Internal helper target that generates the complete combined system HEX file.

Generated file:

- [target.hex](C:/st_apps/coffee_machine/build/VisualGDB/Debug/target.hex)

Developer note:

- represents the combined image view
- mainly useful for packaging and full-system programming workflows

### `artifact_intflash_hex`

Internal helper target that generates an internal-flash-oriented HEX artifact.

Generated file:

- [intflash.hex](C:/st_apps/coffee_machine/build/VisualGDB/Debug/intflash.hex)

Developer note:

- specialized helper artifact
- not part of the primary day-to-day developer workflow

## Recommended Mental Model

### Projects

These are the main things a developer builds and debugs:

- `coffee_machine`
- `extmem_bootloader`

### Flash Targets

These are the main things a developer uses to program the board:

- `flash_app`
- `flash_bootloader`
- `flash_system`

### Internal Artifacts

These exist to support packaging, flashing, and output generation:

- `artifact_app_hex`
- `artifact_boot_hex`
- `artifact_boot_bin`
- `artifact_system_hex`
- `artifact_intflash_hex`

Developers should usually start from the first two groups and only look at artifacts when needed.
