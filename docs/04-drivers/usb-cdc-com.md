# USB CDC / COM

## Goal

Explain the USB-CDC path used by the optional board-side system-test channel.

This chapter is about the host-visible communication path between:

- the STM32H750B-DK application
- the USB FS device interface on the board
- the COM port seen by the PC

## Purpose In This Project

The USB-CDC path is used for the board-side Python system tests.

Its role is:

- expose a stable command channel to the PC
- allow semantic test commands such as `get_state` or `select_drink`
- avoid dependence on the ST-LINK VCP debug path for the system-test workflow

## Physical Path

The system-test USB path uses:

- the board USB FS device interface
- the dedicated USB connector `CN13`

This is intentionally separate from the ST-LINK virtual COM path used for early UART diagnostics.

## Firmware Ownership

The relevant project areas are:

- [USB_DEVICE/App/usb_device.c](C:/st_apps/coffee_machine/USB_DEVICE/App/usb_device.c)
- [USB_DEVICE/App/usbd_cdc_if.c](C:/st_apps/coffee_machine/USB_DEVICE/App/usbd_cdc_if.c)
- [USB_DEVICE/Target/usbd_conf.c](C:/st_apps/coffee_machine/USB_DEVICE/Target/usbd_conf.c)
- [coffee_machine/app_test_channel.cpp](C:/st_apps/coffee_machine/coffee_machine/app_test_channel.cpp)
- [tests_python/harness/board_uart.py](C:/st_apps/coffee_machine/tests_python/harness/board_uart.py)

## Host Driver Expectation

On Windows, the board should appear as a normal USB-CDC serial device and therefore as a COM port.

Typical expectation:

- after connecting `CN13`, Windows assigns a COM number such as `COM6`
- the Python system-test launcher uses that COM port

In normal use, this means:

- no Visual Studio test adapter is required
- the tester works directly through the shell-based Python launcher

## Practical Test Use

Recommended workflow:

1. connect the board through `CN13`
2. wait until Windows enumerates the USB-CDC device
3. note the assigned COM port
4. run the Python system tests against that COM port

Example:

```powershell
python tests_python/run_pytest.py --port COM6 --suite all
```

## Common Symptoms

### No COM port appears

Check:

- `CN13` is the connected USB port
- the board is powered and running firmware with `ENABLE_SYSTEM_TEST_CHANNEL=ON`
- the USB cable supports data, not only charging

### COM port exists but the test cannot connect

Check:

- another tool is not still holding the same COM port
- the board-side firmware was rebuilt and flashed after USB-CDC changes
- the Python harness uses the correct COM number

### COM port disappears and reappears during reboot

This is expected during the software-reset startup test.

The Python harness therefore includes reconnect handling for the startup test path.

## Related Chapters

- [docs/09-system-testing/README.md](C:/st_apps/coffee_machine/docs/09-system-testing/README.md)
- [docs/04-drivers/uart-debug.md](C:/st_apps/coffee_machine/docs/04-drivers/uart-debug.md)
