cmake_minimum_required(VERSION 3.10)
project(game_of_life VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(game_of_life_lib src/game_of_life.cpp)
add_executable(game-of-life src/main.cpp)

target_include_directories(game-of-life PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(game-of-life PRIVATE game_of_life_lib)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(game_of_life_test tests/game_of_life_test.cpp)
target_include_directories(game_of_life_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(game_of_life_test PRIVATE game_of_life_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(game_of_life_test)
