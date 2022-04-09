//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/StringView.h>

#include <algorithm>
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

StringView StringView::drop_left(Size n) const
{
    auto k = std::min(s64(size()), s64(n));
    return { data() + k, s64(size()) - s64(k) };
}

StringView StringView::take_left(Size n) const
{
    auto k = std::min(s64(size()), s64(n));
    return { data(), k };
}

s64 StringView::last_index_of(char c) const
{
    for (auto i = s64(size() - 1); i >= 0; --i) {
        if (at(i) == c)
            return i;
    }
    return -1;
}

void StringView::replace(char c, char replacement)
{
    for (auto i = 0; i < size(); ++i) {
        if (at(i) == c)
            at(i) = replacement;
    }
}

void Formatter<StringView>::format(StringBuilder &b, const StringView &value)
{
    b.append(value);
}

}  // namespace core