#pragma once

#ifndef NEO_TYPES_OPERATOR_TRAITS_HPP
#define NEO_TYPES_OPERATOR_TRAITS_HPP

namespace neo_types
{
namespace operator_traits
{

namespace detail
{

template<typename>
using void_t = void;

} // namespace detail

// Implicit Conversion
//---------------------

template<typename T1, typename T2, typename = void>
struct implicit_conversion_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct implicit_conversion_traits<T1, T2, detail::void_t<decltype(T2{*static_cast<T1*>(nullptr)})>>
{
    static constexpr bool value = true;
};

// Explicit Conversion
//---------------------

template<typename T1, typename T2, typename = void>
struct explicit_conversion_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct explicit_conversion_traits<T1, T2, detail::void_t<decltype(static_cast<T2>(T1()))>>
{
    static constexpr bool value = true;
};

// Copy Construction
//-------------------

template<typename T1, typename T2, typename = void>
struct copy_construction_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct copy_construction_traits<T1, T2, detail::void_t<decltype(T1{*static_cast<T2*>(nullptr)})>>
{
    static constexpr bool value = true;
};

// Move Construction
//-------------------

template<typename T1, typename T2, typename = void>
struct move_construction_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct move_construction_traits<T1, T2, detail::void_t<decltype(T1{std::move(*static_cast<T2*>(nullptr))})>>
{
    static constexpr bool value = true;
};

// Copy Assignment
//-----------------

template<typename T1, typename T2, typename = void>
struct copy_assignment_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct copy_assignment_traits<T1, T2, detail::void_t<decltype(*static_cast<T1*>(nullptr) = *static_cast<T2*>(nullptr))>>
{
    static constexpr bool value = true;
};

// Move Assignment
//-----------------

template<typename T1, typename T2, typename = void>
struct move_assignment_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct move_assignment_traits<T1, T2, detail::void_t<decltype(*static_cast<T1*>(nullptr) = std::move(*static_cast<T2*>(nullptr)))>>
{
    static constexpr bool value = true;
};

// Equals
//--------

template<typename T1, typename T2, typename = void>
struct equals_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct equals_traits<T1, T2, detail::void_t<decltype(T1() == T2())>>
{
    static constexpr bool value = true;
    using type = decltype(T1() == T2());
};

template<typename T1, typename T2>
using equals_traits_t = typename equals_traits<T1, T2>::type;

// Add
//-----

template<typename T1, typename T2, typename = void>
struct add_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct add_traits<T1, T2, detail::void_t<decltype(T1() + T2())>>
{
    static constexpr bool value = true;
    using type = decltype(T1() + T2());
};

template<typename T1, typename T2>
using add_traits_t = typename add_traits<T1, T2>::type;

// And
//-----

template<typename T1, typename T2, typename = void>
struct and_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct and_traits<T1, T2, detail::void_t<decltype(T1() && T2())>>
{
    static constexpr bool value = true;
    using type = decltype(T1() && T2());
};

template<typename T1, typename T2>
using and_traits_t = typename and_traits<T1, T2>::type;

// Or
//----

template<typename T1, typename T2, typename = void>
struct or_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct or_traits<T1, T2, detail::void_t<decltype(T1() || T2())>>
{
    static constexpr bool value = true;
    using type = decltype(T1() || T2());
};

template<typename T1, typename T2>
using or_traits_t = typename or_traits<T1, T2>::type;

// Bitand
//--------

template<typename T1, typename T2, typename = void>
struct bitand_traits
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct bitand_traits<T1, T2, detail::void_t<decltype(T1() & T2())>>
{
    static constexpr bool value = true;
    using type = decltype(T1() & T2());
};

template<typename T1, typename T2>
using bitand_traits_t = typename bitand_traits<T1, T2>::type;

} // namespace operator_traits
} // namespace neo_types

#endif // NEO_TYPES_OPERATOR_TRAITS_HPP
