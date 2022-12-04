cmake_minimum_required(VERSION 3.16)

project("Advent of Code, 2022")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(GTest REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

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
  "${CMAKE_CURRENT_SOURCE_DIR}/*")

# Each subdirectory will be either the implementation of a day, or a separate
# proper CMake project. If there is a CMakeLists.txt, the dir is added with
# add_subdirectory, otherwise the following is expected:
#
# * Directory 'x' contains 'x.cpp'
# * 'x.cpp' contains at least the following two functions: 'x()', 'x_adv()'
# * 'x()', and 'x_adv()' both contain the following signature:
#   void(std::istream&, std::ostream&)
# * Any other source files in this directory will be linked with `x.cpp`
#
# Given a day implementation matching this, two executables will be created
# using main.cpp in this directory; one called 'x' and one called 'x_adv'; they
# will use the matching functions as the "processing function" and will set up
# the I/O streams and then run that function.
function(add_day DAY_DIR)
  get_filename_component(DIR_NAME "${PROJ}" NAME)
  file(GLOB DAY_FILES CONFIGURE_DEPENDS "${DAY_DIR}/*.cpp")

  message(STATUS "Adding Day: ${DAY_DIR} - ${DAY_FILES}")

  add_library("${DIR_NAME}_objs" OBJECT)
  target_compile_features("${DIR_NAME}_objs" PUBLIC cxx_std_14)
  target_sources("${DIR_NAME}_objs" PRIVATE "${DAY_FILES}")
  target_link_libraries("${DIR_NAME}_objs" PUBLIC GTest::gtest)

  target_link_libraries(advent_tests PRIVATE "${DIR_NAME}_objs")

  add_executable("${DIR_NAME}")
  target_link_libraries("${DIR_NAME}" PRIVATE "${DIR_NAME}_objs")
  target_sources("${DIR_NAME}" PRIVATE main.cpp)
  target_compile_definitions("${DIR_NAME}" PRIVATE "-DPROCESS_FN=${DIR_NAME}")

  add_executable("${DIR_NAME}_adv")
  target_link_libraries("${DIR_NAME}_adv" PRIVATE "${DIR_NAME}_objs")
  target_sources("${DIR_NAME}_adv" PRIVATE main.cpp)
  target_compile_definitions("${DIR_NAME}_adv"
                             PRIVATE "-DPROCESS_FN=${DIR_NAME}_adv")
endfunction()

foreach(PROJ IN LISTS SUB_PROJS)
  get_filename_component(DIR_NAME "${PROJ}" NAME)

  if(EXISTS "${PROJ}/CMakeLists.txt")
    add_subdirectory(${PROJ})
  elseif(EXISTS "${PROJ}/${DIR_NAME}.cpp")
    add_day("${PROJ}")
  endif()
endforeach()
