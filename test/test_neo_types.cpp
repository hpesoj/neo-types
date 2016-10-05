#include <neo/neo.hpp>
#include <operator_traits.hpp>
#include <catch.hpp>

#include <cmath>
#include <iostream>
#include <type_traits>

using namespace neo::literals;
using namespace neo_types::operator_traits;

using byte = char;
using llong = long long;

using ubyte = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ullong = unsigned long long;

using ldouble = long double;

bool is_zero(int v)
{
    return v == 0;
}

bool is_zero(llong v)
{
    return v == 0;
}

bool is_zero_2(neo::int_ v)
{
    return v == 0;
}

bool is_zero_2(neo::llong v)
{
    return v == 0;
}

struct base
{
};

struct derived : base
{
};

struct counter
{
    neo::int_ value;

    counter(neo::int_ i) :
        value(i)
    {
        std::cout << "constructed\n";
    }

    counter(counter const&)
    {
        std::cout << "copy constructed\n";
    }

    counter& operator=(counter const&)
    {
        std::cout << "copy assigned\n";
        return *this;
    }

    counter(counter&&)
    {
        std::cout << "move constructed\n";
    }

    counter& operator=(counter&&)
    {
        std::cout << "move assigned\n";
        return *this;
    }

    ~counter()
    {
        std::cout << "destructed\n";
    }
};

TEST_CASE("neo<T> is the same size as T", "neo<T>")
{
    //something(neo_array<int, 4>(1, 2, 3, 4));
    //something(neo_array<int, 4>(42));

    //neo_array<counter, 4> counts;
    //neo_array<counter, 1> counts(counter(6));
    //neo_array<counter, 4> counts(counter(1), counter(2), counter(3), counter(4));
    //std::cout << counts[0u].value << std::endl;
    //counter counts[4] = { counter(1), counter(2), counter(3), counter(4) };

    auto ff = 1.0f;
    //auto f = neo_addr(ff);
    //f += 0;
    //f[0u];

    base b0;
    derived d0;
    neo::ptr<base> b1 = &d0;
    neo::ptr<derived> d1 = &d0;

    derived ds[10];
    //neo<derived[10]> d10 = ds;

    //b1 - d1;
    //byte b = static_cast<neo::byte>(neo::int_());

    //is_zero_2(1);

    //is_zero_2(int());

    neo::int_ i = 42;
    i = 0;
    CHECK(is_zero(i.get()));

    CHECK(sizeof(neo::void_) == 1u);

    CHECK(sizeof(neo::bool_) == sizeof(bool));

    CHECK(sizeof(neo::byte) == sizeof(byte));
    CHECK(sizeof(neo::short_) == sizeof(short));
    CHECK(sizeof(neo::int_) == sizeof(int));
    CHECK(sizeof(neo::long_) == sizeof(long));
    CHECK(sizeof(neo::llong) == sizeof(llong));

    CHECK(sizeof(neo::ubyte) == sizeof(ubyte));
    CHECK(sizeof(neo::ushort) == sizeof(ushort));
    CHECK(sizeof(neo::uint) == sizeof(uint));
    CHECK(sizeof(neo::ulong) == sizeof(ulong));
    CHECK(sizeof(neo::ullong) == sizeof(ullong));

    CHECK(sizeof(neo::float_) == sizeof(float));
    CHECK(sizeof(neo::double_) == sizeof(double));
    CHECK(sizeof(neo::ldouble) == sizeof(ldouble));

    CHECK(sizeof(neo::ptr<int>) == sizeof(int*));
}

TEST_CASE("neo<T> is trivially copyable", "neo<T>")
{
    CHECK(std::is_trivially_copyable<neo::void_>::value);

    CHECK(std::is_trivially_copyable<neo::bool_>::value);

    CHECK(std::is_trivially_copyable<neo::byte>::value);
    CHECK(std::is_trivially_copyable<neo::short_>::value);
    CHECK(std::is_trivially_copyable<neo::int_>::value);
    CHECK(std::is_trivially_copyable<neo::long_>::value);
    CHECK(std::is_trivially_copyable<neo::llong>::value);

    CHECK(std::is_trivially_copyable<neo::ubyte>::value);
    CHECK(std::is_trivially_copyable<neo::ushort>::value);
    CHECK(std::is_trivially_copyable<neo::uint>::value);
    CHECK(std::is_trivially_copyable<neo::ulong>::value);
    CHECK(std::is_trivially_copyable<neo::ullong>::value);

    CHECK(std::is_trivially_copyable<neo::float_>::value);
    CHECK(std::is_trivially_copyable<neo::double_>::value);
    CHECK(std::is_trivially_copyable<neo::ldouble>::value);

    CHECK(std::is_trivially_copyable<neo::ptr<int>>::value);
    CHECK(std::is_trivially_copyable<neo::ptr<int const>>::value);
    CHECK(std::is_trivially_copyable<neo::ref<int>>::value);
    CHECK(std::is_trivially_copyable<neo::ref<int const>>::value);
}

