//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/String.h>
#include <core/StringView.h>
#include <core/Types.h>

namespace core {

class StringBuilder
{
public:
    StringBuilder() = default;
    ~StringBuilder();

    StringBuilder(const StringBuilder &) = delete;
    StringBuilder(StringBuilder &&) = delete;
    StringBuilder &operator=(const StringBuilder &) = delete;
    StringBuilder &operator=(StringBuilder &&) = delete;

    void append(char value) { append(StringView(&value, 1)); }
    void append(const char *value) { append(StringView(value)); }
    void append(const StringView &);
    void append(const String &other) { append(other.view()); }

    void append(s8);
    void append(s16);
    void append(s32);
    void append(s64);

    void append(u8);
    void append(u16);
    void append(u32);
    void append(u64);

    void append(float);
    void append(double);

    String to_string();
    [[nodiscard]] StringView to_view() const;

private:
    char *writeable_location_with_space_for(u64 byte_count);

    enum class Memory : u8
    {
        Inline,
        Heap,
    };
    constexpr static auto InlineCapacity { 4096 };
    Memory m_memory { Memory::Inline };
    char m_storage[InlineCapacity] { 0 };

    char *m_data { m_storage };
    u64 m_capacity { InlineCapacity };
    u64 m_size { 0 };
};

}  // namespace core

using core::StringBuilder;
