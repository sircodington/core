//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>  // std::move

#include <core/ListView.h>
#include <core/Types.h>

namespace core {

// @TODO: Allow inline storage
// @TODO: Allocate into arena?
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

    [[nodiscard]] inline Size size() const { return m_size; }
    [[nodiscard]] inline bool is_empty() const { return size() == 0; }
    [[nodiscard]] inline bool non_empty() const { return not is_empty(); }
    [[nodiscard]] inline bool in_bounds(Index index) const
    {
        return index < size();
    }

    void ensure_capacity(Size new_capacity);

    bool contains(const T &value) const;
    void add(const T &);
    void add(T &&);

    const T &operator[](Index) const;
    T &operator[](Index);
    const T &first() const { return (*this)[0]; }
    T &first() { return (*this)[0]; }
    const T &last() const { return (*this)[size() - 1]; }
    T &last() { return (*this)[size() - 1]; }

    ListView<T> to_view() const { return ListView<T>(m_data, size()); }
    ListView<T> to_view() { return ListView<T>(m_data, size()); }
    auto begin() const { return to_view().begin(); }
    auto end() const { return to_view().end(); }
    auto begin() { return to_view().begin(); }
    auto end() { return to_view().end(); }

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
bool List<T>::contains(const T &value) const
{
    for (const auto &elem : *this)
        if (elem == value)
            return true;
    return false;
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
        // @TODO: Missing destructor calls, here
        for (Index i = 0; i < size(); ++i) (*this)[i].~T();
        free(m_data);
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
}

}  // namespace core

using core::List;
