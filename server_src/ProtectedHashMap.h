#ifndef SERVER_SRC_PROTECTEDHASHMAP_H_
#define SERVER_SRC_PROTECTEDHASHMAP_H_

#include <mutex>  // NOLINT [build/c++11]
#include <unordered_map>
#include <algorithm>
#include <utility>

/// Implementation of a Protected HashMap.
/// \tparam K Type for keys.
/// \tparam V Type for values.
template <typename K, typename V>
class ProtectedHashMap {
  std::mutex mutex_;
  std::unordered_map<K, V> map_;

 public:
  /// Creates the Protected HashMap.
  ProtectedHashMap() = default;

  /// Destroys the Protected HashMap.
  virtual ~ProtectedHashMap() = default;

  /// Adds an element to the Map, if the key doesn't already exist.
  /// \param key Key to add.
  /// \param value Element to add.
  /// \return True if the element was added, false otherwise.
  bool putIfNotExists(K key, V const &value);

  /// Returns an element from the Map.
  /// \param key Key to retrieve.
  /// \return Element associated with the key.
  /// \throws out_of_range If no such data is present.
  V get(K key) const;

  /// Removes an element from the map.
  /// \param key Key to remove.
  void remove(K key);

  /// Checks if the map is empty.
  /// \return True if the map is empty, false otherwise.
  bool empty() const;

  /// Removes all the elements from the map.
  void clear();

  /// Applies a function to every element int he map.
  /// \param func Unary function that will receive each value
  /// stored in the map.
  void forEach(void func(V));
};

/****************** IMPLEMENTATION ******************/
template<typename K, typename V>
bool ProtectedHashMap<K, V>::putIfNotExists(const K key, V const &value) {
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
    map_.erase(key);
}

template<typename K, typename V>
bool ProtectedHashMap<K, V>::empty() const {
    return map_.empty();
}

template<typename K, typename V>
void ProtectedHashMap<K, V>::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    map_.clear();
}

template<typename K, typename V>
void ProtectedHashMap<K, V>::forEach(void func(V)) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::for_each(map_.begin(), map_.end(), [&func](std::pair<K, V> pair) {
      func(pair.second);
    });
}

#endif  // SERVER_SRC_PROTECTEDHASHMAP_H_
