cmake_minimum_required(VERSION 3.15)

project(coffee_machine LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.project_importers.stm32.cubemx
	SOURCE_PROJECT coffee_machine.ioc)

add_bsp_based_executable(
	NAME coffee_machine
	GENERATE_MAP)

set(TOUCHGFX_ROOT ${CMAKE_CURRENT_LIST_DIR}/Middlewares/ST/touchgfx)
set(TOUCHGFX_LIB ${TOUCHGFX_ROOT}/lib/core/cortex_m7/gcc/libtouchgfx-float-abi-hard.a)
set(TARGET_HEX ${CMAKE_CURRENT_BINARY_DIR}/target.hex)
set(APP_TARGET_HEX ${CMAKE_CURRENT_BINARY_DIR}/app_target.hex)
set(INTFLASH_HEX ${CMAKE_CURRENT_BINARY_DIR}/intflash.hex)
set(QSPI_EXTERNAL_LOADER MT25TL01G_STM32H750B-DISCO.stldr)
set(EXTMEM_BOOTLOADER_ELF ${CMAKE_CURRENT_BINARY_DIR}/extmem_bootloader)
set(BOOT_HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/boot.hex)
set(BOOT_BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/boot.bin)

set(BSP_ROOT ${CMAKE_CURRENT_LIST_DIR}/Drivers/BSP)
set(BSP_BOARD_ROOT ${BSP_ROOT}/STM32H750B-DK)
set(BSP_COMMON_ROOT ${BSP_ROOT}/Components/Common)
set(BSP_FT5336_ROOT ${BSP_ROOT}/Components/ft5336)
set(BSP_MT25_ROOT ${BSP_ROOT}/Components/mt25tl01g)
set(BSP_MT48_ROOT ${BSP_ROOT}/Components/mt48lc4m32b2)

file(GLOB_RECURSE TOUCHGFX_APP_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/gui/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/fonts/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/gui_generated/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/images/src/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/texts/src/*.cpp)

set(BSP_DRIVER_SOURCES
	${BSP_BOARD_ROOT}/stm32h750b_discovery_bus.c
	${BSP_BOARD_ROOT}/stm32h750b_discovery_qspi.c
	${BSP_BOARD_ROOT}/stm32h750b_discovery_sdram.c
	${BSP_BOARD_ROOT}/stm32h750b_discovery_ts.c
	${BSP_FT5336_ROOT}/ft5336.c
	${BSP_FT5336_ROOT}/ft5336_reg.c
	${BSP_MT25_ROOT}/mt25tl01g.c
	${BSP_MT48_ROOT}/mt48lc4m32b2.c)

set(APP_SUPPORT_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_app.cpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_board.cpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_simulation.cpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/countdown_formatter.cpp)

set(APP_SUPPORT_HEADERS
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_app.hpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_board.hpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/coffee_machine_simulation.hpp
	${CMAKE_CURRENT_LIST_DIR}/coffee_machine/countdown_formatter.hpp)

set(EXTMEM_BOOT_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/fmc.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/main.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/memory_msp.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/qspi.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/stm32h7xx_it.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Src/system_stm32h7xx.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
	${BSP_MT25_ROOT}/mt25tl01g.c
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/gcc/startup_stm32h750xx.s)

if(TARGET BSP)
	target_include_directories(BSP PRIVATE
		${TOUCHGFX_ROOT}/framework/include
		${CMAKE_CURRENT_LIST_DIR}/TouchGFX/gui/include
		${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/fonts/include
		${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/gui_generated/include
		${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/images/include
		${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/texts/include
		${BSP_BOARD_ROOT}
		${BSP_COMMON_ROOT}
		${BSP_FT5336_ROOT}
		${BSP_MT25_ROOT}
		${BSP_MT48_ROOT}
		${CMAKE_CURRENT_LIST_DIR}/Core/Inc
		${CMAKE_CURRENT_LIST_DIR}/coffee_machine)

	target_sources(BSP PRIVATE
		${TOUCHGFX_APP_SOURCES}
		${BSP_DRIVER_SOURCES}
		${APP_SUPPORT_SOURCES}
		${APP_SUPPORT_HEADERS})

	get_target_property(BSP_SOURCES BSP SOURCES)
	if(BSP_SOURCES)
		list(FILTER BSP_SOURCES EXCLUDE REGEX ".*/TouchGFX/generated/simulator/.*")
		list(FILTER BSP_SOURCES EXCLUDE REGEX ".*/TouchGFX/simulator/.*")
		set_property(TARGET BSP PROPERTY SOURCES "${BSP_SOURCES}")
	endif()

	source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/coffee_machine
		PREFIX "Header Files\\coffee_machine"
		FILES ${APP_SUPPORT_HEADERS})
	source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/coffee_machine
		PREFIX "Source Files\\coffee_machine"
		FILES ${APP_SUPPORT_SOURCES})
endif()

target_link_libraries(coffee_machine PRIVATE
	${TOUCHGFX_LIB})

