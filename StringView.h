//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>

#include "Logger.h"
#include "Types.h"

namespace core {

class StringView
{
public:
    StringView(const char *data, s64 size = -1);

    [[nodiscard]] const char *data() const { return m_data; }
    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] char operator[](Index index) const
    {
        assert(index < size());
        return *(data() + index);
    }

private:
    const char *m_data { nullptr };
    Size m_size { 0 };
};

template<>
class Logger<StringView>
{
public:
    static void log(const StringView &);
};

}  // namespace core

using core::StringView;