TEST_CASE("neo<T> has user defined literals", "neo<T>")
{
    auto i = 0_ni;
    CHECK(i == neo::int_());
    CHECK(0_ni == neo::int_(0));
    CHECK(42_ni == neo::int_(42));
}

TEST_CASE("neo::void_ can be constructed", "neo::void_")
{
    neo::void_ v;
}

TEST_CASE("neo::int_ is zero-initialized by default", "neo::int_")
{
    CHECK(neo::int_() == neo::int_(0));
}

TEST_CASE("neo::int_ can be explicitly uninitialized", "neo::int_")
{
    neo::int_ a = 10;
    +a;
    using neo_int = neo::int_;
    a.~neo_int();
    new (&a) neo::int_(neo::undefined);

    CHECK(a == 10);
}

TEST_CASE("neo::int_ can be copy constructed", "neo::int_")
{
    neo::int_ a = 10;
    neo::int_ b = a;

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo::int_ can be copy assigned", "neo::int_")
{
    neo::int_ a = 10;
    neo::int_ b;

    b = a;

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo::int_ can be move constructed", "neo::int_")
{
    neo::int_ a = 10;
    neo::int_ b = std::move(a);

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo::int_ can be move assigned", "neo::int_")
{
    neo::int_ a = 10;
    neo::int_ b;

    b = std::move(a);

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo::int_ implicit construction", "neo::int_")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!implicit_conversion_traits<bool, neo::int_>::value));
    }

    SECTION("is defined for narrower signed integer types")
    {
        CHECK((implicit_conversion_traits<byte, neo::int_>::value));
        CHECK((implicit_conversion_traits<short, neo::int_>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo::int_(int(1)) == neo::int_(byte(1)));
            CHECK(neo::int_(int(1)) == neo::int_(short(1)));
            CHECK(neo::int_(int(1)) == neo::int_(int(1)));
        }
    }

    SECTION("is not defined for wider signed integer types")
    {
        CHECK((!implicit_conversion_traits<llong, neo::int_>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!implicit_conversion_traits<ubyte, neo::int_>::value));
        CHECK((!implicit_conversion_traits<ushort, neo::int_>::value));
        CHECK((!implicit_conversion_traits<uint, neo::int_>::value));
        CHECK((!implicit_conversion_traits<ullong, neo::int_>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!implicit_conversion_traits<float, neo::int_>::value));
        CHECK((!implicit_conversion_traits<double, neo::int_>::value));
    }
}

TEST_CASE("neo::int_ implicit conversion", "neo::int_")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!std::is_convertible<neo::int_, bool>::value));
    }

    SECTION("is not defined for narrower signed integer types")
    {
        CHECK((!std::is_convertible<neo::int_, byte>::value));
        CHECK((!std::is_convertible<neo::int_, short>::value));
    }

    SECTION("is defined for wider signed integer types")
    {
        CHECK(int(1) == int{neo::int_(1)});
        CHECK(llong(1) == llong{neo::int_(1)});
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!std::is_convertible<neo::int_, ubyte>::value));
        CHECK((!std::is_convertible<neo::int_, ushort>::value));
        CHECK((!std::is_convertible<neo::int_, uint>::value));
        CHECK((!std::is_convertible<neo::int_, ullong>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!std::is_convertible<neo::int_, float>::value));
        CHECK((!std::is_convertible<neo::int_, double>::value));
    }
}

TEST_CASE("neo::int_ explicit conversion", "neo::int_")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!explicit_conversion_traits<neo::int_, bool>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((explicit_conversion_traits<neo::int_, byte>::value));
        CHECK((explicit_conversion_traits<neo::int_, short>::value));
        CHECK((explicit_conversion_traits<neo::int_, int>::value));
        CHECK((explicit_conversion_traits<neo::int_, llong>::value));

        SECTION("and gives the correct results")
        {
            CHECK(byte(1) == static_cast<byte>(neo::int_(1)));
            CHECK(short(1) == static_cast<short>(neo::int_(1)));
            CHECK(int(1) == static_cast<int>(neo::int_(1)));
            CHECK(llong(1) == static_cast<llong>(neo::int_(1)));
        }
    }

    SECTION("is defined for unsigned integer types")
    {
        neo::int_ const ni;
        int i = ni;
        CHECK((explicit_conversion_traits<neo::int_, ubyte>::value));
        CHECK((explicit_conversion_traits<neo::int_, ushort>::value));
        CHECK((explicit_conversion_traits<neo::int_, uint>::value));
        CHECK((explicit_conversion_traits<neo::int_, ullong>::value));

        SECTION("and gives the correct results")
        {
            CHECK(ubyte(1) == static_cast<ubyte>(neo::int_(1)));
            CHECK(ushort(1) == static_cast<ushort>(neo::int_(1)));
            CHECK(uint(1) == static_cast<uint>(neo::int_(1)));
            CHECK(ullong(1) == static_cast<llong>(neo::int_(1)));
        }
    }

    SECTION("is defined for floating point types")
    {
        CHECK((explicit_conversion_traits<neo::int_, float>::value));
        CHECK((explicit_conversion_traits<neo::int_, double>::value));

        SECTION("and gives the correct results")
        {
            CHECK(float(1) == static_cast<float>(neo::int_(1)));
            CHECK(double(1) == static_cast<double>(neo::int_(1)));
        }
    }
}

