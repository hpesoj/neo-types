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

} // namespace detail

} // namespace neo

#endif // NEO_TYPE_TRAITS_HPP
