//
// Copyright (c) 2021-2023, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <cstring>

#include <core/Formatter.h>
#include <core/Types.h>

namespace core {

class StringView
{
public:
    constexpr StringView()
        : StringView({}, nullptr, 0)
    {
    }

    template<size_t N>
    constexpr StringView(const char (&data)[N])
        : StringView({}, &data[0], N - 1)
    {
    }

    constexpr StringView(const char *data, s64 size = -1)
        : m_data(const_cast<char *>(data))
        , m_size(data ? (size > -1 ? size : strlen(data)) : 0)
    {
    }

    [[nodiscard]] constexpr const char *data() const { return m_data; }
    [[nodiscard]] constexpr char *data() { return const_cast<char *>(m_data); }
    [[nodiscard]] constexpr Size size() const { return m_size; }
    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }
    [[nodiscard]] constexpr bool non_empty() const { return not is_empty(); }
    [[nodiscard]] constexpr const char &at(Index index) const
    {
        assert(index < size());
        return *(data() + index);
    }
    [[nodiscard]] constexpr char &at(Index index)
    {
        assert(index < size());
        return *(data() + index);
    }
    constexpr const char &operator[](Index index) const { return at(index); }
    constexpr char &operator[](Index index) { return at(index); }

    bool operator==(const StringView &other) const;
    bool operator!=(const StringView &other) const
    {
        return not(*this == other);
    }

    [[nodiscard]] StringView drop_left(Size) const;
    [[nodiscard]] StringView take_left(Size) const;
    template<typename Predicate>
    Size prefix_length(Predicate &&) const;
    template<typename Predicate>
    [[nodiscard]] StringView drop_while(Predicate &&p) const
    {
        return drop_left(prefix_length(p));
    }
    template<typename Predicate>
    [[nodiscard]] StringView take_while(Predicate &&p) const
    {
        return take_left(prefix_length(p));
    }

    [[nodiscard]] s64 last_index_of(char) const;

    void replace(char c, char replacement);

private:
    constexpr StringView(bool, const char *data, u64 size)
        : m_data(const_cast<char *>(data))
        , m_size(size)
    {
    }

    char *m_data { nullptr };
    Size m_size { 0 };
};

template<>
class Formatter<StringView>
{
public:
    static void format(StringBuilder &, const StringView &);
};

template<typename Predicate>
Size StringView::prefix_length(Predicate &&predicate) const
{
    Size length = 0;
    for (auto i = 0; i < size(); ++i) {
        if (not predicate(at(i)))
            break;
        ++length;
    }
    return length;
}

}  // namespace core

using core::StringView;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"
[[nodiscard]] inline StringView operator"" sv(const char *p, size_t length)
{
    return { p, s64(length) };
}
#pragma clang diagnostic pop