/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_TYPE_TRAITS_HPP
#define NEO_TYPE_TRAITS_HPP

#include <cstddef>
#include <limits>
#include <type_traits>

namespace neo
{

template<typename T, typename U>
struct is_same
{
    static constexpr bool value = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value;
};

template<typename T>
struct is_numeric
{
    static constexpr bool value = std::is_arithmetic<T>::value & !is_same<T, bool>::value;
};

template<typename T>
struct is_unsigned_integral
{
    static constexpr bool value = std::is_integral<T>::value && !std::is_signed<T>::value;
};

template<typename, typename, typename = void>
struct are_similar
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct are_similar<T1, T2, std::enable_if_t<
    std::is_same<std::remove_cv_t<T1>, std::remove_cv_t<T2>>::value || (
        is_numeric<T1>::value == is_numeric<T2>::value &&
        std::is_integral<T1>::value == std::is_integral<T2>::value &&
        std::is_signed<T1>::value == std::is_signed<T2>::value &&
        std::is_floating_point<T1>::value == std::is_floating_point<T2>::value
    )>>
{
    static constexpr bool value = true;
};

template<typename T1, typename T2>
struct are_similar_numeric
{
    static constexpr bool value = is_numeric<T1>::value && are_similar<T1, T2>::value;
};

template<typename T1, typename T2>
struct are_similar_unsigned_integral
{
    static constexpr bool value = is_unsigned_integral<T1>::value && are_similar<T1, T2>::value;
};

// XXX VC++ 2015 doesn't like this as the constexpr.
template<typename T, typename U, bool =
    std::numeric_limits<T>::lowest() >= std::numeric_limits<U>::lowest() &&
    std::numeric_limits<T>::max() <= std::numeric_limits<U>::max()
>
struct is_bounded
{
    static constexpr bool value = false;
};

template<typename T, typename U>
struct is_bounded<T, U, true>
{
    static constexpr bool value = true;
};

template<typename From, typename To, bool = is_numeric<From>::value && are_similar<From, To>::value>
struct is_widening
{
    static constexpr bool value = false;
};

template<typename From, typename To>
struct is_widening<From, To, true>
{
    static constexpr bool value = is_bounded<From, To>::value;
};

template<typename From, typename To, bool = is_numeric<From>::value && are_similar<From, To>::value>
struct is_narrowing
{
    static constexpr bool value = is_numeric<To>::value;
};

template<typename From, typename To>
struct is_narrowing<From, To, true>
{
    static constexpr bool value = !is_bounded<From, To>::value;
};

template<typename T1, typename T2>
struct are_widening
{
    static constexpr bool value = is_widening<T1, T2>::value || is_widening<T2, T1>::value;
};

template<typename T1, typename T2, bool = is_widening<T1, T2>::value>
struct wider
{
    using type = T1;
};

template<typename T1, typename T2>
struct wider<T1, T2, true>
{
    using type = T2;
};

template<typename T1, typename T2>
using wider_t = typename wider<T1, T2>::type;

template<typename T1, typename T2>
struct are_related
{
    static constexpr bool value =
        std::is_base_of<T1, T2>::value ||
        std::is_base_of<T2, T1>::value;
};

} // namespace neo

#endif // NEO_TYPE_TRAITS_HPP