add_executable(extmem_bootloader ${EXTMEM_BOOT_SOURCES})
target_include_directories(extmem_bootloader PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/Inc
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
	${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
	${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
	${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/Include
	${BSP_MT25_ROOT}
	${CMAKE_CURRENT_LIST_DIR}/Drivers/BSP/Components)
target_compile_definitions(extmem_bootloader PRIVATE
	STM32H750xx
	USE_HAL_DRIVER)
target_compile_options(extmem_bootloader PRIVATE
	-mcpu=cortex-m7
	-mfpu=fpv5-sp-d16
	-mfloat-abi=softfp
	-mthumb
	-fno-exceptions
	-ffunction-sections
	-fdata-sections
	-Wno-psabi)
target_link_options(extmem_bootloader PRIVATE
	-T${CMAKE_CURRENT_LIST_DIR}/ExtMem_Boot/gcc/STM32H750XBHx_FLASH.ld
	-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/extmem_bootloader.map
	-Wl,--gc-sections
	-mcpu=cortex-m7
	-mfpu=fpv5-sp-d16
	-mfloat-abi=softfp
	-mthumb)
set_target_properties(extmem_bootloader PROPERTIES
	OUTPUT_NAME extmem_bootloader)

if(NOT CMAKE_OBJCOPY)
	find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
endif()

find_program(STM32_PROGRAMMER_CLI
	NAMES STM32_Programmer_CLI.exe STM32_Programmer_CLI
	PATHS
		"C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
		"C:/Program Files (x86)/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
		"C:/ST/STM32CubeCLT/STM32CubeProgrammer/bin")

if(STM32_PROGRAMMER_CLI)
	get_filename_component(STM32_PROGRAMMER_DIR "${STM32_PROGRAMMER_CLI}" DIRECTORY)
	set(STM32_EXTERNAL_LOADER "${STM32_PROGRAMMER_DIR}/ExternalLoader/${QSPI_EXTERNAL_LOADER}")
endif()

if(CMAKE_OBJCOPY)
	add_custom_command(
		OUTPUT ${TARGET_HEX}
		COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:coffee_machine> ${TARGET_HEX}
		DEPENDS coffee_machine
		COMMENT "Generating combined target.hex for internal bootloader and QSPI application"
		VERBATIM)

	add_custom_command(
		OUTPUT ${APP_TARGET_HEX}
		COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:coffee_machine> ${APP_TARGET_HEX}
		DEPENDS coffee_machine
		COMMENT "Generating app_target.hex for QSPI XIP application"
		VERBATIM)

	add_custom_command(
		OUTPUT ${INTFLASH_HEX}
		COMMAND ${CMAKE_OBJCOPY} -O ihex --remove-section=ExtFlashSection --remove-section=FontFlashSection --remove-section=TextFlashSection $<TARGET_FILE:coffee_machine> ${INTFLASH_HEX}
		DEPENDS coffee_machine
		COMMENT "Generating intflash.hex"
		VERBATIM)

	add_custom_target(artifact_system_hex DEPENDS ${TARGET_HEX})
	add_custom_target(artifact_app_hex DEPENDS ${APP_TARGET_HEX})
	add_custom_target(artifact_intflash_hex DEPENDS ${INTFLASH_HEX})

	add_custom_command(
		OUTPUT ${BOOT_HEX_FILE}
		COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:extmem_bootloader> ${BOOT_HEX_FILE}
		DEPENDS extmem_bootloader
		COMMENT "Generating boot.hex"
		VERBATIM)

	add_custom_command(
		OUTPUT ${BOOT_BIN_FILE}
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:extmem_bootloader> ${BOOT_BIN_FILE}
		DEPENDS extmem_bootloader
		COMMENT "Generating boot.bin"
		VERBATIM)

	add_custom_target(artifact_boot_hex DEPENDS ${BOOT_HEX_FILE})
	add_custom_target(artifact_boot_bin DEPENDS ${BOOT_BIN_FILE})
endif()

if(STM32_PROGRAMMER_CLI AND EXISTS "${STM32_EXTERNAL_LOADER}" AND CMAKE_OBJCOPY)
	add_custom_target(flash_system
		COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -d ${BOOT_HEX_FILE}
		COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -d ${APP_TARGET_HEX} -el ${STM32_EXTERNAL_LOADER} -hardRst
		DEPENDS artifact_boot_hex artifact_app_hex
		COMMENT "Flashing boot.hex to internal flash and app.hex to QSPI"
		VERBATIM)

	add_custom_target(flash_app
		COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -d ${APP_TARGET_HEX} -el ${STM32_EXTERNAL_LOADER} -hardRst
		DEPENDS artifact_app_hex
		COMMENT "Flashing only app.hex to QSPI for debugger-driven app sessions"
		VERBATIM)
endif()

if(STM32_PROGRAMMER_CLI AND CMAKE_OBJCOPY)
	add_custom_target(flash_bootloader
		COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -d ${BOOT_HEX_FILE} -hardRst
		DEPENDS artifact_boot_hex
		COMMENT "Flashing boot.hex to internal flash"
		VERBATIM)
endif()

set_source_files_properties(
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/include/simulator/mainBase.hpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/include/simulator/video/SoftwareMJPEGDecoder.hpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/include/simulator/video/MJPEGDecoder.hpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/include/simulator/video/DirectFrameBufferVideoController.hpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/src/mainBase.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/generated/simulator/src/video/SoftwareMJPEGDecoder.cpp
	${CMAKE_CURRENT_LIST_DIR}/TouchGFX/simulator/main.cpp
	PROPERTIES HEADER_FILE_ONLY TRUE)
