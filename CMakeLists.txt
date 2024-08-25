cmake_minimum_required (VERSION 3.7)
project (NES_emulator)

set (CC_OPTIONS -Wall -DLOG_LEVEL=3)
set (DEBUG_FLAGS "-DDEBUG")

set (EMULATOR_TARGET "NES_emulator")
set (TEST_TARGET "tests")

file (GLOB_RECURSE SOURCES src/*.cpp)

add_executable (${EMULATOR_TARGET} ${SOURCES})
target_compile_options (${EMULATOR_TARGET} PUBLIC ${CC_OPTIONS})
target_include_directories (${EMULATOR_TARGET} PUBLIC src/)

add_executable (debug ${SOURCES})
target_compile_options (debug PUBLIC ${COMPILE_FLAGS} ${DEBUG_FLAGS})
set_target_properties (debug PROPERTIES OUTPUT_NAME "debug")

find_package (GTest)

if (GTest_FOUND)
    # Test target
    file (GLOB_RECURSE TEST_SOURCES test/*.cpp)

    add_executable (${TEST_TARGET} ${TEST_SOURCES})
    target_compile_options (${TEST_TARGET} PUBLIC ${CC_OPTIONS})
    target_include_directories (${TEST_TARGET} PUBLIC src/ test/)

    target_link_libraries (${TEST_TARGET} ${GTEST_BOTH_LIBRARIES})
endif()
