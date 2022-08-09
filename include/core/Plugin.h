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
#include <core/Types.h>

namespace core {

class Plugin
{
public:
    static Either<core::String, Plugin *> load(
        core::List<core::StringView> dll_dirs, core::String file_path);

    Plugin();

    Plugin(const Plugin &) = delete;
    Plugin(Plugin &&) = delete;

    virtual ~Plugin();

    Plugin &operator=(const Plugin &) = delete;
    Plugin &operator=(Plugin &&) = delete;

private:
    u8 m_internal[48];
};

#define CORE_PLUGIN_HEADER(Type)                   \
    extern "C"                                     \
    {                                              \
        core::Plugin *core_make_plugin_instance(); \
    }

#define CORE_PLUGIN_SOURCE(Type)                                         \
    extern "C"                                                           \
    {                                                                    \
        core::Plugin *core_make_plugin_instance() { return new Type(); } \
    }

}  // namespace core