TEST_CASE("neo::int_ equality comparison", "neo::int_")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!equals_traits<neo::int_, neo::bool_>::value));
        CHECK((!equals_traits<neo::int_, bool>::value));
        CHECK((!equals_traits<bool, neo::int_>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, neo::byte>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, byte>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<byte, neo::int_>>::value));

        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, neo::short_>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, short>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<short, neo::int_>>::value));

        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, neo::int_>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, int>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<int, neo::int_>>::value));

        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, neo::llong>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits_t<neo::int_, llong>>::value));
        CHECK((std::is_same<neo::bool_, equals_traits<llong, neo::int_>::type>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo::int_(1) == neo::byte(byte(1)));
            CHECK(neo::int_(1) == byte(1));
            CHECK(byte(1) == neo::int_(1));

            CHECK(neo::int_(1) == neo::short_(short(1)));
            CHECK(neo::int_(1) == short(1));
            CHECK(short(1) == neo::int_(1));

            CHECK(neo::int_(1) == neo::int_(int(1)));
            CHECK(neo::int_(1) == int(1));
            CHECK(int(1) == neo::int_(1));

            CHECK(neo::int_(1) == neo::llong(llong(1)));
            CHECK(neo::int_(1) == llong(1));
            CHECK(llong(1) == neo::int_(1));
        }
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!equals_traits<neo::int_, neo::ubyte>::value));
        CHECK((!equals_traits<neo::int_, ubyte>::value));
        CHECK((!equals_traits<ubyte, neo::int_>::value));

        CHECK((!equals_traits<neo::int_, neo::ushort>::value));
        CHECK((!equals_traits<neo::int_, ushort>::value));
        CHECK((!equals_traits<ushort, neo::int_>::value));

        CHECK((!equals_traits<neo::int_, neo::uint>::value));
        CHECK((!equals_traits<neo::int_, uint>::value));
        CHECK((!equals_traits<uint, neo::int_>::value));

        CHECK((!equals_traits<neo::int_, neo::ullong>::value));
        CHECK((!equals_traits<neo::int_, ullong>::value));
        CHECK((!equals_traits<ullong, neo::int_>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!equals_traits<neo::int_, neo::float_>::value));
        CHECK((!equals_traits<neo::int_, float>::value));
        CHECK((!equals_traits<float, neo::int_>::value));

        CHECK((!equals_traits<neo::int_, neo::double_>::value));
        CHECK((!equals_traits<neo::int_, double>::value));
        CHECK((!equals_traits<double, neo::int_>::value));
    }
}

TEST_CASE("neo::int_ addition", "neo::int_")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!add_traits<neo::int_, neo::bool_>::value));
        CHECK((!add_traits<neo::int_, bool>::value));
        CHECK((!add_traits<bool, neo::int_>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, neo::byte>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, byte>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<byte, neo::int_>>::value));

        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, neo::short_>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, short>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<short, neo::int_>>::value));

        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, neo::int_>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<neo::int_, int>>::value));
        CHECK((std::is_same<neo::int_, add_traits_t<int, neo::int_>>::value));

        CHECK((std::is_same<neo::llong, add_traits_t<neo::int_, neo::llong>>::value));
        CHECK((std::is_same<neo::llong, add_traits_t<neo::int_, llong>>::value));
        CHECK((std::is_same<neo::llong, add_traits_t<llong, neo::int_>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo::int_(3) == neo::int_(2) + neo::byte(byte(1)));
            CHECK(neo::int_(3) == neo::int_(2) + byte(1));
            CHECK(neo::int_(3) == byte(2) + neo::int_(1));

            CHECK(neo::int_(3) == neo::int_(2) + neo::short_(short(1)));
            CHECK(neo::int_(3) == neo::int_(2) + short(1));
            CHECK(neo::int_(3) == short(2) + neo::int_(1));

            CHECK(neo::int_(3) == neo::int_(2) + neo::int_(int(1)));
            CHECK(neo::int_(3) == neo::int_(2) + int(1));
            CHECK(neo::int_(3) == int(2) + neo::int_(1));

            CHECK(neo::llong(3) == neo::int_(2) + neo::llong(llong(1)));
            CHECK(neo::llong(3) == neo::int_(2) + llong(1));
            CHECK(neo::llong(3) == llong(2) + neo::int_(1));
        }
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!add_traits<neo::int_, neo::ubyte>::value));
        CHECK((!add_traits<neo::int_, ubyte>::value));
        CHECK((!add_traits<ubyte, neo::int_>::value));

        CHECK((!add_traits<neo::int_, neo::ushort>::value));
        CHECK((!add_traits<neo::int_, ushort>::value));
        CHECK((!add_traits<ushort, neo::int_>::value));

        CHECK((!add_traits<neo::int_, neo::uint>::value));
        CHECK((!add_traits<neo::int_, uint>::value));
        CHECK((!add_traits<uint, neo::int_>::value));

        CHECK((!add_traits<neo::int_, neo::ullong>::value));
        CHECK((!add_traits<neo::int_, ullong>::value));
        CHECK((!add_traits<ullong, neo::int_>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!add_traits<neo::int_, neo::float_>::value));
        CHECK((!add_traits<neo::int_, float>::value));
        CHECK((!add_traits<float, neo::int_>::value));

        CHECK((!add_traits<neo::int_, neo::double_>::value));
        CHECK((!add_traits<neo::int_, double>::value));
        CHECK((!add_traits<double, neo::int_>::value));
    }
}

