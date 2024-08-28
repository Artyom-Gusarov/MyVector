cmake_minimum_required(VERSION 3.14)

project(MyVector)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.zip
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(
        benchmark-my-vector-2
        benchmark_vector.cpp
)
target_compile_definitions(benchmark-my-vector-2 PRIVATE TEST_MY_2)
target_link_libraries(benchmark-my-vector-2 benchmark::benchmark)

add_executable(
        benchmark-my-vector-1-5
        benchmark_vector.cpp
)
target_compile_definitions(benchmark-my-vector-1-5 PRIVATE TEST_MY_1_5)
target_link_libraries(benchmark-my-vector-1-5 benchmark::benchmark)

add_executable(
        benchmark-stl-vector
        benchmark_vector.cpp
)
target_compile_definitions(benchmark-stl-vector PRIVATE TEST_STL)
target_link_libraries(benchmark-stl-vector benchmark::benchmark)

add_executable(
        benchmark-deque-as-vector
        benchmark_vector.cpp
)
target_compile_definitions(benchmark-deque-as-vector PRIVATE TEST_DEQUE)
target_link_libraries(benchmark-deque-as-vector benchmark::benchmark)

# Custom target to run all benchmarks
add_custom_target(run-all-benchmarks
        DEPENDS benchmark-my-vector-2 benchmark-my-vector-1-5 benchmark-stl-vector benchmark-deque-as-vector
        COMMAND benchmark-my-vector-2 --benchmark_out=../BM_results/benchmark_my_vector_2.txt --benchmark_out_format=console
        COMMAND benchmark-my-vector-1-5 --benchmark_out=../BM_results/benchmark_my_vector_1_5.txt --benchmark_out_format=console
        COMMAND benchmark-stl-vector --benchmark_out=../BM_results/benchmark_stl_vector.txt --benchmark_out_format=console
        COMMAND benchmark-deque-as-vector --benchmark_out=../BM_results/benchmark_deque_as_vector.txt --benchmark_out_format=console
)