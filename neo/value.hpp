/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_VALUE_HPP
#define NEO_VALUE_HPP

#include <neo/undefined.hpp>

#include <neo/detail/type_traits.hpp>

#include <cstddef>
#include <iosfwd>
#include <limits>

namespace neo
{

namespace detail
{

template<typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template<typename T1, typename T2>
struct is_same : std::integral_constant<bool,
        std::is_same<remove_cv_t<T1>, remove_cv_t<T2>>::value
    >
{
};

template<typename T>
struct is_numeric : std::integral_constant<bool,
        std::is_arithmetic<T>::value & !is_same<T, bool>::value
    >
{
};

template<typename T>
struct is_unsigned_integral: std::integral_constant<bool,
        std::is_integral<T>::value &&
        std::is_unsigned<T>::value &&
        !is_same<T, bool>::value
    >
{
};

template<typename T1, typename T2>
struct is_bounded : std::integral_constant<bool,
        std::numeric_limits<T1>::lowest() >= std::numeric_limits<T2>::lowest() &&
        std::numeric_limits<T1>::max() <= std::numeric_limits<T2>::max()
    >
{
};

template<typename T1, typename T2>
struct are_numeric: std::integral_constant<bool,
        is_numeric<T1>::value && is_numeric<T2>::value
    >
{
};

template<typename T1, typename T2>
struct are_unsigned_integral: std::integral_constant<bool,
        is_unsigned_integral<T1>::value && is_unsigned_integral<T2>::value
    >
{
};

template<typename T1, typename T2>
struct are_similar: std::integral_constant<bool,
        are_numeric<T1, T2>::value &&
        std::is_integral<T1>::value == std::is_integral<T2>::value &&
        std::is_floating_point<T1>::value == std::is_floating_point<T2>::value &&
        std::is_signed<T1>::value == std::is_signed<T2>::value
    >
{
};

template<typename T1, typename T2>
struct are_similar_numeric: std::integral_constant<bool,
        are_numeric<T1, T2>::value && are_similar<T1, T2>::value
    >
{
};

template<typename From, typename To, bool =
    are_similar<From, To>::value
>
struct is_safely_convertible : std::integral_constant<bool,
        is_bounded<From, To>::value
    >
{
};

template<typename From, typename To>
struct is_safely_convertible<From, To, false> : std::false_type
{
};

template<typename From, typename To, bool =
    are_numeric<From, To>::value
>
struct is_unsafely_convertible : std::integral_constant<bool,
        !are_similar<From, To>::value ||
        !is_bounded<From, To>::value
    >
{
};

template<typename From, typename To>
struct is_unsafely_convertible<From, To, false> : std::false_type
{
};

template<typename T1, typename T2, bool =
    is_safely_convertible<T1, T2>::value
>
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

} // namespace detail

template<typename T>
class value
{
private:
    T m_value;

public:
    value() :
        m_value()
    {
    }

