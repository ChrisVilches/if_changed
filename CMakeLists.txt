cmake_minimum_required(VERSION 3.20)
project(if_changed VERSION 1.0.0 LANGUAGES CXX)

# -----------------------------
# Project-wide build options
# -----------------------------
include(CTest) # defines BUILD_TESTING

# Interface target for shared build settings (modern CMake pattern)
add_library(project_options INTERFACE)

# Require C++23
target_compile_features(project_options INTERFACE cxx_std_23)

# Warnings (compiler-aware)
target_compile_options(project_options INTERFACE
  $<$<CXX_COMPILER_ID:GNU,Clang>:
    -Wall -Wextra -Werror -Wpedantic
    -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast
    -Wcast-align -Wunused -Woverloaded-virtual -Wnull-dereference
    -Wdouble-promotion -Wformat=2
  >
  $<$<CXX_COMPILER_ID:MSVC>:
    /W4 /permissive-
  >
)

# Sanitizers in Debug
target_compile_options(project_options INTERFACE
  $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
target_link_options(project_options INTERFACE
  $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# -----------------------------
# Core library
# -----------------------------
add_library(if_changed_lib STATIC
  src/lib.cpp
  src/io.cpp
  src/cli.cpp
)

target_include_directories(if_changed_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(if_changed_lib PRIVATE project_options)

# -----------------------------
# Main executable
# -----------------------------
add_executable(if_changed
  src/main.cpp
)

target_link_libraries(if_changed PRIVATE if_changed_lib project_options)

# -----------------------------
# Tests
# -----------------------------
if(BUILD_TESTING)
  find_package(GTest REQUIRED)

  add_executable(if_changed_tests
    tests/lib_test.cpp
  )

  target_link_libraries(if_changed_tests
    PRIVATE
      if_changed_lib
      project_options
      GTest::gtest
      GTest::gtest_main
  )

  add_test(NAME if_changed_tests COMMAND if_changed_tests)
endif()
