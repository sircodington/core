//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include "StringView.h"

#include <cstdio>
#include <cstring>

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

void Logger<StringView>::log(const StringView &value)
{
    for (Index i = 0; i < value.size(); ++i) { printf("%c", value[i]); }
}

}  // namespace core