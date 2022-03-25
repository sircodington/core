//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/String.h>
#include <core/StringView.h>

namespace core {

class LexicalFile
{
public:
    LexicalFile();
    explicit LexicalFile(core::String file_path);
    explicit LexicalFile(core::StringView file_path);

    [[nodiscard]] bool is_absolute() const;
    [[nodiscard]] inline bool is_relative() const { return not is_absolute(); }
    [[nodiscard]] LexicalFile parent() const;
    [[nodiscard]] LexicalFile child(core::StringView file_name) const;
    [[nodiscard]] core::StringView path() const { return m_file_path.view(); }

private:
    enum class Kind
    {
        Unix,
        Windows,
    };
    Kind m_kind;
    core::String m_file_path;
};

}  // namespace core

using core::LexicalFile;