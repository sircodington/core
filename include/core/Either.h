//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <cstring>
#include <utility>  // std::move

#include <core/Types.h>

namespace core {

template<typename L, typename R>
class Either
{
public:
    static Either<L, R> left(const L &l)
    {
        Either<L, R> result(Side::Left);
        new (result.m_storage) L(l);
        return result;
    }
    static Either<L, R> left(L &&l)
    {
        Either<L, R> result(Side::Left);
        new (result.m_storage) L(std::move(l));
        return result;
    }

    static Either<L, R> right(const R &r)
    {
        Either<L, R> result(Side::Right);
        new (result.m_storage) R(r);
        return result;
    }
    static Either<L, R> right(R &&r)
    {
        Either<L, R> result(Side::Right);
        new (result.m_storage) R(std::move(r));
        return result;
    }

    ~Either()
    {
        switch (m_side) {
            case Side::Left: reinterpret_cast<L &>(m_storage).~L(); break;
            case Side::Right: reinterpret_cast<R &>(m_storage).~R(); break;
            default: assert(false and "Unreachable!");
        }
    }

    // @TODO: Wrong naming convention
    [[nodiscard]] bool isLeft() const { return Side::Left == m_side; }
    [[nodiscard]] bool isRight() const { return Side::Right == m_side; }

    const L &left() const
    {
        assert(isLeft());
        return reinterpret_cast<const L &>(m_storage);
    }
    L &&release_left()
    {
        assert(isLeft());
        return std::move(reinterpret_cast<L &>(m_storage));
    }

    const R &right() const
    {
        assert(isRight());
        return reinterpret_cast<const R &>(m_storage);
    }
    R &&release_right()
    {
        assert(isRight());
        return std::move(reinterpret_cast<R &>(m_storage));
    }

private:
    enum class Side : u8
    {
        Left,
        Right,
    };
    explicit Either(Side side)
        : m_side(side)
        , m_storage()
    {
    }

    Side m_side;

    union Storage
    {
        L l;
        R r;
    };
    alignas(Storage) u8 m_storage[sizeof(Storage)];
};

}  // namespace core

using core::Either;
