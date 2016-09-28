/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_UNDEFINED_HPP
#define NEO_UNDEFINED_HPP

namespace neo
{

struct undefined_t
{
    constexpr undefined_t() {}
};

constexpr undefined_t undefined;

} // namespace neo

#endif // NEO_UNDEFINED_HPP
