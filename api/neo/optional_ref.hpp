/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_OPTIONAL_REF_HPP
#define NEO_OPTIONAL_REF_HPP

#include <neo/nullopt.hpp>
#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <neo/detail/type_traits.hpp>

#include <functional>

namespace neo
{

template<typename T>
class optional_ref
{
public:
    using element_type = T;
    using pointer = element_type*;

private:
    pointer m_value;

public:
    constexpr optional_ref(undefined_t) noexcept
    {
    }

    constexpr optional_ref() noexcept :
        optional_ref(nullptr)
    {
    }

    constexpr optional_ref(nullopt_t) noexcept :
        optional_ref(nullptr)
    {
    }

    constexpr optional_ref(std::nullptr_t) noexcept :
        m_value()
    {
    }

    constexpr optional_ref(element_type& value) noexcept :
        optional_ref(&value)
    {
    }

    optional_ref& operator=(element_type& value) noexcept
    {
        return *this = &value;
    }

    constexpr optional_ref(pointer value) noexcept :
        m_value(value)
    {
    }

    optional_ref& operator=(pointer value) noexcept
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    constexpr optional_ref(optional_ref<U> const& other) noexcept :
        m_value(&other.value())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    optional_ref& operator=(optional_ref<U> const& other) noexcept
    {
        m_value = &other.value();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    constexpr optional_ref(ref<U> const& other) noexcept :
        m_value(&other.value())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    optional_ref& operator=(ref<U> const& other) noexcept
    {
        m_value = &other.value();
        return *this;
    }

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    constexpr explicit operator ptr<bool>() const noexcept
    {
        return has_value();
    }

    constexpr element_type& operator*() const noexcept
    {
        return value();
    }

    constexpr pointer operator->() const noexcept
    {
        return m_value;
    }

    constexpr value<bool> has_value() const noexcept
    {
        return m_value != nullptr;
    }

    constexpr element_type& value() const noexcept
    {
        return *m_value;
    }

    template <typename U>
    constexpr element_type value_or(U&& default_value) const noexcept
    {
        return has_value() ? value() : static_cast<T>(std::forward<U>(default_value));
    }
};

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator==(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return &lhs.value() == &rhs.value();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator!=(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return &lhs.value() != &rhs.value();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return std::less<detail::common_type_t<T1*, T2*>>(&lhs.value(), &rhs.value());
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<=(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return rhs < lhs;
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>=(optional_ref<T1> const& lhs, optional_ref<T2> const& rhs) noexcept
{
    return !(lhs < rhs);
}

template<typename T>
constexpr optional_ref<T> make_optional_ref(T& object) noexcept
{
    return object;
}

} // namespace neo

#endif // NEO_OPTIONAL_REF_HPP
