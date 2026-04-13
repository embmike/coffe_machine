cmake_minimum_required(VERSION 3.20)

project(coffee_machine_tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(coffee_machine_unit_tests
    unit/countdown_formatter_tests.cpp
    unit/coffee_machine_simulation_tests.cpp
    unit/model_tests.cpp
    unit/slection_screen_presenter_tests.cpp
    unit/brewing_screen_presenter_tests.cpp
    unit/splash_screen_presenter_tests.cpp
)

target_compile_features(coffee_machine_unit_tests PRIVATE cxx_std_17)

target_include_directories(coffee_machine_unit_tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../coffee_machine
    ${CMAKE_CURRENT_LIST_DIR}/../Core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/../TouchGFX/gui/include
    ${CMAKE_CURRENT_LIST_DIR}/../Middlewares/ST/touchgfx/framework/include
)

target_sources(coffee_machine_unit_tests PRIVATE
    ../coffee_machine/countdown_formatter.cpp
    ../coffee_machine/coffee_machine_simulation.cpp
    ../TouchGFX/gui/src/model/Model.cpp
    ../TouchGFX/gui/src/slection_screen_screen/slection_screenPresenter.cpp
    ../TouchGFX/gui/src/brewing_screen_screen/brewing_screenPresenter.cpp
    ../TouchGFX/gui/src/splash_screen_screen/splash_screenPresenter.cpp
)

target_compile_definitions(coffee_machine_unit_tests PRIVATE
    UNIT_TEST
)

target_link_libraries(coffee_machine_unit_tests PRIVATE
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(coffee_machine_unit_tests)