TEST_CASE("neo::int_ bitwise and", "neo::int_")
{
    SECTION("is not defined for any types")
    {
        CHECK((!bitand_traits<neo::int_, neo::bool_>::value));
        CHECK((!bitand_traits<neo::int_, bool>::value));
        CHECK((!bitand_traits<bool, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::byte>::value));
        CHECK((!bitand_traits<neo::int_, byte>::value));
        CHECK((!bitand_traits<byte, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::short_>::value));
        CHECK((!bitand_traits<neo::int_, short>::value));
        CHECK((!bitand_traits<short, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::int_>::value));
        CHECK((!bitand_traits<neo::int_, int>::value));
        CHECK((!bitand_traits<int, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::llong>::value));
        CHECK((!bitand_traits<neo::int_, llong>::value));
        CHECK((!bitand_traits<llong, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::ubyte>::value));
        CHECK((!bitand_traits<neo::int_, ubyte>::value));
        CHECK((!bitand_traits<ubyte, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::ushort>::value));
        CHECK((!bitand_traits<neo::int_, ushort>::value));
        CHECK((!bitand_traits<ushort, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::uint>::value));
        CHECK((!bitand_traits<neo::int_, uint>::value));
        CHECK((!bitand_traits<uint, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::ullong>::value));
        CHECK((!bitand_traits<neo::int_, ullong>::value));
        CHECK((!bitand_traits<ullong, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::float_>::value));
        CHECK((!bitand_traits<neo::int_, float>::value));
        CHECK((!bitand_traits<float, neo::int_>::value));

        CHECK((!bitand_traits<neo::int_, neo::double_>::value));
        CHECK((!bitand_traits<neo::int_, double>::value));
        CHECK((!bitand_traits<double, neo::int_>::value));
    }
}

TEST_CASE("neo::int_ and", "neo::int_")
{
    SECTION("is not defined for any types")
    {
        CHECK((!and_traits<neo::int_, neo::bool_>::value));
        CHECK((!and_traits<neo::int_, bool>::value));
        CHECK((!and_traits<bool, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::byte>::value));
        CHECK((!and_traits<neo::int_, byte>::value));
        CHECK((!and_traits<byte, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::short_>::value));
        CHECK((!and_traits<neo::int_, short>::value));
        CHECK((!and_traits<short, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::int_>::value));
        CHECK((!and_traits<neo::int_, int>::value));
        CHECK((!and_traits<int, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::llong>::value));
        CHECK((!and_traits<neo::int_, llong>::value));
        CHECK((!and_traits<llong, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::ubyte>::value));
        CHECK((!and_traits<neo::int_, ubyte>::value));
        CHECK((!and_traits<ubyte, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::ushort>::value));
        CHECK((!and_traits<neo::int_, ushort>::value));
        CHECK((!and_traits<ushort, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::uint>::value));
        CHECK((!and_traits<neo::int_, uint>::value));
        CHECK((!and_traits<uint, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::ullong>::value));
        CHECK((!and_traits<neo::int_, ullong>::value));
        CHECK((!and_traits<ullong, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::float_>::value));
        CHECK((!and_traits<neo::int_, float>::value));
        CHECK((!and_traits<float, neo::int_>::value));

        CHECK((!and_traits<neo::int_, neo::double_>::value));
        CHECK((!and_traits<neo::int_, double>::value));
        CHECK((!and_traits<double, neo::int_>::value));
    }
}

