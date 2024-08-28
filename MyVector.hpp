#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace MyVector {
template <typename T, std::size_t capacity_step = 20, typename Alloc = std::allocator<T>>
class vector {
private:
    std::size_t v_capacity;
    T *data;
    std::size_t v_size;

    void check_out_of_bounds(std::size_t i) const {
        if (i >= v_size) {
            std::string what_msg = "requested element: " + std::to_string(i) +
                                   ", vector size: " + std::to_string(v_size) +
                                   "\n";
            throw std::out_of_range(what_msg);
        }
    }

    void clear_dealloc_data() noexcept {
        clear();
        if (v_capacity > 0) {
            Alloc().deallocate(data, v_capacity);
        }
    }

    [[nodiscard]] static T *safe_allocate(std::size_t n) {
        if (n == 0) {
            return nullptr;
        } else {
            return Alloc().allocate(n);
        }
    }

    [[nodiscard]] static std::size_t calc_capacity(std::size_t n) noexcept {
        if (n == 0) {
            return 0;
        }
        std::size_t new_capacity = 1;
        while (n > new_capacity) {
            new_capacity *= capacity_step;
            new_capacity /= 10;
            ++new_capacity;
        }
        return new_capacity;
    }

    void reserve_empty(std::size_t k) & {
        if (v_capacity >= k) {
            clear();
            return;
        }

        std::size_t new_capacity = calc_capacity(k);
        T *new_data = safe_allocate(new_capacity);

        clear_dealloc_data();
        v_size = 0;
        v_capacity = new_capacity;
        data = new_data;
    }

    template <typename TPtr>
    TPtr save_reserve(std::size_t k, TPtr save_ptr) & {
        // reserve and save ptr to object valid in case it point to object in
        // old data
        if (v_capacity >= k) {
            return save_ptr;
        }

        std::size_t new_capacity = calc_capacity(k);
        T *new_data = safe_allocate(new_capacity);
        std::size_t new_size = 0;

        for (; new_size < v_size; ++new_size) {
            if (data + new_size ==
                save_ptr) {  // check if ptr point to old data
                save_ptr = new_data + new_size;
            }
            new (new_data + new_size) T(std::move(data[new_size]));
        }
        clear_dealloc_data();
        v_size = new_size;
        v_capacity = new_capacity;
        data = new_data;
        return save_ptr;
    }

public:
    vector() noexcept : v_capacity(0), data(nullptr), v_size(0) {
    }

    explicit vector(std::size_t n)
        : v_capacity(calc_capacity(n)),
          data(safe_allocate(v_capacity)),
          v_size(0) {
        for (; v_size < n; ++v_size) {
            new (data + v_size) T();
        }
    }

    vector(std::size_t n, const T &t)
        : v_capacity(calc_capacity(n)),
          data(safe_allocate(v_capacity)),
          v_size(0) {
        for (; v_size < n; ++v_size) {
            new (data + v_size) T(t);
        }
    }

    vector(std::size_t n, T &&t)
        : v_capacity(calc_capacity(n)),
          data(safe_allocate(v_capacity)),
          v_size(0) {
        if (n == 0) {
            return;
        }
        for (; v_size < n - 1; ++v_size) {
            new (data + v_size) T(t);
        }
        new (data + v_size++) T(std::move(t));
    }

    vector(vector &&other) noexcept
        : v_capacity(std::exchange(other.v_capacity, 0)),
          data(std::exchange(other.data, nullptr)),
          v_size(std::exchange(other.v_size, 0)) {
    }

    vector(const vector &other)
        : v_capacity(calc_capacity(other.v_size)),
          data(safe_allocate(v_capacity)),
          v_size(0) {
        for (; v_size < other.v_size; ++v_size) {
            new (data + v_size) T(other[v_size]);
        }
    }

    vector &operator=(vector &&other) noexcept {
        clear();
        std::swap(v_capacity, other.v_capacity);
        std::swap(data, other.data);
        std::swap(v_size, other.v_size);
        return *this;
    }

