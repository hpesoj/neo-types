/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_OPTIONAL_REF_HPP
#define NEO_OPTIONAL_REF_HPP

#include <neo/ptr.hpp>
#include <neo/type_traits.hpp>
#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <iosfwd>
#include <type_traits>
#include <utility>

namespace neo
{

struct nullref_t
{
};

constexpr nullref_t nullref;

template<typename T>
class optional_ref
{
    template<typename U>
    friend class optional_ref;

private:
    ptr<T> m_value;

public:
    optional_ref() = default;

    optional_ref(nullref_t)
    {
    }

    optional_ref(undefined_t) :
        m_value(undefined)
    {
    }

    optional_ref(T& value) :
        m_value(&value)
    {
    }

    optional_ref& operator=(T& value)
    {
        m_value = &value;
        return *this;
    }

    optional_ref(T&& value) = delete;
    optional_ref& operator=(T&& value) = delete;

    optional_ref(optional_ref& other) :
        m_value(other.m_value)
    {
    }

    optional_ref& operator=(optional_ref& other)
    {
        m_value = other.m_value;
        return *this;
    }

    optional_ref(optional_ref&& other) :
        m_value(std::move(other.m_value))
    {
    }

    optional_ref& operator=(optional_ref&& other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_const<T>::value && (is_same<T, U>::value || std::is_base_of<T, U>::value)>>
    optional_ref(optional_ref<U> const& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_const<T>::value && (is_same<T, U>::value || std::is_base_of<T, U>::value)>>
    optional_ref& operator=(optional_ref<U> const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<!std::is_const<U>::value && std::is_base_of<T, U>::value>>
    optional_ref(optional_ref<U>& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = std::enable_if_t<!std::is_const<U>::value && std::is_base_of<T, U>::value>>
    optional_ref& operator=(optional_ref<U>& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<(std::is_const<T>::value || !std::is_const<U>::value) && std::is_base_of<T, U>::value>>
    optional_ref(optional_ref<U>&& other) :
        m_value(std::move(other.m_value))
    {
    }

    template<typename U, typename = std::enable_if_t<(std::is_const<T>::value || !std::is_const<U>::value) && std::is_base_of<T, U>::value>>
    optional_ref& operator=(optional_ref<U>&& other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator ptr<bool>() const
    {
        return m_value != nullptr;
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

// optional_ref<T> - optional_ref<T>
//-----------------------------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(optional_ref<T> const& lhs, optional_ref<U> const& rhs)
{
    return &*lhs == &*rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(optional_ref<T> const& lhs, optional_ref<U> const& rhs)
{
    return &*lhs != &*rhs;
}

// optional_ref<T> - U
//---------------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(optional_ref<T> const& lhs, U const& rhs)
{
    return &*lhs == &rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(optional_ref<T> const& lhs, U const& rhs)
{
    return &*lhs != &rhs;
}

// U - optional_ref<T>
//---------------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, optional_ref<T> const& rhs)
{
    return &lhs == &*rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, optional_ref<T> const& rhs)
{
    return &lhs != &*rhs;
}

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, optional_ref<T> const& v)
{
    s << &*v;
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, optional_ref<T>& v)
{
    s >> &*v;
    return s;
}

// Utilities
//-----------

template<typename T>
optional_ref<T> make_optional_ref(T& value)
{
    return value;
}

namespace aliases
{

template<typename T>
using neo_optional_ref = optional_ref<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_OPTIONAL_REF_HPP
