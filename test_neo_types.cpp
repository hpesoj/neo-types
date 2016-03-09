#include <neo/neo.hpp>
#include <operator_traits.hpp>
#include <catch.hpp>

#include <cmath>
#include <iostream>
#include <type_traits>

using namespace neo::aliases;
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

void zero(llong& v)
{
    v = 0;
}

void zero(int& v)
{
    v = 0;
}

bool is_zero(int v)
{
    return v == 0;
}

bool is_zero(llong v)
{
    return v == 0;
}

bool is_zero_2(neo_int v)
{
    return v == 0;
}

bool is_zero_2(neo_llong v)
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
    neo_int value;

    counter(neo_int i) :
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
    neo_ptr<base> b1 = &d0;
    neo_ptr<derived> d1 = &d0;

    derived ds[10];
    //neo<derived[10]> d10 = ds;

    //b1 - d1;
    //byte b = static_cast<neo_byte>(neo_int());

    //is_zero_2(1);

    //is_zero_2(int());

    neo_int i = 42;
    zero(i);
    CHECK(is_zero(i.get()));

    CHECK(sizeof(neo_void) == 1u);

    CHECK(sizeof(neo_bool) == sizeof(bool));

    CHECK(sizeof(neo_byte) == sizeof(byte));
    CHECK(sizeof(neo_short) == sizeof(short));
    CHECK(sizeof(neo_int) == sizeof(int));
    CHECK(sizeof(neo_long) == sizeof(long));
    CHECK(sizeof(neo_llong) == sizeof(llong));

    CHECK(sizeof(neo_ubyte) == sizeof(ubyte));
    CHECK(sizeof(neo_ushort) == sizeof(ushort));
    CHECK(sizeof(neo_uint) == sizeof(uint));
    CHECK(sizeof(neo_ulong) == sizeof(ulong));
    CHECK(sizeof(neo_ullong) == sizeof(ullong));

    CHECK(sizeof(neo_float) == sizeof(float));
    CHECK(sizeof(neo_double) == sizeof(double));
    CHECK(sizeof(neo_ldouble) == sizeof(ldouble));

    CHECK(sizeof(neo_ptr<int>) == sizeof(int*));
}

TEST_CASE("neo<T> has user defined literals", "neo<T>")
{
    auto i = 0_ni;
    CHECK(i == neo_int());
    CHECK(0_ni == neo_int(0));
    CHECK(42_ni == neo_int(42));
}

TEST_CASE("neo_void can be constructed", "neo_void")
{
    neo_void v;
}

TEST_CASE("neo_int is zero-initialized by default", "neo_int")
{
    CHECK(neo_int() == neo_int(0));
}

TEST_CASE("neo_int can be explicitly uninitialized", "neo_int")
{
    neo_int a = 10;
    +a;
    a.~neo_int();
    new (&a) neo_int(neo::undefined);

    CHECK(a == 10);
}

TEST_CASE("neo_int can be copy constructed", "neo_int")
{
    neo_int a = 10;
    neo_int b = a;

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo_int can be copy assigned", "neo_int")
{
    neo_int a = 10;
    neo_int b;

    b = a;

    CHECK(a == 10);
    CHECK(b == 10);
    CHECK(a == b);
}

TEST_CASE("neo_int can be move constructed", "neo_int")
{
    neo_int a = 10;
    neo_int b = std::move(a);

    CHECK(a == 0);
    CHECK(b == 10);
    CHECK(a != b);
}

TEST_CASE("neo_int can be move assigned", "neo_int")
{
    neo_int a = 10;
    neo_int b;

    b = std::move(a);

    CHECK(a == 0);
    CHECK(b == 10);
    CHECK(a != b);
}

TEST_CASE("neo_int implicit construction", "neo_int")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!implicit_conversion_traits<bool, neo_int>::value));
    }

    SECTION("is defined for narrower signed integer types")
    {
        CHECK((implicit_conversion_traits<byte, neo_int>::value));
        CHECK((implicit_conversion_traits<short, neo_int>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo_int(int(1)) == neo_int(byte(1)));
            CHECK(neo_int(int(1)) == neo_int(short(1)));
            CHECK(neo_int(int(1)) == neo_int(int(1)));
        }
    }

    SECTION("is not defined for wider signed integer types")
    {
        CHECK((!implicit_conversion_traits<llong, neo_int>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!implicit_conversion_traits<ubyte, neo_int>::value));
        CHECK((!implicit_conversion_traits<ushort, neo_int>::value));
        CHECK((!implicit_conversion_traits<uint, neo_int>::value));
        CHECK((!implicit_conversion_traits<ullong, neo_int>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!implicit_conversion_traits<float, neo_int>::value));
        CHECK((!implicit_conversion_traits<double, neo_int>::value));
    }
}

