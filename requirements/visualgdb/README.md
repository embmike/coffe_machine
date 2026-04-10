# VisualGDB debug profiles

This folder keeps two switchable VisualGDB project profiles.

## Profiles

- `coffee_machine.bootloader_debug.vgdbcmake`
  - Debug start target: `extmem_bootloader`
  - Loads `coffee_machine` symbols at `0x90000000`
  - Best for:
    - bootloader work
    - realistic XiP boot path
    - `DebugProbe_*`-based app tracing

- `coffee_machine.app_direct_debug.vgdbcmake`
  - Debug start target: `coffee_machine`
  - No automatic `load`
  - No automatic bootloader hand-off
  - Best for:
    - trying a more IDE-centric app debug flow after `flash_debug_app`
    - experiments with direct app boot/debug setups

- `coffee_machine.boot_to_app_debug.vgdbcmake`
  - Debug start target: `coffee_machine`
  - Adds `extmem_bootloader` symbols at `0x08000000`
  - Does not call `load`
  - Best for:
    - trying an IDE-centric app debug flow while the board still boots through the internal bootloader
    - experiments with GUI breakpoints in the app without switching option bytes

## Switch commands

From the project root:

```powershell
powershell -ExecutionPolicy Bypass -File .\requirements\visualgdb\use_bootloader_debug.ps1
```

```powershell
powershell -ExecutionPolicy Bypass -File .\requirements\visualgdb\use_app_direct_debug.ps1
```

```powershell
powershell -ExecutionPolicy Bypass -File .\requirements\visualgdb\use_boot_to_app_debug.ps1
```

After switching, reload the Visual Studio project/solution so VisualGDB picks up the new `coffee_machine.vgdbcmake`.
