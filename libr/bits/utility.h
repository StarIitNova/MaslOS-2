#pragma once

#include <libr/bits/traits_ref.h>

namespace std {

template<typename T>
constexpr typename std::remove_reference<T>::type &&move(T &&arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
}

template<typename T>
T &&forward(std::remove_reference_t<T> &__fd) noexcept {
    return static_cast<T &&>(__fd);
}

template<typename T>
T &&forward(std::remove_reference_t<T> &&__fd) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "Bad forward call: T is lvalue reference but __fd is rvalue");
    return static_cast<T &&>(__fd);
}

template<typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept {
    static_assert(false, "declval not allowed in an evaluated context");
}

}
