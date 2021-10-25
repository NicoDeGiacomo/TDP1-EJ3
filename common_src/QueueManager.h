#ifndef COMMON_SRC_QUEUEMANAGER_H_
#define COMMON_SRC_QUEUEMANAGER_H_

#include <string>

#include "BlockingQueue.h"
#include "ProtectedHashMap.h"

template <typename T>
class QueueManager {
  std::mutex mutex_;
  ProtectedHashMap<std::string, BlockingQueue<T> *> queueMap_;

 public:
  QueueManager() = default;
  virtual ~QueueManager();

  void addQueue(std::string name);
  void removeQueue(std::string name);
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
    std::unique_lock<std::mutex> lock(mutex_);
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
