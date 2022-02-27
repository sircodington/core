//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>  // std::move

#include <core/Types.h>

namespace core {

template<typename T>
class ListViewIterator
{
public:
    constexpr explicit ListViewIterator(T *elem)
        : m_elem(elem)
    {
    }

    constexpr bool operator==(const ListViewIterator<T> &other) const
    {
        return m_elem == other.m_elem;
    }
    constexpr bool operator!=(const ListViewIterator<T> &other) const
    {
        return not(*this == other);
    }

    constexpr const T &operator*() const { return *m_elem; }
    constexpr T &operator*() { return *m_elem; }

    constexpr ListViewIterator<T> &operator++()
    {
        ++m_elem;
        return *this;
    }
    constexpr ListViewIterator<T> operator++(int) const
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
    constexpr ListView(const T *, Size);
    constexpr ListView(T *, Size);
    [[nodiscard]] constexpr Size size() const { return m_size; }
    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }
    [[nodiscard]] constexpr bool non_empty() const { return not is_empty(); }
    [[nodiscard]] constexpr bool in_bounds(Index index) const
    {
        return index < size();
    }

    bool contains(const T &value) const;

    const T &operator[](Index) const;
    T &operator[](Index);
    const T &first() const { return (*this)[0]; }
    T &first() { return (*this)[0]; }
    const T &last() const { return (*this)[size() - 1]; }
    T &last() { return (*this)[size() - 1]; }

    constexpr auto begin() const { return ListViewIterator<T>(m_data); }
    constexpr auto end() const { return ListViewIterator<T>(m_data + size()); }
    constexpr auto begin() { return ListViewIterator<T>(m_data); }
    constexpr auto end() { return ListViewIterator<T>(m_data + size()); }

    Size m_size { 0 };
    T *m_data { nullptr };
};

template<typename T>
constexpr ListView<T>::ListView(const T *data, Size size)
    : ListView(const_cast<T *>(data), size)
{
}

template<typename T>
constexpr ListView<T>::ListView(T *data, Size size)
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