TEST_CASE("neo_int implicit conversion", "neo_int")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!implicit_conversion_traits<neo_int, bool>::value));
    }

    SECTION("is not defined for narrower signed integer types")
    {
        CHECK((!implicit_conversion_traits<neo_int, byte>::value));
        CHECK((!implicit_conversion_traits<neo_int, short>::value));
    }

    SECTION("is defined for wider signed integer types")
    {
        CHECK(int(1) == int{neo_int(1)});
        CHECK(llong(1) == llong{neo_int(1)});
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!implicit_conversion_traits<neo_int, ubyte>::value));
        CHECK((!implicit_conversion_traits<neo_int, ushort>::value));
        CHECK((!implicit_conversion_traits<neo_int, uint>::value));
        CHECK((!implicit_conversion_traits<neo_int, ullong>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!implicit_conversion_traits<neo_int, float>::value));
        CHECK((!implicit_conversion_traits<neo_int, double>::value));
    }
}

TEST_CASE("neo_int explicit conversion", "neo_int")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!explicit_conversion_traits<neo_int, bool>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((explicit_conversion_traits<neo_int, byte>::value));
        CHECK((explicit_conversion_traits<neo_int, short>::value));
        CHECK((explicit_conversion_traits<neo_int, int>::value));
        CHECK((explicit_conversion_traits<neo_int, llong>::value));

        SECTION("and gives the correct results")
        {
            CHECK(byte(1) == static_cast<byte>(neo_int(1)));
            CHECK(short(1) == static_cast<short>(neo_int(1)));
            CHECK(int(1) == static_cast<int>(neo_int(1)));
            CHECK(llong(1) == static_cast<llong>(neo_int(1)));
        }
    }

    SECTION("is defined for unsigned integer types")
    {
        CHECK((explicit_conversion_traits<neo_int, ubyte>::value));
        CHECK((explicit_conversion_traits<neo_int, ushort>::value));
        CHECK((explicit_conversion_traits<neo_int, uint>::value));
        CHECK((explicit_conversion_traits<neo_int, ullong>::value));

        SECTION("and gives the correct results")
        {
            CHECK(ubyte(1) == static_cast<ubyte>(neo_int(1)));
            CHECK(ushort(1) == static_cast<ushort>(neo_int(1)));
            CHECK(uint(1) == static_cast<uint>(neo_int(1)));
            CHECK(ullong(1) == static_cast<llong>(neo_int(1)));
        }
    }

    SECTION("is defined for floating point types")
    {
        CHECK((explicit_conversion_traits<neo_int, float>::value));
        CHECK((explicit_conversion_traits<neo_int, double>::value));

        SECTION("and gives the correct results")
        {
            CHECK(float(1) == static_cast<float>(neo_int(1)));
            CHECK(double(1) == static_cast<double>(neo_int(1)));
        }
    }
}

TEST_CASE("neo_int equality comparison", "neo_int")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!equals_traits<neo_int, neo_bool>::value));
        CHECK((!equals_traits<neo_int, bool>::value));
        CHECK((!equals_traits<bool, neo_int>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, neo_byte>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, byte>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<byte, neo_int>>::value));

        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, neo_short>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, short>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<short, neo_int>>::value));

        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, neo_int>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, int>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<int, neo_int>>::value));

        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, neo_llong>>::value));
        CHECK((std::is_same<neo_bool, equals_traits_t<neo_int, llong>>::value));
        CHECK((std::is_same<neo_bool, equals_traits<llong, neo_int>::type>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo_int(1) == neo_byte(byte(1)));
            CHECK(neo_int(1) == byte(1));
            CHECK(byte(1) == neo_int(1));

            CHECK(neo_int(1) == neo_short(short(1)));
            CHECK(neo_int(1) == short(1));
            CHECK(short(1) == neo_int(1));

            CHECK(neo_int(1) == neo_int(int(1)));
            CHECK(neo_int(1) == int(1));
            CHECK(int(1) == neo_int(1));

            CHECK(neo_int(1) == neo_llong(llong(1)));
            CHECK(neo_int(1) == llong(1));
            CHECK(llong(1) == neo_int(1));
        }
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!equals_traits<neo_int, neo_ubyte>::value));
        CHECK((!equals_traits<neo_int, ubyte>::value));
        CHECK((!equals_traits<ubyte, neo_int>::value));

        CHECK((!equals_traits<neo_int, neo_ushort>::value));
        CHECK((!equals_traits<neo_int, ushort>::value));
        CHECK((!equals_traits<ushort, neo_int>::value));

        CHECK((!equals_traits<neo_int, neo_uint>::value));
        CHECK((!equals_traits<neo_int, uint>::value));
        CHECK((!equals_traits<uint, neo_int>::value));

        CHECK((!equals_traits<neo_int, neo_ullong>::value));
        CHECK((!equals_traits<neo_int, ullong>::value));
        CHECK((!equals_traits<ullong, neo_int>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!equals_traits<neo_int, neo_float>::value));
        CHECK((!equals_traits<neo_int, float>::value));
        CHECK((!equals_traits<float, neo_int>::value));

        CHECK((!equals_traits<neo_int, neo_double>::value));
        CHECK((!equals_traits<neo_int, double>::value));
        CHECK((!equals_traits<double, neo_int>::value));
    }
}