TEST_CASE("neo::uint bitwise and", "neo::uint")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!bitand_traits<neo::uint, neo::bool_>::value));
        CHECK((!bitand_traits<neo::uint, bool>::value));
        CHECK((!bitand_traits<bool, neo::uint>::value));
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!bitand_traits<neo::uint, neo::byte>::value));
        CHECK((!bitand_traits<neo::uint, byte>::value));
        CHECK((!bitand_traits<byte, neo::uint>::value));

        CHECK((!bitand_traits<neo::uint, neo::short_>::value));
        CHECK((!bitand_traits<neo::uint, short>::value));
        CHECK((!bitand_traits<short, neo::uint>::value));

        CHECK((!bitand_traits<neo::uint, neo::int_>::value));
        CHECK((!bitand_traits<neo::uint, int>::value));
        CHECK((!bitand_traits<int, neo::uint>::value));

        CHECK((!bitand_traits<neo::uint, neo::llong>::value));
        CHECK((!bitand_traits<neo::uint, llong>::value));
        CHECK((!bitand_traits<llong, neo::uint>::value));
    }

    SECTION("is defined for unsigned integer types")
    {
        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, neo::ubyte>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, ubyte>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<ubyte, neo::uint>>::value));

        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, neo::ushort>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, ushort>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<ushort, neo::uint>>::value));

        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, neo::uint>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<neo::uint, uint>>::value));
        CHECK((std::is_same<neo::uint, add_traits_t<uint, neo::uint>>::value));

        CHECK((std::is_same<neo::ullong, add_traits_t<neo::uint, neo::ullong>>::value));
        CHECK((std::is_same<neo::ullong, add_traits_t<neo::uint, ullong>>::value));
        CHECK((std::is_same<neo::ullong, add_traits_t<ullong, neo::uint>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo::uint(8u) == (neo::uint(12u) & neo::ubyte(ubyte(10u))));
            CHECK(neo::uint(8u) == (neo::uint(12u) & ubyte(10u)));
            CHECK(neo::uint(8u) == (ubyte(12u) & neo::uint(10u)));

            CHECK(neo::uint(8u) == (neo::uint(12u) & neo::ushort(ushort(10u))));
            CHECK(neo::uint(8u) == (neo::uint(12u) & ushort(10u)));
            CHECK(neo::uint(8u) == (ushort(12u) & neo::uint(10u)));

            CHECK(neo::uint(8u) == (neo::uint(12u) & neo::uint(uint(10u))));
            CHECK(neo::uint(8u) == (neo::uint(12u) & uint(10u)));
            CHECK(neo::uint(8u) == (uint(12u) & neo::uint(10u)));

            CHECK(neo::ullong(8u) == (neo::uint(12u) & neo::ullong(ullong(10u))));
            CHECK(neo::ullong(8u) == (neo::uint(12u) & ullong(10u)));
            CHECK(neo::ullong(8u) == (ullong(12u) & neo::uint(10u)));
        }
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!bitand_traits<neo::uint, neo::float_>::value));
        CHECK((!bitand_traits<neo::uint, float>::value));
        CHECK((!bitand_traits<float, neo::uint>::value));

        CHECK((!bitand_traits<neo::uint, neo::double_>::value));
        CHECK((!bitand_traits<neo::uint, double>::value));
        CHECK((!bitand_traits<double, neo::uint>::value));
    }
}

TEST_CASE("neo::int_ can be incremented", "neo::int_")
{
    neo::int_ a = 10;

    CHECK(++a == 11);
    CHECK(a++ == 11);
    CHECK(a == 12);
}

TEST_CASE("neo::int_ can be decremented", "neo::int_")
{
    neo::int_ a = 10;

    CHECK(--a == 9);
    CHECK(a-- == 9);
    CHECK(a == 8);
}

TEST_CASE("neo::int_ can be incremented in place", "neo::int_")
{
    neo::int_ a = 10;

    CHECK((a += byte(2)) == 12);
    CHECK((a += short(2)) == 14);
    CHECK((a += int(2)) == 16);

    CHECK((a += neo::byte(byte(2))) == 18);
    CHECK((a += neo::short_(short(2))) == 20);
    CHECK((a += neo::int_(int(2))) == 22);
}

TEST_CASE("neo::int_ can be decremented in place", "neo::int_")
{
    neo::int_ a = 10;

    CHECK((a -= byte(2)) == 8);
    CHECK((a -= short(2)) == 6);
    CHECK((a -= int(2)) == 4);

    CHECK((a -= neo::byte(byte(2))) == 2);
    CHECK((a -= neo::short_(short(2))) == 0);
    CHECK((a -= neo::int_(int(2))) == -2);
}

TEST_CASE("neo::byte and neo::short_ do not exhibit integral promotion", "neo::int_")
{
    CHECK((std::is_same<int, decltype(byte() + byte())>::value));
    CHECK((std::is_same<neo::byte, decltype(neo::byte() + neo::byte())>::value));

    CHECK((std::is_same<int, decltype(short() + short())>::value));
    CHECK((std::is_same<neo::short_, decltype(neo::short_() + neo::short_())>::value));
}

TEST_CASE("neo::bool_ is can be constructed from bool", "neo::bool_")
{
    neo::bool_ f = false;
    neo::bool_ t = true;

    CHECK(f == false);
    REQUIRE(t == true);
}

