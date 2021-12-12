//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/List.h>

namespace core {

template<typename T>
class Set
{
public:
    bool contains(const T &value) const { return m_list.contains(value); }
    u64 size() const { return m_list.size(); }
    void add(const T &);
    void add(T &&);

    bool operator==(const Set<T> &) const;
    bool operator!=(const Set<T> &other) const { return not(*this == other); }

    auto begin() const { return m_list.begin(); }
    auto end() const { return m_list.end(); }
    auto begin() { return m_list.begin(); }
    auto end() { return m_list.end(); }

private:
    List<T> m_list;
};

template<typename T>
void Set<T>::add(const T &value)
{
    if (contains(value))
        return;
    m_list.add(value);
}

template<typename T>
void Set<T>::add(T &&value)
{
    if (contains(value))
        return;
    m_list.add(std::move(value));
}

template<typename T>
bool Set<T>::operator==(const Set<T> &other) const
{
    if (size() != other.size())
        return false;

    for (const auto &elem : *this)
        if (not other.contains(elem))
            return false;
    for (const auto &elem : other)
        if (not contains(elem))
            return false;
    return true;
}

}  // namespace core

using core::Set;
