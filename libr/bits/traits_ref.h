#pragma once

namespace std {

namespace __bits {

template<class T>
struct type_identity { using type = T; };

template<class T>
auto try_add_lvalue_reference(int) -> type_identity<T&>;
template<class T>
auto try_add_lvalue_reference(...) -> type_identity<T>;

template<class T>
auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template<class T>
auto try_add_rvalue_reference(...) -> type_identity<T>;

}

template<class T>
struct add_lvalue_reference : decltype(__bits::try_add_lvalue_reference<T>(0)) {};

template<class T>
struct add_rvalue_reference : decltype(__bits::try_add_rvalue_reference<T>(0)) {};

template<typename T>
struct remove_reference {
    using type = T;
};

template<typename T>
struct remove_reference<T &> {
    using type = T;
};

template<typename T>
struct remove_reference<T &&> {
    using type = T;
};

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T>
struct is_lvalue_reference {
    static constexpr bool value = false;
};

template<typename T>
struct is_lvalue_reference<T&> {
    static constexpr bool value = true;
};

}