TEST_CASE("neo_int addition", "neo_int")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!add_traits<neo_int, neo_bool>::value));
        CHECK((!add_traits<neo_int, bool>::value));
        CHECK((!add_traits<bool, neo_int>::value));
    }

    SECTION("is defined for signed integer types")
    {
        CHECK((std::is_same<neo_int, add_traits_t<neo_int, neo_byte>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<neo_int, byte>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<byte, neo_int>>::value));

        CHECK((std::is_same<neo_int, add_traits_t<neo_int, neo_short>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<neo_int, short>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<short, neo_int>>::value));

        CHECK((std::is_same<neo_int, add_traits_t<neo_int, neo_int>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<neo_int, int>>::value));
        CHECK((std::is_same<neo_int, add_traits_t<int, neo_int>>::value));

        CHECK((std::is_same<neo_llong, add_traits_t<neo_int, neo_llong>>::value));
        CHECK((std::is_same<neo_llong, add_traits_t<neo_int, llong>>::value));
        CHECK((std::is_same<neo_llong, add_traits_t<llong, neo_int>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo_int(3) == neo_int(2) + neo_byte(byte(1)));
            CHECK(neo_int(3) == neo_int(2) + byte(1));
            CHECK(neo_int(3) == byte(2) + neo_int(1));

            CHECK(neo_int(3) == neo_int(2) + neo_short(short(1)));
            CHECK(neo_int(3) == neo_int(2) + short(1));
            CHECK(neo_int(3) == short(2) + neo_int(1));

            CHECK(neo_int(3) == neo_int(2) + neo_int(int(1)));
            CHECK(neo_int(3) == neo_int(2) + int(1));
            CHECK(neo_int(3) == int(2) + neo_int(1));

            CHECK(neo_llong(3) == neo_int(2) + neo_llong(llong(1)));
            CHECK(neo_llong(3) == neo_int(2) + llong(1));
            CHECK(neo_llong(3) == llong(2) + neo_int(1));
        }
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!add_traits<neo_int, neo_ubyte>::value));
        CHECK((!add_traits<neo_int, ubyte>::value));
        CHECK((!add_traits<ubyte, neo_int>::value));

        CHECK((!add_traits<neo_int, neo_ushort>::value));
        CHECK((!add_traits<neo_int, ushort>::value));
        CHECK((!add_traits<ushort, neo_int>::value));

        CHECK((!add_traits<neo_int, neo_uint>::value));
        CHECK((!add_traits<neo_int, uint>::value));
        CHECK((!add_traits<uint, neo_int>::value));

        CHECK((!add_traits<neo_int, neo_ullong>::value));
        CHECK((!add_traits<neo_int, ullong>::value));
        CHECK((!add_traits<ullong, neo_int>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!add_traits<neo_int, neo_float>::value));
        CHECK((!add_traits<neo_int, float>::value));
        CHECK((!add_traits<float, neo_int>::value));

        CHECK((!add_traits<neo_int, neo_double>::value));
        CHECK((!add_traits<neo_int, double>::value));
        CHECK((!add_traits<double, neo_int>::value));
    }
}

TEST_CASE("neo_int bitwise and", "neo_int")
{
    SECTION("is not defined for any types")
    {
        CHECK((!bitand_traits<neo_int, neo_bool>::value));
        CHECK((!bitand_traits<neo_int, bool>::value));
        CHECK((!bitand_traits<bool, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_byte>::value));
        CHECK((!bitand_traits<neo_int, byte>::value));
        CHECK((!bitand_traits<byte, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_short>::value));
        CHECK((!bitand_traits<neo_int, short>::value));
        CHECK((!bitand_traits<short, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_int>::value));
        CHECK((!bitand_traits<neo_int, int>::value));
        CHECK((!bitand_traits<int, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_llong>::value));
        CHECK((!bitand_traits<neo_int, llong>::value));
        CHECK((!bitand_traits<llong, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_ubyte>::value));
        CHECK((!bitand_traits<neo_int, ubyte>::value));
        CHECK((!bitand_traits<ubyte, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_ushort>::value));
        CHECK((!bitand_traits<neo_int, ushort>::value));
        CHECK((!bitand_traits<ushort, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_uint>::value));
        CHECK((!bitand_traits<neo_int, uint>::value));
        CHECK((!bitand_traits<uint, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_ullong>::value));
        CHECK((!bitand_traits<neo_int, ullong>::value));
        CHECK((!bitand_traits<ullong, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_float>::value));
        CHECK((!bitand_traits<neo_int, float>::value));
        CHECK((!bitand_traits<float, neo_int>::value));

        CHECK((!bitand_traits<neo_int, neo_double>::value));
        CHECK((!bitand_traits<neo_int, double>::value));
        CHECK((!bitand_traits<double, neo_int>::value));
    }
}