    value(undefined_t)
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value(value<U> const& other) :
        m_value(other.get())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator=(value<U> const& other)
    {
        m_value = other.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value(U const& value) :
        m_value(value)
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator=(U const& value)
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<T, U>::value>
    >
    operator U const&() const
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<T, U>::value>
    >
    operator U&()
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<T, U>::value>
    >
    operator U() const
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsafely_convertible<T, U>::value>,
        typename = void
    >
    explicit operator U() const
    {
        return static_cast<U>(m_value);
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsafely_convertible<T, U>::value>
    >
    explicit operator value<U>() const
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

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator+=(value<U> const& rhs)
    {
        m_value += rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator+=(U const& rhs)
    {
        m_value += rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator-=(value<U> const& rhs)
    {
        m_value -= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator-=(U const& rhs)
    {
        m_value -= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator*=(value<U> const& rhs)
    {
        m_value *= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator*=(U const& rhs)
    {
        m_value *= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator/=(value<U> const& rhs)
    {
        m_value /= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator/=(U const& rhs)
    {
        m_value /= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator%=(value<U> const& rhs)
    {
        m_value %= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator%=(U const& rhs)
    {
        m_value %= rhs;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value& operator++()
    {
        ++m_value;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value operator++(int)
    {
        return m_value++;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value& operator--()
    {
        --m_value;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value operator--(int)
    {
        return m_value--;
    }

    value<T> operator+() const
    {
        return m_value;
    }

    value<T> operator-() const
    {
        return -m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator&=(value<U> const& rhs)
    {
        m_value &= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator&=(U const& rhs)
    {
        m_value &= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator|=(value<U> const& rhs)
    {
        m_value |= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator|=(U const& rhs)
    {
        m_value |= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator^=(value<U> const& rhs)
    {
        m_value ^= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator^=(U const& rhs)
    {
        m_value ^= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator<<=(value<U> const& rhs)
    {
        m_value <<= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator<<=(U const& rhs)
    {
        m_value <<= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator>>=(value<U> const& rhs)
    {
        m_value >>= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator>>=(U const& rhs)
    {
        m_value >>= rhs;
        return *this;
    }

    template<typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value>>
    value operator~() const
    {
        return ~m_value;
    }
};

template<>
class value<bool>
{
private:
    bool m_value;

public:
    value() :
        m_value()
    {
    }

    value(undefined_t)
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<U, bool>::value>
    >
    value(U const& value) :
        m_value(value)
    {
    }

    operator bool const&() const
    {
        return m_value;
    }

    operator bool&()
    {
        return m_value;
    }

    value<bool> operator!() const
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

template<>
class value<void>
{
};

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator==(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator!=(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() < rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<=(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() <= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() > rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>=(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() >= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator+(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() + rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator-(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() - rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator*(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() * rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator/(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() / rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator%(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() % rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator&(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() & rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator|(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() | rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator^(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() ^ rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator<<(value<T1> const& lhs, value<T2> const& rhs)
{
    return detail::wider_t<T1, T2>(lhs.get() << rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator>>(value<T1> const& lhs, value<T2> const& rhs)
{
    return lhs.get() >> rhs.get();
}

// value<T1> - T2
//------------

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator==(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() == rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator!=(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() != rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() < rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<=(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() <= rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() > rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>=(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() >= rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator+(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() + rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator-(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() - rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator*(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() * rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator/(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() / rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator%(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() % rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator&(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() & rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator|(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() | rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator^(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() ^ rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator<<(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() << rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator>>(value<T1> const& lhs, T2 const& rhs)
{
    return lhs.get() >> rhs;
}

// T1 - value<T2>
//------------

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator==(T1 const& lhs, value<T2> const& rhs)
{
    return lhs == rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
value<bool> operator!=(T1 const& lhs, value<T2> const& rhs)
{
    return lhs != rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<(T1 const& lhs, value<T2> const& rhs)
{
    return lhs < rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator<=(T1 const& lhs, value<T2> const& rhs)
{
    return lhs <= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>(T1 const& lhs, value<T2> const& rhs)
{
    return lhs > rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<bool> operator>=(T1 const& lhs, value<T2> const& rhs)
{
    return lhs >= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator+(T1 const& lhs, value<T2> const& rhs)
{
    return lhs + rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator-(T1 const& lhs, value<T2> const& rhs)
{
    return lhs - rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator*(T1 const& lhs, value<T2> const& rhs)
{
    return lhs * rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator/(T1 const& lhs, value<T2> const& rhs)
{
    return lhs / rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator%(T1 const& lhs, value<T2> const& rhs)
{
    return lhs % rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator&(T1 const& lhs, value<T2> const& rhs)
{
    return lhs & rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator|(T1 const& lhs, value<T2> const& rhs)
{
    return lhs | rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
value<detail::wider_t<T1, T2>> operator^(T1 const& lhs, value<T2> const& rhs)
{
    return lhs ^ rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator<<(T1 const& lhs, value<T2> const& rhs)
{
    return lhs << rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
value<T1> operator>>(T1 const& lhs, value<T2> const& rhs)
{
    return lhs >> rhs.get();
}

// Deleted value<bool> boolean operations
//--------------------------------------

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
bool operator&&(value<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
bool operator||(value<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
bool operator&&(T const& lhs, value<bool> const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
bool operator||(T const& lhs, value<bool> const& rhs) = delete;

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, value<T> const& v)
{
    s << v.get();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, value<T>& v)
{
    s >> v.get();
    return s;
}

// Utilities
//-----------

template<typename T>
value<T> make_value(T const& value)
{
    return value;
}

namespace aliases
{

using neo_void = value<void>;

using neo_bool = value<bool>;

using neo_byte = value<char>;
using neo_short = value<short>;
using neo_int = value<int>;
using neo_long = value<long>;
using neo_llong = value<long long>;

using neo_ubyte = value<unsigned char>;
using neo_ushort = value<unsigned short>;
using neo_uint = value<unsigned int>;
using neo_ulong = value<unsigned long>;
using neo_ullong = value<unsigned long long>;

using neo_float = value<float>;
using neo_double = value<double>;
using neo_ldouble = value<long double>;

using neo_size = value<std::size_t>;
using neo_ptrdiff = value<std::ptrdiff_t>;
using neo_max_align = value<std::max_align_t>;

using neo_char = value<char>;
using neo_wchar = value<wchar_t>;
using neo_char16 = value<char16_t>;
using neo_char32 = value<char32_t>;

} // namespace aliases

using namespace aliases;

namespace literals
{

namespace detail
{

struct integral_literal_builder
{
    template<typename T, unsigned long long Value, char Digit, char... Digits>
    static constexpr value<T> build()
    {
        static_assert(Digit >= '0' && Digit <= '9',
                "literal may contain only numeric characters");
        return build<T, 10 * Value + Digit - '0', Digits...>();
    }

    template<typename T, unsigned long long Value>
    static constexpr value<T> build()
    {
        static_assert(Value <= std::numeric_limits<T>::max(),
                "literal exceeds max value");
        return static_cast<T>(Value);
    }
};

template<typename T, char... Digits>
constexpr value<T> build_integral_literal()
{
    static_assert(sizeof...(Digits) <= std::numeric_limits<T>::digits10 + 1,
            "literal exceeds max value");
    return integral_literal_builder::build<T, 0, Digits...>();
}

} // namespace detail

static const value<bool> neo_true = true;
static const value<bool> neo_false = false;

template<char... Digits>
constexpr value<char> operator"" _nb()
{
    return detail::build_integral_literal<char, Digits...>();
}

template<char... Digits>
constexpr value<short> operator"" _ns()
{
    return detail::build_integral_literal<short, Digits...>();
}

template<char... Digits>
constexpr value<int> operator"" _ni()
{
    return detail::build_integral_literal<int, Digits...>();
}

template<char... Digits>
constexpr value<long> operator"" _nl()
{
    return detail::build_integral_literal<long, Digits...>();
}

template<char... Digits>
constexpr value<long long> operator"" _nll()
{
    return detail::build_integral_literal<long long, Digits...>();
}

template<char... Digits>
constexpr value<unsigned char> operator"" _nub()
{
    return detail::build_integral_literal<unsigned char, Digits...>();
}

template<char... Digits>
constexpr value<unsigned short> operator"" _nus()
{
    return detail::build_integral_literal<unsigned short, Digits...>();
}

template<char... Digits>
constexpr value<unsigned int> operator"" _nui()
{
    return detail::build_integral_literal<unsigned int, Digits...>();
}

template<char... Digits>
constexpr value<unsigned long> operator"" _nul()
{
    return detail::build_integral_literal<unsigned long, Digits...>();
}

template<char... Digits>
constexpr value<unsigned long long> operator"" _null()
{
    return detail::build_integral_literal<unsigned long long, Digits...>();
}

inline value<float> operator"" _nf(long double value)
{
    return static_cast<float>(value);
}

inline value<double> operator"" _nd(long double value)
{
    return static_cast<double>(value);
}

inline value<long double> operator"" _nld(long double value)
{
    return value;
}

template<char... Digits>
constexpr value<std::size_t> operator"" _nz()
{
    return detail::build_integral_literal<std::size_t, Digits...>();
}

template<char... Digits>
constexpr value<std::ptrdiff_t> operator"" _npd()
{
    return detail::build_integral_literal<std::ptrdiff_t, Digits...>();
}

template<char... Digits>
constexpr value<std::max_align_t> operator"" _nma()
{
    return detail::build_integral_literal<std::max_align_t, Digits...>();
}

inline value<char> operator"" _n(char value)
{
    return value;
}

inline value<wchar_t> operator"" _n(wchar_t value)
{
    return value;
}

inline value<char16_t> operator"" _n(char16_t value)
{
    return value;
}

inline value<char32_t> operator"" _n(char32_t value)
{
    return value;
}

} // namespace literals

} // namespace neo

#endif // NEO_VALUE_HPP
