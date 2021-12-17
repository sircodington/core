//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>  // std::move

#include "Types.h"

namespace core {

template<typename T>
class ListViewIterator
{
public:
    explicit ListViewIterator(T *elem)
        : m_elem(elem)
    {
    }

    bool operator==(const ListViewIterator<T> &other) const
    {
        return m_elem == other.m_elem;
    }
    bool operator!=(const ListViewIterator<T> &other) const
    {
        return not(*this == other);
    }

    const T &operator*() const { return *m_elem; }
    T &operator*() { return *m_elem; }

    ListViewIterator<T> &operator++()
    {
        ++m_elem;
        return *this;
    }
    ListViewIterator<T> operator++(int) const
    {
        ListViewIterator<T> iterator = *this;
        ++(*this);
        return iterator;
    }

private:
    T *m_elem { nullptr };
};

template<typename T>
class ListView
{
public:
    ListView(const T *, Size);
    ListView(T *, Size);
    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] bool is_empty() const { return size() == 0; }
    [[nodiscard]] bool non_empty() const { return not is_empty(); }
    [[nodiscard]] bool in_bounds(Index index) const { return index < size(); }

    bool contains(const T &value) const;

    const T &operator[](Index) const;
    T &operator[](Index);
    const T &first() const { return (*this)[0]; }
    T &first() { return (*this)[0]; }
    const T &last() const { return (*this)[size() - 1]; }
    T &last() { return (*this)[size() - 1]; }

    auto begin() const { return ListViewIterator<T>(m_data); }
    auto end() const { return ListViewIterator<T>(m_data + size()); }
    auto begin() { return ListViewIterator<T>(m_data); }
    auto end() { return ListViewIterator<T>(m_data + size()); }

    Size m_size { 0 };
    T *m_data { nullptr };
};

template<typename T>
ListView<T>::ListView(const T *data, Size size)
    : ListView(const_cast<T *>(data), size)
{
}

template<typename T>
ListView<T>::ListView(T *data, Size size)
    : m_data(data)
    , m_size(size)
{
}

template<typename T>
bool ListView<T>::contains(const T &value) const
{
    for (const auto &elem : *this)
        if (elem == value)
            return true;
    return false;
}

template<typename T>
const T &ListView<T>::operator[](Index index) const
{
    assert(in_bounds(index) and "List index out of bounds");
    return m_data[index];
}

template<typename T>
T &ListView<T>::operator[](Index index)
{
    assert(in_bounds(index) and "List index out of bounds");
    return m_data[index];
}

}  // namespace core

using core::ListView;
