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

## neo_ref

A `neo_ref<T>` is a mixture between a reference and a pointer. Like a reference, it must be initialized on construction (though it may be `undefined`) and its wrapped reference will be `const` whenever the `neo_ref<T>` itself is `const`. Like a pointer, it may be copied, but a `neo_ref<T> const` may only be copied as a `neo_ref<T const>`. This is because allowing copying as a `neo_ref<T>` would bypass const-correctness, as a `neo_ref<T> const` actually represents a `T const&`.

## Interesting Use Cases

### `std::vector<neo_bool>`

### Uninitialized `std::vector`

## Known Issues