    vector &operator=(const vector &other) {
        if (this == &other) {
            return *this;
        }

        if (other.v_size <= v_size) {
            while (other.v_size < v_size) {
                pop_back();
            }
            for (std::size_t i = 0; i < v_size; ++i) {
                data[i] = other[i];
            }
        } else if (other.v_size > v_capacity) {
            reserve_empty(other.v_size);
            for (v_size = 0; v_size < other.v_size; ++v_size) {
                new (data + v_size) T(other[v_size]);
            }
        } else {
            for (std::size_t i = 0; i < v_size; ++i) {
                data[i] = other[i];
            }
            for (; v_size < other.v_size; ++v_size) {
                new (data + v_size) T(other[v_size]);
            }
        }

        return *this;
    }

    T &operator[](std::size_t i) & noexcept {
        return data[i];
    }

    [[nodiscard]] const T &operator[](std::size_t i) const & noexcept {
        return data[i];
    }

    [[nodiscard]] T &&operator[](std::size_t i) && noexcept {
        return std::move(data[i]);
    }

    T &at(std::size_t i) & {
        check_out_of_bounds(i);
        return data[i];
    }

    [[nodiscard]] const T &at(std::size_t i) const & {
        check_out_of_bounds(i);
        return data[i];
    }

    [[nodiscard]] T &&at(std::size_t i) && {
        check_out_of_bounds(i);
        return std::move(data[i]);
    }

    void pop_back() & noexcept {
        data[--v_size].~T();
    }

    void reserve(std::size_t k) & {
        if (v_capacity >= k) {
            return;
        }

        std::size_t new_capacity = calc_capacity(k);
        T *new_data = safe_allocate(new_capacity);
        std::size_t new_size = 0;

        for (; new_size < v_size; ++new_size) {
            new (new_data + new_size) T(std::move(data[new_size]));
        }
        clear_dealloc_data();
        v_size = new_size;
        v_capacity = new_capacity;
        data = new_data;
    }

    void push_back(const T &t) & {
        if (v_size < v_capacity) {
            new (data + v_size++) T(t);
        } else {
            const T *ptr_t = save_reserve(v_size + 1, &t);
            new (data + v_size++) T(*ptr_t);
        }
    }

    void push_back(T &&t) & {
        if (v_size < v_capacity) {
            new (data + v_size++) T(std::move(t));
        } else {
            T *ptr_t = save_reserve(v_size + 1, &t);
            new (data + v_size++) T(std::move(*ptr_t));
        }
    }

    void resize(std::size_t k, const T &t) & {
        if (k <= v_size) {
            while (k < v_size) {
                pop_back();
            }
            return;
        }

        if (k > v_capacity) {
            const T *ptr_t = save_reserve(k, &t);
            while (k > v_size) {
                push_back(*ptr_t);
            }
        } else {
            while (k > v_size) {
                push_back(t);
            }
        }
    }

    void resize(std::size_t k, T &&t) & {
        if (k <= v_size) {
            while (k < v_size) {
                pop_back();
            }
            return;
        }

        if (k > v_capacity) {
            T *ptr_t = save_reserve(k, &t);
            while (k > v_size + 1) {
                push_back(*ptr_t);
            }
            push_back(std::move(*ptr_t));
        } else {
            while (k > v_size + 1) {
                push_back(t);
            }
            push_back(std::move(t));
        }
    }

    void resize(std::size_t k) & {
        if (k <= v_size) {
            while (k < v_size) {
                pop_back();
            }
            return;
        }
        reserve(k);
        while (k > v_size) {
            push_back(T());
        }
    }

    void clear() & noexcept {
        for (std::size_t i = 0; i < v_size; i++) {
            data[i].~T();
        }
        v_size = 0;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return v_size;
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return v_capacity;
    }

    [[nodiscard]] bool empty() const noexcept {
        return v_size == 0;
    }

    ~vector() {
        clear_dealloc_data();
    }
};
}  // namespace vector_naive

#endif