TEST_CASE("neo_int and", "neo_int")
{
    SECTION("is not defined for any types")
    {
        CHECK((!and_traits<neo_int, neo_bool>::value));
        CHECK((!and_traits<neo_int, bool>::value));
        CHECK((!and_traits<bool, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_byte>::value));
        CHECK((!and_traits<neo_int, byte>::value));
        CHECK((!and_traits<byte, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_short>::value));
        CHECK((!and_traits<neo_int, short>::value));
        CHECK((!and_traits<short, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_int>::value));
        CHECK((!and_traits<neo_int, int>::value));
        CHECK((!and_traits<int, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_llong>::value));
        CHECK((!and_traits<neo_int, llong>::value));
        CHECK((!and_traits<llong, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_ubyte>::value));
        CHECK((!and_traits<neo_int, ubyte>::value));
        CHECK((!and_traits<ubyte, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_ushort>::value));
        CHECK((!and_traits<neo_int, ushort>::value));
        CHECK((!and_traits<ushort, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_uint>::value));
        CHECK((!and_traits<neo_int, uint>::value));
        CHECK((!and_traits<uint, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_ullong>::value));
        CHECK((!and_traits<neo_int, ullong>::value));
        CHECK((!and_traits<ullong, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_float>::value));
        CHECK((!and_traits<neo_int, float>::value));
        CHECK((!and_traits<float, neo_int>::value));

        CHECK((!and_traits<neo_int, neo_double>::value));
        CHECK((!and_traits<neo_int, double>::value));
        CHECK((!and_traits<double, neo_int>::value));
    }
}

TEST_CASE("neo_uint bitwise and", "neo_uint")
{
    SECTION("is not defined for boolean types")
    {
        CHECK((!bitand_traits<neo_uint, neo_bool>::value));
        CHECK((!bitand_traits<neo_uint, bool>::value));
        CHECK((!bitand_traits<bool, neo_uint>::value));
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!bitand_traits<neo_uint, neo_byte>::value));
        CHECK((!bitand_traits<neo_uint, byte>::value));
        CHECK((!bitand_traits<byte, neo_uint>::value));

        CHECK((!bitand_traits<neo_uint, neo_short>::value));
        CHECK((!bitand_traits<neo_uint, short>::value));
        CHECK((!bitand_traits<short, neo_uint>::value));

        CHECK((!bitand_traits<neo_uint, neo_int>::value));
        CHECK((!bitand_traits<neo_uint, int>::value));
        CHECK((!bitand_traits<int, neo_uint>::value));

        CHECK((!bitand_traits<neo_uint, neo_llong>::value));
        CHECK((!bitand_traits<neo_uint, llong>::value));
        CHECK((!bitand_traits<llong, neo_uint>::value));
    }

    SECTION("is defined for unsigned integer types")
    {
        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, neo_ubyte>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, ubyte>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<ubyte, neo_uint>>::value));

        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, neo_ushort>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, ushort>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<ushort, neo_uint>>::value));

        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, neo_uint>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<neo_uint, uint>>::value));
        CHECK((std::is_same<neo_uint, add_traits_t<uint, neo_uint>>::value));

        CHECK((std::is_same<neo_ullong, add_traits_t<neo_uint, neo_ullong>>::value));
        CHECK((std::is_same<neo_ullong, add_traits_t<neo_uint, ullong>>::value));
        CHECK((std::is_same<neo_ullong, add_traits_t<ullong, neo_uint>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(neo_uint(8u) == (neo_uint(12u) & neo_ubyte(ubyte(10u))));
            CHECK(neo_uint(8u) == (neo_uint(12u) & ubyte(10u)));
            CHECK(neo_uint(8u) == (ubyte(12u) & neo_uint(10u)));

            CHECK(neo_uint(8u) == (neo_uint(12u) & neo_ushort(ushort(10u))));
            CHECK(neo_uint(8u) == (neo_uint(12u) & ushort(10u)));
            CHECK(neo_uint(8u) == (ushort(12u) & neo_uint(10u)));

            CHECK(neo_uint(8u) == (neo_uint(12u) & neo_uint(uint(10u))));
            CHECK(neo_uint(8u) == (neo_uint(12u) & uint(10u)));
            CHECK(neo_uint(8u) == (uint(12u) & neo_uint(10u)));

            CHECK(neo_ullong(8u) == (neo_uint(12u) & neo_ullong(ullong(10u))));
            CHECK(neo_ullong(8u) == (neo_uint(12u) & ullong(10u)));
            CHECK(neo_ullong(8u) == (ullong(12u) & neo_uint(10u)));
        }
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!bitand_traits<neo_uint, neo_float>::value));
        CHECK((!bitand_traits<neo_uint, float>::value));
        CHECK((!bitand_traits<float, neo_uint>::value));

        CHECK((!bitand_traits<neo_uint, neo_double>::value));
        CHECK((!bitand_traits<neo_uint, double>::value));
        CHECK((!bitand_traits<double, neo_uint>::value));
    }
}

