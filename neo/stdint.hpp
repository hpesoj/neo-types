/*
 * Neo Types Library
 * Copyright 2016 Joseph Thomson
 */

#ifndef NEO_STDINT_HPP
#define NEO_STDINT_HPP

#include <neo/value.hpp>

#include <cstdint>

namespace neo
{

namespace aliases
{

using neo_int8 = value<std::int8_t>;
using neo_int16 = value<std::int16_t>;
using neo_int32 = value<std::int32_t>;
using neo_int64 = value<std::int64_t>;

using neo_int_fast8 = value<std::int_fast8_t>;
using neo_int_fast16 = value<std::int_fast16_t>;
using neo_int_fast32 = value<std::int_fast32_t>;
using neo_int_fast64 = value<std::int_fast64_t>;

using neo_int_least8 = value<std::int_least8_t>;
using neo_int_least16 = value<std::int_least16_t>;
using neo_int_least32 = value<std::int_least32_t>;
using neo_int_least64 = value<std::int_least64_t>;

using neo_intmax = value<std::intmax_t>;
using neo_intptr = value<std::intptr_t>;

using neo_uint8 = value<std::uint8_t>;
using neo_uint16 = value<std::uint16_t>;
using neo_uint32 = value<std::uint32_t>;
using neo_uint64 = value<std::uint64_t>;

using neo_uint_fast8 = value<std::uint_fast8_t>;
using neo_uint_fast16 = value<std::uint_fast16_t>;
using neo_uint_fast32 = value<std::uint_fast32_t>;
using neo_uint_fast64 = value<std::uint_fast64_t>;

using neo_uint_least8 = value<std::uint_least8_t>;
using neo_uint_least16 = value<std::uint_least16_t>;
using neo_uint_least32 = value<std::uint_least32_t>;
using neo_uint_least64 = value<std::uint_least64_t>;

using neo_uintmax = value<std::uintmax_t>;
using neo_uintptr = value<std::uintptr_t>;

} // namespace aliases

using namespace aliases;

} // namespace neo

#endif // NEO_STDINT_HPP
