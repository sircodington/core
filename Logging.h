//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include "Logger.h"

namespace core {

class Logging
{
public:
    template<typename... Args>
    inline static void log(Args... args)
    {
        (log1(args), ...);
    }

private:
    template<typename T>
    inline static void log1(T value)
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
class Logger<int>
{
public:
    static void log(int);
};

}  // namespace core

#define debug_log(...)                   \
    do {                                 \
        core::Logging::log(__VA_ARGS__); \
    } while (0)
