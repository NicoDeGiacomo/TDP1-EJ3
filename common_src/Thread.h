#ifndef COMMON_SRC_THREAD_H_
#define COMMON_SRC_THREAD_H_

#include <thread>  // NOLINT [build/c++11]

/// Implementation of a Thread.
class Thread {
  std::thread thread_;

 public:
  /// Creates the Thread.
  Thread() = default;

  /// Destroys the Thread.
  virtual ~Thread() = default;

  /// Start the Thread.
  void start();

  /// Join the Thread.
  void join();

  /// Function to run on the Thread.
  virtual void run() = 0;
};

#endif  // COMMON_SRC_THREAD_H_
