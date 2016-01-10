#pragma once

#ifndef NEO_TYPES_HPP
#define NEO_TYPES_HPP

#include <iosfwd>
#include <limits>
#include <type_traits>

namespace neo_types
{

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

template<typename, typename, typename = void>
struct is_convertible
{
    static constexpr bool value = false;
};

template<typename From, typename To>
struct is_convertible<From, To, typename std::enable_if<
    std::is_same<std::remove_cv_t<From>, std::remove_cv_t<To>>::value ||
    std::is_integral<From>::value == std::is_integral<To>::value &&
    std::is_floating_point<From>::value == std::is_floating_point<To>::value &&
    std::is_signed<From>::value == std::is_signed<To>::value
    >::type>
{
    static constexpr bool value = is_bounded<From, To>::value;
};

template<typename A, typename B>
struct is_either_bool
{
    static constexpr bool value =
        std::is_same<std::remove_cv_t<A>, bool>::value ||
        std::is_same<std::remove_cv_t<B>, bool>::value;
};

template<typename From, typename To>
struct is_widening
{
    static constexpr bool value =
        is_convertible<From, To>::value &&
        !is_either_bool<From, To>::value;
};

template<typename From, typename To>
struct is_narrowing
{
    static constexpr bool value =
        !is_convertible<From, To>::value &&
        !is_either_bool<From, To>::value;
};

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
        m_value = std::move(other.m_value);
        other.m_value = T();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<is_widening<U, T>::value>>
    neo(U value) :
        m_value(value)
    {
    }

    template<typename U, typename = std::enable_if_t<is_widening<T, U>::value>>
    operator U() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<is_widening<T, U>::value>>
    operator neo<U>() const
    {
        return m_value;
    }

    template<typename U, typename = std::enable_if_t<is_narrowing<T, U>::value>, typename = void>
    explicit operator U() const
    {
        return static_cast<U>(m_value);
    }

    template<typename U, typename = std::enable_if_t<is_narrowing<T, U>::value>, typename = void>
    explicit operator neo<U>() const
    {
        return static_cast<U>(m_value);
    }
};

// neo<T> - neo<T>
//-----------------

template<typename T>
bool operator==(neo<T> const& lhs, neo<T> const& rhs)
{
    return static_cast<T>(lhs) == static_cast<T>(rhs);
}

template<typename T>
bool operator!=(neo<T> const& lhs, neo<T> const& rhs)
{
    return static_cast<T>(lhs) != static_cast<T>(rhs);
}

template<typename T>
neo<T> operator+(neo<T> const& lhs, neo<T> const& rhs)
{
    return neo<T>(static_cast<T>(lhs) + static_cast<T>(rhs));
}

template<typename T>
neo<T> operator-(neo<T> const& lhs, neo<T> const& rhs)
{
    return neo<T>(static_cast<T>(lhs) - static_cast<T>(rhs));
}

template<typename T>
neo<T> operator*(neo<T> const& lhs, neo<T> const& rhs)
{
    return neo<T>(static_cast<T>(lhs) * static_cast<T>(rhs));
}

template<typename T>
neo<T> operator/(neo<T> const& lhs, neo<T> const& rhs)
{
    return neo<T>(static_cast<T>(lhs) / static_cast<T>(rhs));
}

template<typename T>
neo<T> operator%(neo<T> const& lhs, neo<T> const& rhs)
{
    return neo<T>(static_cast<T>(lhs) % static_cast<T>(rhs));
}

// neo<T> - T
//------------

template<typename T, typename U>
bool operator==(neo<T> const& lhs, U const& rhs)
{
    return static_cast<T>(lhs) == rhs;
}

template<typename T, typename U>
bool operator!=(neo<T> const& lhs, U const& rhs)
{
    return static_cast<T>(lhs) != rhs;
}

template<typename T, typename U>
neo<T> operator+(neo<T> const& lhs, U const& rhs)
{
    return neo<T>(static_cast<T>(lhs) + rhs);
}

template<typename T, typename U>
neo<T> operator-(neo<T> const& lhs, U const& rhs)
{
    return neo<T>(static_cast<T>(lhs) - rhs);
}

template<typename T, typename U>
neo<T> operator*(neo<T> const& lhs, U const& rhs)
{
    return neo<T>(static_cast<T>(lhs) * rhs);
}

template<typename T, typename U>
neo<T> operator/(neo<T> const& lhs, U const& rhs)
{
    return neo<T>(static_cast<T>(lhs) / rhs);
}

template<typename T, typename U>
neo<T> operator%(neo<T> const& lhs, U const& rhs)
{
    return neo<T>(static_cast<T>(lhs) % rhs);
}

// IOStream
//----------

template<typename T>
std::ostream& operator<<(std::ostream& s, neo<T> const& v)
{
    s << static_cast<T>(v);
    return s;
}

using neo_char = neo<char>;
using neo_short = neo<short>;
using neo_int = neo<int>;
using neo_long = neo<long>;
using neo_long_long = neo<long long>;
using neo_float = neo<float>;
using neo_double = neo<double>;

} // namespace simi

#endif // NEO_TYPES_HPP
