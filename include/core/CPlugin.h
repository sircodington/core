//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/Either.h>
#include <core/List.h>
#include <core/String.h>
#include <core/StringView.h>

namespace core {

class CPlugin
{
public:
    CPlugin() = default;

    CPlugin(const CPlugin &) = delete;
    CPlugin(CPlugin &&other) noexcept { consume(std::move(other)); }

    ~CPlugin() { clear(); }

    CPlugin &operator=(const CPlugin &) = delete;
    CPlugin &operator=(CPlugin &&) noexcept;

    core::String add_dll_directory(core::StringView file_path);

    ///! Return an error message, or an empty string on success
    core::String load(core::String file_path);

    template<typename Function>
    Function *proc_address(core::String proc_name) const
    {
        auto address = proc_address_impl(std::move(proc_name));
        return reinterpret_cast<Function *>(address);
    }

private:
    [[nodiscard]] void *proc_address_impl(core::String proc_name) const;

    void clear();
    void consume(CPlugin &&) noexcept;

    core::List<void *> m_directory_handles;
    core::String m_file_path;
    void *m_plugin_handle { nullptr };
};

}  // namespace core