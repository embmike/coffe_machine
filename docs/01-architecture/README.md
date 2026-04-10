# Architecture

## Goal

Describe the runtime architecture and explain why it was chosen.

## Runtime Layout

TBD.

## Boot Paths

TBD.

## Memory Layout

TBD.

## Why This Architecture

TBD.

## Diagram

```mermaid
flowchart TD
    A["Internal flash boot"] --> B["extmem_bootloader"]
    B --> C["Initialize external memories"]
    C --> D["Set VTOR / MSP"]
    D --> E["Jump to XIP application"]
    E --> F["coffee_machine main()"]
```