TEST_CASE("neo::bool_ and", "neo::bool_")
{
    SECTION("is defined for boolean types")
    {
        CHECK((std::is_same<bool, and_traits_t<neo::bool_, neo::bool_>>::value));
        CHECK((std::is_same<bool, and_traits_t<neo::bool_, bool>>::value));
        CHECK((std::is_same<bool, and_traits_t<bool, neo::bool_>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(false == (neo::bool_(false) && neo::bool_(false)));
            CHECK(false == (neo::bool_(false) && false));
            CHECK(false == (false && neo::bool_(false)));

            CHECK(false == (neo::bool_(false) && neo::bool_(true)));
            CHECK(false == (neo::bool_(false) && true));
            CHECK(false == (false && neo::bool_(true)));

            CHECK(false == (neo::bool_(true) && neo::bool_(false)));
            CHECK(false == (neo::bool_(true) && false));
            CHECK(false == (true && neo::bool_(false)));

            CHECK(true == (neo::bool_(true) && neo::bool_(true)));
            CHECK(true == (neo::bool_(true) && true));
            CHECK(true == (true && neo::bool_(true)));
        }
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!and_traits<neo::bool_, neo::byte>::value));
        CHECK((!and_traits<neo::bool_, byte>::value));
        CHECK((!and_traits<byte, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::short_>::value));
        CHECK((!and_traits<neo::bool_, short>::value));
        CHECK((!and_traits<short, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::int_>::value));
        CHECK((!and_traits<neo::bool_, int>::value));
        CHECK((!and_traits<int, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::llong>::value));
        CHECK((!and_traits<neo::bool_, llong>::value));
        CHECK((!and_traits<llong, neo::bool_>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!and_traits<neo::bool_, neo::ubyte>::value));
        CHECK((!and_traits<neo::bool_, ubyte>::value));
        CHECK((!and_traits<ubyte, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::ushort>::value));
        CHECK((!and_traits<neo::bool_, ushort>::value));
        CHECK((!and_traits<ushort, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::uint>::value));
        CHECK((!and_traits<neo::bool_, uint>::value));
        CHECK((!and_traits<uint, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::ullong>::value));
        CHECK((!and_traits<neo::bool_, ullong>::value));
        CHECK((!and_traits<ullong, neo::bool_>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!and_traits<neo::bool_, neo::float_>::value));
        CHECK((!and_traits<neo::bool_, float>::value));
        CHECK((!and_traits<float, neo::bool_>::value));

        CHECK((!and_traits<neo::bool_, neo::double_>::value));
        CHECK((!and_traits<neo::bool_, double>::value));
        CHECK((!and_traits<double, neo::bool_>::value));
    }
}

TEST_CASE("neo::bool_ or", "neo::bool_")
{
    SECTION("is defined for boolean types")
    {
        CHECK((std::is_same<bool, or_traits_t<neo::bool_, neo::bool_>>::value));
        CHECK((std::is_same<bool, or_traits_t<neo::bool_, bool>>::value));
        CHECK((std::is_same<bool, or_traits_t<bool, neo::bool_>>::value));

        SECTION("or gives the correct results")
        {
            CHECK(false == (neo::bool_(false) || neo::bool_(false)));
            CHECK(false == (neo::bool_(false) || false));
            CHECK(false == (false || neo::bool_(false)));

            CHECK(true == (neo::bool_(false) || neo::bool_(true)));
            CHECK(true == (neo::bool_(false) || true));
            CHECK(true == (false || neo::bool_(true)));

            CHECK(true == (neo::bool_(true) || neo::bool_(false)));
            CHECK(true == (neo::bool_(true) || false));
            CHECK(true == (true || neo::bool_(false)));

            CHECK(true == (neo::bool_(true) || neo::bool_(true)));
            CHECK(true == (neo::bool_(true) || true));
            CHECK(true == (true || neo::bool_(true)));
        }
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!or_traits<neo::bool_, neo::byte>::value));
        CHECK((!or_traits<neo::bool_, byte>::value));
        CHECK((!or_traits<byte, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::short_>::value));
        CHECK((!or_traits<neo::bool_, short>::value));
        CHECK((!or_traits<short, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::int_>::value));
        CHECK((!or_traits<neo::bool_, int>::value));
        CHECK((!or_traits<int, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::llong>::value));
        CHECK((!or_traits<neo::bool_, llong>::value));
        CHECK((!or_traits<llong, neo::bool_>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!or_traits<neo::bool_, neo::ubyte>::value));
        CHECK((!or_traits<neo::bool_, ubyte>::value));
        CHECK((!or_traits<ubyte, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::ushort>::value));
        CHECK((!or_traits<neo::bool_, ushort>::value));
        CHECK((!or_traits<ushort, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::uint>::value));
        CHECK((!or_traits<neo::bool_, uint>::value));
        CHECK((!or_traits<uint, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::ullong>::value));
        CHECK((!or_traits<neo::bool_, ullong>::value));
        CHECK((!or_traits<ullong, neo::bool_>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!or_traits<neo::bool_, neo::float_>::value));
        CHECK((!or_traits<neo::bool_, float>::value));
        CHECK((!or_traits<float, neo::bool_>::value));

        CHECK((!or_traits<neo::bool_, neo::double_>::value));
        CHECK((!or_traits<neo::bool_, double>::value));
        CHECK((!or_traits<double, neo::bool_>::value));
    }
}

TEST_CASE("neo::bool_ can be inverted", "neo::bool_")
{
    neo::bool_ f = false;
    neo::bool_ t = true;

    REQUIRE(!f == true);
    REQUIRE(!t == false);
}

