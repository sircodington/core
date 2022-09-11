//
// Copyright (c) 2022, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <algorithm>
#include <string>

#define COMPILETIME_UNSUPPORTED_OS(func) \
  static_assert(false, (func " is not implemented for this OS, yet!"))
#define RUNTIME_UNSUPPORTED_OS(func) \
  assert(false and (func " is not implemented for this OS, yet!"))

#if defined(_WIN32) || defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__unix__)
#include <dlfcn.h>
#else
COMPILETIME_UNSUPPORTED_OS("core::CPlugin");
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
#elif defined(__unix__)
    // :LinuxDllDirectory:
    RUNTIME_UNSUPPORTED_OS("CPlugin::add_dll_directory");
    return {};
#else
    COMPILETIME_UNSUPPORTED_OS("CPlugin::add_dll_directory");
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
#elif defined(__unix__)
    // @FIXME: Find out the correct set of flags
    constexpr static int Flags { RTLD_LAZY | RTLD_GLOBAL };
    auto plugin_handle = dlopen(file_path.data(), Flags);

    const core::StringView error_string(dlerror());
    if (not plugin_handle)
        return error_string;

    m_file_path = std::move(file_path);
    m_plugin_handle = plugin_handle;
    return {};
#else
    COMPILETIME_UNSUPPORTED_OS("CPlugin::load");
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
#elif defined(__unix__)
    auto address = dlsym(m_plugin_handle, proc_name.data());
    return address;
#else
    COMPILETIME_UNSUPPORTED_OS("CPlugin::proc_address_impl");
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
#elif defined(__unix__)
        // :LinuxDllDirectory:
#else
        COMPILETIME_UNSUPPORTED_OS("CPlugin::clear");
#endif
    }
    m_directory_handles.clear();

    if (m_plugin_handle) {
#if defined(_WIN32) || defined(WIN32)
        auto result = FreeLibrary(reinterpret_cast<HMODULE>(m_plugin_handle));
        assert(result);
#elif defined(__unix__)
        auto result = dlclose(m_plugin_handle);
        assert(not result);
#else
        COMPILETIME_UNSUPPORTED_OS("CPlugin::clear");
#endif
        m_plugin_handle = nullptr;
    }
}

void CPlugin::consume(CPlugin &&other) noexcept
{
    m_file_path = std::exchange(other.m_file_path, {});
    m_plugin_handle = std::exchange(other.m_plugin_handle, nullptr);
}

}  // namespace core
