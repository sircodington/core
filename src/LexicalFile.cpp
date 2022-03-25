//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/LexicalFile.h>

#include <core/StringBuilder.h>

namespace core {

LexicalFile::LexicalFile()
    : LexicalFile(String())
{
}

inline static bool is_letter(char c)
{
    return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z');
}

inline static bool starts_with_drive_letter(const core::StringView &path)
{
    return path.size() >= 2 and is_letter(path.at(0)) and path.at(1) == ':';
}

LexicalFile::LexicalFile(core::String file_path)
    : m_kind(Kind::Unix)
    , m_file_path(std::move(file_path))
{
    m_file_path.replace('\\', '/');
    auto size = s64(m_file_path.size());
    while (size > 1 and m_file_path.view().at(size - 1) == '/') --size;
    m_file_path = StringView(m_file_path.data(), size);
    if (starts_with_drive_letter(m_file_path.view()))
        m_kind = Kind::Windows;
}

LexicalFile::LexicalFile(core::StringView file_path)
    : LexicalFile(String(file_path))
{
}

[[nodiscard]] bool LexicalFile::is_absolute() const
{
    switch (m_kind) {
        case Kind::Unix: return path().non_empty() and path().at(0) == '/';
        case Kind::Windows: return starts_with_drive_letter(path());
    }
    assert(false and "Unreachable");
    return false;
}

LexicalFile LexicalFile::parent() const
{
    if (auto index = path().last_index_of('/'); index >= 0) {
        if (index == 0)
            return LexicalFile("/"sv);
        const StringView view(path().data(), index);
        return LexicalFile(view);
    }
    return {};
}

LexicalFile LexicalFile::child(core::StringView file_name) const
{
    if (m_file_path.is_empty())
        return LexicalFile(file_name);
    StringBuilder builder;
    builder.append(path());
    if (not(m_file_path.size() == 1 and path().at(0) == '/'))
        builder.append('/');
    builder.append(file_name);
    auto child_path = builder.to_string();
    return LexicalFile(std::move(child_path));
}

}  // namespace core