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
    [[nodiscard]] bool is_empty() const { return size() == 0; }
    [[nodiscard]] bool non_empty() const { return not is_empty(); }
    [[nodiscard]] const char &at(Index index) const
    {
        assert(index < size());
        return *(data() + index);
    }
    [[nodiscard]] char &at(Index index)
    {
        assert(index < size());
        return *(data() + index);
    }
    inline const char &operator[](Index index) const { return at(index); }
    inline char &operator[](Index index) { return at(index); }

    bool operator==(const StringView &other) const;
    bool operator!=(const StringView &other) const
    {
        return not(*this == other);
    }

    [[nodiscard]] s64 last_index_of(char) const;

    void replace(char c, char replacement);

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"
[[nodiscard]] inline StringView operator"" sv(const char *p, size_t length)
{
    return { p, s64(length) };
}
#pragma clang diagnostic pop