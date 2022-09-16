//
// Copyright (c) 2021-2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/String.h>

#include <cstdlib>
#include <cstring>

#include <core/Formatting.h>

namespace core {

String::String()
    : String(nullptr, 0)
{
}

String::String(const char *data, Size size)
    : m_data(const_cast<char *>(data))
    , m_size(data ? size : 0)
{
}

void String::clear()
{
    if (m_data != nullptr)
        free(const_cast<char *>(m_data));
}

String &String::operator=(const String &other)
{
    if (this != &other) {
        clear();
        copy_here(other);
    }
    return *this;
}

String &String::operator=(String &&other) noexcept
{
    if (this != &other) {
        clear();
        consume(std::move(other));
    }
    return *this;
}

void String::copy_here(const StringView &other)
{
    const auto byte_count = sizeof(other[0]) * other.size();
    const auto allocated_byte_count = byte_count + sizeof(other[0]);
    m_data = (char *)malloc(allocated_byte_count);
    assert(m_data and "malloc returned nullptr");
    m_size = other.size();
    memcpy(m_data, other.data(), byte_count);
    m_data[other.size()] = '\0';
}

void String::consume(String &&other)
{
    m_data = std::exchange(other.m_data, nullptr);
    m_size = std::exchange(other.m_size, 0);
}

void Formatter<String>::format(StringBuilder &builder, const String &value)
{
    Formatting::format_into(builder, value.view());
}

}  // namespace core