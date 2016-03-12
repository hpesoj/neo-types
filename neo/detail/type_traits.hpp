/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_TYPE_TRAITS_HPP
#define NEO_TYPE_TRAITS_HPP

#include <type_traits>

namespace neo
{

namespace detail
{

template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T1, typename T2>
struct are_related : std::integral_constant<bool,
    std::is_base_of<T1, T2>::value ||
    std::is_base_of<T2, T1>::value
    >
{
};

} // namespace detail

} // namespace neo

#endif // NEO_TYPE_TRAITS_HPP
