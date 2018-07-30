
/**@file*/

#ifndef __NOTSTD_BIDIR_MAP__
#define __NOTSTD_BIDIR_MAP__

#include <type_traits>
#include <utility>
#include <memory>
#include <functional>
#include <unordered_map>

#include "notstdtypetraits.h"

namespace notstd {
/**bidirectional map that can be backed by any map class
 * all calls are the same as std::*map (eg std::unordered_map) just with added reverse_*
 * functions
 *
 * @tparam MapType a template template parameter of the backing map type
 * @tparam Key key type
 * @tparam T the value type
 * @tparam Hash the hasher (defaults to std::hash<Key>)
 * @tparam Pred the Predicate (equivalence test) (defaults to std::equal_to<Key>)
 * @tparam Alloc the allocation scheme (defaults to std::allocator<std::pair<const Key, T>>)
 *
 * */
template<template<class, class, class, class, class> class MapType, class Key, class T, class Hash = std::hash<Key>, class Pred = std::equal_to<
    Key>, class Alloc = std::allocator<std::pair<const Key, T>>>
class bidir_map {
 public:
  /**forward map type*/
  using MapType_t = MapType<Key, T, Hash, Pred, Alloc>;
  /**reverse lookup map type*/
  using MapType_r_t = MapType<T, Key, Hash, Pred, Alloc>;

  //TODO more than just two constructors
  //Constructors
  bidir_map() {
  }

  /**constructor that takes in non default hasher, predicate, allocator, and size*/
  bidir_map(size_t n,
            const typename MapType_t::hasher& hf = typename MapType_t::hasher{},
            const typename MapType_t::key_equal& eql = typename MapType_t::key_equal{},
            const typename MapType_t::allocator_type& alloc = typename MapType_t::allocator_type{}) :
      m_forwardMap(MapType_t(n, hf, eql, alloc)) {
    static auto hasNeededMembers = is_valid([](auto&& t) -> decltype(
    t.size(),
        t.max_size(),
        t.begin(),
        t.cbegin(),
        t.end(),
        t.cend(),
        t.operator[](std::declval<Key>()),
        t.at(std::declval<const Key>()),
        t.find(std::declval<const Key>()),
        /*t.insert(std::declval<std::pair<const Key, T>()),*/ //I think this is a bug, casuses compiler to lose heap space
        t.erase(std::declval<const Key>()),
        t.clear()) {});

    static_assert(decltype(hasNeededMembers(m_forwardMap))::value,
                  "MapType backing bidir_map does not have all the needed member functions");
    m_reverseMap = MapType_r_t(n, hf, eql, alloc);
  }

  // Capacity
  bool empty() const noexcept {
    return m_forwardMap.empty();
  }
  size_t size() const noexcept {
    return m_forwardMap.size();
  }
  size_t max_size() const noexcept {
    return m_forwardMap.max_size();
  }

  // Iterators
  auto begin() noexcept {
    return m_forwardMap.begin();
  }
  auto reverse_begin() noexcept {
    return m_reverseMap.begin();
  }
  auto end() noexcept {
    return m_forwardMap.end();
  }
  auto reverse_end() noexcept {
    return m_reverseMap.end();
  }
  auto cbegin() noexcept {
    return m_forwardMap.cbegin();
  }
  auto reverse_cbegin() noexcept {
    return m_reverseMap.cbegin();
  }
  auto cend() noexcept {
    return m_forwardMap.cend();
  }
  auto reverse_cend() noexcept {
    return m_reverseMap.cend();
  }

  // Access
  const T& at(const Key& k) const {
    return m_forwardMap.at(k);
  }
  const Key& reverse_at(const T& v) const {
    return m_reverseMap.at(v);
  }
  auto find(const Key& k) {
    return m_forwardMap.find(k);
  }
  auto find(const Key& k) const {
    return m_forwardMap.find(k);
  }
  auto reverse_find(const T& v) {
    return m_reverseMap.find(v);
  }
  auto reverse_find(const T& v) const {
    return m_reverseMap.find(v);
  }

  // Modification
  void put(const Key& k, const T& v) {
    m_forwardMap[k] = v;
    m_reverseMap[v] = k;
  }
  void erase(const Key& k) {
    auto v = m_forwardMap[k];
    m_forwardMap.erase(k);
    m_reverseMap.erase(v);
  }
  void reverse_erase(const T& v) {
    auto k = m_reverseMap[v];
    m_forwardMap.erase(k);
    m_reverseMap.erase(v);
  }
  void clear() {
    m_forwardMap.clear();
    m_reverseMap.clear();
  }

 private:
  MapType_t m_forwardMap;   ///< The forward map
  MapType_r_t m_reverseMap; ///< the reverse map for inverse O(1) lookup
};

/**specialization for unorderd_map backed bidir_map*/
template<class Key, class T, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>, class Alloc = std::allocator<
    std::pair<const Key, T> > >
using unordered_bidir_map = bidir_map<std::unordered_map, Key, T, Hash, Pred, Alloc>;
} /*notstd*/

#endif
