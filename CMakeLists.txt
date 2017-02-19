cmake_minimum_required (VERSION 3.7)
project (NES_emulator)

include_directories ("${PROJECT_SOURCE_DIR}/include")

set (COMPILE_FLAGS "-Wall")
set (DEBUG_FLAGS "-DDEBUG")

set (SOURCES
      src/main.cpp
      src/interpreter.cpp
      src/cpu_ram.cpp
      src/system.cpp
      )

add_executable (emu ${SOURCES})
target_compile_options (emu PUBLIC ${COMPILE_FLAGS})
set_target_properties (emu PROPERTIES OUTPUT_NAME "NES_emulator")

add_executable (debug ${SOURCES})
target_compile_options (debug PUBLIC ${COMPILE_FLAGS} ${DEBUG_FLAGS})
set_target_properties (debug PROPERTIES OUTPUT_NAME "debug")

enable_testing ()
find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS})

set (TEST_SOURCES
      test/test_interpreter.cpp
      )

add_executable (test_interpreter ${TEST_SOURCES})
target_link_libraries (test_interpreter ${GTEST_BOTH_LIBRARIES})

add_test (tests
           test_interpreter
           )

add_custom_target (all_tests COMMAND ${CMAKE_CTEST_COMMAND}
                    DEPENDS test_interpreter
                    )

