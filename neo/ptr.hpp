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
    ptr() :
        m_value()
    {
    }

    ptr(undefined_t)
    {
    }

    ptr(pointer value) :
        m_value(value)
    {
    }

    ptr& operator=(pointer value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, pointer>::value>>
    ptr(ptr<U> const& other) :
        m_value(other.get())
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, pointer>::value>>
    ptr& operator=(ptr<U> const& other)
    {
        m_value = other.get();
        return *this;
    }

    operator pointer() const
    {
        return m_value;
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator ptr<bool>() const
    {
        return m_value != nullptr;
    }

    pointer* operator&() const
    {
        return &m_value;
    }

    element_type& operator*() const
    {
        return *m_value;
    }

    pointer operator->() const
    {
        return m_value;
    }

    pointer const& get() const
    {
        return m_value;
    }

    pointer& get()
    {
        return m_value;
    }
};

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator==(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator!=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator<(ptr<T> const& lhs, ptr<U> const& rhs)
{
    using common_type = typename std::common_type<typename ptr<T>::pointer, U*>::type;
    return std::less<common_type>(lhs.get(), rhs.get());
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator<=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator>(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator>=(ptr<T> const& lhs, ptr<U> const& rhs)
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
ptr<T> make_ptr(T* object)
{
    return object;
}

namespace aliases
{

template<typename T>
using neo_ptr = ptr<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_PTR_HPP
