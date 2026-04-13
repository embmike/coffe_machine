cmake_minimum_required(VERSION 3.20)

project(coffee_machine_tests LANGUAGES CXX)

include(FetchContent)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(coffee_machine_unittest
    unit/coffee_machine_app_tests.cpp
    unit/countdown_formatter_tests.cpp
    unit/coffee_machine_simulation_tests.cpp
    unit/model_tests.cpp
    unit/slection_screen_presenter_tests.cpp
    unit/brewing_screen_presenter_tests.cpp
    unit/splash_screen_presenter_tests.cpp
)

set_target_properties(coffee_machine_unittest PROPERTIES
    FOLDER "unit_test"
)

target_compile_features(coffee_machine_unittest PRIVATE cxx_std_17)

target_include_directories(coffee_machine_unittest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../coffee_machine
    ${CMAKE_CURRENT_LIST_DIR}/../Core/Inc
    ${CMAKE_CURRENT_LIST_DIR}/../TouchGFX/gui/include
    ${CMAKE_CURRENT_LIST_DIR}/../Middlewares/ST/touchgfx/framework/include
)

target_sources(coffee_machine_unittest PRIVATE
    ../coffee_machine/coffee_machine_app.cpp
    ../coffee_machine/countdown_formatter.cpp
    ../coffee_machine/coffee_machine_simulation.cpp
    ../TouchGFX/gui/src/model/Model.cpp
    ../TouchGFX/gui/src/slection_screen_screen/slection_screenPresenter.cpp
    ../TouchGFX/gui/src/brewing_screen_screen/brewing_screenPresenter.cpp
    ../TouchGFX/gui/src/splash_screen_screen/splash_screenPresenter.cpp
)

target_compile_definitions(coffee_machine_unittest PRIVATE
    UNIT_TEST
)

target_link_libraries(coffee_machine_unittest PRIVATE
    GTest::gtest_main
    GTest::gmock
)

foreach(google_test_target IN ITEMS gtest gtest_main gmock gmock_main)
    if(TARGET ${google_test_target})
        set_target_properties(${google_test_target} PROPERTIES
            FOLDER "third_party\\googletest"
        )
    endif()
endforeach()

include(GoogleTest)
gtest_discover_tests(
    coffee_machine_unittest
    DISCOVERY_MODE PRE_TEST
)
