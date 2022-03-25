//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "test_lexical_file.h"

#include <core/Formatting.h>
#include <core/LexicalFile.h>

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

void test_lexical_file_parent()
{
    LexicalFile file;

    file = LexicalFile("a/b"sv).parent();
    expect_eq(file.path(), "a"sv);

    file = LexicalFile("a/b/").parent();
    expect_eq(file.path(), "a"sv);

    file = LexicalFile("a").parent();
    expect_eq(file.path(), ""sv);

    file = LexicalFile("/a/b").parent();
    expect_eq(file.path(), "/a"sv);

    file = LexicalFile("/a/b/").parent();
    expect_eq(file.path(), "/a"sv);

    file = LexicalFile("/a").parent();
    expect_eq(file.path(), "/"sv);
}

void test_lexical_file_child()
{
    LexicalFile file;

    file = LexicalFile("a").child("b"sv);
    expect_eq(file.path(), "a/b"sv);

    file = LexicalFile("a/b/").child("c"sv);
    expect_eq(file.path(), "a/b/c"sv);

    file = LexicalFile("").child("a");
    expect_eq(file.path(), "a"sv);

    file = LexicalFile("/a").child("b"sv);
    expect_eq(file.path(), "/a/b"sv);

    file = LexicalFile("/").child("a"sv);
    expect_eq(file.path(), "/a"sv);
}

void test_lexical_file()
{
    test_lexical_file_parent();
    test_lexical_file_child();
}

}  // namespace core
