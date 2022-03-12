//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>

#include <core/Types.h>

namespace core {

template<u64 Size, typename Component = u8>
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
    static constexpr ::Size ComponentBits { sizeof(Component) * 8 };

    constexpr static u64 Additional = Size % ComponentBits == 0 ? 0 : 1;
    Component m_data[Size / ComponentBits + Additional] {};
};

template<u64 Size, typename Component>
bool BitField<Size, Component>::get(Index i) const
{
    assert(in_bounds(i));
    const auto byte = m_data[i / ComponentBits];
    const auto mask = 1u << Component(i % ComponentBits);
    return (byte & mask) == mask;
}

template<u64 Size, typename Component>
bool BitField<Size, Component>::get(Index i)
{
    return const_cast<const BitField<Size, Component> &>(*this).get(i);
}

template<u64 Size, typename Component>
void BitField<Size, Component>::set(Index i, bool value)
{
    assert(in_bounds(i));
    auto &byte = m_data[i / ComponentBits];
    const auto mask = 1u << Component(i % ComponentBits);
    byte &= ~mask;
    if (value)
        byte |= mask;
}

}  // namespace core

using core::BitField;
