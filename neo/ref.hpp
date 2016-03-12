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
    ref(undefined_t)
    {
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

    ref(element_type&&) = delete;
    ref& operator=(element_type&&) = delete;

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, pointer>::value>>
    ref(ref<U> const& other) :
        m_value(other.get())
    {
    }

    template<typename U, typename = detail::enable_if_t<std::is_convertible<U*, pointer>::value>>
    ref& operator=(ref<U> const& other)
    {
        m_value = other.get();
        return *this;
    }

    operator element_type&() const
    {
        return *m_value;
    }

    pointer operator&() const
    {
        return m_value;
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
value<bool> operator==(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() == &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator!=(ref<T> const& lhs, ref<U> const& rhs)
{
    return &lhs.get() != &rhs.get();
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator<(ref<T> const& lhs, ref<U> const& rhs)
{
    using common_type = typename std::common_type<typename ref<T>::pointer, U*>::type;
    return std::less<common_type>(&lhs.get(), &rhs.get());
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator<=(ref<T> const& lhs, ref<U> const& rhs)
{
    return !(rhs < lhs);
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator>(ref<T> const& lhs, ref<U> const& rhs)
{
    return rhs < lhs;
}

template<typename T, typename U, typename = detail::enable_if_t<detail::are_related<T, U>::value>>
value<bool> operator>=(ref<T> const& lhs, ref<U> const& rhs)
{
    return !(lhs < rhs);
}

template<typename T>
std::ostream& operator<<(std::ostream& s, ref<T> const& v)
{
    s << v.get();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, ref<T>& v)
{
    s >> v.get();
    return s;
}

template<typename T>
ref<T> make_ref(T& object)
{
    return object;
}

namespace aliases
{

template<typename T>
using neo_ref = ref<T>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_REF_HPP
