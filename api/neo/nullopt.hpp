/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_NULLOPT_HPP
#define NEO_NULLOPT_HPP

namespace neo
{

struct nullopt_t
{
    constexpr nullopt_t() {}
};

constexpr nullopt_t nullopt;

} // namespace neo

#endif // NEO_NULLOPT_HPP
