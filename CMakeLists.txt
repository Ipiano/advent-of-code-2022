cmake_minimum_required(VERSION 3.16)

project("Advent of Code, 2022")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(GTest REQUIRED)

file(
  GLOB SUB_PROJS CONFIGURE_DEPENDS
  LIST_DIRECTORIES true
  ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(PROJ IN LISTS SUB_PROJS)
  add_subdirectory(${PROJ})
endforeach()
