//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <utility>  // std::forward

#include <core/List.h>
#include <core/Types.h>

namespace core {

class Arena
{
public:
    Arena();
    Arena(const Arena &) = delete;
    Arena(Arena &&) = default;
    ~Arena();

    Arena &operator=(const Arena &) = delete;
    Arena &operator=(Arena &&) = default;

    void *alloc(u64);

    template<typename T, typename... Args>
    T *construct(Args &&...args)
    {
        auto ptr = alloc(sizeof(T));
        assert(ptr and "core::Arena::alloc returned nullptr");
        return new (ptr) T(std::forward<Args>(args)...);
    }

private:
    constexpr static auto Size = 16384;

    struct Block
    {
        void *memory { nullptr };
        u64 size { 0 };
        u64 capacity { 0 };
    };
    static Block empty_block(u64 size);

    List<Block> m_blocks;
};

}  // namespace core

using core::Arena;
