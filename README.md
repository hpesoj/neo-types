# NeoTypes

NeoTypes is a header-only library containing wrappers for the C++ fundamental types (e.g. `bool`, `int`, `float`), pointers and references. These wrappers aim to offer safer, saner, more consistent behaviour than their fundamental counterparts.

## Quick Example

`neo::value<T>`, may be used in place of any fundamental type, `T`.

    neo::value<int> i;
    neo::value<float> f;
    neo::value<bool> b;

Type aliases exist for convenience.

    using namespace neo::aliases;

    neo_int i;
    neo_float f;
    neo_bool b;

`neo::ptr<T>` may be used in place of any regular pointer, `T*`.

    neo::ptr<int> p;

The `neo_ptr` template type alias may also be used.

    using namespace neo::aliases;

    neo_ptr<int> p;
    
A `neo::ptr` may point to any type, including other Neo types.

    neo::ptr<neo_int> p;

`neo::ref<T>` provides many of the features of a regular reference, `T&`, but has some different syntax and features. As such, `neo::ref` is expected to be useful in specific situations.

    neo::ref<int> ri = i;


## Features

### Zero Initialization

Fundamental objects (objects of fundamental type) with initializer may or may not be initialized to zero depending on the context. Neo objects (objects of Neo type) are _always_ initialized to zero by default.

    struct person
    {
        neo_int age;
        neo_float height;
        neo_bool is_alive;
    };
    
    person bob;
    assert(bob.age == 0);
    assert(bob.height == 0.0f);
    assert(!bob.is_alive);

> Rationale: Accidentally forgetting to initialize a variable can be a source of bugs, including undefined behaviour. Always initializing to zero by default provides safety and consistency.

### Explicit Uninitialization

The value of Neo objects can be explicitly left undefined. Undefined objects are _never_ initialized to zero in any context.

    neo_double d = undefined;

> Rationale: Initialization to zero, while a sensible default, can be costly when many objects must be initialized. Giving the option to forego initialization allows us to _not pay for what we don't use_. In addition, Neo objects may be left uninitialized in contexts where fundamental objects would always be initialized to zero.

### Move Behaviour

Moving a fundamental object is the same as copying it. Moving a Neo object sets the moved-from object to zero.

    neo_uint a = 15u;
    neo_uint b = std::move(a);
    assert(a == 0u);

> Rationale: Zeroing the data members of a moved-from object is more often than not the desired behaviour. Making this the default behaviour allows the use of default move constructors. Data members may be manually copied if extreme efficiency is required.

### No Implicit Narrowing Conversions

Fundamental objects may implicitly convert from type `T` to type `U`, where all values representable by type `T` may be represented by type `U` (widening conversions). You must use `static_cast` to explicitly convert from type `T` to type `U` when some values representable by type `T` may not be represented by type `U` (narrowing conversions).

    neo_short s = neo_int();   // invalid: short cannot represent all int values
    
    neo_int   i = neo_uint();  // invalid: int cannot represent all uint values
    neo_uint  u = neo_int();   // invalid: uint cannot represent all int values
    
    neo_float f = neo_int();   // invalid: float cannot accurately represent all int values
    neo_int   i = neo_float(); // invalid: int cannot represent all float values

> Rationale: Implicit narrowing conversions may result in unexpected loss and/or corruption of information. If the programmer must explicitly perform narrowing conversions, they are forced to consider the potential dangers and, hopefully, ensure the correctness of their code.

> Note that even though some conversions from unsigned to signed integer types (e.g. `std::uint16_t` to `std::int32_t`) are widening, they not implicitly allowed. This is because signed and unsigned integers have different arithmetic behaviours (unsigned types use modulo arithmetic), and allowing implicit conversion can have surprising results.

### No Conversion To Bool

The fundamental type, `bool`, is an integral type, and as such may implicitly convert to and from other numeric types. The Neo type, `neo_bool`, cannot implicitly _or_ explicitly convert to numeric types.

    if (neo_int()) {} // invalid
    if (neo_uint()) {} // invalid
    if (neo_float()) {} // invalid

> Rationale: `bool` is an integral type for compatibility with C, where the boolean values `true` and `false` were represented by the integer values `1` and `0` respectively. Conceptually, `true` and `false` have no corresponding integer values (they may be represented by any two numbers), and simply represent the result of a logical or boolean operation.

### No Bitwise Operations On Signed Integers

### No Unexpected Integer Promotion

### No Void Pointer Arithmetic (GCC Extension)

## neo::ref and neo::optional_ref

A `neo::ref<T>` is a mixture between a reference and a pointer. Like a reference, it must be initialized on construction (though it may be `undefined`) and its wrapped reference will be `const` whenever the `neo::ref<T>` itself is `const`. Like a pointer, it may be copied, but a `neo::ref<T> const` may only be copied as a `neo::ref<T const>`. This is because allowing copying as a `neo::ref<T>` would violate const-correctness, as a `neo::ref<T> const` represents a `T const&`.

             neo_int   i;
    neo::ref<neo_int> ri = i;

Unlike references, `neo::ref<T const>` cannot bind to and extend the life of r-values.

             int const&  ri = 42; // legal
    neo::ref<int const> nri = 42; // illegal

This means that `neo::ref` is not suited for use in function parameters. Instead, `neo::ref` is designed as a safer replacement for pointers, where they are used _like_ references (as opposed to arrays or owning pointers). Here is an example.

    struct person
    {
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

A const person not only cannot switch their pet, they cannot modify their pet in any way. This is unlike with either references or pointers, and ensures that our person is const-correct.

    person const bob = { fluffy };
    bob.pet = fido; // error: neo_ref<animal>::operator=(animal&) is non-const
    bob.pet->walk() // error: animal::walk() is non-const

But a person can never have no pet. However, forcing pet ownership seems a bit draconian, so we can use `neo::optional_ref` to allow optional pet ownership.

    struct person
    {
        neo::optional_ref<animal> pet;
    };

Now we are not forced to provide a pet.

    person bob; // legal

Though we can still assign a pet.

    bob.pet = fluffy;

But we may want to check for pet ownership to avoid interacting with a non-existent pet.

    if (bob.pet) bob.pet->walk();

Pet ownership can be rescinded.

    bob.pet = neo::nullref;

## Interesting Use Cases

### `std::vector<neo_bool>`

`std::vector<bool>` is not a true STL container, because it does not contain `bool`s, and its `operator[]` does not return a `bool&`, but a _proxy reference object_. `std::vector<neo_bool>` on the other hand is not specialized, and so _is_ a true STL container.

    template<class T>
    void f(std::vector<T>& v)
    {
        for (auto& t : v);
    }
    
    std::vector<bool> sbv;
    f(sbv); // error

    std::vector<neo_bool> nbv;
    f(nbv); // valid

### Uninitialized `std::vector`

`std::vector` is designed to be easy to use safely; as such, `std::vector`s cannot be created or resized without providing values to fill it with. You can ensure that your `std::vector` has allocated enough memory to fit a specific number of values by calling `std::vector::reserve`.

    constexpr int count = 10;

    std::vector<int> v;
    v.reserve(count);

This works fine, but there may be situations where it would be easier to resize the `std::vector` first, and define its content later. With the Neo types, this is possible.

    template<typename Container, typename... Args>
    void emplace_back_n(Container& c, std::size_t n, Args const&... args)
    {
        for (std::size_t i = 0; i < n; ++i)
        {
            c.emplace_back(args...);
        }
    }

    constexpr int count = 10;

    std::vector<neo_int> v;
    v.reserve(count);
    emplace_back_n(v, count, neo::undefined);

## Known Issues
