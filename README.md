# NeoTypes

NeoTypes is a header-only library containing wrappers for the C++ fundamental types (e.g. `bool`, `int`, `float`), pointers and references. These wrappers aim to offer safer, saner, more consistent behaviour than their fundamental counterparts.

## Quick Example

`neo::value<T>`, may be used in place of any fundamental type, `T`.

    neo::value<int> i;
    neo::value<unsigned int> i;
    neo::value<float> f;
    neo::value<bool> b;

Type aliases exist for convenience. Trailing underscores are used where aliases would otherwise be C++ keywords.

    neo::int_ i;
    neo::uint i;
    neo::float_ f;
    neo::bool_ b;

`neo::ptr<T>` may be used in place of any regular pointer, `T*`.

    neo::ptr<int> p;

A `neo::ptr` may point to any type, including other Neo types.

    neo::ptr<neo::int_> p;

`neo::ref<T>` provides many of the features of a regular reference, `T&`, but has some different syntax and features. As such, `neo::ref` is expected to be useful in specific situations.

    neo::ref<int> ri = i;

## Features

### Zero Initialization

Fundamental objects (objects of fundamental type) with initializer may or may not be initialized to zero depending on the context. Neo objects (objects of Neo type) are _always_ initialized to zero by default.

    struct person {
        neo::int_ age;
        neo::float_ height;
        neo::bool_ is_alive;
    };
    
    person bob;

    assert(bob.age == 0);
    assert(bob.height == 0.0f);
    assert(!bob.is_alive);

> Rationale: Accidentally forgetting to initialize a variable can be a source of bugs, including undefined behaviour. Always initializing to zero by default provides safety and consistency.

### Explicit Uninitialization

The value of Neo objects can be explicitly left undefined. Undefined objects are _never_ initialized to zero in any context.

    neo::double_ d = neo::undefined;

> Rationale: Initialization to zero, while a sensible default, can be costly when many objects must be initialized. Giving the option to forego initialization allows us to _not pay for what we don't use_. In addition, Neo objects may be left uninitialized in contexts where fundamental objects would always be initialized to zero.

> Note: Copying uninitialized objects is undefined behaviour. Be extra careful when using `neo::undefined`. This feature is intended for cases where extreme optimization is required. Generally, default construction is what you should use.

### No Implicit Narrowing Conversions

Fundamental objects may implicitly convert from type `T` to type `U`, where all values representable by type `T` may be represented by type `U` (widening conversions). You must use `static_cast` to explicitly convert from type `T` to type `U` when some values representable by type `T` may not be represented by type `U` (narrowing conversions).

    neo::short_ add_a_dozen(int value) {
        return value + 12; // error: short cannot represent all int values
    }

> Rationale: Implicit narrowing conversions may result in unexpected loss and/or corruption of information. If the programmer must explicitly perform narrowing conversions, they are forced to consider the potential dangers and, hopefully, ensure the correctness of their code.

### No Mixing Signed and Unsigned Types

Signed and unsigned Neo types do not implicitly convert to each other, and cannot be mixed in the same arithmetic expression.

    neo::int_ get_index(neo::int_ offset, neo::size count) {
        return offset + count; // error: mixing of signed and unsigned types
    }

> Rationale: Even though some conversions from unsigned to signed integer types (e.g. `std::uint16_t` to `std::int32_t`) are widening, they not implicitly allowed. This is because signed and unsigned integers have different arithmetic behaviours (unsigned types use modulo arithmetic), and allowing implicit conversion can have surprising results.

### No Conversion To Bool

