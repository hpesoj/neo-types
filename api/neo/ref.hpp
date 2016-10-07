/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_REF_HPP
#define NEO_REF_HPP

#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <neo/detail/type_traits.hpp>

#include <iosfwd>
#include <functional>

namespace neo
{

template<typename T>
class ref
{
public:
    using element_type = T;
    using pointer = element_type*;

private:
    pointer m_value;

public:
    constexpr ref(undefined_t) noexcept
    {
    }

    constexpr ref(element_type& value) noexcept :
        m_value(&value)
    {
    }

    ref& operator=(element_type& value) noexcept
    {
        m_value = &value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    constexpr ref(ref<U> const& other) noexcept :
        m_value(&other.value())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    ref& operator=(ref<U> const& other) noexcept
    {
        m_value = &other.value();
        return *this;
    }

    constexpr explicit operator bool() const noexcept
    {
        return true;
    }

    constexpr explicit operator ptr<bool>() const noexcept
    {
        return static_cast<bool>(*this);
    }

    constexpr element_type& operator*() const noexcept
    {
        return value();
    }

    constexpr pointer operator->() const noexcept
    {
        return m_value;
    }

    constexpr element_type& value() const noexcept
    {
        return *m_value;
    }
};

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator==(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return &lhs.value() == &rhs.value();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator!=(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return &lhs.value() != &rhs.value();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return std::less<detail::common_type_t<T1*, T2*>>(&lhs.value(), &rhs.value());
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<=(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return rhs < lhs;
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>=(ref<T1> const& lhs, ref<T2> const& rhs) noexcept
{
    return !(lhs < rhs);
}

template<typename T>
constexpr ref<T> make_ref(T& object) noexcept
{
    return object;
}

} // namespace neo

#endif // NEO_REF_HPP
