//
// Copyright (c) 2023, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/SourceLocation.h>

#include <core/Formatting.h>

namespace core {

void Formatter<SourceLocation>::format(
    StringBuilder &b, const SourceLocation &location)
{
    Formatting::format_into(
        b,
        location.function_name(),
        "@"sv,
        location.file_name(),
        ":"sv,
        location.line(),
        ":"sv,
        location.column());
}

}  // namespace core