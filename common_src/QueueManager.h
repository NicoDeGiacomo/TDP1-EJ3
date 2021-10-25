#ifndef COMMON_SRC_QUEUEMANAGER_H_
#define COMMON_SRC_QUEUEMANAGER_H_

#include <string>
#include "BlockingQueue.h"
#include "ProtectedHashMap.h"

/// Implementation (thread-safe) of a HashMap of BlockingQueues.
/// \tparam T Type of the elements contained in the BlockingQueues.
template <typename T>
class QueueManager {
  std::mutex mutex_;
  ProtectedHashMap<std::string, BlockingQueue<T> *> queueMap_;

 public:
  /// Creates the manager.
  QueueManager() = default;

  /// Destroys the manager.
  virtual ~QueueManager();

  /// Adds a Queue associated with a Key,
  /// if it doesnt already exist in the Manager.
  /// \param name Key to add.
  void addQueue(std::string name);

  /// Removes the Queue associated with the Key.
  /// \param name Key to remove.
  void removeQueue(std::string name);

  /// Returns the Queue associated with the Key.
  /// \param name Key to retrieve.
  /// \return A BlockingQueue.
  /// \throw Exceptions thrown by the ProtectedHashMap.
  BlockingQueue<T>* getQueue(std::string name);
};

/****************** IMPLEMENTATION ******************/
template<typename T>
void QueueManager<T>::addQueue(std::string name) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto* queue = new BlockingQueue<T>();
    if (!queueMap_.putIfNotExists(name, queue)) {
        delete queue;
    }
}

template<typename T>
void QueueManager<T>::removeQueue(std::string name) {
    std::unique_lock<std::mutex> lock(mutex_);
    BlockingQueue<T>* queue = getQueue(name);
    delete queue;
    queueMap_.remove(name);
}

template<typename T>
BlockingQueue<T>* QueueManager<T>::getQueue(std::string name) {
    return queueMap_.get(name);
}
template<typename T>
QueueManager<T>::~QueueManager() {
    queueMap_.forEach([](BlockingQueue<T> *queue) {
      delete queue;
    });
    queueMap_.clear();
}

#endif  // COMMON_SRC_QUEUEMANAGER_H_