TEST_CASE("neo_int can be incremented", "neo_int")
{
    neo_int a = 10;

    CHECK(++a == 11);
    CHECK(a++ == 11);
    CHECK(a == 12);
}

TEST_CASE("neo_int can be decremented", "neo_int")
{
    neo_int a = 10;

    CHECK(--a == 9);
    CHECK(a-- == 9);
    CHECK(a == 8);
}

TEST_CASE("neo_int can be incremented in place", "neo_int")
{
    neo_int a = 10;

    CHECK((a += byte(2)) == 12);
    CHECK((a += short(2)) == 14);
    CHECK((a += int(2)) == 16);

    CHECK((a += neo_byte(byte(2))) == 18);
    CHECK((a += neo_short(short(2))) == 20);
    CHECK((a += neo_int(int(2))) == 22);
}

TEST_CASE("neo_int can be decremented in place", "neo_int")
{
    neo_int a = 10;

    CHECK((a -= byte(2)) == 8);
    CHECK((a -= short(2)) == 6);
    CHECK((a -= int(2)) == 4);

    CHECK((a -= neo_byte(byte(2))) == 2);
    CHECK((a -= neo_short(short(2))) == 0);
    CHECK((a -= neo_int(int(2))) == -2);
}

TEST_CASE("neo_byte and neo_short do not exhibit integral promotion", "neo_int")
{
    CHECK((std::is_same<int, decltype(byte() + byte())>::value));
    CHECK((std::is_same<neo_byte, decltype(neo_byte() + neo_byte())>::value));

    CHECK((std::is_same<int, decltype(short() + short())>::value));
    CHECK((std::is_same<neo_short, decltype(neo_short() + neo_short())>::value));
}

TEST_CASE("neo_bool is can be constructed from bool", "neo_bool")
{
    neo_bool f = false;
    neo_bool t = true;

    CHECK(f == false);
    REQUIRE(t == true);
}

TEST_CASE("neo_bool and", "neo_bool")
{
    SECTION("is defined for boolean types")
    {
        CHECK((std::is_same<bool, and_traits_t<neo_bool, neo_bool>>::value));
        CHECK((std::is_same<bool, and_traits_t<neo_bool, bool>>::value));
        CHECK((std::is_same<bool, and_traits_t<bool, neo_bool>>::value));

        SECTION("and gives the correct results")
        {
            CHECK(false == (neo_bool(false) && neo_bool(false)));
            CHECK(false == (neo_bool(false) && false));
            CHECK(false == (false && neo_bool(false)));

            CHECK(false == (neo_bool(false) && neo_bool(true)));
            CHECK(false == (neo_bool(false) && true));
            CHECK(false == (false && neo_bool(true)));

            CHECK(false == (neo_bool(true) && neo_bool(false)));
            CHECK(false == (neo_bool(true) && false));
            CHECK(false == (true && neo_bool(false)));

            CHECK(true == (neo_bool(true) && neo_bool(true)));
            CHECK(true == (neo_bool(true) && true));
            CHECK(true == (true && neo_bool(true)));
        }
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!and_traits<neo_bool, neo_byte>::value));
        CHECK((!and_traits<neo_bool, byte>::value));
        CHECK((!and_traits<byte, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_short>::value));
        CHECK((!and_traits<neo_bool, short>::value));
        CHECK((!and_traits<short, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_int>::value));
        CHECK((!and_traits<neo_bool, int>::value));
        CHECK((!and_traits<int, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_llong>::value));
        CHECK((!and_traits<neo_bool, llong>::value));
        CHECK((!and_traits<llong, neo_bool>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!and_traits<neo_bool, neo_ubyte>::value));
        CHECK((!and_traits<neo_bool, ubyte>::value));
        CHECK((!and_traits<ubyte, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_ushort>::value));
        CHECK((!and_traits<neo_bool, ushort>::value));
        CHECK((!and_traits<ushort, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_uint>::value));
        CHECK((!and_traits<neo_bool, uint>::value));
        CHECK((!and_traits<uint, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_ullong>::value));
        CHECK((!and_traits<neo_bool, ullong>::value));
        CHECK((!and_traits<ullong, neo_bool>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!and_traits<neo_bool, neo_float>::value));
        CHECK((!and_traits<neo_bool, float>::value));
        CHECK((!and_traits<float, neo_bool>::value));

        CHECK((!and_traits<neo_bool, neo_double>::value));
        CHECK((!and_traits<neo_bool, double>::value));
        CHECK((!and_traits<double, neo_bool>::value));
    }
}

