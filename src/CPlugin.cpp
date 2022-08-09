//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <algorithm>
#include <string>

#if defined(_WIN32) || defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
static_assert(false, "Only implemened for windows");
#endif

#include <core/CPlugin.h>

namespace core {

#if defined(_WIN32) || defined(WIN32)
inline static core::String windows_get_last_error()
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

core::String CPlugin::add_dll_directory(core::StringView file_path)
{
#if defined(_WIN32) || defined(WIN32)
    std::wstring file_path_std(
        file_path.data(), file_path.data() + file_path.size());
    std::replace(
        std::begin(file_path_std), std::end(file_path_std), L'/', L'\\');

    if (auto handle = AddDllDirectory(file_path_std.data())) {
        m_directory_handles.add(handle);
        return {};
    }

    return windows_get_last_error();
#else
    static_assert(false, "Only implemened for windows");
#endif
}

core::String CPlugin::load(core::String file_path)
{
#if defined(_WIN32) || defined(WIN32)
    file_path.replace('/', '\\');

    auto plugin_handle = LoadLibraryExA(
        file_path.data(), nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

    if (not plugin_handle)
        return windows_get_last_error();

    m_file_path = std::move(file_path);
    m_plugin_handle = plugin_handle;
    return {};
#else
    static_assert(false, "Only implemened for windows");
    return {};
#endif
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
    for (auto directory_handle : m_directory_handles) {
#if defined(_WIN32) || defined(WIN32)
        RemoveDllDirectory(directory_handle);
#else
        static_assert(false, "Only implemened for windows");
#endif
    }
    m_directory_handles.clear();

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
