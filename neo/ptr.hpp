/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_PTR_HPP
#define NEO_PTR_HPP

#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <neo/detail/type_traits.hpp>

#include <iosfwd>
#include <functional>

namespace neo
{

template<typename T>
class ptr
{
public:
    using element_type = T;
    using pointer = element_type*;

private:
    pointer m_value;

public:
    constexpr ptr() noexcept :
        m_value()
    {
    }

    ptr(undefined_t) noexcept
    {
    }

    constexpr ptr(pointer value) noexcept :
        m_value(value)
    {
    }

    ptr& operator=(pointer value) noexcept
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    constexpr ptr(ptr<U> const& other) noexcept :
        m_value(other.get())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        std::is_convertible<U*, T*>::value>
    >
    ptr& operator=(ptr<U> const& other) noexcept
    {
        m_value = other.get();
        return *this;
    }

    constexpr operator pointer() const noexcept
    {
        return m_value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return m_value != nullptr;
    }

    constexpr explicit operator ptr<bool>() const noexcept
    {
        return m_value != nullptr;
    }

    constexpr pointer* operator&() const noexcept
    {
        return &m_value;
    }

    constexpr element_type& operator*() const noexcept
    {
        return *m_value;
    }

    constexpr pointer operator->() const noexcept
    {
        return m_value;
    }

    constexpr pointer const& get() const noexcept
    {
        return m_value;
    }

    pointer& get() noexcept
    {
        return m_value;
    }
};

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator==(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator!=(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return std::less<detail::common_type_t<T1*, T2*>>(lhs.get(), rhs.get());
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator<=(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return rhs < lhs;
}

template<typename T1, typename T2, typename =
    detail::common_type_t<T1*, T2*>
>
constexpr value<bool> operator>=(ptr<T1> const& lhs, ptr<T2> const& rhs) noexcept
{
    return !(lhs < rhs);
}

template<typename T>
std::ostream& operator<<(std::ostream& s, ptr<T> const& v)
{
    s << v.get();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, ptr<T>& v)
{
    s >> v.get();
    return s;
}

template<typename T>
constexpr ptr<T> make_ptr(T* object) noexcept
{
    return object;
}

} // namespace neo

#endif // NEO_PTR_HPP
