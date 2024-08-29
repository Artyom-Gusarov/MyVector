#include <benchmark/benchmark.h>
#include <string>

#ifdef TEST_STL
#include <vector>
template <typename T>
using vector = std::vector<T>;
#endif
#ifdef TEST_MY_2
#include "MyVector.hpp"
template <typename T>
using vector = MyVector::vector<T, 20>;
#endif
#ifdef TEST_MY_5
#include "MyVector.hpp"
template <typename T>
using vector = MyVector::vector<T, 50>;
#endif
#ifdef TEST_DEQUE
#include "deque"
template <typename T>
using vector = std::deque<T>;
#endif

namespace {

class NonTriviallyCopyableInt {
public:
    NonTriviallyCopyableInt(int value = 0) : value_(value) {
    }

    NonTriviallyCopyableInt(const NonTriviallyCopyableInt &other)
        : value_(other.value_) {
    }

    NonTriviallyCopyableInt &operator=(const NonTriviallyCopyableInt &other) {
        if (this != &other) {
            value_ = other.value_;
        }
        return *this;
    }

    ~NonTriviallyCopyableInt() {
    }

    int getValue() const {
        return value_;
    }

private:
    int value_;
};

template <
    typename T = int,
    std::size_t iterations = 1000,
    std::size_t size = 100>
void push_back_BM(benchmark::State &state) {
    T obj;
    if constexpr (std::is_same_v<T, std::string>) {
        obj = std::string(size, 'a');
    } else if constexpr (std::is_same_v<T, int>) {
        obj = 12345;
    }

    for (auto _ : state) {
        vector<T> v;
        for (std::size_t i = 0; i < iterations; ++i) {
            v.push_back(obj);
        }
        benchmark::DoNotOptimize(v);
    }
}

template <std::size_t size = 1000>
void access_BM(benchmark::State &state) {
    vector<int> v(size, 12345);

    for (auto _ : state) {
        for (std::size_t i = 0; i < size; ++i) {
            benchmark::DoNotOptimize(v[i]);
        }
    }
}

}  // namespace

BENCHMARK(push_back_BM<int, 1000>);
BENCHMARK(push_back_BM<NonTriviallyCopyableInt, 1000>);
BENCHMARK(push_back_BM<int, 100000>);
BENCHMARK(push_back_BM<NonTriviallyCopyableInt, 100000>);
BENCHMARK(push_back_BM<std::string, 1000, 10>);
BENCHMARK(push_back_BM<std::string, 100000, 10>);
BENCHMARK(push_back_BM<std::string, 1000, 1000>);
BENCHMARK(push_back_BM<std::string, 100000, 1000>);
BENCHMARK(access_BM<1000>);
BENCHMARK(access_BM<100000>);
BENCHMARK(access_BM<1000000>);

BENCHMARK_MAIN();