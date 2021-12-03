//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/Types.h>

namespace core {

class StringBuilder;

template<typename T>
class Formatter
{
public:
    Formatter() = delete;
};

template<>
class Formatter<char>
{
public:
    static void format(StringBuilder &, char);
    static void format(StringBuilder &, const char *);
};

template<>
class Formatter<s8>
{
public:
    static void format(StringBuilder &, s8);
};
template<>
class Formatter<s16>
{
public:
    static void format(StringBuilder &, s16);
};
template<>
class Formatter<s32>
{
public:
    static void format(StringBuilder &, s32);
};
template<>
class Formatter<s64>
{
public:
    static void format(StringBuilder &, s64);
};

template<>
class Formatter<u8>
{
public:
    static void format(StringBuilder &, u8);
};
template<>
class Formatter<u16>
{
public:
    static void format(StringBuilder &, u16);
};
template<>
class Formatter<u32>
{
public:
    static void format(StringBuilder &, u32);
};
template<>
class Formatter<u64>
{
public:
    static void format(StringBuilder &, u64);
};

template<>
class Formatter<float>
{
public:
    static void format(StringBuilder &, float);
};
template<>
class Formatter<double>
{
public:
    static void format(StringBuilder &, double);
};

}  // namespace core

using core::Formatter;
