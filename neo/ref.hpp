/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_REF_HPP
#define NEO_REF_HPP

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
private:
    T* m_value;

public:
    ref(undefined_t)
    {
    }

    ref(ref&& other) :
        m_value(&other.get())
    {
    }

    ref& operator=(ref&& other)
    {
        m_value = &other.get();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value && std::is_const<T>::value>>
    ref(ref<U> const& other) :
        m_value(&other.get())
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value && std::is_const<T>::value>>
    ref& operator=(ref<U> const& other)
    {
        m_value = &other.get();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ref(ref<U>& other) :
        m_value(&other.get())
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ref& operator=(ref<U>& other)
    {
        m_value = &other.get();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ref(ref<U>&& other) :
        m_value(other)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ref& operator=(ref<U>&& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    ref(U& value) :
        m_value(&value)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    ref& operator=(U& value)
    {
        m_value = &value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<U, T>::value || std::is_void<U>::value>>
    operator U&() const
    {
        return *m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>, typename = void>
    explicit operator U&() const
    {
        return *m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<U, T>::value || std::is_void<U>::value>>
    explicit operator ref<U>() const
    {
        return static_cast<U&>(*m_value);
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

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() == &rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() != &rhs.get();
}

// ref<T> - U
//------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, U const& rhs)
{
    return &lhs.get() == &rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, U const& rhs)
{
    return &lhs.get() != &rhs;
}

// U - ref<T>
//------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, ref<T> const& rhs)
{
    return &lhs == &rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, ref<T> const& rhs)
{
    return &lhs != &rhs.get();
}

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, ref<T> const& v)
{
    s << &v.get();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, ref<T>& v)
{
    s >> &v.get();
    return s;
}

// Utilities
//-----------

template<typename T>
ref<T> make_ref(T& value)
{
    return value;
}

namespace aliases
{

template<typename T>
using neo_ref = ref<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_REF_HPP
