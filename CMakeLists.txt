cmake_minimum_required(VERSION 3.20)
project(TDP1_EJ3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(./common_src)
include_directories(./client_src)
include_directories(./server_src)
include_directories(./tests)
include_directories(./tests/doctest)

add_executable(CLIENT
        client_src/client_main.cpp client_src/client.cpp client_src/client.h common_src/QueueManager.h common_src/BlockingQueue.h common_src/ProtectedHashMap.h common_src/Socket.cpp common_src/Socket.h common_src/Protocol.cpp common_src/Protocol.h common_src/Thread.cpp common_src/Thread.h)

add_executable(SERVER
        server_src/server_main.cpp server_src/server.cpp server_src/server.h common_src/QueueManager.h common_src/BlockingQueue.h common_src/ProtectedHashMap.h common_src/Socket.cpp common_src/Socket.h common_src/Protocol.cpp common_src/Protocol.h common_src/Thread.cpp common_src/Thread.h)

add_executable(TESTS
        tests/Tests.cpp common_src/BlockingQueue.h common_src/ProtectedHashMap.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(CLIENT Threads::Threads)
target_link_libraries(SERVER Threads::Threads)