TEST_CASE("neo_bool or", "neo_bool")
{
    SECTION("is defined for boolean types")
    {
        CHECK((std::is_same<bool, or_traits_t<neo_bool, neo_bool>>::value));
        CHECK((std::is_same<bool, or_traits_t<neo_bool, bool>>::value));
        CHECK((std::is_same<bool, or_traits_t<bool, neo_bool>>::value));

        SECTION("or gives the correct results")
        {
            CHECK(false == (neo_bool(false) || neo_bool(false)));
            CHECK(false == (neo_bool(false) || false));
            CHECK(false == (false || neo_bool(false)));

            CHECK(true == (neo_bool(false) || neo_bool(true)));
            CHECK(true == (neo_bool(false) || true));
            CHECK(true == (false || neo_bool(true)));

            CHECK(true == (neo_bool(true) || neo_bool(false)));
            CHECK(true == (neo_bool(true) || false));
            CHECK(true == (true || neo_bool(false)));

            CHECK(true == (neo_bool(true) || neo_bool(true)));
            CHECK(true == (neo_bool(true) || true));
            CHECK(true == (true || neo_bool(true)));
        }
    }

    SECTION("is not defined for signed integer types")
    {
        CHECK((!or_traits<neo_bool, neo_byte>::value));
        CHECK((!or_traits<neo_bool, byte>::value));
        CHECK((!or_traits<byte, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_short>::value));
        CHECK((!or_traits<neo_bool, short>::value));
        CHECK((!or_traits<short, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_int>::value));
        CHECK((!or_traits<neo_bool, int>::value));
        CHECK((!or_traits<int, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_llong>::value));
        CHECK((!or_traits<neo_bool, llong>::value));
        CHECK((!or_traits<llong, neo_bool>::value));
    }

    SECTION("is not defined for unsigned integer types")
    {
        CHECK((!or_traits<neo_bool, neo_ubyte>::value));
        CHECK((!or_traits<neo_bool, ubyte>::value));
        CHECK((!or_traits<ubyte, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_ushort>::value));
        CHECK((!or_traits<neo_bool, ushort>::value));
        CHECK((!or_traits<ushort, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_uint>::value));
        CHECK((!or_traits<neo_bool, uint>::value));
        CHECK((!or_traits<uint, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_ullong>::value));
        CHECK((!or_traits<neo_bool, ullong>::value));
        CHECK((!or_traits<ullong, neo_bool>::value));
    }

    SECTION("is not defined for floating point types")
    {
        CHECK((!or_traits<neo_bool, neo_float>::value));
        CHECK((!or_traits<neo_bool, float>::value));
        CHECK((!or_traits<float, neo_bool>::value));

        CHECK((!or_traits<neo_bool, neo_double>::value));
        CHECK((!or_traits<neo_bool, double>::value));
        CHECK((!or_traits<double, neo_bool>::value));
    }
}

TEST_CASE("neo_bool can be inverted", "neo_bool")
{
    neo_bool f = false;
    neo_bool t = true;

    REQUIRE(!f == true);
    REQUIRE(!t == false);
}

TEST_CASE("neo_ptr cannot be deleted", "neo_ptr")
{
    int i = 42;
    neo_ptr<int> p;
    p = new int(42);

    //CHECK(p[0u] == 42);
    //CHECK(*(p + 0) == 42);

    delete p.get();
}

