cmake_minimum_required(VERSION 3.20)

project(coffee_machine_tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(coffee_machine_unit_tests
    unit/countdown_formatter_tests.cpp
    unit/coffee_machine_simulation_tests.cpp
)

target_compile_features(coffee_machine_unit_tests PRIVATE cxx_std_17)

target_include_directories(coffee_machine_unit_tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../coffee_machine
)

target_sources(coffee_machine_unit_tests PRIVATE
    ../coffee_machine/countdown_formatter.cpp
    ../coffee_machine/coffee_machine_simulation.cpp
)

target_link_libraries(coffee_machine_unit_tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(coffee_machine_unit_tests)
