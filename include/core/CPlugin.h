//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/Either.h>
#include <core/String.h>

namespace core {

class CPlugin
{
public:
    static Either<core::String, CPlugin> load(core::String file_path);

    CPlugin(const CPlugin &) = delete;
    CPlugin(CPlugin &&other) noexcept { consume(std::move(other)); }

    ~CPlugin() { clear(); }

    CPlugin &operator=(const CPlugin &) = delete;
    CPlugin &operator=(CPlugin &&) noexcept;

    template<typename Function>
    Function *proc_address(core::String proc_name) const
    {
        auto address = proc_address_impl(std::move(proc_name));
        return reinterpret_cast<Function *>(address);
    }

private:
    CPlugin(core::String file_path, void *plugin_handle);

    void *proc_address_impl(core::String proc_name) const;

    void clear();
    void consume(CPlugin &&) noexcept;

    core::String m_file_path;
    void *m_plugin_handle { nullptr };
};

}  // namespace core