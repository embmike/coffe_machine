cmake_minimum_required(VERSION 3.15)

project(coffee_machine LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.project_importers.stm32.cubemx SOURCE_PROJECT coffee_machine.ioc)

add_bsp_based_executable(NAME coffee_machine
	GENERATE_BIN
	GENERATE_MAP)

