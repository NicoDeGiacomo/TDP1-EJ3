#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "tests/doctest/doctest.h"
#include "BlockingQueue.h"
#include "ProtectedHashMap.h"
#include "QueueManager.h"

TEST_SUITE("Blocking Queue Tests") {
TEST_CASE("Produce and consume") {
    BlockingQueue<std::string> queue;

    queue.produce("value1");
    CHECK_EQ(queue.top(), "value1");
}

TEST_CASE("Produce, consume and remove") {
    BlockingQueue<std::string> queue;

    queue.produce("value1");
    queue.produce("value2");

    CHECK_EQ(queue.top(), "value1");
    queue.pop();
    CHECK_EQ(queue.top(), "value2");
}

TEST_CASE("Multiple items") {
    BlockingQueue<std::string> queue;

    queue.produce("value1");
    queue.produce("value2");
    queue.produce("value3");
    queue.produce("value4");
    queue.produce("value5");
    queue.produce("value6");

    CHECK_EQ(queue.top(), "value1");
    queue.pop();
    CHECK_EQ(queue.top(), "value2");
    queue.pop();
    CHECK_EQ(queue.top(), "value3");
    queue.pop();
    CHECK_EQ(queue.top(), "value4");
    queue.pop();
    CHECK_EQ(queue.top(), "value5");
    queue.pop();
    CHECK_EQ(queue.top(), "value6");
}

TEST_CASE("Close") {
    BlockingQueue<std::string> queue;

    queue.produce("value1");
    queue.close();

    CHECK_THROWS_AS(queue.produce("value2"), ClosedQueueException);
    CHECK_EQ(queue.top(), "value1");
    queue.pop();
    CHECK_THROWS_AS(queue.top(), ClosedQueueException);
    CHECK_THROWS_AS(queue.pop(), ClosedQueueException);
}
}

TEST_SUITE("Protected HashMap Tests") {
TEST_CASE("Put and get") {
    ProtectedHashMap<std::string, std::string> map;

    CHECK_THROWS_AS(map.get("key1"), std::out_of_range);

    map.putIfNotExists("key1", "value1");
    CHECK_NOTHROW(map.get("key1"));
    CHECK_EQ(map.get("key1"), "value1");
}

TEST_CASE("Empty") {
    ProtectedHashMap<std::string, std::string> map;

    CHECK(map.empty());
    map.putIfNotExists("key1", "value1");
    CHECK(!map.empty());
}

TEST_CASE("Double put") {
    ProtectedHashMap<std::string, std::string> map;

    map.putIfNotExists("key1", "value1");
    map.putIfNotExists("key1", "value2");
    CHECK_EQ(map.get("key1"), "value1");
}

TEST_CASE("Remove") {
    ProtectedHashMap<std::string, std::string> map;

    map.putIfNotExists("key1", "value1");
    map.remove("key1");
    CHECK_THROWS_AS(map.get("key1"), std::out_of_range);
}

TEST_CASE("Multiple items") {
    ProtectedHashMap<std::string, std::string> map;

    map.putIfNotExists("key1", "value1");
    map.putIfNotExists("key2", "value2");
    map.putIfNotExists("key3", "value3");
    map.putIfNotExists("key4", "value4");
    map.putIfNotExists("key5", "value5");
    map.putIfNotExists("key6", "value6");

    CHECK_EQ(map.get("key1"), "value1");
    CHECK_EQ(map.get("key2"), "value2");
    CHECK_EQ(map.get("key3"), "value3");
    CHECK_EQ(map.get("key4"), "value4");
    CHECK_EQ(map.get("key5"), "value5");
    CHECK_EQ(map.get("key6"), "value6");

}
}

TEST_SUITE("QueueManager") {

TEST_CASE("Add and remove queue") {
    QueueManager<std::string> queue_manager;

    queue_manager.addQueue("queue1");
    queue_manager.getQueue("queue1")->produce("value1");

    CHECK_EQ(queue_manager.getQueue("queue1")->top(), "value1");

    queue_manager.removeQueue("queue1");

    CHECK_THROWS_AS(queue_manager.getQueue("queue1"), std::out_of_range);
}

TEST_CASE("Remove queue") {
    QueueManager<std::string> queue_manager;

    queue_manager.addQueue("queue1");
    queue_manager.getQueue("queue1")->produce("value1");

    queue_manager.removeQueue("queue1");
    queue_manager.addQueue("queue1");
    queue_manager.getQueue("queue1")->produce("value2");

    CHECK_EQ(queue_manager.getQueue("queue1")->top(), "value2");
}

TEST_CASE("Add queues and values") {
    QueueManager<std::string> queue_manager;

    queue_manager.addQueue("queue1");
    queue_manager.getQueue("queue1")->produce("value1");
    queue_manager.addQueue("queue2");
    queue_manager.getQueue("queue2")->produce("value2a");
    queue_manager.getQueue("queue2")->produce("value2b");
    queue_manager.addQueue("queue3");
    queue_manager.getQueue("queue3")->produce("value3");

    CHECK_EQ(queue_manager.getQueue("queue1")->top(), "value1");
    CHECK_EQ(queue_manager.getQueue("queue2")->top(), "value2a");
    queue_manager.getQueue("queue2")->pop();
    CHECK_EQ(queue_manager.getQueue("queue2")->top(), "value2b");
    CHECK_EQ(queue_manager.getQueue("queue3")->top(), "value3");
}
}
