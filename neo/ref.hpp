/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_REF_HPP
#define NEO_REF_HPP

#include <neo/ptr.hpp>
#include <neo/type_traits.hpp>
#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <iosfwd>
#include <type_traits>

namespace neo
{

template<typename T>
class ref
{
    template<typename U>
    friend class ref;

private:
    ptr<T> m_value;

public:
    ref(undefined_t) :
        m_value(undefined)
    {
    }

    ref(T& value) :
        m_value(&value)
    {
    }

    ref& operator=(T& value)
    {
        m_value = &value;
        return *this;
    }

    ref(T&& value) = delete;
    ref& operator=(T&& value) = delete;

    ref(ref const& other) :
        m_value(other.m_value)
    {
    }

    ref& operator=(ref const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template<typename U, typename = enable_if_t<std::is_convertible<U*, T*>::value>>
    ref(ref<U> const& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = enable_if_t<std::is_convertible<U*, T*>::value>>
    ref& operator=(ref<U> const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template<typename U, typename = enable_if_t<std::is_convertible<U*, T*>::value>>
    ref(ref<U>&& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = enable_if_t<std::is_convertible<U*, T*>::value>>
    ref& operator=(ref<U>&& other)
    {
        m_value = other.m_value;
        return *this;
    }

    T const& get() const
    {
        return *m_value;
    }

    T& get()
    {
        return *m_value;
    }

    T const& operator*() const
    {
        return *m_value;
    }

    T const* operator->() const
    {
        return m_value;
    }

    T& operator*()
    {
        return *m_value;
    }

    T* operator->()
    {
        return m_value;
    }
};

// ref<T> - ref<T>
//-----------------

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, ref<U> const& rhs)
{
    return &*lhs == &*rhs;
}

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, ref<U> const& rhs)
{
    return &*lhs != &*rhs;
}

// ref<T> - U
//------------

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, U const& rhs)
{
    return &*lhs == &rhs;
}

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, U const& rhs)
{
    return &*lhs != &rhs;
}

// U - ref<T>
//------------

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, ref<T> const& rhs)
{
    return &lhs == &*rhs;
}

template<typename T, typename U, typename = enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, ref<T> const& rhs)
{
    return &lhs != &*rhs;
}

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, ref<T> const& v)
{
    s << &*v;
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, ref<T>& v)
{
    s >> &*v;
    return s;
}

// Utilities
//-----------

template<typename T>
ref<T> make_ref(T& value)
{
    return value;
}

template<typename T>
ptr<T> addr(ref<T>& object)
{
    return object.get();
}

namespace aliases
{

template<typename T>
using neo_ref = ref<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_REF_HPP
