cmake_minimum_required (VERSION 3.7)
project (NES_emulator)

set (CC_OPTIONS -Wall -DLOG_LEVEL=3)

set (EMULATOR_TARGET "NES_emulator")
set (TEST_TARGET "tests")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(SFML_LIBS sfml-graphics sfml-window sfml-system)

file (GLOB_RECURSE SOURCES src/*.cpp)
list (REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable (${EMULATOR_TARGET} ${SOURCES} src/main.cpp)
target_compile_options (${EMULATOR_TARGET} PUBLIC ${CC_OPTIONS})
target_include_directories (${EMULATOR_TARGET} PUBLIC src/)
target_link_libraries(${EMULATOR_TARGET} ${SFML_LIBS})

find_package (GTest)

if (GTest_FOUND)
    # Test target
    file (GLOB_RECURSE TEST_SOURCES test/*.cpp)

    add_executable (${TEST_TARGET} ${SOURCES} ${TEST_SOURCES})
    target_compile_options (${TEST_TARGET} PUBLIC ${CC_OPTIONS})
    target_include_directories (${TEST_TARGET} PUBLIC src/ test/)

    target_link_libraries (${TEST_TARGET} ${GTEST_BOTH_LIBRARIES} ${SFML_LIBS})
endif()
