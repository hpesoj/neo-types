/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_PTR_HPP
#define NEO_PTR_HPP

#include <neo/type_traits.hpp>
#include <neo/undefined.hpp>
#include <neo/value.hpp>

#include <iosfwd>
#include <type_traits>

namespace neo
{

template<typename T>
class ptr
{
private:
    T* m_value;

public:
    ptr() :
        m_value()
    {
    }

    ptr(undefined_t)
    {
    }

    ptr(ptr const& other) :
        m_value(other)
    {
    }

    ptr& operator=(ptr const& other)
    {
        m_value = other;
        return *this;
    }

    ptr(ptr&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    ptr& operator=(ptr&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ptr(ptr<U> const& other) :
        m_value(other)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ptr& operator=(ptr<U> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ptr(ptr<U>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    ptr& operator=(ptr<U>&& other)
    {
        m_value = other;
        other = nullptr;
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

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    ptr(U* value) :
        m_value(value)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    ptr& operator=(U* value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<U, T>::value || std::is_void<U>::value>>
    operator U*() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<!std::is_same<T, U>::value && std::is_base_of<T, U>::value>, typename = void>
    explicit operator U*() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<U, T>::value || std::is_void<U>::value>>
    explicit operator ptr<U>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator ptr<bool>() const
    {
        return m_value != nullptr;
    }

    T* const& get() const
    {
        return m_value;
    }

    T*& get()
    {
        return m_value;
    }

    T& operator*() const
    {
        return *m_value;
    }

    T* operator->() const
    {
        return m_value;
    }

    T& operator[](value<std::size_t> index) const
    {
        return m_value[index.get()];
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::size_t>::value>>
    T& operator[](U const& index) const
    {
        return m_value[index];
    }

    ptr& operator++()
    {
        ++m_value;
        return *this;
    }

    ptr operator++(int)
    {
        return m_value++;
    }

    ptr& operator--()
    {
        --m_value;
        return *this;
    }

    ptr operator--(int)
    {
        return m_value--;
    }

    ptr& operator+=(ptr<std::ptrdiff_t> const& rhs)
    {
        m_value += rhs.get();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
    ptr& operator+=(U const& rhs)
    {
        m_value += rhs;
        return *this;
    }

    ptr& operator-=(ptr<std::ptrdiff_t> const& rhs)
    {
        m_value -= rhs.get();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
    ptr& operator-=(U const& rhs)
    {
        m_value -= rhs;
        return *this;
    }
};

template<>
class ptr<void const>
{
private:
    void const* m_value;

public:
    ptr() :
        m_value()
    {
    }

    ptr(undefined_t)
    {
    }

    ptr(ptr const& other) :
        m_value(other)
    {
    }

    ptr& operator=(ptr const& other)
    {
        m_value = other;
        return *this;
    }

    ptr(ptr&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    ptr& operator=(ptr&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U>
    ptr(ptr<U const> const& other) :
        m_value(other)
    {
    }

    template<typename U>
    ptr& operator=(ptr<U const> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U>
    ptr(ptr<U const>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U>
    ptr& operator=(ptr<U const>&& other)
    {
        m_value = other;
        other = nullptr;
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

    template<typename U>
    ptr(U* value) :
        m_value(value)
    {
    }

    template<typename U>
    ptr& operator=(U* value)
    {
        m_value = value;
        return *this;
    }

    operator void const*() const
    {
        return m_value;
    }

    template<typename U>
    explicit operator U const*() const
    {
        return static_cast<U const*>(m_value);
    }

    template<typename U>
    explicit operator ptr<U const>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator value<bool>() const
    {
        return m_value != nullptr;
    }

    void const* const& get() const
    {
        return m_value;
    }

    void const*& get()
    {
        return m_value;
    }
};

template<>
class ptr<void>
{
private:
    void* m_value;

public:
    ptr() :
        m_value()
    {
    }

    ptr(undefined_t)
    {
    }

    ptr(ptr const& other) :
        m_value(other)
    {
    }

    ptr& operator=(ptr const& other)
    {
        m_value = other;
        return *this;
    }

    ptr(ptr&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    ptr& operator=(ptr&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U>
    ptr(ptr<U> const& other) :
        m_value(other)
    {
    }

    template<typename U>
    ptr& operator=(ptr<U> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U>
    ptr(ptr<U>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U>
    ptr& operator=(ptr<U>&& other)
    {
        m_value = other;
        other = nullptr;
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

    ptr(void* value) :
        m_value(value)
    {
    }

    ptr& operator=(void* value)
    {
        m_value = value;
        return *this;
    }

    operator void*() const
    {
        return m_value;
    }

    operator ptr<void const>() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, void>::value>>
    operator U*() const
    {
        return m_value;
    }

    template<typename U>
    explicit operator U*() const
    {
        return static_cast<U*>(m_value);
    }

    template<typename U>
    explicit operator ptr<U>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator value<bool>() const
    {
        return m_value != nullptr;
    }

    void* const& get() const
    {
        return m_value;
    }

    void*& get()
    {
        return m_value;
    }
};

// ptr<T> - ptr<T>
//-----------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() < rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() <= rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() > rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ptr<T> const& lhs, ptr<U> const& rhs)
{
    return lhs.get() >= rhs.get();
}

// ptr<T> - U
//------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() == rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() != rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() < rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() <= rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() > rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() >= rhs;
}

// U - ptr<T>
//------------

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator==(U const& lhs, ptr<T> const& rhs)
{
    return lhs == rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator!=(U const& lhs, ptr<T> const& rhs)
{
    return lhs != rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<(U const& lhs, ptr<T> const& rhs)
{
    return lhs < rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator<=(U const& lhs, ptr<T> const& rhs)
{
    return lhs <= rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>(U const& lhs, ptr<T> const& rhs)
{
    return lhs > rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<are_related<T, U>::value>>
value<bool> operator>=(U const& lhs, ptr<T> const& rhs)
{
    return lhs >= rhs.get();
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

template<typename T>
value<bool> operator<(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() < nullptr;
}

template<typename T>
value<bool> operator<=(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() <= nullptr;
}

template<typename T>
value<bool> operator>(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() > nullptr;
}

template<typename T>
value<bool> operator>=(ptr<T> const& lhs, std::nullptr_t)
{
    return lhs.get() >= nullptr;
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

template<typename T>
value<bool> operator<(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr < rhs.get();
}

template<typename T>
value<bool> operator<=(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr <= rhs.get();
}

template<typename T>
value<bool> operator>(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr > rhs.get();
}

template<typename T>
value<bool> operator>=(std::nullptr_t, ptr<T> const& rhs)
{
    return nullptr >= rhs.get();
}

// Pointer arithmetic
//--------------------

template<typename T>
ptr<T> operator+(ptr<T> const& lhs, value<std::ptrdiff_t> const& rhs)
{
    return lhs.get() + rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
ptr<T> operator+(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() + rhs;
}

template<typename T>
ptr<T> operator+(value<std::ptrdiff_t> const& lhs, ptr<T> const& rhs)
{
    return lhs.get() + rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
ptr<T> operator+(U const& lhs, ptr<T> const& rhs)
{
    return lhs + rhs.get();
}

template<typename T>
ptr<T> operator-(ptr<T> const& lhs, value<std::ptrdiff_t> const& rhs)
{
    return lhs.get() - rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
ptr<T> operator-(ptr<T> const& lhs, U const& rhs)
{
    return lhs.get() - rhs;
}

template<typename T>
value<std::ptrdiff_t> operator-(ptr<T> const& lhs, ptr<T> const& rhs)
{
    return lhs.get() - lhs.get();
}

template<typename T>
value<std::ptrdiff_t> operator-(ptr<T> const& lhs, T* rhs)
{
    return lhs.get() - rhs;
}

template<typename T>
value<std::ptrdiff_t> operator-(T* lhs, ptr<T> const& rhs)
{
    return lhs - lhs.get();
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
