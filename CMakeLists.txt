cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MyVector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Fetch GoogleBenchmark
FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.zip
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(
        benchmark-my-vector
        benchmark_vector.cpp
)
target_link_libraries(benchmark-my-vector benchmark::benchmark)
add_custom_command(TARGET benchmark-my-vector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f ../benchmark_my_vector.txt
        COMMAND benchmark-my-vector --benchmark_out=../benchmark_my_vector.txt --benchmark_out_format=console
)

add_executable(
        benchmark-stl-vector
        benchmark_vector.cpp
)
target_compile_definitions(benchmark-stl-vector PRIVATE TEST_STL)
target_link_libraries(benchmark-stl-vector benchmark::benchmark)
add_custom_command(TARGET benchmark-stl-vector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f ../benchmark_stl_vector.txt
        COMMAND benchmark-stl-vector --benchmark_out=../benchmark_stl_vector.txt --benchmark_out_format=console
)