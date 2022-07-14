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

Either<core::String, Plugin *> Plugin::load(core::String file_path)
{
    using Result = Either<core::String, Plugin *>;

    auto either_plugin = CPlugin::load(std::move(file_path));
    if (either_plugin.isLeft()) {
        auto message = std::move(either_plugin.release_left());
        return Result::left(std::move(message));
    }

    auto c_plugin = std::move(either_plugin.release_right());

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
