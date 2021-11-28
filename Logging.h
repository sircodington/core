//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include "Logger.h"
#include "Types.h"

namespace core {

class Logging
{
public:
    template<typename... Args>
    inline static void log(Args... args)
    {
        (log_one(args), ...);
    }

private:
    template<typename T>
    inline static void log_one(T value)
    {
        Logger<T>::log(value);
    }
};

template<>
class Logger<const char *>
{
public:
    static void log(const char *);
};

template<>
class Logger<s8>
{
public:
    static void log(s8);
};
template<>
class Logger<s16>
{
public:
    static void log(s16);
};
template<>
class Logger<s32>
{
public:
    static void log(s32);
};
template<>
class Logger<s64>
{
public:
    static void log(s64);
};

template<>
class Logger<u8>
{
public:
    static void log(u8);
};
template<>
class Logger<u16>
{
public:
    static void log(u16);
};
template<>
class Logger<u32>
{
public:
    static void log(u32);
};
template<>
class Logger<u64>
{
public:
    static void log(u64);
};

template<>
class Logger<float>
{
public:
    static void log(float);
};
template<>
class Logger<double>
{
public:
    static void log(double);
};

}  // namespace core

#define debug_log(...)                   \
    do {                                 \
        core::Logging::log(__VA_ARGS__); \
    } while (0)
