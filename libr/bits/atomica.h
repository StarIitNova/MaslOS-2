#pragma once

#include <libr/std/type_traits>

namespace std {

template<typename T>
class atomic {
    static_assert(std::is_integral<T>::value, "std::atomic only supports integral types in this implementation");

public:
    constexpr atomic() noexcept : mValue_{} {}
    constexpr atomic(T __desired) noexcept : mValue_(__desired) {}

    atomic(const atomic &) = delete;
    atomic &operator=(const atomic &) = delete;

    T load() const noexcept {
        // TODO: insert memory barrier as necessary (compiler/CPU fence)

        return mValue_; // WARN: unsafe
    }

    void store(T __desired) noexcept {
        // TODO: insert memory barrier as necessary

        mValue_ = __desired; // WARN: unsafe
    }

    T exchange(T __desired) noexcept {
        // TODO: implement memory atomic exchange in OS atomic primitive

        // WARN: unsafe
        T __old = mValue_;
        mValue_ = __desired;
        return __old;
    }

    bool compare_exchange_strong(T &__expected, T __desired) noexcept {
        // replace with OS primitive atomic_compare_exchange(&value, &expected, desired)

        // WARN: unsafe
        if (mValue_ == __expected) {
            mValue_ = __desired;
            return true;
        } else {
            __expected = mValue_;
            return false;
        }
    }

    T fetch_add(T __arg) noexcept {
        // TODO: implement primitive atomic_fetch_add(&value, arg)

        // WARN: unsafe
        T __old = mValue_;
        mValue_ += __arg;
        return __old;
    }

    T fetch_sub(T __arg) noexcept {
        // TODO: implement primitive atomic_fetch_sub(&value, arg)

        // WARN: unsafe
        T __old = mValue_;
        mValue_ -= __arg;
        return __old;
    }

    atomic<T> &operator=(const T &__v) {
        store(__v);
        return *this;
    }

    T operator++() noexcept {
        return fetch_add(1) + 1;
    }

    T operator++(int) noexcept {
        return fetch_add(1);
    }

    T operator--() noexcept {
        return fetch_sub(1) - 1;
    }

    T operator--(int) noexcept {
        return fetch_sub(1);
    }

    bool operator==(const T &__other) noexcept {
        return load() == __other;
    }

    bool operator!=(const T &__other) noexcept {
        return load() != __other;
    }

    bool operator>(const T &__other) noexcept {
        return load() > __other;
    }

    bool operator<(const T &__other) noexcept {
        return load() < __other;
    }

    bool operator>=(const T &__other) noexcept {
        return load() >= __other;
    }

    bool operator<=(const T &__other) noexcept {
        return load() <= __other;
    }

private:
    volatile T mValue_;
};

}
