//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <utility>  // std::move

#include <core/Formatter.h>
#include <core/StringView.h>
#include <core/Types.h>

namespace core {

class String
{
    friend class StringBuilder;

public:
    String();
    String(const StringView &view) { copy_here(view); }
    String(const String &other) { copy_here(other); }
    String(String &&other) { consume(std::move(other)); }
    ~String() { clear(); }

    String &operator=(const String &);
    String &operator=(String &&) noexcept;

    bool operator==(const String &other) const { return *this == other.view(); }
    bool operator!=(const String &other) const { return not(*this == other); }
    bool operator==(const StringView &other) const { return view() == other; }
    bool operator!=(const StringView &other) const
    {
        return not(*this == other);
    }

    [[nodiscard]] const char *data() const { return m_data; }
    [[nodiscard]] char *data() { return m_data; }
    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] bool is_empty() const { return size() == 0; }
    [[nodiscard]] bool non_empty() const { return not is_empty(); }
    [[nodiscard]] char operator[](Index index) const
    {
        assert(index < size());
        return *(data() + index);
    }

    [[nodiscard]] StringView view() const { return { m_data, s64(m_size) }; }

private:
    String(const char *data, Size size);  //! Take ownership

    void clear();
    void copy_here(const StringView &);
    void copy_here(const String &other) { copy_here(other.view()); }
    void consume(String &&);

    char *m_data { nullptr };
    Size m_size { 0 };
};

template<>
class Formatter<String>
{
public:
    static void format(StringBuilder &, const String &);
};

}  // namespace core

using core::String;
