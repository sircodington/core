//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>

#include <core/Formatter.h>
#include <core/Types.h>

namespace core {

class StringView
{
public:
    StringView();
    StringView(const char *data, s64 size = -1);

    [[nodiscard]] const char *data() const { return m_data; }
    [[nodiscard]] char *data() { return const_cast<char *>(m_data); }
    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] char operator[](Index index) const
    {
        assert(index < size());
        return *(data() + index);
    }

private:
    char *m_data { nullptr };
    Size m_size { 0 };
};

template<>
class Formatter<StringView>
{
public:
    static void format(StringBuilder &, const StringView &);
};

}  // namespace core

using core::StringView;
