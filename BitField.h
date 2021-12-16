//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>

#include <core/Types.h>

namespace core {

template<u64 Size>
class BitField
{
public:
    BitField() = default;

    constexpr auto size() const { return Size; }
    [[nodiscard]] constexpr bool in_bounds(Index i) const { return i < size(); }
    [[nodiscard]] bool get(Index) const;
    [[nodiscard]] bool get(Index);
    void set(Index, bool);

private:
    constexpr static u64 Additional = Size % 8 == 0 ? 0 : 1;
    u8 m_data[Size / 8 + Additional] {};
};

template<u64 Size>
bool BitField<Size>::get(Index i) const
{
    assert(in_bounds(i));
    const auto byte = m_data[i / 8];
    const auto mask = 1u << u8(i % 8);
    return (byte & mask) == mask;
}

template<u64 Size>
bool BitField<Size>::get(Index i)
{
    return const_cast<const BitField<Size> &>(*this).get(i);
}

template<u64 Size>
void BitField<Size>::set(Index i, bool value)
{
    assert(in_bounds(i));
    auto &byte = m_data[i / 8];
    const auto mask = 1u << u8(i % 8);
    byte &= ~mask;
    if (value)
        byte |= mask;
}

}  // namespace core

using core::BitField;