TEST_CASE("neo::ptr cannot be deleted", "neo::ptr")
{
    int i = 42;
    neo::ptr<int> p;
    p = new int(42);

    //CHECK(p[0u] == 42);
    //CHECK(*(p + 0) == 42);

    delete p.get();
}

TEST_CASE("neo::ptr", "neo::ptr")
{
    neo::ptr<neo::int_> p;

    CHECK(!p);
    CHECK(p == nullptr);

    neo::int_ i[] = { 1, 2, 3, 4, 5 };
    //p = i;
    p = &i[0];

/*
    CHECK(p);
    CHECK(p != nullptr);
    CHECK(p == &i[0]);
    CHECK(*p == 1);

    CHECK(p++ == &i[0]);
    CHECK(p);
    CHECK(p != nullptr);
    CHECK(p == &i[1]);
    CHECK(*p == 2);

    CHECK(++p == &i[2]);
    CHECK(p);
    CHECK(p != nullptr);
    CHECK(p == &i[2]);
    CHECK(*p == 3);

    CHECK((p += 2) == &i[4]);
    CHECK(p);
    CHECK(p != nullptr);
    CHECK(p == &i[4]);
    CHECK(*p == 5);
*/
}

TEST_CASE("neo::ref", "neo::ref")
{
             int const&  ri = 42; // legal
//    neo::ref<int const> nri = 42; // illegal

    neo::int_ i[] = { 1, 2, 3, 4, 5 };

    auto ar = neo::make_ref(i[0]);
    neo::ref<neo::int_> p = neo::make_ref(i[0]);

    CHECK(*p == 1);
    *p = 3;
    CHECK(i[0] == 3);

    p = make_ref(i[1]);

    CHECK(*p == 2);

    int myint = 0;
    neo::ref<int const> r = neo::make_ref(myint);
    neo::ptr<int const> pr = &r;
    neo::ref<int const> pp = *r;

    neo::ptr<derived> asdasd;
    neo::ptr<base> asdasdasd = asdasd;
    base* lkjfas = asdasd;
}

