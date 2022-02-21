//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/String.h>
#include <core/StringView.h>

namespace core {

class Files
{
public:
    Files() = delete;

    static String read_completely(StringView file_path);
    static void write_completely(StringView file_path, StringView contents);
};

}  // namespace core

using core::Files;
