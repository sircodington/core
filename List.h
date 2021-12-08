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

// @TODO: Allow inline storage

template<typename T>
class ListIterator
{
public:
    explicit ListIterator(T *elem)
        : m_elem(elem)
    {
    }

    bool operator==(const ListIterator<T> &other) const
    {
        return m_elem == other.m_elem;
    }
    bool operator!=(const ListIterator<T> &other) const
    {
        return not(*this == other);
    }

    const T &operator*() const { return *m_elem; }
    T &operator*() { return *m_elem; }

    ListIterator<T> &operator++()
    {
        ++m_elem;
        return *this;
    }
    ListIterator<T> operator++(int) const
    {
        ListIterator<T> iterator = *this;
        ++(*this);
        return iterator;
    }

private:
    T *m_elem { nullptr };
};

template<typename T>
class List
{
public:
    explicit List(Size initial_capacity = 4)
    {
        ensure_capacity(initial_capacity);
    }
    List(const List<T> &other) { copy_here(other); }
    List(List<T> &&other) noexcept { consume(std::move(other)); }
    ~List() { clear(); }

    List<T> &operator=(const List<T> &);
    List<T> &operator=(List<T> &&) noexcept;

    [[nodiscard]] Size size() const { return m_size; }
    [[nodiscard]] bool is_empty() const { return size() == 0; }
    [[nodiscard]] bool non_empty() const { return not is_empty(); }
    [[nodiscard]] bool in_bounds(Index index) const { return index < size(); }

    void ensure_capacity(Size new_capacity);

    void add(const T &);
    void add(T &&);

    const T &operator[](Index) const;
    T &operator[](Index);
    const T &first() const { return (*this)[0]; }
    T &first() { return (*this)[0]; }
    const T &last() const { return (*this)[size() - 1]; }
    T &last() { return (*this)[size() - 1]; }

    auto begin() const { return ListIterator<T>(m_data); }
    auto end() const { return ListIterator<T>(m_data + size()); }
    auto begin() { return ListIterator<T>(m_data); }
    auto end() { return ListIterator<T>(m_data + size()); }

    void clear();

private:
    void copy_here(const List<T> &);
    void consume(List<T> &&);

    Size m_size { 0 };
    Size m_capacity { 0 };
    T *m_data { nullptr };
};

template<typename T>
List<T> &List<T>::operator=(const List<T> &other)
{
    clear();
    if (this != &other)
        copy_here(other);
    return *this;
}

template<typename T>
List<T> &List<T>::operator=(List<T> &&other) noexcept
{
    clear();
    if (this != &other)
        consume(std::move(other));
    return *this;
}

template<typename T>
void List<T>::ensure_capacity(Size new_capacity)
{
    if (m_capacity >= new_capacity)
        return;

    auto capacity = m_capacity;
    if (capacity == 0)
        capacity = 1;
    while (capacity < new_capacity) capacity *= 2;

    auto data = realloc(m_data, sizeof(T) * capacity);
    assert(data and "realloc returned nullptr!");
    m_data = reinterpret_cast<T *>(data);
    m_capacity = capacity;
}

template<typename T>
void List<T>::add(const T &value)
{
    ensure_capacity(size() + 1);
    new (&m_data[size()]) T(value);
    ++m_size;
}

template<typename T>
void List<T>::add(T &&value)
{
    ensure_capacity(size() + 1);
    new (&m_data[size()]) T(std::move(value));
    ++m_size;
}

template<typename T>
const T &List<T>::operator[](Index index) const
{
    assert(in_bounds(index) and "List index out of bounds");
    return m_data[index];
}

template<typename T>
T &List<T>::operator[](Index index)
{
    assert(in_bounds(index) and "List index out of bounds");
    return m_data[index];
}

template<typename T>
void List<T>::copy_here(const List<T> &other)
{
    for (const auto &elem : other) { add(elem); }
}

template<typename T>
void List<T>::consume(List<T> &&other)
{
    m_size = std::exchange(other.m_size, 0);
    m_capacity = std::exchange(other.m_capacity, 0);
    m_data = std::exchange(other.m_data, nullptr);
}

template<typename T>
void List<T>::clear()
{
    if (m_data) {
        free(m_data);
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}

}  // namespace core

using core::List;
