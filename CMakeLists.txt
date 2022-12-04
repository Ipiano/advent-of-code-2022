cmake_minimum_required(VERSION 3.16)

project("Advent of Code, 2022")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(GTest REQUIRED)

# Sub-directories can add tests to this exectuable as-needed, and it will be the
# main method of developing each day; files can use the ADVENT_TESTS macro to
add_executable(advent_tests)
target_link_libraries(advent_tests PRIVATE GTest::gtest_main)
target_sources(advent_tests PRIVATE dummy.cpp)

# Run the tests after every build, for convenience
add_custom_target(
  check ALL
  COMMAND advent_tests
  USES_TERMINAL)

file(
  GLOB SUB_PROJS CONFIGURE_DEPENDS
  LIST_DIRECTORIES true
  ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(PROJ IN LISTS SUB_PROJS)
  add_subdirectory(${PROJ})
endforeach()
