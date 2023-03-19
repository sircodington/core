//
// Copyright (c) 2023, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#if __has_include(<source_location>)
#include <source_location>
#define HAS_STD_SOURCE_LOCATION
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
namespace std {
// clang-format off
using source_location = experimental::source_location; // NOLINT(cert-dcl58-cpp)
// clang-format on
}  // namespace std
#define HAS_STD_SOURCE_LOCATION
#endif

#include <core/Formatter.h>
#include <core/StringView.h>

namespace core {

#ifdef HAS_STD_SOURCE_LOCATION
#define SOURCE_LOCATION_HERE (core::SourceLocation::std_current())
#else
#define SOURCE_LOCATION_HERE \
    (SourceLocation(__FILE__, __LINE__, 0, __FUNCTION__))
#endif

class SourceLocation
{
public:
    constexpr SourceLocation(
        core::StringView file_name,
        u32 line,
        u32 column,
        core::StringView function_name)
        : m_file_name(file_name)
        , m_line(line)
        , m_column(column)
        , m_function_name(function_name)
    {
    }

#ifdef HAS_STD_SOURCE_LOCATION
    constexpr static core::SourceLocation from_std(
        std::source_location location)
    {
        return SourceLocation {
            location.file_name(),
            location.line(),
            location.column(),
            location.function_name(),
        };
    }

    constexpr static core::SourceLocation std_current(
        std::source_location location = std::source_location::current())
    {
        return core::SourceLocation::from_std(location);
    }
#endif

    constexpr static core::SourceLocation here(
        core::SourceLocation location = SOURCE_LOCATION_HERE)
    {
        return location;
    }
    constexpr static core::SourceLocation current(
        core::SourceLocation location = SOURCE_LOCATION_HERE)
    {
        return location;
    }

    [[nodiscard]] constexpr const core::StringView &file_name() const
    {
        return m_file_name;
    }
    [[nodiscard]] constexpr u32 line() const { return m_line; }
    [[nodiscard]] constexpr u32 column() const { return m_column; }
    [[nodiscard]] constexpr const core::StringView &function_name() const
    {
        return m_function_name;
    }

private:
    core::StringView m_file_name;
    u32 m_line { 0 };
    u32 m_column { 0 };
    core::StringView m_function_name;
};

template<>
class Formatter<SourceLocation>
{
public:
    static void format(StringBuilder &, const SourceLocation &);
};

}  // namespace core

using core::SourceLocation;