TEST_CASE("neo::ref<T> copy construction", "neo::ref")
{
    SECTION("is defined for references")
    {
        CHECK((std::is_constructible<neo::ref<int>, int&>::value));
        CHECK((std::is_constructible<neo::ref<int>, neo::ref<int>>::value));
        CHECK((std::is_constructible<neo::ref<int>, neo::ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(std::is_constructible<neo::ref<int>, int const&>::value));
        CHECK(!(std::is_constructible<neo::ref<int>, neo::ref<int const>>::value));
        CHECK(!(std::is_constructible<neo::ref<int>, neo::ref<int const> const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((std::is_constructible<neo::ref<base>, derived&>::value));
        CHECK((std::is_constructible<neo::ref<base>, neo::ref<derived>>::value));
        CHECK((std::is_constructible<neo::ref<base>, neo::ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(std::is_constructible<neo::ref<base>, derived const&>::value));
        CHECK(!(std::is_constructible<neo::ref<base>, neo::ref<derived const>>::value));
        CHECK(!(std::is_constructible<neo::ref<base>, neo::ref<derived const> const>::value));
    }
}

TEST_CASE("neo::ref<T const> copy construction", "neo::ref")
{
    SECTION("is defined for all references")
    {
        CHECK((std::is_constructible<neo::ref<int const>, int&>::value));
        CHECK((std::is_constructible<neo::ref<int const>, int const&>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int>>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int> const>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int const>>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int const> const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((std::is_constructible<neo::ref<base const>, derived&>::value));
        CHECK((std::is_constructible<neo::ref<base const>, derived const&>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived>>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived> const>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived const>>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived const> const>::value));
    }
}

TEST_CASE("neo::ref<T> move construction", "neo::ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(std::is_constructible<neo::ref<int>, int&&>::value));
        CHECK(!(std::is_constructible<neo::ref<int>, int const&&>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((std::is_constructible<neo::ref<int>, neo::ref<int>&&>::value));
        CHECK((std::is_constructible<neo::ref<int>, neo::ref<int> const&&>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(std::is_constructible<neo::ref<int>, neo::ref<int const>&&>::value));
        CHECK(!(std::is_constructible<neo::ref<int>, neo::ref<int const> const&&>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(std::is_constructible<neo::ref<base>, derived&&>::value));
        CHECK(!(std::is_constructible<neo::ref<base>, derived const&&>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((std::is_constructible<neo::ref<base>, neo::ref<derived>&&>::value));
        CHECK((std::is_constructible<neo::ref<base>, neo::ref<derived> const&&>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(std::is_constructible<neo::ref<base>, neo::ref<derived const>&&>::value));
        CHECK(!(std::is_constructible<neo::ref<base>, neo::ref<derived const> const&&>::value));
    }
}

TEST_CASE("neo::ref<T const> move construction", "neo::ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(std::is_constructible<neo::ref<int const>, int&&>::value));
        CHECK(!(std::is_constructible<neo::ref<int const>, int const&&>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int>&&>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int> const&&>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int const>&&>::value));
        CHECK((std::is_constructible<neo::ref<int const>, neo::ref<int const> const&&>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(std::is_constructible<neo::ref<base const>, derived&&>::value));
        CHECK(!(std::is_constructible<neo::ref<base const>, derived const&&>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived>&&>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived> const&&>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived const>&&>::value));
        CHECK((std::is_constructible<neo::ref<base const>, neo::ref<derived const> const&&>::value));
    }
}

TEST_CASE("neo::ref<T> copy assignment", "neo::ref")
{
    SECTION("is defined for references")
    {
        CHECK((copy_assignment_traits<neo::ref<int>, int>::value));
        CHECK((copy_assignment_traits<neo::ref<int>, neo::ref<int>>::value));
        CHECK((copy_assignment_traits<neo::ref<int>, neo::ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(copy_assignment_traits<neo::ref<int>, int const>::value));
        CHECK(!(copy_assignment_traits<neo::ref<int>, neo::ref<int const>>::value));
        CHECK(!(copy_assignment_traits<neo::ref<int>, neo::ref<int const> const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((copy_assignment_traits<neo::ref<base>, derived>::value));
        CHECK((copy_assignment_traits<neo::ref<base>, neo::ref<derived>>::value));
        CHECK((copy_assignment_traits<neo::ref<base>, neo::ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(copy_assignment_traits<neo::ref<base>, derived const>::value));
        CHECK(!(copy_assignment_traits<neo::ref<base>, neo::ref<derived const>>::value));
        CHECK(!(copy_assignment_traits<neo::ref<base>, neo::ref<derived const> const>::value));
    }
}

TEST_CASE("neo::ref<T const> copy assignment", "neo::ref")
{
    SECTION("is defined for all references")
    {
        CHECK((copy_assignment_traits<neo::ref<int const>, int>::value));
        CHECK((copy_assignment_traits<neo::ref<int const>, int const>::value));
        CHECK((copy_assignment_traits<neo::ref<int const>, neo::ref<int>>::value));
        CHECK((copy_assignment_traits<neo::ref<int const>, neo::ref<int> const>::value));
        CHECK((copy_assignment_traits<neo::ref<int const>, neo::ref<int const>>::value));
        CHECK((copy_assignment_traits<neo::ref<int const>, neo::ref<int const> const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((copy_assignment_traits<neo::ref<base const>, derived>::value));
        CHECK((copy_assignment_traits<neo::ref<base const>, derived const>::value));
        CHECK((copy_assignment_traits<neo::ref<base const>, neo::ref<derived>>::value));
        CHECK((copy_assignment_traits<neo::ref<base const>, neo::ref<derived> const>::value));
        CHECK((copy_assignment_traits<neo::ref<base const>, neo::ref<derived const>>::value));
        CHECK((copy_assignment_traits<neo::ref<base const>, neo::ref<derived const> const>::value));
    }
}

TEST_CASE("neo::ref<T> move assignment", "neo::ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_assignment_traits<neo::ref<int>, int>::value));
        CHECK(!(move_assignment_traits<neo::ref<int>, int const>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((move_assignment_traits<neo::ref<int>, neo::ref<int>>::value));
        CHECK((move_assignment_traits<neo::ref<int>, neo::ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(move_assignment_traits<neo::ref<int>, neo::ref<int const>>::value));
        CHECK(!(move_assignment_traits<neo::ref<int>, neo::ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_assignment_traits<neo::ref<base>, derived>::value));
        CHECK(!(move_assignment_traits<neo::ref<base>, derived const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((move_assignment_traits<neo::ref<base>, neo::ref<derived>>::value));
        CHECK((move_assignment_traits<neo::ref<base>, neo::ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(move_assignment_traits<neo::ref<base>, neo::ref<derived const>>::value));
        CHECK(!(move_assignment_traits<neo::ref<base>, neo::ref<derived const> const>::value));
    }
}

TEST_CASE("neo::ref<T const> move assignment", "neo::ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_assignment_traits<neo::ref<int const>, int>::value));
        CHECK(!(move_assignment_traits<neo::ref<int const>, int const>::value));
    }

    SECTION("is defined for all references")
    {
        CHECK((move_assignment_traits<neo::ref<int const>, neo::ref<int>>::value));
        CHECK((move_assignment_traits<neo::ref<int const>, neo::ref<int> const>::value));
        CHECK((move_assignment_traits<neo::ref<int const>, neo::ref<int const>>::value));
        CHECK((move_assignment_traits<neo::ref<int const>, neo::ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_assignment_traits<neo::ref<base const>, derived>::value));
        CHECK(!(move_assignment_traits<neo::ref<base const>, derived const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((move_assignment_traits<neo::ref<base const>, neo::ref<derived>>::value));
        CHECK((move_assignment_traits<neo::ref<base const>, neo::ref<derived> const>::value));
        CHECK((move_assignment_traits<neo::ref<base const>, neo::ref<derived const>>::value));
        CHECK((move_assignment_traits<neo::ref<base const>, neo::ref<derived const> const>::value));
    }
}