TEST_CASE("neo_ptr", "neo_ptr")
{
    neo_ptr<neo_int> p;

    CHECK(!p);
    CHECK(p == nullptr);

    neo_int i[] = { 1, 2, 3, 4, 5 };
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

TEST_CASE("neo_ref", "neo_ref")
{
             int const&  ri = 42; // legal
//    neo::ref<int const> nri = 42; // illegal

    neo_int i[] = { 1, 2, 3, 4, 5 };

    auto ar = make_ref(i[0]);
    neo_ref<neo_int> p = i[0];

    CHECK(*p == 1);
    *p = 3;
    CHECK(i[0] == 3);

    p = i[1];

    CHECK(*p == 2);

    int myint = 0;
    neo_ref<int const> r = myint;
    neo_ptr<int const> pr = &r.get();
}

TEST_CASE("neo_ref<T> copy construction", "neo_ref")
{
    SECTION("is defined for references")
    {
        CHECK((copy_construction_traits<neo_ref<int>, int>::value));
        CHECK((copy_construction_traits<neo_ref<int>, neo_ref<int>>::value));
        CHECK((copy_construction_traits<neo_ref<int>, neo_ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(copy_construction_traits<neo_ref<int>, int const>::value));
        CHECK(!(copy_construction_traits<neo_ref<int>, neo_ref<int const>>::value));
        CHECK(!(copy_construction_traits<neo_ref<int>, neo_ref<int const> const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((copy_construction_traits<neo_ref<base>, derived>::value));
        CHECK((copy_construction_traits<neo_ref<base>, neo_ref<derived>>::value));
        CHECK((copy_construction_traits<neo_ref<base>, neo_ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(copy_construction_traits<neo_ref<base>, derived const>::value));
        CHECK(!(copy_construction_traits<neo_ref<base>, neo_ref<derived const>>::value));
        CHECK(!(copy_construction_traits<neo_ref<base>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T const> copy construction", "neo_ref")
{
    SECTION("is defined for all references")
    {
        CHECK((copy_construction_traits<neo_ref<int const>, int>::value));
        CHECK((copy_construction_traits<neo_ref<int const>, int const>::value));
        CHECK((copy_construction_traits<neo_ref<int const>, neo_ref<int>>::value));
        CHECK((copy_construction_traits<neo_ref<int const>, neo_ref<int> const>::value));
        CHECK((copy_construction_traits<neo_ref<int const>, neo_ref<int const>>::value));
        CHECK((copy_construction_traits<neo_ref<int const>, neo_ref<int const> const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((copy_construction_traits<neo_ref<base const>, derived>::value));
        CHECK((copy_construction_traits<neo_ref<base const>, derived const>::value));
        CHECK((copy_construction_traits<neo_ref<base const>, neo_ref<derived>>::value));
        CHECK((copy_construction_traits<neo_ref<base const>, neo_ref<derived> const>::value));
        CHECK((copy_construction_traits<neo_ref<base const>, neo_ref<derived const>>::value));
        CHECK((copy_construction_traits<neo_ref<base const>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T> move construction", "neo_ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_construction_traits<neo_ref<int>, int>::value));
        CHECK(!(move_construction_traits<neo_ref<int>, int const>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((move_construction_traits<neo_ref<int>, neo_ref<int>>::value));
        CHECK((move_construction_traits<neo_ref<int>, neo_ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(move_construction_traits<neo_ref<int>, neo_ref<int const>>::value));
        CHECK(!(move_construction_traits<neo_ref<int>, neo_ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_construction_traits<neo_ref<base>, derived>::value));
        CHECK(!(move_construction_traits<neo_ref<base>, derived const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((move_construction_traits<neo_ref<base>, neo_ref<derived>>::value));
        CHECK((move_construction_traits<neo_ref<base>, neo_ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(move_construction_traits<neo_ref<base>, neo_ref<derived const>>::value));
        CHECK(!(move_construction_traits<neo_ref<base>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T const> move construction", "neo_ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_construction_traits<neo_ref<int const>, int>::value));
        CHECK(!(move_construction_traits<neo_ref<int const>, int const>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((move_construction_traits<neo_ref<int const>, neo_ref<int>>::value));
        CHECK((move_construction_traits<neo_ref<int const>, neo_ref<int> const>::value));
        CHECK((move_construction_traits<neo_ref<int const>, neo_ref<int const>>::value));
        CHECK((move_construction_traits<neo_ref<int const>, neo_ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_construction_traits<neo_ref<base const>, derived>::value));
        CHECK(!(move_construction_traits<neo_ref<base const>, derived const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((move_construction_traits<neo_ref<base const>, neo_ref<derived>>::value));
        CHECK((move_construction_traits<neo_ref<base const>, neo_ref<derived> const>::value));
        CHECK((move_construction_traits<neo_ref<base const>, neo_ref<derived const>>::value));
        CHECK((move_construction_traits<neo_ref<base const>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T> copy assignment", "neo_ref")
{
    SECTION("is defined for references")
    {
        CHECK((copy_assignment_traits<neo_ref<int>, int>::value));
        CHECK((copy_assignment_traits<neo_ref<int>, neo_ref<int>>::value));
        CHECK((copy_assignment_traits<neo_ref<int>, neo_ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(copy_assignment_traits<neo_ref<int>, int const>::value));
        CHECK(!(copy_assignment_traits<neo_ref<int>, neo_ref<int const>>::value));
        CHECK(!(copy_assignment_traits<neo_ref<int>, neo_ref<int const> const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((copy_assignment_traits<neo_ref<base>, derived>::value));
        CHECK((copy_assignment_traits<neo_ref<base>, neo_ref<derived>>::value));
        CHECK((copy_assignment_traits<neo_ref<base>, neo_ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(copy_assignment_traits<neo_ref<base>, derived const>::value));
        CHECK(!(copy_assignment_traits<neo_ref<base>, neo_ref<derived const>>::value));
        CHECK(!(copy_assignment_traits<neo_ref<base>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T const> copy assignment", "neo_ref")
{
    SECTION("is defined for all references")
    {
        CHECK((copy_assignment_traits<neo_ref<int const>, int>::value));
        CHECK((copy_assignment_traits<neo_ref<int const>, int const>::value));
        CHECK((copy_assignment_traits<neo_ref<int const>, neo_ref<int>>::value));
        CHECK((copy_assignment_traits<neo_ref<int const>, neo_ref<int> const>::value));
        CHECK((copy_assignment_traits<neo_ref<int const>, neo_ref<int const>>::value));
        CHECK((copy_assignment_traits<neo_ref<int const>, neo_ref<int const> const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((copy_assignment_traits<neo_ref<base const>, derived>::value));
        CHECK((copy_assignment_traits<neo_ref<base const>, derived const>::value));
        CHECK((copy_assignment_traits<neo_ref<base const>, neo_ref<derived>>::value));
        CHECK((copy_assignment_traits<neo_ref<base const>, neo_ref<derived> const>::value));
        CHECK((copy_assignment_traits<neo_ref<base const>, neo_ref<derived const>>::value));
        CHECK((copy_assignment_traits<neo_ref<base const>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T> move assignment", "neo_ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_assignment_traits<neo_ref<int>, int>::value));
        CHECK(!(move_assignment_traits<neo_ref<int>, int const>::value));
    }

    SECTION("is defined for references")
    {
        CHECK((move_assignment_traits<neo_ref<int>, neo_ref<int>>::value));
        CHECK((move_assignment_traits<neo_ref<int>, neo_ref<int> const>::value));
    }

    SECTION("is not defined for references to const")
    {
        CHECK(!(move_assignment_traits<neo_ref<int>, neo_ref<int const>>::value));
        CHECK(!(move_assignment_traits<neo_ref<int>, neo_ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_assignment_traits<neo_ref<base>, derived>::value));
        CHECK(!(move_assignment_traits<neo_ref<base>, derived const>::value));
    }

    SECTION("is defined for subclass references")
    {
        CHECK((move_assignment_traits<neo_ref<base>, neo_ref<derived>>::value));
        CHECK((move_assignment_traits<neo_ref<base>, neo_ref<derived> const>::value));
    }

    SECTION("is not defined for subclass references to const")
    {
        CHECK(!(move_assignment_traits<neo_ref<base>, neo_ref<derived const>>::value));
        CHECK(!(move_assignment_traits<neo_ref<base>, neo_ref<derived const> const>::value));
    }
}

TEST_CASE("neo_ref<T const> move assignment", "neo_ref")
{
    SECTION("is not defined for regular references")
    {
        CHECK(!(move_assignment_traits<neo_ref<int const>, int>::value));
        CHECK(!(move_assignment_traits<neo_ref<int const>, int const>::value));
    }

    SECTION("is defined for all references")
    {
        CHECK((move_assignment_traits<neo_ref<int const>, neo_ref<int>>::value));
        CHECK((move_assignment_traits<neo_ref<int const>, neo_ref<int> const>::value));
        CHECK((move_assignment_traits<neo_ref<int const>, neo_ref<int const>>::value));
        CHECK((move_assignment_traits<neo_ref<int const>, neo_ref<int const> const>::value));
    }

    SECTION("is not defined for regular subclass references")
    {
        CHECK(!(move_assignment_traits<neo_ref<base const>, derived>::value));
        CHECK(!(move_assignment_traits<neo_ref<base const>, derived const>::value));
    }

    SECTION("is defined for all subclass references")
    {
        CHECK((move_assignment_traits<neo_ref<base const>, neo_ref<derived>>::value));
        CHECK((move_assignment_traits<neo_ref<base const>, neo_ref<derived> const>::value));
        CHECK((move_assignment_traits<neo_ref<base const>, neo_ref<derived const>>::value));
        CHECK((move_assignment_traits<neo_ref<base const>, neo_ref<derived const> const>::value));
    }
}
