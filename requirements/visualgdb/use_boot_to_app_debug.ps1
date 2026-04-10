$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$source = Join-Path $PSScriptRoot "coffee_machine.boot_to_app_debug.vgdbcmake"
$target = Join-Path $projectRoot "coffee_machine.vgdbcmake"
Copy-Item -LiteralPath $source -Destination $target -Force
Write-Host "Activated VisualGDB profile: Boot to app debug"