The fundamental type, `bool`, is an integral type, and as such may implicitly convert to and from other numeric types. The Neo type, `neo::bool_`, cannot implicitly _or_ explicitly convert to numeric types.

    void foo(neo::int_ value) {
        if (value) { // error: not a boolean expression
            …

> Rationale: `bool` is an integral type for compatibility with C, where the boolean values `true` and `false` were represented by the integer values `1` and `0` respectively. Conceptually, `true` and `false` have no corresponding integer values (they may be represented by any two numbers), and simply represent the result of a logical or boolean operation.

### No Bitwise Operations On Signed Integers

Unsigned Neo integers support the bitwise operations `&`, `|` and `^`, while signed Neo integers do not.

    neo::int_ mask_it(neo::int_ value, neo::int_ mask) {
        return foo | mask; // error: signed bitwise operation
    }

> Rationale: Bitwise operations treat integers as though they are a sequence of bits. While all integers, signed and unsigned, have binary representations, signed integers necessarily use some kind of encoding to be able to represent negative numbers. This encoding is implementation-dependent, so bitwise operations on signed integers are also implementation-dependent. Implementation-dependent behaviour is seen as potentially unsafe, and is disallowed by the Neo library. In addition, signed bitwise operations are potentially confusing, so even though most modern C++ programs will use a _two's complement_ representation, it is still preferable to disallow such operations by default.

### No Unexpected Integer Promotion

What would you expect the return type here to be?

    auto sub(char a, char b) {
        return a - b;
    }

That's right! It's an `int`. But what about now?

    auto combine(unsigned char a, unsigned char b) {
        return a | b;
    }

Right again! It's still `int`. Wait. What...?

This surprising behaviour is in the interest of performance, but not in the interest of common sense. Neo integers do not promote unexpectedly. The type of the result of an arithmetic operation is always the type of the wider of the two operands. Here, the return type is `neo::uchar`.

    auto foo(neo::uchar a, neo::uchar b) {
        return = a | b;
    }

## neo::ref and neo::ptr

A `neo::ref<T>` is a mixture between a reference and a pointer. Like a reference, it must be initialized on construction (though it may be `neo::undefined`), but like a pointer, it may be reassigned.

             neo::int_   i;
    neo::ref<neo::int_> ri = neo::undefined;
                        ri = i;

Unlike references, `neo::ref<T const>` cannot bind to and extend the life of r-values.

             int const&  ri = 42; // legal
    neo::ref<int const> nri = 42; // illegal

This means that `neo::ref` is not suited for use with function parameters. Instead, `neo::ref` is designed as a safer replacement for pointers, where they are used _like_ references (as opposed to arrays or owning pointers). Here is an example.

    struct person {
        neo::ref<animal> pet;
    };

Here the person has a pet, but the lifetime of the pet is not tied to the owner. In fact, the person _must_ own a pet, because `person::pet` cannot be default constructed, like with references.

    person bob; // error

So the caller is forced to provide a pet for the person.

    animal fluffy;
    person bob = { fluffy };

We use pointer-like syntax to interact with the pet.

    bob.pet->walk();

And like with pointers, we _can_ change the person's pet.

    animal fido;
    bob.pet = fido;

We can check the identity of a person's pet.

    if (bob.pet == fido) { … }

But a person can never have no pet. However, perhaps forcing pet ownership seems a bit draconian, so we can instead use `neo::ptr` to allow optional pet ownership.

    struct person {
        neo::ptr<animal> pet;
    };

Now we are not forced to provide a pet.

    person bob; // legal

Though we can still assign a pet (as with regular pointers, we must take the address of the pet).

    bob.pet = &fluffy;

We may want to check for pet ownership to avoid interacting with a non-existent pet.

    if (bob.pet) bob.pet->walk();

And pet ownership can of course be rescinded.

    bob.pet = nullptr;

## Interesting Use Cases

### `std::vector<neo::bool_>`

`std::vector<bool>` is not a true STL container, because it does not contain `bool`s, and its `operator[]` does not return a `bool&`, but a _proxy reference object_. `std::vector<neo::bool_>` on the other hand is not specialized, and so _is_ a true STL container.

    template<class T>
    void foo(std::vector<T>& v) {
        for (auto& t : v);
    }
    
    std::vector<bool> sbv;
    f(sbv); // error

    std::vector<neo::bool_> nbv;
    f(nbv); // valid

### Uninitialized `std::vector`

`std::vector` is designed to be easy to use safely; as such, `std::vector`s cannot be created or resized without providing values to fill it with. You can ensure that your `std::vector` has allocated enough memory to fit a specific number of values by calling `std::vector::reserve`.

    constexpr std::size_t count = 10;

    std::vector<int> v;
    v.reserve(count);

This works fine, but there may be situations where it would be easier to resize the `std::vector` first, and define its content later. With the Neo types, this is possible.

    template<typename Container, typename... Args>
    void emplace_back_n(Container& c, neo::size n, Args const&... args) {
        for (neo::size i = 0u; i < n; ++i) {
            c.emplace_back(args...);
        }
    }

    constexpr neo::size count = 10u;

    std::vector<neo::int_> v;
    v.reserve(count);
    emplace_back_n(v, count, neo::undefined);

> Note: We have to be careful to `reserve` space in the vector, and construct the values in-place using `emplace_back`, as not calling `reserve` with sufficient space, using `push_back`, or calling the `std::vector` constructor would cause the `neo::undefined` Neo values to be copied, and copying undefined values is undefined behaviour.

## Known Issues

# Function Overloading
