//
// Copyright (c) 2021, Jan Sladek <keddelzz@web.de>
//
// SPDX-License-Identifier: BSD-2-Clause
//

#pragma once

#include <core/List.h>
#include <core/Set.h>

namespace core {

template<typename K, typename V>
class Map
{
public:
    bool contains(const K &value) const { return m_keys.contains(value); }
    void add(const K &, const V &);
    V &get(const K &);
    void clear();

    auto begin() const { return m_keys.begin(); }
    auto end() const { return m_keys.end(); }
    auto begin() { return m_keys.begin(); }
    auto end() { return m_keys.end(); }

private:
    struct Pair
    {
        K key;
        V value;
    };

    Set<K> m_keys;
    List<Pair> m_list;
};

template<typename K, typename V>
void Map<K, V>::add(const K &key, const V &value)
{
    if (contains(key))
        return;
    m_keys.add(key);
    m_list.add({ key, value });
}

template<typename K, typename V>
V &Map<K, V>::get(const K &key)
{
    assert(contains(key));

    Pair *result_pair = nullptr;
    for (auto &pair : m_list) {
        if (pair.key == key) {
            result_pair = &pair;
            break;
        }
    }

    return result_pair->value;
}

template<typename K, typename V>
void Map<K, V>::clear()
{
    m_keys.clear();
    m_list.clear();
}

}  // namespace core

using core::Map;
