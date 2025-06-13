#pragma once

#include <stddef.h>

#include <libr/std/new>
#include <libr/std/utility>

namespace std {

template<typename T>
struct allocator {
    using value_type = T;

    allocator() noexcept = default;

    T *allocate(size_t __n) {
        if (__n > max_size()) throw std::bad_alloc();
        return static_cast<T*>(::operator new(__n * sizeof(T)));
    }

    void deallocate(T *__p, size_t) noexcept {
        ::operator delete(__p);
    }

    size_t max_size() const noexcept {
        return ((size_t)(~0) / sizeof(T));
    }

    template<typename U, typename ...Args>
    void construct(U *__p, Args &&...__args) {
        ::new ((void *)__p) U(std::forward<Args>(__args)...);
    }

    template<typename U>
    void destroy(U *__p) {
        __p->~U();
    }
};

}
