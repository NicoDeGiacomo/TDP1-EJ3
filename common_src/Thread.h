#ifndef COMMON_SRC_THREAD_H_
#define COMMON_SRC_THREAD_H_

#include <thread>  // NOLINT [build/c++11]

class Thread {
  std::thread thread_;

 public:
  Thread() = default;
  virtual ~Thread() = default;
  virtual void run() = 0;
  void start();
  void join();
};

#endif  // COMMON_SRC_THREAD_H_
