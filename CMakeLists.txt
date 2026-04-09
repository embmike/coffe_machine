cmake_minimum_required(VERSION 3.15)

project(coffee_machine LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.project_importers.stm32.cubemx SOURCE_PROJECT coffee_machine.ioc)

add_bsp_based_executable(NAME coffee_machine
	GENERATE_BIN
	GENERATE_MAP)

target_sources(BSP PRIVATE
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
	Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c)

