#ifndef COMMON_SRC_THREAD_H_
#define COMMON_SRC_THREAD_H_

#include <thread>  // NOLINT [build/c++11]

/// Implementation of a Thread.
class Thread {
  std::thread thread_;

  /// Function to run_ on the Thread.
  virtual void run_() = 0;

 public:
  /// Creates the Thread.
  Thread() = default;

  /// Destroys the Thread.
  virtual ~Thread() = default;

  /// Start the Thread.
  void start();

  /// Join the Thread.
  void join();
};

#endif  // COMMON_SRC_THREAD_H_
