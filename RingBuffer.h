//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <utility>  // std::move

#include <core/Types.h>

namespace core {

template<typename T, Size Capacity>
class RingBuffer
{
public:
    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool full() const;
    [[nodiscard]] Size size() const;

    void write(T &&);
    const T &lookahead(Index offset);
    T consume();

private:
    inline static Index wrap(Index index) { return index % Capacity; }
    inline static void advance(Index &index) { index = wrap(index + 1); }

    T m_data[Capacity] {};
    Index m_write { 0 };
    Index m_read { 0 };
    bool m_full { false };
};

template<typename T, Size Capacity>
bool RingBuffer<T, Capacity>::empty() const
{
    return m_read == m_write and not m_full;
}

template<typename T, Size Capacity>
bool RingBuffer<T, Capacity>::full() const
{
    return m_read == m_write and m_full;
}

template<typename T, Size Capacity>
Size RingBuffer<T, Capacity>::size() const
{
    if (full())
        return Capacity;
    return wrap(m_write + Capacity - m_read);
}

template<typename T, Size Capacity>
void RingBuffer<T, Capacity>::write(T &&value)
{
    assert(not full());
    m_data[m_write] = std::move(value);
    advance(m_write);
    m_full = m_write == m_read;
}

template<typename T, Size Capacity>
const T &RingBuffer<T, Capacity>::lookahead(Index offset)
{
    assert(offset < size());
    return m_data[wrap(m_read + offset)];
}

template<typename T, Size Capacity>
T RingBuffer<T, Capacity>::consume()
{
    assert(not empty());
    auto element = std::move(m_data[m_read]);
    advance(m_read);
    m_full = false;
    return element;
}

}  // namespace core

using core::RingBuffer;
