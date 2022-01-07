//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/StringBuilder.h>

#include <cstdio>
#include <cstring>

namespace core {

StringBuilder::~StringBuilder()
{
    switch (m_memory) {
        case Memory::Inline: break;
        case Memory::Heap: free(m_data); break;
    }
}

char *StringBuilder::writeable_location_with_space_for(u64 byte_count)
{
    if (m_size + byte_count > m_capacity) {
        auto capacity = m_capacity;
        while (capacity < m_size + byte_count) capacity *= 2;

        switch (m_memory) {
            case Memory::Inline:
                m_data = (char *)malloc(capacity);
                assert(m_data and "malloc returned nullptr");
                memcpy(m_data, m_storage, sizeof(m_storage));
                m_memory = Memory::Heap;
                break;

            case Memory::Heap:
                m_data = (char *)realloc(m_data, capacity);
                assert(m_data and "realloc returned nullptr");
                break;
        }

        m_capacity = capacity;
    }

    return &m_data[m_size];
}

void StringBuilder::append(const StringView &view)
{
    auto location = writeable_location_with_space_for(view.size());
    memcpy(location, view.data(), view.size());
    m_size += view.size();
}

template<typename T>
inline static void append_impl(StringBuilder &builder, T value, const char *fmt)
{
    char buffer[4096] { 0 };
    int count = snprintf(buffer, sizeof(buffer), fmt, value);
    assert(count >= 0 and "snprintf failed");
    const StringView view(buffer, count);
    builder.append(view);
};

void StringBuilder::append(s8 value) { append_impl(*this, value, "%" PRId8); }
void StringBuilder::append(s16 value) { append_impl(*this, value, "%" PRId16); }
void StringBuilder::append(s32 value) { append_impl(*this, value, "%" PRId32); }
void StringBuilder::append(s64 value) { append_impl(*this, value, "%" PRId64); }

void StringBuilder::append(u8 value) { append_impl(*this, value, "%" PRIu8); }
void StringBuilder::append(u16 value) { append_impl(*this, value, "%" PRIu8); }
void StringBuilder::append(u32 value) { append_impl(*this, value, "%" PRIu8); }
void StringBuilder::append(u64 value) { append_impl(*this, value, "%" PRIu8); }

#define PRIf32 "f"
#define PRIf64 "f"
void StringBuilder::append(float value)
{
    append_impl(*this, value, "%" PRIf32);
}
void StringBuilder::append(double value)
{
    append_impl(*this, value, "%" PRIf64);
}

String StringBuilder::to_string()
{
    switch (m_memory) {
        case Memory::Inline: {
            String string(StringView(m_storage, s64(m_size)));
            m_size = 0;
            return string;
        }

        case Memory::Heap: {
            String string(m_data, m_size);
            m_size = 0;
            m_data = m_storage;
            m_memory = Memory::Inline;
            return string;
        }

        default: assert(false and "Unreachable"); return {};
    }
}

StringView StringBuilder::to_view() const
{
    return StringView(m_data, s64(m_size));
}

}  // namespace core