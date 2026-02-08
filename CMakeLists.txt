cmake_minimum_required(VERSION 3.16)
project(if_changed VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -std=c++23
  -Wall
  -Wextra
  -Werror
  -pedantic
  -Wunreachable-code
  -Wconversion
  -Wshadow
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wnull-dereference
  -Wdouble-promotion
  -Wformat=2
)

# Build options
option(BUILD_TESTING "Build unit tests" ON)

# Main executable
add_executable(if_changed
  src/main.cpp
  src/lib.cpp
  src/io.cpp
  src/cli.cpp
)

target_include_directories(if_changed PRIVATE src)

# Testing
if(BUILD_TESTING)
  enable_testing()
  find_package(GTest REQUIRED)

  # Test executable
  add_executable(if_changed_tests
    tests/lib_test.cpp
    src/lib.cpp
    src/io.cpp
  )

  target_include_directories(if_changed_tests PRIVATE src)
  target_link_libraries(if_changed_tests PRIVATE GTest::gtest GTest::gtest_main)

  add_test(NAME if_changed_tests COMMAND if_changed_tests)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()
