#pragma once

#include <libr/std/utility>

namespace std {

template<class T> struct remove_cv { typedef T type; };
template<class T> struct remove_cv<const T> { typedef T type; };
template<class T> struct remove_cv<volatile T> { typedef T type; };
template<class T> struct remove_cv<const volatile T> { typedef T type; };

template<class T> struct remove_const { typedef T type; };
template<class T> struct remove_const<const T> { typedef T type; };

template<class T> struct remove_volatile { typedef T type; };
template<class T> struct remove_volatile<volatile T> { typedef T type; };

template<class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template<class T, class U>
struct is_same : std::false_type {};

template<class T>
struct is_same<T, T> : std::true_type {};

template<class T>
struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

namespace __bits {
    template<class T>
    auto test_returnable(int) -> decltype(void(static_cast<T(*)()>(nullptr)), std::true_type{});
    template<class>
    auto test_returnable(...) -> std::false_type;
    template<class F, class T>
    auto test_implicitly_convertible(int) -> decltype(void(std::declval<void(&)(T)>()(std::declval<F>())), std::true_type{});
    template<class, class>
    auto test_implicitly_convertible(...) -> std::false_type;
}

template<class F, class T>
struct is_convertible : std::integral_constant<bool,
    (decltype(__bits::test_returnable<T>(0))::value &&
     decltype(__bits::test_implicitly_convertible<F, T>(0))::value) ||
    (std::is_void<F>::value && std::is_void<T>::value)
> {};

template<typename T>
struct is_integral {
    static constexpr bool value = false;
};

template<> struct is_integral<bool>               { static constexpr bool value = true; };
template<> struct is_integral<char>               { static constexpr bool value = true; };
template<> struct is_integral<signed char>        { static constexpr bool value = true; };
template<> struct is_integral<unsigned char>      { static constexpr bool value = true; };
template<> struct is_integral<wchar_t>            { static constexpr bool value = true; };
template<> struct is_integral<char16_t>           { static constexpr bool value = true; };
template<> struct is_integral<char32_t>           { static constexpr bool value = true; };
template<> struct is_integral<short>              { static constexpr bool value = true; };
template<> struct is_integral<unsigned short>     { static constexpr bool value = true; };
template<> struct is_integral<int>                { static constexpr bool value = true; };
template<> struct is_integral<unsigned int>       { static constexpr bool value = true; };
template<> struct is_integral<long>               { static constexpr bool value = true; };
template<> struct is_integral<unsigned long>      { static constexpr bool value = true; };
template<> struct is_integral<long long>          { static constexpr bool value = true; };
template<> struct is_integral<unsigned long long> { static constexpr bool value = true; };

#if __cplusplus < 201103L // Pre-C++11 to ensure static_assert exists

template<bool>
struct StaticAssert;

template<>
struct StaticAssert<true> {};

#define static_assert(cond, msg) \
    typedef StaticAssert<(cond)> static_assertion_##msg##_failed

#endif

}
