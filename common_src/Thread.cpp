#include "Thread.h"

void Thread::start() {
    thread_ = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread_.join();
}
