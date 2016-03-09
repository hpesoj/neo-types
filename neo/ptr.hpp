/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_PTR_HPP
#define NEO_PTR_HPP

#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <neo/detail/type_traits.hpp>

#include <cstddef>
#include <iosfwd>
#include <functional>

namespace neo
{

template<typename T1, typename T2>
struct are_related
{
    static constexpr bool value =
        std::is_base_of<T1, T2>::value ||
        std::is_base_of<T2, T1>::value;
};

template<typename T>
class ptr
{
public:
    using element_type = T;

private:
    element_type* m_value;

public:
    ptr() :
        m_value()
    {
    }

    ptr(undefined_t)
    {
    }

    ptr(ptr const& other) = default;
    ptr& operator=(ptr const& other) = default;

    ptr(ptr&& other) = default;
    ptr& operator=(ptr&& other) = default;

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr(ptr<U> const& other) :
        m_value(other)
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr& operator=(ptr<U> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr(ptr<U>&& other) :
        m_value(other)
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr& operator=(ptr<U>&& other)
    {
        m_value = other;
        return *this;
    }

    ptr(std::nullptr_t) :
        m_value()
    {
    }

    ptr& operator=(std::nullptr_t)
    {
        m_value = nullptr;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr(U* value) :
        m_value(value)
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ptr& operator=(U* value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<T*, U*>::value>>
    operator U*() const
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
            !std::is_convertible<T*, U*>::value &&
            std::is_convertible<U*, T*>::value
        >, typename = void>
    explicit operator U*() const
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

    element_type& operator*() const
    {
        return *m_value;
    }

    element_type* operator->() const
    {
        return m_value;
    }

    element_type* const& get() const
    {
        return m_value;
    }

    element_type*& get()
    {
        return m_value;
    }
};

// ptr<T> - ptr<U>
//-----------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ptr<T> const& lhs, ptr<U> const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(lhs.get(), rhs.get());
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return !(lhs < rhs);
}

// ptr<T> - U
//------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() == rhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() != rhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ptr<T> const& lhs, U const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(lhs.get(), rhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ptr<T> const& lhs, U const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ptr<T> const& lhs, U const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ptr<T> const& lhs, U const& rhs)
{
    return !(lhs < rhs);
}

// U - ptr<T>
//------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, ptr<T> const& rhs)
{
    return lhs == rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, ptr<T> const& rhs)
{
    return lhs != rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(U const& lhs, ptr<T> const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(lhs.get(), rhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(U const& lhs, ptr<T> const& rhs)
{
    return !(rhs > lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(U const& lhs, ptr<T> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(U const& lhs, ptr<T> const& rhs)
{
    return !(lhs < rhs);
}

// ptr<T> - std::nullptr_t
//--------------------------

template<typename T>
value<bool> operator==(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() == nullptr;
}

template<typename T>
value<bool> operator!=(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() != nullptr;
}

// std::nullptr_t - ptr<T>
//--------------------------

template<typename T>
value<bool> operator==(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr == rhs.get();
}

template<typename T>
value<bool> operator!=(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr != rhs.get();
}

// IOStream
//----------

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

// Utilities
//-----------

template<typename T>
ptr<T> make_ptr(T* object)
{
    return object;
}

template<typename T>
ptr<T> addr(T& object)
{
    return &object;
}

namespace aliases
{

template<typename T>
using neo_ptr = ptr<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_PTR_HPP
