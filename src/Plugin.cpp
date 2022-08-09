//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/Plugin.h>

#include <core/CPlugin.h>

namespace core {

Plugin::Plugin()
{
    static_assert(sizeof(CPlugin) <= sizeof(Plugin::m_internal));
    memset(m_internal, 0, sizeof(m_internal));
}

Either<core::String, Plugin *> Plugin::load(
    core::List<core::StringView> dll_dirs, core::String file_path)
{
    using Result = Either<core::String, Plugin *>;

    CPlugin c_plugin;
    for (auto dll_dir : dll_dirs) {
        auto error_message = c_plugin.add_dll_directory(dll_dir);
        if (error_message.non_empty())
            return Result::left(std::move(error_message));
    }
    auto error_message = c_plugin.load(std::move(file_path));
    if (error_message.non_empty())
        return Result::left(std::move(error_message));

    Plugin *(*make_plugin)() = nullptr;
    make_plugin = reinterpret_cast<Plugin *(*)()>(
        c_plugin.proc_address<decltype(make_plugin)>(
            "core_make_plugin_instance"sv));
    if (not make_plugin)
        return Result::left("Failed to instantiate plugin"sv);

    auto plugin = (*make_plugin)();
    if (not plugin)
        return Result::left("Failed to instantiate plugin"sv);

    new (plugin->m_internal) CPlugin(std::move(c_plugin));
    return Result::right(plugin);
}

Plugin::~Plugin()
{
    static_assert(sizeof(CPlugin) <= sizeof(Plugin::m_internal));
    auto c_plugin = reinterpret_cast<CPlugin *>(m_internal);
    c_plugin->~CPlugin();
    memset(m_internal, 0, sizeof(m_internal));
}

}  // namespace core
