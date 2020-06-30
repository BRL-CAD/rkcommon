// Copyright 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

namespace rkcommon {
  namespace containers {

    // A small map data structure with a similar interface to std::map<>, but
    // uses an underlying std::vector<> to store the key/value pairs instead of
    // a tree. This makes lookups O(n), but inserts are O(1) and it is sortable
    // like an array to enable things like std::binary_search() on either the
    // keys or values.
    template <typename KEY, typename VALUE>
    struct FlatMap
    {
      using item_t       = std::pair<KEY, VALUE>;
      using storage_t    = std::vector<item_t>;
      using iterator_t   = decltype(std::declval<storage_t>().begin());
      using citerator_t  = decltype(std::declval<storage_t>().cbegin());
      using riterator_t  = decltype(std::declval<storage_t>().rbegin());
      using criterator_t = decltype(std::declval<storage_t>().crbegin());

      FlatMap()  = default;
      ~FlatMap() = default;

      // Key-based lookups //

      VALUE &at(const KEY &key);
      const VALUE &at(const KEY &key) const;

      VALUE &operator[](const KEY &key);
      const VALUE &operator[](const KEY &key) const;

      // Index-based lookups //

      item_t &at_index(size_t index);
      const item_t &at_index(size_t index) const;

      // Property queries //

      size_t size() const;
      size_t empty() const;

      bool contains(const KEY &key) const;

      // Storage mutation //

      void erase(const KEY &key);

      void clear();
      void reserve(size_t size);

      // Iterators //

      iterator_t begin();
      citerator_t begin() const;
      citerator_t cbegin() const;

      iterator_t end();
      citerator_t end() const;
      citerator_t cend() const;

      riterator_t rbegin();
      criterator_t rbegin() const;
      criterator_t crbegin() const;

      riterator_t rend();
      criterator_t rend() const;
      criterator_t crend() const;

     private:
      // Helpers //

      iterator_t lookup(const KEY &key);
      citerator_t lookup(const KEY &key) const;

      // Data //

      storage_t values;
    };

    // Inlined definitions ////////////////////////////////////////////////////

    template <typename KEY, typename VALUE>
    inline VALUE &FlatMap<KEY, VALUE>::at(const KEY &key)
    {
      auto itr = lookup(key);
      if (itr == values.end())
        throw std::out_of_range("key wasn't found in FlatMap<>");
      return itr->second;
    }

    template <typename KEY, typename VALUE>
    inline const VALUE &FlatMap<KEY, VALUE>::at(const KEY &key) const
    {
      auto itr = lookup(key);
      if (itr == values.end())
        throw std::out_of_range("key wasn't found in FlatMap<>");
      return itr->second;
    }

    template <typename KEY, typename VALUE>
    inline VALUE &FlatMap<KEY, VALUE>::operator[](const KEY &key)
    {
      auto itr = lookup(key);
      if (itr == values.end()) {
        values.push_back(std::make_pair(key, VALUE()));
        return values.back().second;
      } else {
        return itr->second;
      }
    }

    template <typename KEY, typename VALUE>
    inline const VALUE &FlatMap<KEY, VALUE>::operator[](const KEY &key) const
    {
      auto itr = lookup(key);
      if (itr == values.end()) {
        values.push_back(std::make_pair(key, VALUE()));
        return values.back().second;
      } else {
        return itr->second;
      }
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::item_t &FlatMap<KEY, VALUE>::at_index(
        size_t index)
    {
      return values.at(index);
    }

    template <typename KEY, typename VALUE>
    inline const typename FlatMap<KEY, VALUE>::item_t &
    FlatMap<KEY, VALUE>::at_index(size_t index) const
    {
      return values.at(index);
    }

    template <typename KEY, typename VALUE>
    inline size_t FlatMap<KEY, VALUE>::size() const
    {
      return values.size();
    }

    template <typename KEY, typename VALUE>
    inline size_t FlatMap<KEY, VALUE>::empty() const
    {
      return values.empty();
    }

    template <typename KEY, typename VALUE>
    inline bool FlatMap<KEY, VALUE>::contains(const KEY &key) const
    {
      return lookup(key) != values.cend();
    }

    template <typename KEY, typename VALUE>
    inline void FlatMap<KEY, VALUE>::erase(const KEY &key)
    {
      auto itr = std::stable_partition(
          values.begin(), values.end(), [&](const item_t &i) {
            return i.first != key;
          });

      values.resize(std::distance(values.begin(), itr));
    }

    template <typename KEY, typename VALUE>
    inline void FlatMap<KEY, VALUE>::clear()
    {
      values.clear();
    }

    template <typename KEY, typename VALUE>
    inline void FlatMap<KEY, VALUE>::reserve(size_t size)
    {
      return values.reserve(size);
    }

    // Iterators //

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::iterator_t FlatMap<KEY, VALUE>::begin()
    {
      return values.begin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::citerator_t
    FlatMap<KEY, VALUE>::begin() const
    {
      return cbegin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::citerator_t
    FlatMap<KEY, VALUE>::cbegin() const
    {
      return values.cbegin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::iterator_t FlatMap<KEY, VALUE>::end()
    {
      return values.end();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::citerator_t FlatMap<KEY, VALUE>::end()
        const
    {
      return cend();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::citerator_t FlatMap<KEY, VALUE>::cend()
        const
    {
      return values.cend();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::riterator_t
    FlatMap<KEY, VALUE>::rbegin()
    {
      return values.rbegin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::criterator_t
    FlatMap<KEY, VALUE>::rbegin() const
    {
      return crbegin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::criterator_t
    FlatMap<KEY, VALUE>::crbegin() const
    {
      return values.crbegin();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::riterator_t FlatMap<KEY, VALUE>::rend()
    {
      return values.rend();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::criterator_t
    FlatMap<KEY, VALUE>::rend() const
    {
      return crend();
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::criterator_t
    FlatMap<KEY, VALUE>::crend() const
    {
      return values.crend();
    }

    // Helper functions //

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::iterator_t FlatMap<KEY, VALUE>::lookup(
        const KEY &key)
    {
      return std::find_if(values.begin(), values.end(), [&](item_t &item) {
        return item.first == key;
      });
    }

    template <typename KEY, typename VALUE>
    inline typename FlatMap<KEY, VALUE>::citerator_t
    FlatMap<KEY, VALUE>::lookup(const KEY &key) const
    {
      return std::find_if(
          values.cbegin(), values.cend(), [&](const item_t &item) {
            return item.first == key;
          });
    }

  }  // namespace containers
}  // namespace rkcommon
