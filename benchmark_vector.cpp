#include <benchmark/benchmark.h>
#include <iostream>
#include <string>

#ifdef TEST_STL
#include <vector>
template <typename T>
using vector = std::vector<T>;
#else
#include "MyVector.hpp"
template <typename T>
using vector = MyVector::vector<T>;
#endif

namespace {

template <typename T = int, std::size_t iterations = 1000, std::size_t size = 100>
void push_back_BM(benchmark::State &state) {
    T obj;
    if constexpr (std::is_same_v<T, std::string>) {
        obj = std::string(size, 'a');
    } else if constexpr (std::is_same_v<T, int>) {
        obj = 12345;
    }

    for (auto _ : state) {
        vector<T> v;
        for (std::size_t i = 0; i < iterations; ++i) v.push_back(obj);
    }
}

}  // namespace

BENCHMARK(push_back_BM<int, 1000>);
BENCHMARK(push_back_BM<int, 100000>);
BENCHMARK(push_back_BM<std::string, 1000, 10>);
BENCHMARK(push_back_BM<std::string, 100000, 10>);
BENCHMARK(push_back_BM<std::string, 1000, 1000>);
BENCHMARK(push_back_BM<std::string, 100000, 1000>);

BENCHMARK_MAIN();