//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cstdio>
#include <utility>

#include <core/Formatter.h>
#include <core/StringBuilder.h>
#include <core/Types.h>

namespace core {

class Formatting
{
public:
    template<typename... Args>
    inline static String format(Args &&...args)
    {
        core::StringBuilder builder;
        core::Formatting::format_into(builder, std::forward<Args>(args)...);
        return builder.toString();
    }
    template<typename... Args>
    inline static void format_into(StringBuilder &builder, Args &&...args)
    {
        (format_one(builder, std::forward<Args>(args)), ...);
    }

private:
    template<typename T>
    inline static void format_one(StringBuilder &builder, const T &value)
    {
        Formatter<T>::format(builder, std::forward<const T &>(value));
    }
    template<typename T>
    inline static void format_one(StringBuilder &builder, const T *value)
    {
        Formatter<T>::format(builder, std::forward<const T *>(value));
    }
};

}  // namespace core

using core::Formatting;

#define debug_log(...)                                       \
    do {                                                     \
        auto string = core::Formatting::format(__VA_ARGS__); \
        auto view = string.view();                           \
        printf("%.*s", int(view.size()), view.data());       \
    } while (0)

#define compose(...) core::Formatting::format(__VA_ARGS__)
