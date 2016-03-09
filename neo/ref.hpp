/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_REF_HPP
#define NEO_REF_HPP

#include <neo/ptr.hpp>
#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <neo/detail/type_traits.hpp>

#include <iosfwd>

namespace neo
{

template<typename T>
class ref
{
    template<typename U>
    friend class ref;

public:
    using element_type = T;

private:
    ptr<element_type> m_value;

public:
    ref(undefined_t) :
        m_value(undefined)
    {
    }

    ref(ref const& other) :
        m_value(other.m_value)
    {
    }

    ref& operator=(ref const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    ref(ref&& other) :
        m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    ref& operator=(ref&& other)
    {
        m_value = other.m_value;
        other.m_value = nullptr;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ref(ref<U> const& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ref& operator=(ref<U> const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ref(ref<U>&& other) :
        m_value(other.m_value)
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, T*>::value>>
    ref& operator=(ref<U>&& other)
    {
        m_value = other.m_value;
        return *this;
    }

    ref(element_type& value) :
        m_value(&value)
    {
    }

    ref& operator=(element_type& value)
    {
        m_value = &value;
        return *this;
    }

    ref(element_type&& value) = delete;
    ref& operator=(element_type&& value) = delete;

    template<typename U, typename = detail::enable_if_t<std::is_convertible<T*, U*>::value>>
    operator U&() const
    {
        return *m_value;
    }

    template<typename U, typename = detail::enable_if_t<
            !std::is_convertible<T*, U*>::value &&
            std::is_convertible<U*, T*>::value
        >, typename = void>
    explicit operator U&() const
    {
        return static_cast<U&>(*m_value);
    }

    ptr<element_type> const& operator&() const
    {
        return m_value;
    }

    element_type& operator*() const
    {
        return *m_value;
    }

    element_type* operator->() const
    {
        return m_value;
    }

    element_type const& get() const
    {
        return *m_value;
    }

    element_type& get()
    {
        return *m_value;
    }
};

// ref<T> - ref<T>
//-----------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() == &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() != &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ref<T> const& lhs, ref<U> const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(&lhs.get(), &rhs.get());
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ref<T> const& lhs, ref<U> const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ref<T> const& lhs, ref<U> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ref<T> const& lhs, ref<U> const& rhs)
{
    return !(lhs < rhs);
}

// ref<T> - U
//------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ref<T> const& lhs, U const& rhs)
{
    return &lhs.get() == &rhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, U const& rhs)
{
    return &lhs.get() != &rhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ref<T> const& lhs, U const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(&lhs.get(), &rhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ref<T> const& lhs, U const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ref<T> const& lhs, U const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ref<T> const& lhs, U const& rhs)
{
    return !(lhs < rhs);
}

// U - ref<T>
//------------

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, ref<T> const& rhs)
{
    return &lhs == &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, ref<T> const& rhs)
{
    return &lhs != &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(U const& lhs, ref<T> const& rhs)
{
    using common_type = typename std::common_type<T*, U*>::type;
    return std::less<common_type>(&lhs.get(), &rhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(U const& lhs, ref<T> const& rhs)
{
    return !(rhs > lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(U const& lhs, ref<T> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(U const& lhs, ref<T> const& rhs)
{
    return !(lhs < rhs);
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
