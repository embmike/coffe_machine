# VisualGDB debug profiles

This folder keeps the switchable VisualGDB profile templates and helper scripts for this project.

The active VisualGDB configuration remains:

- [coffee_machine.vgdbcmake](C:/st_apps/coffee_machine/coffee_machine.vgdbcmake)

The files in this folder are templates that can replace the active root file.

## Profiles

- `coffee_machine.bootloader_debug.vgdbcmake`
  - Debug start target: `extmem_bootloader`
  - Loads `coffee_machine` symbols at `0x90000000`
  - Best for:
    - bootloader work
    - realistic XIP boot path
    - `DebugProbe_*`-based app tracing

- `coffee_machine.boot_to_app_debug.vgdbcmake`
  - Debug start target: `coffee_machine`
  - Adds `extmem_bootloader` symbols at `0x08000000`
  - Does not call `load`
  - Best for:
    - normal application debugging through the real bootloader hand-off
    - GUI breakpoints in the application
    - the validated day-to-day IDE workflow

## Switch commands

From the project root:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\visualgdb\use_bootloader_debug.ps1
```

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\visualgdb\use_boot_to_app_debug.ps1
```

After switching, reload the Visual Studio project/solution so VisualGDB picks up the new [coffee_machine.vgdbcmake](C:/st_apps/coffee_machine/coffee_machine.vgdbcmake).
