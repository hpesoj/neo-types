#include <catch.hpp>

#include <neo_types.hpp>

using namespace neo_types;

TEST_CASE("neo_int is zero-constructed by default", "neo_int")
{
    neo_int a;
    REQUIRE(a == 0);
}

TEST_CASE("neo_int can be explicitly uninitialized", "neo_int")
{
    neo_int a = undefined;

    REQUIRE(a != 0);
}

TEST_CASE("neo_int can be copy constructed", "neo_int")
{
    neo_int a = 10;
    neo_int b = a;

    REQUIRE(a == 10);
    REQUIRE(b == 10);
    REQUIRE(a == b);
}

TEST_CASE("neo_int can be copy assigned", "neo_int")
{
    neo_int a = 10;
    neo_int b;

    b = a;

    REQUIRE(a == 10);
    REQUIRE(b == 10);
    REQUIRE(a == b);
}

TEST_CASE("neo_int can be move constructed", "neo_int")
{
    neo_int a = 10;
    neo_int b = std::move(a);

    REQUIRE(a == 0);
    REQUIRE(b == 10);
    REQUIRE(a != b);
}

TEST_CASE("neo_int can be move assigned", "neo_int")
{
    neo_int a = 10;
    neo_int b;

    b = std::move(a);

    REQUIRE(a == 0);
    REQUIRE(b == 10);
    REQUIRE(a != b);
}

TEST_CASE("neo_int can be constructed from char, short and int", "neo_int")
{
    neo_int a = char(10);
    neo_int b = short(10);
    neo_int c = int(10);

    REQUIRE(a == 10);
    REQUIRE(b == 10);
    REQUIRE(c == 10);
}

TEST_CASE("neo_int can be implicitly converted to int and long long", "neo_int")
{
    neo_int n = 10;

    int a = n;
    long long b = n;

    REQUIRE(a == 10);
    REQUIRE(b == 10);
}

TEST_CASE("neo_int can be explicitly converted to char and short", "neo_int")
{
    neo_int n = 10;

    auto a = static_cast<char>(n);
    auto b = static_cast<short>(n);

    REQUIRE(a == 10);
    REQUIRE(b == 10);
}

TEST_CASE("neo_int can be explicitly converted to float and double", "neo_int")
{
    neo_int n = 10;

    auto a = static_cast<float>(n);
    auto b = static_cast<double>(n);

    REQUIRE(a == 10.0f);
    REQUIRE(b == 10.0);
}

TEST_CASE("neo_int can be added", "neo_int")
{
    neo_int a = 10;
    neo_int b = 4;

    REQUIRE(a + b == 14);
    REQUIRE(a + 4 == 14);
}
