# Coffee Machine

## User View

TBD.

## Developer View

See:

- [Architecture](./docs/01-architecture/README.md)
- [Build and Flash](./docs/02-build-and-flash/README.md)
- [Debugging](./docs/03-debugging/README.md)
- [Drivers](./docs/04-drivers/README.md)
- [Artifacts](./docs/05-artifacts/README.md)
- [TouchGFX](./docs/06-touchgfx/README.md)

## Quick Start

TBD.

## Architecture Overview
### Flow from boot to an app
```mermaid
flowchart TD
    A["Reset @ 0x08000000"]
    A --> B["extmem_bootloader"]
    B --> C["QSPI + SDRAM init"]
    C --> D["Jump to app @ 0x90000000"]
    D --> E["coffee_machine"]
    E --> F["LTDC framebuffer in SDRAM"]
    E --> G["TouchGFX / application logic"]
```

