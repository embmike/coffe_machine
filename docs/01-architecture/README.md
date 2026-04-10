# Architecture

## Goal

Describe the runtime architecture and explain why it was chosen.

## Runtime Layout

This project uses a two-stage runtime layout:

1. An internal bootloader starts from internal flash at `0x08000000`
2. The main application executes in place (XIP) from external QSPI flash at `0x90000000`

The framebuffer is placed in external SDRAM.

This split was chosen because the STM32H750 device has limited internal flash, while the application, generated UI assets, and TouchGFX-related code are much larger than what should comfortably live in internal flash.

## Boot Paths

### Normal runtime path

```mermaid
flowchart TD
    A["Reset"] --> B["Boot from internal flash @ 0x08000000"]
    B --> C["extmem_bootloader"]
    C --> D["Initialize QSPI and SDRAM"]
    D --> E["Disable/clean interrupt state"]
    E --> F["Set VTOR to application vector table"]
    F --> G["Load application MSP"]
    G --> H["Jump to application reset handler @ 0x90000000"]
    H --> I["coffee_machine main()"]
```

### Why the direct application start is not the default path

Starting directly at `0x90000000` was evaluated, but it was not robust enough in the current setup.

The main reason is architectural:

- the external QSPI XIP path must already be valid at the moment the CPU fetches the first application instructions
- in practice, the project was stabilized by letting the internal bootloader initialize external memory first

That is why the default and recommended runtime path remains:

- option byte boot address -> internal flash
- bootloader first
- application second

## Memory Layout

The relevant memory regions are:

- internal bootloader flash at `0x08000000`
- external QSPI application flash at `0x90000000`
- external QSPI assets flash at `0x90200000`
- SDRAM framebuffer at `0xD0000000`
- stack in DTCM RAM

This is reflected in:

- [STM32H750XBHX_FLASH.ld](C:/st_apps/coffee_machine/STM32H750XBHX_FLASH.ld)

At a high level:

```mermaid
flowchart LR
    A["Internal flash\n0x08000000"] --> B["extmem_bootloader"]
    C["External QSPI\n0x90000000"] --> D["coffee_machine code"]
    E["External QSPI\n0x90200000"] --> F["assets / generated data"]
    G["External SDRAM\n0xD0000000"] --> H["LTDC framebuffer"]
    I["DTCM RAM"] --> J["runtime stack"]
```

### Practical meaning

- internal flash contains only the minimal runtime entry path
- external QSPI contains the main application
- SDRAM contains large display data
- the application itself does not have to fit into internal flash

## Why This Architecture

This architecture was chosen because it solved the actual problems encountered during bring-up.

### 1. Internal flash alone is not the right home for the full application

The application, generated GUI code, TouchGFX runtime, and supporting assets are a better fit for an external-memory design.

### 2. External XIP must be initialized before the application can run safely

The stable solution was:

- start from internal flash
- initialize QSPI memory-mapped mode and SDRAM in the bootloader
- only then jump into the XIP application

### 3. The bootloader must hand over control cleanly

The final hand-off had to do more than just branch to the application.

The critical pieces were:

- disable and clean pending interrupts
- stop SysTick
- set `SCB->VTOR` to the application vector table
- set `MSP` from the application vector table
- jump to the application reset handler

This logic lives in:

- [ExtMem_Boot/Src/main.c](C:/st_apps/coffee_machine/ExtMem_Boot/Src/main.c)

### 4. The application must not re-break the early memory setup

During stabilization, several early application-side behaviors had to be adjusted:

- no premature QSPI reconfiguration during XIP bring-up
- no fragile early FMC speculative-access handling in `SystemInit()`
- correct SDRAM initialization sequence in the application-side FMC path
- a tick-independent fallback for the SDRAM component delay path

Those fixes were necessary to move from:

- “boots sometimes / faults early”

to:

- “bootloader starts”
- “jump works”
- “application runs”
- “test pattern and UART diagnostics work”

## Bootloader Hand-off Details

The internal bootloader is not just a packaging helper. It is the runtime bridge between reset and application startup.

