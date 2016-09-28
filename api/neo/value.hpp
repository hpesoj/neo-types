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
    constexpr value() noexcept :
        m_value()
    {
    }

    value(undefined_t) noexcept
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    constexpr value(value<U> const& other) noexcept :
        m_value(other.get())
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator=(value<U> const& other) noexcept
    {
        m_value = other.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    constexpr value(U const& value) noexcept :
        m_value(value)
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator=(U const& value) noexcept
    {
        m_value = value;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<T, U>::value>
    >
    constexpr operator U const&() const noexcept
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<T, U>::value>
    >
    operator U&() noexcept
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<T, U>::value>
    >
    constexpr operator U() const noexcept
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsafely_convertible<T, U>::value>,
        typename = void
    >
    constexpr explicit operator U() const noexcept
    {
        return static_cast<U>(m_value);
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsafely_convertible<T, U>::value>
    >
    constexpr explicit operator value<U>() const noexcept
    {
        return static_cast<U>(m_value);
    }

    constexpr T const& get() const noexcept
    {
        return m_value;
    }

    T& get() noexcept
    {
        return m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator+=(value<U> const& rhs) noexcept
    {
        m_value += rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator+=(U const& rhs) noexcept
    {
        m_value += rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator-=(value<U> const& rhs) noexcept
    {
        m_value -= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator-=(U const& rhs) noexcept
    {
        m_value -= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator*=(value<U> const& rhs) noexcept
    {
        m_value *= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator*=(U const& rhs) noexcept
    {
        m_value *= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator/=(value<U> const& rhs) noexcept
    {
        m_value /= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator/=(U const& rhs) noexcept
    {
        m_value /= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator%=(value<U> const& rhs) noexcept
    {
        m_value %= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator%=(U const& rhs) noexcept
    {
        m_value %= rhs;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value& operator++() noexcept
    {
        ++m_value;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value operator++(int) noexcept
    {
        return m_value++;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value& operator--() noexcept
    {
        --m_value;
        return *this;
    }

    template<typename = detail::enable_if_t<
        std::is_integral<T>::value>
    >
    value operator--(int) noexcept
    {
        return m_value--;
    }

    constexpr value<T> operator+() const noexcept
    {
        return m_value;
    }

    constexpr value<T> operator-() const noexcept
    {
        return -m_value;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator&=(value<U> const& rhs) noexcept
    {
        m_value &= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator&=(U const& rhs) noexcept
    {
        m_value &= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator|=(value<U> const& rhs) noexcept
    {
        m_value |= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator|=(U const& rhs) noexcept
    {
        m_value |= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator^=(value<U> const& rhs) noexcept
    {
        m_value ^= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        detail::is_safely_convertible<U, T>::value>
    >
    value& operator^=(U const& rhs) noexcept
    {
        m_value ^= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator<<=(value<U> const& rhs) noexcept
    {
        m_value <<= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator<<=(U const& rhs) noexcept
    {
        m_value <<= rhs;
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator>>=(value<U> const& rhs) noexcept
    {
        m_value >>= rhs.get();
        return *this;
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value &&
        std::is_integral<U>::value>
    >
    value& operator>>=(U const& rhs) noexcept
    {
        m_value >>= rhs;
        return *this;
    }

    template<typename = detail::enable_if_t<
        detail::is_unsigned_integral<T>::value>>
    constexpr value operator~() const noexcept
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
    constexpr value() noexcept :
        m_value()
    {
    }

    value(undefined_t) noexcept
    {
    }

    template<typename U, typename = detail::enable_if_t<
        detail::is_same<U, bool>::value>
    >
    constexpr value(U const& value) noexcept :
        m_value(value)
    {
    }

    constexpr operator bool const&() const noexcept
    {
        return m_value;
    }

    operator bool&() noexcept
    {
        return m_value;
    }

    constexpr value<bool> operator!() const noexcept
    {
        return !m_value;
    }

    constexpr bool const& get() const noexcept
    {
        return m_value;
    }

    bool& get() noexcept
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
constexpr value<bool> operator==(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
constexpr value<bool> operator!=(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() < rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<=(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() <= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() > rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>=(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() >= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator+(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() + rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator-(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() - rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator*(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() * rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator/(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() / rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator%(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() % rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator&(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() & rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator|(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() | rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator^(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() ^ rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator<<(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return detail::wider_t<T1, T2>(lhs.get() << rhs.get());
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator>>(value<T1> const& lhs, value<T2> const& rhs) noexcept
{
    return lhs.get() >> rhs.get();
}

// value<T1> - T2
//------------

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
constexpr value<bool> operator==(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() == rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
constexpr value<bool> operator!=(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() != rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() < rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<=(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() <= rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() > rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>=(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() >= rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator+(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() + rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator-(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() - rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator*(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() * rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator/(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() / rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator%(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() % rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator&(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() & rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator|(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() | rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator^(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() ^ rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator<<(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() << rhs;
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator>>(value<T1> const& lhs, T2 const& rhs) noexcept
{
    return lhs.get() >> rhs;
}

// T1 - value<T2>
//------------

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
constexpr value<bool> operator==(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs == rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar<T1, T2>::value>
>
constexpr value<bool> operator!=(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs != rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs < rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator<=(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs <= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs > rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<bool> operator>=(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs >= rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator+(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs + rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator-(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs - rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator*(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs * rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator/(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs / rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_similar_numeric<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator%(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs % rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator&(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs & rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator|(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs | rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::are_unsigned_integral<T1, T2>::value>
>
constexpr value<detail::wider_t<T1, T2>> operator^(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs ^ rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator<<(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs << rhs.get();
}

template<typename T1, typename T2, typename = detail::enable_if_t<
    detail::is_unsigned_integral<T1>::value &&
    std::is_integral<T2>::value>
>
constexpr value<T1> operator>>(T1 const& lhs, value<T2> const& rhs) noexcept
{
    return lhs >> rhs.get();
}

// Deleted value<bool> boolean operations
//--------------------------------------

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
constexpr bool operator&&(value<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
constexpr bool operator||(value<bool> const& lhs, T const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
constexpr bool operator&&(T const& lhs, value<bool> const& rhs) = delete;

template<typename T, typename = detail::enable_if_t<
    detail::is_numeric<T>::value>
>
constexpr bool operator||(T const& lhs, value<bool> const& rhs) = delete;

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
constexpr value<T> make_value(T const& value) noexcept
{
    return value;
}

using void_ = value<void>;

using bool_ = value<bool>;

using byte = value<char>;
using short_ = value<short>;
using int_ = value<int>;
using long_ = value<long>;
using llong = value<long long>;

using ubyte = value<unsigned char>;
using ushort = value<unsigned short>;
using uint = value<unsigned int>;
using ulong = value<unsigned long>;
using ullong = value<unsigned long long>;

using float_ = value<float>;
using double_ = value<double>;
using ldouble = value<long double>;

using size = value<std::size_t>;
using ptrdiff = value<std::ptrdiff_t>;
using max_align = value<std::max_align_t>;

using char_ = value<char>;
using wchar = value<wchar_t>;
using char16 = value<char16_t>;
using char32 = value<char32_t>;

namespace literals
{

namespace detail
{

struct integral_literal_builder
{
    template<typename T, unsigned long long Value, char Digit, char... Digits>
    static constexpr value<T> build() noexcept
    {
        static_assert(Digit >= '0' && Digit <= '9',
                "literal may contain only numeric characters");
        return build<T, 10 * Value + Digit - '0', Digits...>();
    }

    template<typename T, unsigned long long Value>
    static constexpr value<T> build() noexcept
    {
        static_assert(Value <= std::numeric_limits<T>::max(),
                "literal exceeds max value");
        return static_cast<T>(Value);
    }
};

template<typename T, char... Digits>
constexpr value<T> build_integral_literal() noexcept
{
    static_assert(sizeof...(Digits) <= std::numeric_limits<T>::digits10 + 1,
            "literal exceeds max value");
    return integral_literal_builder::build<T, 0, Digits...>();
}

} // namespace detail

static const value<bool> true_ = true;
static const value<bool> false_ = false;

template<char... Digits>
constexpr value<char> operator"" _nb() noexcept
{
    return detail::build_integral_literal<char, Digits...>();
}

template<char... Digits>
constexpr value<short> operator"" _ns() noexcept
{
    return detail::build_integral_literal<short, Digits...>();
}

template<char... Digits>
constexpr value<int> operator"" _ni() noexcept
{
    return detail::build_integral_literal<int, Digits...>();
}

template<char... Digits>
constexpr value<long> operator"" _nl() noexcept
{
    return detail::build_integral_literal<long, Digits...>();
}

template<char... Digits>
constexpr value<long long> operator"" _nll() noexcept
{
    return detail::build_integral_literal<long long, Digits...>();
}

template<char... Digits>
constexpr value<unsigned char> operator"" _nub() noexcept
{
    return detail::build_integral_literal<unsigned char, Digits...>();
}

template<char... Digits>
constexpr value<unsigned short> operator"" _nus() noexcept
{
    return detail::build_integral_literal<unsigned short, Digits...>();
}

template<char... Digits>
constexpr value<unsigned int> operator"" _nui() noexcept
{
    return detail::build_integral_literal<unsigned int, Digits...>();
}

template<char... Digits>
constexpr value<unsigned long> operator"" _nul() noexcept
{
    return detail::build_integral_literal<unsigned long, Digits...>();
}

template<char... Digits>
constexpr value<unsigned long long> operator"" _null() noexcept
{
    return detail::build_integral_literal<unsigned long long, Digits...>();
}

constexpr inline value<float> operator"" _nf(long double value) noexcept
{
    return static_cast<float>(value);
}

constexpr inline value<double> operator"" _nd(long double value) noexcept
{
    return static_cast<double>(value);
}

constexpr inline value<long double> operator"" _nld(long double value) noexcept
{
    return value;
}

template<char... Digits>
constexpr value<std::size_t> operator"" _nz() noexcept
{
    return detail::build_integral_literal<std::size_t, Digits...>();
}

template<char... Digits>
constexpr value<std::ptrdiff_t> operator"" _npd() noexcept
{
    return detail::build_integral_literal<std::ptrdiff_t, Digits...>();
}

template<char... Digits>
constexpr value<std::max_align_t> operator"" _nma() noexcept
{
    return detail::build_integral_literal<std::max_align_t, Digits...>();
}

constexpr inline value<char> operator"" _n(char value) noexcept
{
    return value;
}

constexpr inline value<wchar_t> operator"" _n(wchar_t value) noexcept
{
    return value;
}

constexpr inline value<char16_t> operator"" _n(char16_t value) noexcept
{
    return value;
}

constexpr inline value<char32_t> operator"" _n(char32_t value) noexcept
{
    return value;
}

} // namespace literals

} // namespace neo

#endif // NEO_VALUE_HPP
