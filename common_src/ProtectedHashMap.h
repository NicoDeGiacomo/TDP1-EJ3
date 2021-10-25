#ifndef COMMON_SRC_PROTECTEDHASHMAP_H_
#define COMMON_SRC_PROTECTEDHASHMAP_H_

#include <unordered_map>
#include <algorithm>
#include <utility>

/// Implementation of a Blocking HashMap.
/// \tparam K Content type for keys.
/// \tparam V Content type for values.
template <typename K, typename V>
class ProtectedHashMap {
  std::mutex mutex_;
  std::unordered_map<K, V> map_;

 public:
  ProtectedHashMap() = default;
  virtual ~ProtectedHashMap() = default;

  bool putIfNotExists(K key, V const &value);
  V get(K key) const;
  void remove(K key);
  bool empty() const;
  void clear();
  void forEach(void func(V));
};

/****************** IMPLEMENTATION ******************/
template<typename K, typename V>
bool ProtectedHashMap<K, V>::putIfNotExists(const K key, V const &value) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!map_.count(key)) {
        map_.emplace(key, value);
        return true;
    }

    return false;
}

template<typename K, typename V>
V ProtectedHashMap<K, V>::get(const K key) const {
    return map_.at(key);
}

template<typename K, typename V>
void ProtectedHashMap<K, V>::remove(K key) {
    std::unique_lock<std::mutex> lock(mutex_);
    map_.erase(key);
}

template<typename K, typename V>
bool ProtectedHashMap<K, V>::empty() const {
    return map_.empty();
}

template<typename K, typename V>
void ProtectedHashMap<K, V>::clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    map_.clear();
}

template<typename K, typename V>
void ProtectedHashMap<K, V>::forEach(void func(V)) {
    std::unique_lock<std::mutex> lock(mutex_);
    std::for_each(map_.begin(), map_.end(), [&func](std::pair<K, V> pair) {
      func(pair.second);
    });
}

#endif  // COMMON_SRC_PROTECTEDHASHMAP_H_