### Responsibilities of `extmem_bootloader`

- configure clocks required for boot-time peripheral initialization
- initialize external memory infrastructure
- prepare QSPI XIP access
- prepare SDRAM access
- clean interrupt state before hand-off
- transfer control to the external application

### Critical hand-off operations

The important operations in the hand-off path are:

- `SCB->VTOR = APPLICATION_ADDRESS`
- `__set_MSP(*(uint32_t*)APPLICATION_ADDRESS)`
- read reset handler from `APPLICATION_ADDRESS + 4`
- jump via function pointer

Additionally, the bootloader now performs NVIC cleanup before the jump:

- disable interrupts
- clear pending interrupt state
- stop SysTick

Without this cleanup, stale interrupt state can leak across the hand-off and cause confusing early faults.

## Key Project Files

These files are central to understanding the final architecture:

- [CMakeLists.txt](C:/st_apps/coffee_machine/CMakeLists.txt)
  - defines projects, flash targets, and generated artifacts
- [STM32H750XBHX_FLASH.ld](C:/st_apps/coffee_machine/STM32H750XBHX_FLASH.ld)
  - defines the memory layout of the XIP application
- [ExtMem_Boot/Src/main.c](C:/st_apps/coffee_machine/ExtMem_Boot/Src/main.c)
  - bootloader startup and jump-to-application logic
- [Core/Src/main.cpp](C:/st_apps/coffee_machine/Core/Src/main.cpp)
  - application entry path, diagnostics, test pattern, and current TouchGFX integration point
- [Core/Src/fmc.c](C:/st_apps/coffee_machine/Core/Src/fmc.c)
  - application-side SDRAM initialization
- [Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_qspi.c](C:/st_apps/coffee_machine/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_qspi.c)
  - board-level QSPI support
- [Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_sdram.c](C:/st_apps/coffee_machine/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_sdram.c)
  - board-level SDRAM support

## Important Project and Board Settings

These settings turned out to be essential.

### Option bytes

The normal and recommended board boot address is:

- `BOOT_CM7_ADD0 = 0x08000000`

That keeps the internal bootloader in control of startup.

### Debug strategy

The stable debug strategy is split by developer use case:

- bootloader debug via `extmem_bootloader`
- application debug via `coffee_machine`
- board still starts through the bootloader in the validated boot-to-app path

### Debugger breakpoint behavior

For the current VisualGDB-based boot-to-app debug path, hardware breakpoints had to be enforced to avoid early startup failures caused by software breakpoint / flash hotpatch behavior during the bootloader-to-application transition.

## ST References

The following ST documents were important references while shaping this architecture:

- [AN5188 - How to execute code from external memory on STM32F7 and STM32H750/H7B0/H730 MCUs](https://www.st.com/resource/en/application_note/an5188-how-to-execute-code-from-external-memory-on-stm32f7-and-stm32h750h7b0h730-mcus-stmicroelectronics.pdf)
- [AN5188 - External memory code execution on STM32F7x0 value line, STM32H750 value line, STM32H7B0 value line and STM32H730 value line MCUs](https://www.st.com/resource/en/application_note/an5188-external-memory-code-execution-on-stm32f7x0-value-line-stm32h750-value-line-stm32h7b0-value-line-and-stm32h730-value-line-mcus-stmicroelectronics.pdf)
- [UM2488 - Discovery kit with STM32H750XB microcontroller](https://www.st.com/resource/en/user_manual/um2488-discovery-kits-with-stm32h745xi-and-stm32h750xb-microcontrollers-stmicroelectronics.pdf)
- [STM32H750 Value Line product page](https://www.st.com/en/microcontrollers-microprocessors/stm32h750-value-line.html)

## Diagram

```mermaid
flowchart TD
    A["Internal flash boot"] --> B["extmem_bootloader"]
    B --> C["Initialize external memories"]
    C --> D["NVIC cleanup + SysTick stop"]
    D --> E["Set VTOR / MSP"]
    E --> F["Jump to XIP application"]
    F --> G["coffee_machine main()"]
```
