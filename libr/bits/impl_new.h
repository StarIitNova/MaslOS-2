#pragma once

#include <stddef.h>

#include <libr/std/exception>

#include <libm/heap/heap.h>

namespace std {
    class bad_alloc : public exception {
    public:
        const char *what() const override {
            return "std::bad_alloc";
        }
    };

    struct nothrow_t { explicit nothrow_t() = default; };
    static const std::nothrow_t nothrow;
}

void *operator new(size_t sz) {
    void *ptr = _Malloc(sz);
    if (!ptr)
        throw std::bad_alloc();
    return ptr;
}

void operator delete(void *ptr) noexcept {
    _Free(ptr);
}

void *operator new[](size_t sz) {
    void *ptr = _Malloc(sz);
    if (!ptr)
        throw std::bad_alloc();
    return ptr;
}

void operator delete[](void *ptr) {
    _Free(ptr);
}

void* operator new(size_t sz, const std::nothrow_t&) noexcept {
    return _Malloc(sz);
}

void* operator new[](size_t sz, const std::nothrow_t&) noexcept {
    return _Malloc(sz);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    _Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    _Free(ptr);
}
