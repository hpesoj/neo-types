# NeoTypes

NeoTypes is a header-only library containing wrappers for the C++ fundamental types (e.g. `bool`, `int`, `float`). These wrappers aim to offer safer, saner, more consistent behaviour than their fundamental counterparts.

## Quick Example

A Neo type, `neo<T>`, may be used in place of any fundamental type, `T`.

    neo<int> i;
    neo<float> f;
    neo<bool> b;

Type aliases exist for convenience.

    neo_int i;
    neo_float f;
    neo_bool b;

`neo<T*>` may also be used in place of any pointer, `T*`.

    neo<int*> p;

The `neo_ptr` template type alias may also be used.

    neo_ptr<int> p;
    
A `neo_ptr` may point to any type, including other Neo types.

    neo_ptr<neo_int> p;

Neo types support a safer subset of the operations supported by their corresponding fundamental types, have safer default behaviour, and even have some features not offered by the fundamental types.

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

Fundamental objects may implicitly convert from type `T` to type `U`, where some values representable by type `T` may not be represented by type `U` (narrowing conversions). Neo objects may only implicitly convert to types which can represent all values representable by the original type.

    neo_short s = neo_int();   // invalid: short cannot represent all int values
    
    neo_int   i = neo_uint();  // invalid: int cannot represent all uint values
    neo_uint  u = neo_int();   // invalid: uint cannot represent all int values
    
    neo_float f = neo_int();   // invalid: float cannot accurately represent all int values
    neo_int   i = neo_float(); // invalid: int cannot represent all float values

> Rationale

### No Conversion To Bool

The fundamental type, `bool`, is an integral type, and as such may implicitly convert to and from other numeric types. The Neo type, `neo_bool`, cannot implicitly _or_ explicitly convert to numeric types.

    if (neo_int()) {} // invalid
    if (neo_uint()) {} // invalid
    if (neo_float()) {} // invalid

> Rationale: `bool` is an integral type for compatibility with C, where the boolean values `true` and `false` were represented by the integer values `1` and `0` respectively. Conceptually, `true` and `false` have no corresponding integer values (they may be represented by any two numbers), and simply represent the result of a logical or boolean operation.

### No Bitwise Operations On Signed Integers

### No Integer Promotion

### No Void Pointer Arithmetic (GCC Extension)

## Interesting Use Cases

### `std::vector<neo_bool>`

### Uninitialized `std::vector`

## Known Issues
