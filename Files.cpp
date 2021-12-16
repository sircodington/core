//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "Files.h"

#include <core/StringBuilder.h>

#include <cstdio>

namespace core {

String Files::read_completely(StringView file_path)
{
    StringBuilder file_path_builder;
    StringView zero_terminated_file_path;
    {
        file_path_builder.append(file_path);
        file_path_builder.append('\0');
        const auto path_view = file_path_builder.to_view();
        zero_terminated_file_path = {
            path_view.data(),
            s64(path_view.size() - 1),
        };
    }

    auto file = fopen(zero_terminated_file_path.data(), "rb");
    assert(file);
    assert(not fseek(file, 0, SEEK_END));

    const auto file_size_bytes = ftell(file);
    assert(file_size_bytes >= 0);
    assert(not fseek(file, 0, SEEK_SET));

    StringBuilder result_buffer;
    char buffer[4069] {};

    while (not feof(file)) {
        const auto count = fread(buffer, 1, sizeof(buffer), file);
        result_buffer.append(StringView { buffer, s64(count) });
    }

    assert(not fclose(file));
    return result_buffer.to_string();
}

}  // namespace core