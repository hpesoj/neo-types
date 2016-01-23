/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 *
 * Primary Advantages:
 * - Default zero-initialized construction by default, everywhere
 * - Optional uninitialized construction, everywhere
 * - Moved values are always zeroed
 * - No implicit narrowing conversions (must use static_cast)
 *   - int -> char
 *   - double -> float
 *   - float -> int
 *   - int -> float
 *   - uint -> int
 *   - int -> uint
 * - No implicit or explicit conversion to bool (bool is not an integral type)
 *   - int -> bool
 *   - float -> bool
 * - No mixing of different type categories in operations (must use static_cast)
 *   - Boolean (bool)
 *   - Signed Integer (char, short, int)
 *   - Unsigned Integer (uchar, ushort, uint)
 *   - Floating Point (float, double)
 * - No integer promotion
 *   - decltype(neo_char() + neo_char()) == neo_char
 * - No bitwise operations on signed integers
 * - No void pointer arithmetic (GCC extension)
 *
 * Secondary Advantages:
 * - std::vector<neo_bool> is a proper container
 * - std::vector<neo<T>> can be filled with uninitialized values
 * - Possible to pass neo_void as a function parameter
 * - neo_ptr<neo_char const> cannot represent strings
 * - neo_ptr<T> disallows i[p] reversed array indexing syntax
 *
 * Disadvantages:
 * - neo_char and neo_short cannot assigned integer constant (esp. neo_short)
 *   - neo_short ns = short{10};
 * - Overload resolution is ambiguous within type category when converting between neo<T> and U
 *   - f(neo_char) and f(neo_int) rank the same for f('a')
 *   - neo_float f; std::sqrt(f.get());
 * - neo_ptr<T> cannot be deleted
 *   - delete ptr.get();
 * - neo<T> is not trivially copyable (may be able to fix using policy)
 *
 * Notes:
 * - Boolean && and || operators not overloaded (preserve short-circuiting)
 * - neo_ptr<int> and neo_ptr<neo_int> are distinct and separate types
 * - Cannot perform narrowing static_cast from U to neo<T>
 *   - neo_char nc = static_cast<neo_char>(i); // illegal
 *   - neo_char nc = static_cast<char>(i); // workaround
 * - operator- exists for unsigned integers for consistency
 *   - -neo_uint(); // same as: 0u - neo_uint();
 */

#pragma once

#ifndef NEO_TYPES_HPP
#define NEO_TYPES_HPP

#include <cstddef>
#include <iosfwd>
#include <limits>
#include <type_traits>

namespace neo_types
{

template<typename T>
struct is_bool
{
    static constexpr bool value = std::is_same<std::remove_cv_t<T>, bool>::value;
};

template<typename T>
struct is_numeric
{
    static constexpr bool value = std::is_arithmetic<T>::value & !is_bool<T>::value;
};

template<typename T>
struct is_unsigned_integral
{
    static constexpr bool value = std::is_integral<T>::value && !std::is_signed<T>::value;
};

template<typename, typename, typename = void>
struct are_similar
{
    static constexpr bool value = false;
};

template<typename T1, typename T2>
struct are_similar<T1, T2, std::enable_if_t<
    std::is_same<std::remove_cv_t<T1>, std::remove_cv_t<T2>>::value || (
        is_numeric<T1>::value == is_numeric<T2>::value &&
        std::is_integral<T1>::value == std::is_integral<T2>::value &&
        std::is_signed<T1>::value == std::is_signed<T2>::value &&
        std::is_floating_point<T1>::value == std::is_floating_point<T2>::value &&
        std::is_pointer<T1>::value == std::is_pointer<T2>::value
    )>>
{
    static constexpr bool value = true;
};

template<typename T1, typename T2>
struct are_similar_numeric
{
    static constexpr bool value = is_numeric<T1>::value && are_similar<T1, T2>::value;
};

template<typename T1, typename T2>
struct are_similar_unsigned_integral
{
    static constexpr bool value = is_unsigned_integral<T1>::value && are_similar<T1, T2>::value;
};

// XXX VC++ 2015 doesn't like this as the constexpr.
template<typename T, typename U, bool =
    std::numeric_limits<T>::lowest() >= std::numeric_limits<U>::lowest() &&
    std::numeric_limits<T>::max() <= std::numeric_limits<U>::max()
>
struct is_bounded
{
    static constexpr bool value = false;
};

template<typename T, typename U>
struct is_bounded<T, U, true>
{
    static constexpr bool value = true;
};

template<typename From, typename To, bool = is_numeric<From>::value && are_similar<From, To>::value>
struct is_widening
{
    static constexpr bool value = false;
};

template<typename From, typename To>
struct is_widening<From, To, true>
{
    static constexpr bool value = is_bounded<From, To>::value;
};

template<typename From, typename To, bool = is_numeric<From>::value && are_similar<From, To>::value>
struct is_narrowing
{
    static constexpr bool value = is_numeric<To>::value;
};

template<typename From, typename To>
struct is_narrowing<From, To, true>
{
    static constexpr bool value = !is_bounded<From, To>::value;
};

template<typename T1, typename T2>
struct are_widening
{
    static constexpr bool value = is_widening<T1, T2>::value || is_widening<T2, T1>::value;
};

template<typename T1, typename T2, bool = is_widening<T1, T2>::value>
struct wider
{
    using type = T1;
};

template<typename T1, typename T2>
struct wider<T1, T2, true>
{
    using type = T2;
};

template<typename T1, typename T2>
using wider_t = typename wider<T1, T2>::type;

struct undefined_t
{
    constexpr undefined_t() {}
};

constexpr undefined_t undefined;

template<typename T>
class neo
{
private:
    T m_value;

public:
    neo() :
        m_value()
    {
    }

