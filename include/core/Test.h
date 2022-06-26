//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

namespace core {

#define expect_eq(actual, expect)                  \
    do {                                           \
        const auto x = (actual);                   \
        const auto y = (expect);                   \
        if (x != y) {                              \
            debug_log(                             \
                "Expected equality of values\n  ", \
                #actual,                           \
                "\n  ",                            \
                x,                                 \
                "\n and\n  ",                      \
                #expect,                           \
                "\n  ",                            \
                y,                                 \
                "\n\n");                           \
        }                                          \
    } while (0)

}  // namespace core
