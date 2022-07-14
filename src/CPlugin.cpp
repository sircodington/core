//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#if defined(_WIN32) || defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
static_assert(false, "Only implemened for windows");
#endif

#include <core/CPlugin.h>

namespace core {

CPlugin::CPlugin(core::String file_path, void *plugin_handle)
    : m_file_path(std::move(file_path))
    , m_plugin_handle(plugin_handle)
{
}

#if defined(_WIN32) || defined(WIN32)
inline static core::String window_get_last_error()
{
    const auto error_id = ::GetLastError();
    if (error_id == 0)
        return {};

    char *message_buffer = nullptr;
    const auto size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_id,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message_buffer,
        0,
        nullptr);

    const core::StringView message_view(message_buffer, size);
    core::String message(message_view);
    LocalFree(message_buffer);
    return message;
}
#endif

Either<core::String, CPlugin> CPlugin::load(core::String file_path)
{
    using Result = Either<core::String, CPlugin>;

#if defined(_WIN32) || defined(WIN32)
    auto plugin_handle = LoadLibraryExA(
        file_path.data(), nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

    if (not plugin_handle)
        return Result::left(window_get_last_error());

    return Result::right(CPlugin(std::move(file_path), plugin_handle));
#else
    static_assert(false, "Only implemened for windows");
    return Result::left("Only implemened for windows"sv);
#endif

    assert(false);
    return Result::left("Unreachable"sv);
}

void *CPlugin::proc_address_impl(core::String proc_name) const
{
#if defined(_WIN32) || defined(WIN32)
    auto address = GetProcAddress(
        reinterpret_cast<HMODULE>(m_plugin_handle), proc_name.data());

    if (not address)
        return nullptr;

    return reinterpret_cast<void *>(address);
#else
    static_assert(false, "Only implemened for windows");
    return nullptr;
#endif
}

CPlugin &CPlugin::operator=(CPlugin &&other) noexcept
{
    if (this != &other) {
        clear();
        consume(std::move(other));
    }
    return *this;
}

void CPlugin::clear()
{
    if (m_plugin_handle) {
#if defined(_WIN32) || defined(WIN32)
        auto result = FreeLibrary(reinterpret_cast<HMODULE>(m_plugin_handle));
        assert(result);
        m_plugin_handle = nullptr;
#else
        static_assert(false, "Only implemened for windows");
#endif
    }
}

void CPlugin::consume(CPlugin &&other) noexcept
{
    m_file_path = std::exchange(other.m_file_path, {});
    m_plugin_handle = std::exchange(other.m_plugin_handle, nullptr);
}

}  // namespace core