    neo(undefined_t)
    {
    }

    neo(neo const& other) :
        m_value(other.m_value)
    {
    }

    neo& operator=(neo const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    neo(neo&& other) :
        m_value(other.m_value)
    {
        other.m_value = T();
    }

    neo& operator=(neo&& other)
    {
        m_value = other.m_value;
        other.m_value = T();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo(neo<U> const& other) :
        m_value(other)
    {
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator=(neo<U> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo(neo<U>&& other) :
        m_value(other)
    {
        other = U();
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator=(neo<U>&& other)
    {
        m_value = other;
        other = U();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo(U value) :
        m_value(value)
    {
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator=(U value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value>>
    operator U&()
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<is_widening<T, U>::value>>
    operator U() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<is_narrowing<T, U>::value>, typename = void>
    explicit operator U() const
    {
        return static_cast<U>(m_value);
    }

    template<typename U, typename = std::enable_if_t<is_narrowing<T, U>::value>>
    explicit operator neo<U>() const
    {
        return static_cast<U>(m_value);
    }

    T const& get() const
    {
        return m_value;
    }

    T& get()
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator+=(neo<U> const& other)
    {
        m_value += U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator+=(U const& other)
    {
        m_value += other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator-=(neo<U> const& other)
    {
        m_value -= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator-=(U const& other)
    {
        m_value -= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator*=(neo<U> const& other)
    {
        m_value *= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator*=(U const& other)
    {
        m_value *= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator/=(neo<U> const& other)
    {
        m_value /= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator/=(U const& other)
    {
        m_value /= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator%=(neo<U> const& other)
    {
        m_value %= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo& operator%=(U const& other)
    {
        m_value %= other;
        return *this;
    }

    template<typename = std::enable_if_t<std::is_integral<T>::value>>
    neo& operator++()
    {
        ++m_value;
        return *this;
    }

    template<typename = std::enable_if_t<std::is_integral<T>::value>>
    neo operator++(int)
    {
        return m_value++;
    }

    template<typename = std::enable_if_t<std::is_integral<T>::value>>
    neo& operator--()
    {
        --m_value;
        return *this;
    }

    template<typename = std::enable_if_t<std::is_integral<T>::value>>
    neo operator--(int)
    {
        return m_value--;
    }

    neo<T> operator+() const
    {
        return m_value;
    }

    neo<T> operator-() const
    {
        return -m_value;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator&=(neo<U> const& other)
    {
        m_value &= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator&=(U const& other)
    {
        m_value &= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator|=(neo<U> const& other)
    {
        m_value |= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator|=(U const& other)
    {
        m_value |= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator^=(neo<U> const& other)
    {
        m_value ^= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && is_widening<U, T>::value>>
    neo& operator^=(U const& other)
    {
        m_value ^= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
    neo& operator<<=(neo<U> const& other)
    {
        m_value <<= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
    neo& operator<<=(U const& other)
    {
        m_value <<= other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
    neo& operator>>=(neo<U> const& other)
    {
        m_value >>= U{other};
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
    neo& operator>>=(U const& other)
    {
        m_value >>= other;
        return *this;
    }

    template<typename = std::enable_if_t<is_unsigned_integral<T>::value>>
    neo operator~() const
    {
        return ~m_value;
    }
};

template<>
class neo<void>
{
};

template<>
class neo<bool>
{
private:
    bool m_value;

public:
    neo() :
        m_value()
    {
    }

    neo(undefined_t)
    {
    }

    neo(neo const& other) :
        m_value(other.m_value)
    {
    }

    neo& operator=(neo const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    neo(neo&& other) :
        m_value(other.m_value)
    {
        other.m_value = bool();
    }

    neo& operator=(neo&& other)
    {
        m_value = other.m_value;
        other.m_value = bool();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_bool<U>::value>>
    neo(U value) :
        m_value(value)
    {
    }

    operator bool() const
    {
        return m_value;
    }

    neo<bool> operator!() const
    {
        return !m_value;
    }

    bool const& get() const
    {
        return m_value;
    }

    bool& get()
    {
        return m_value;
    }
};

template<typename T>
class neo<T*>
{
private:
    T* m_value;

public:
    neo() :
        m_value()
    {
    }

    neo(undefined_t)
    {
    }

    neo(neo const& other) :
        m_value(other)
    {
    }

    neo& operator=(neo const& other)
    {
        m_value = other;
        return *this;
    }

    neo(neo&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    neo& operator=(neo&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    neo(neo<U*> const& other) :
        m_value(other)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    neo& operator=(neo<U*> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    neo(neo<U*>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    neo& operator=(neo<U*>&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    neo(std::nullptr_t) :
        m_value()
    {
    }

    neo& operator=(std::nullptr_t)
    {
        m_value = nullptr;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    neo(U* value) :
        m_value(value)
    {
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<T, U>::value>>
    neo& operator=(U* value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || std::is_base_of<U, T>::value || std::is_void<U>::value>>
    operator U*() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>, typename = void>
    explicit operator U*() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<U, T>::value || std::is_void<U>::value>>
    explicit operator neo<U*>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator neo<bool>() const
    {
        return static_cast<bool>(*this);
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

    T& operator[](neo<std::size_t> index) const
    {
        return (*this)[index.get()];
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::size_t>::value>>
    T& operator[](U const& index) const
    {
        return m_value[index];
    }

    neo& operator++()
    {
        ++m_value;
        return *this;
    }

    neo operator++(int)
    {
        return m_value++;
    }

    neo& operator--()
    {
        --m_value;
        return *this;
    }

    neo operator--(int)
    {
        return m_value--;
    }

    neo& operator+=(neo<std::ptrdiff_t> const& rhs)
    {
        return *this += rhs.get();
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
    neo& operator+=(U const& rhs)
    {
        m_value += rhs;
        return *this;
    }

    neo& operator-=(neo<std::ptrdiff_t> const& rhs)
    {
        return *this -= rhs.get();
    }

    template<typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
    neo& operator-=(U const& rhs)
    {
        m_value -= rhs;
        return *this;
    }
};

template<>
class neo<void*>
{
private:
    void* m_value;

public:
    neo() :
        m_value()
    {
    }

    neo(undefined_t)
    {
    }

    neo(neo const& other) :
        m_value(other)
    {
    }

    neo& operator=(neo const& other)
    {
        m_value = other;
        return *this;
    }

    neo(neo&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    neo& operator=(neo&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U>
    neo(neo<U*> const& other) :
        m_value(other)
    {
    }

    template<typename U>
    neo& operator=(neo<U*> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U>
    neo(neo<U*>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U>
    neo& operator=(neo<U*>&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    neo(std::nullptr_t) :
        m_value()
    {
    }

    neo& operator=(std::nullptr_t)
    {
        m_value = nullptr;
        return *this;
    }

    neo(void* value) :
        m_value(value)
    {
    }

    neo& operator=(void* value)
    {
        m_value = value;
        return *this;
    }

    operator void*() const
    {
        return m_value;
    }

    template<typename U>
    explicit operator U*() const
    {
        return static_cast<U*>(m_value);
    }

    template<typename U>
    explicit operator neo<U*>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator neo<bool>() const
    {
        return static_cast<bool>(*this);
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

template<>
class neo<void const*>
{
private:
    void const* m_value;

public:
    neo() :
        m_value()
    {
    }

    neo(undefined_t)
    {
    }

    neo(neo const& other) :
        m_value(other)
    {
    }

    neo& operator=(neo const& other)
    {
        m_value = other;
        return *this;
    }

    neo(neo&& other) :
        m_value(other.m_value)
    {
        other = nullptr;
    }

    neo& operator=(neo&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    template<typename U>
    neo(neo<U const*> const& other) :
        m_value(other)
    {
    }

    template<typename U>
    neo& operator=(neo<U const*> const& other)
    {
        m_value = other;
        return *this;
    }

    template<typename U>
    neo(neo<U const*>&& other) :
        m_value(other)
    {
        other = nullptr;
    }

    template<typename U>
    neo& operator=(neo<U const*>&& other)
    {
        m_value = other;
        other = nullptr;
        return *this;
    }

    neo(std::nullptr_t) :
        m_value()
    {
    }

    neo& operator=(std::nullptr_t)
    {
        m_value = nullptr;
        return *this;
    }

    neo(void const* value) :
        m_value(value)
    {
    }

    neo& operator=(void const* value)
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
    explicit operator neo<U const*>() const
    {
        return static_cast<U*>(m_value);
    }

    explicit operator bool() const
    {
        return m_value != nullptr;
    }

    explicit operator neo<bool>() const
    {
        return static_cast<bool>(*this);
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

// neo<T> - neo<T>
//-----------------

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator==(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} == U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator!=(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} != U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} < U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<=(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} <= U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} > U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>=(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} >= U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator+(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} + U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator-(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} - U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator*(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} * U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator/(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} / U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator%(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} % U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator&(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} & U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator|(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} | U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator^(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} ^ U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator<<(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} << U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator>>(neo<T> const& lhs, neo<U> const& rhs)
{
    return T{lhs} >> U{rhs};
}

// neo<T> - U
//------------

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator==(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} == rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator!=(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} != rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} < rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<=(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} <= rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} > rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>=(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} >= rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator+(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} + rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator-(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} - rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator*(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} * rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator/(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} / rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator%(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} % rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator&(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} & rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator|(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} | rhs;
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator^(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} ^ rhs;
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator<<(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} << rhs;
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator>>(neo<T> const& lhs, U const& rhs)
{
    return T{lhs} >> rhs;
}

// U - neo<T>
//------------

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator==(U const& lhs, neo<T> const& rhs)
{
    return lhs == T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar<T, U>::value>>
neo<bool> operator!=(U const& lhs, neo<T> const& rhs)
{
    return lhs != T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<(U const& lhs, neo<T> const& rhs)
{
    return lhs < T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator<=(U const& lhs, neo<T> const& rhs)
{
    return lhs <= T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>(U const& lhs, neo<T> const& rhs)
{
    return lhs > T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<bool> operator>=(U const& lhs, neo<T> const& rhs)
{
    return lhs >= T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator+(U const& lhs, neo<T> const& rhs)
{
    return lhs + T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator-(U const& lhs, neo<T> const& rhs)
{
    return lhs - T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator*(U const& lhs, neo<T> const& rhs)
{
    return lhs * T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator/(U const& lhs, neo<T> const& rhs)
{
    return lhs / T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_numeric<T, U>::value>>
neo<wider_t<T, U>> operator%(U const& lhs, neo<T> const& rhs)
{
    return lhs % T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator&(U const& lhs, neo<T> const& rhs)
{
    return lhs & T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator|(U const& lhs, neo<T> const& rhs)
{
    return lhs | T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<are_similar_unsigned_integral<T, U>::value>>
neo<wider_t<T, U>> operator^(U const& lhs, neo<T> const& rhs)
{
    return lhs ^ T{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator<<(T const& lhs, neo<U> const& rhs)
{
    return lhs << U{rhs};
}

template<typename T, typename U, typename = std::enable_if_t<is_unsigned_integral<T>::value && std::is_integral<U>::value>>
neo<T> operator>>(T const& lhs, neo<U> const& rhs)
{
    return lhs >> U{rhs};
}

// Deleted neo<bool> boolean operations
//--------------------------------------

template<typename T, typename = std::enable_if_t<is_numeric<T>::value>>
bool operator&&(neo<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = std::enable_if_t<is_numeric<T>::value>>
bool operator||(neo<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = std::enable_if_t<is_numeric<T>::value>>
bool operator&&(T const& lhs, neo<bool> const& rhs) = delete;

template<typename T, typename = std::enable_if_t<is_numeric<T>::value>>
bool operator||(T const& lhs, neo<bool> const& rhs) = delete;

// neo<T*> - std::nullptr_t
//--------------------------

template<typename T>
neo<bool> operator==(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} == nullptr;
}

template<typename T>
neo<bool> operator!=(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} != nullptr;
}

template<typename T>
neo<bool> operator<(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} < nullptr;
}

template<typename T>
neo<bool> operator<=(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} <= nullptr;
}

template<typename T>
neo<bool> operator>(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} > nullptr;
}

template<typename T>
neo<bool> operator>=(neo<T*> const& lhs, std::nullptr_t)
{
    return (T*){lhs} >= nullptr;
}

// std::nullptr_t - neo<T*>
//--------------------------

template<typename T>
neo<bool> operator==(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr == (T*){rhs};
}

template<typename T>
neo<bool> operator!=(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr != (T*){rhs};
}

template<typename T>
neo<bool> operator<(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr < (T*){rhs};
}

template<typename T>
neo<bool> operator<=(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr <= (T*){rhs};
}

template<typename T>
neo<bool> operator>(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr > (T*){rhs};
}

template<typename T>
neo<bool> operator>=(std::nullptr_t, neo<T*> const& rhs)
{
    return nullptr >= (T*){rhs};
}

// Pointer arithmetic
//--------------------

template<typename T>
neo<T*> operator+(neo<T*> const& lhs, neo<std::ptrdiff_t> const& rhs)
{
    return lhs + rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
neo<T*> operator+(neo<T*> const& lhs, U const& rhs)
{
    return lhs.get() + rhs;
}

template<typename T>
neo<T*> operator+(neo<std::ptrdiff_t> const& lhs, neo<T*> const& rhs)
{
    return lhs.get() + rhs;
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
neo<T*> operator+(U const& lhs, neo<T*> const& rhs)
{
    return lhs + rhs.get();
}

template<typename T>
neo<T*> operator-(neo<T*> const& lhs, neo<std::ptrdiff_t> const& rhs)
{
    return lhs - rhs.get();
}

template<typename T, typename U, typename = std::enable_if_t<std::is_same<U, std::ptrdiff_t>::value>>
neo<T*> operator-(neo<T*> const& lhs, U const& rhs)
{
    return lhs.get() - rhs;
}

template<typename T>
neo<std::ptrdiff_t> operator-(neo<T*> const& lhs, neo<T*> const& rhs)
{
    return (T*){lhs} - (T*){rhs};
}

template<typename T>
neo<std::ptrdiff_t> operator-(neo<T*> const& lhs, T* rhs)
{
    return (T*){lhs} - rhs;
}

template<typename T>
neo<std::ptrdiff_t> operator-(T* lhs, neo<T*> const& rhs)
{
    return lhs - (T*){rhs};
}

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, neo<T> const& v)
{
    s << T{v};
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, neo<T>& v)
{
    s >> static_cast<T&>(v);
    return s;
}

using neo_void = neo<void>;

using neo_bool = neo<bool>;

using neo_byte = neo<char>;
using neo_short = neo<short>;
using neo_int = neo<int>;
using neo_long = neo<long>;
using neo_llong = neo<long long>;

using neo_ubyte = neo<unsigned char>;
using neo_ushort = neo<unsigned short>;
using neo_uint = neo<unsigned int>;
using neo_ulong = neo<unsigned long>;
using neo_ullong = neo<unsigned long long>;

using neo_float = neo<float>;
using neo_double = neo<double>;
using neo_ldouble = neo<long double>;

using neo_size = neo<std::size_t>;
using neo_ptrdiff = neo<std::ptrdiff_t>;
using neo_max_align = neo<std::max_align_t>;

using neo_char = neo<char>;
using neo_uchar = neo<unsigned char>;
using neo_wchar = neo<wchar_t>;
using neo_char16 = neo<char16_t>;
using neo_char32 = neo<char32_t>;

template<typename T>
using neo_ptr = neo<T*>;

} // namespace neo_types

#endif // NEO_TYPES_HPP
