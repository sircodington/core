//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "test_string_view.h"

#include <core/Formatting.h>
#include <core/Test.h>

namespace core {

static void test_string_view_last_index_of()
{
    const auto string = "Hello, World"sv;
    expect_eq(string.last_index_of('H'), 0);
    expect_eq(string.last_index_of('W'), 7);
    expect_eq(string.last_index_of('o'), 8);
    expect_eq(string.last_index_of('!'), -1);
}

void test_string_view() { test_string_view_last_index_of(); }

}  // namespace core