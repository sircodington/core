//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/StringView.h>

#include <cstring>

#include <core/StringBuilder.h>

namespace core {

StringView::StringView()
    : StringView(nullptr)
{
}

StringView::StringView(const char *data, s64 size)
    : m_data(const_cast<char *>(data))
    , m_size(data ? (size > -1 ? size : strlen(data)) : 0)
{
}

bool StringView::operator==(const StringView &other) const
{
    if (size() != other.size())
        return false;
    if (data() == other.data())
        return true;

    return not memcmp(data(), other.data(), size());
}

void Formatter<StringView>::format(StringBuilder &b, const StringView &value)
{
    b.append(value);
}

}  // namespace core