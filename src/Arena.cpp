//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#include <core/Arena.h>

namespace core {

Arena::Arena()
    : m_blocks(16)
{
    m_blocks.add(Arena::empty_block(Size));
}

Arena::~Arena()
{
    for (const auto &block : m_blocks) free(block.memory);
}

Arena::Block Arena::empty_block(u64 size)
{
    Block block {
        malloc(size),
        0,
        size,
    };
    return block;
}

void *Arena::alloc(u64 size)
{
    if (size >= Size) {
        auto block = Arena::empty_block(size);
        block.size = size;

        m_blocks.add(block);
        return block.memory;
    }

    if (size > m_blocks.last().capacity - m_blocks.last().size)
        m_blocks.add(Arena::empty_block(Size));

    auto &current = m_blocks.last();
    auto ptr = (u8 *)current.memory + current.size;
    current.size += size;
    return ptr;
}

}  // namespace core
