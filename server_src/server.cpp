#include "server.h"

#include <sys/syslog.h>
#include <cstdio>
#include <string>
#include <atomic>
#include <list>
#include <utility>
#include <vector>

#include "QueueManager.h"
#include "../common_src/Socket.h"
#include "../common_src/Protocol.h"
#include "Thread.h"

class ClientThread : public Thread {
  std::atomic<bool> keep_talking_;
  std::atomic<bool> is_running_;
  Protocol protocol_;
  QueueManager<std::string>* manager_;

  void run_() override {
      try {
          while (keep_talking_) {
              char command = protocol_.getCommand();
              std::string queue = protocol_.get_word();

              if (command == COMMAND_DEFINE) {
                  manager_->addQueue(queue);
              }
              if (command == COMMAND_PUSH) {
                  manager_->getQueue(queue)->produce(protocol_.get_word());
              }
              if (command == COMMAND_POP) {
                  std::string word = manager_->getQueue(queue)->top();
                  manager_->getQueue(queue)->pop();
                  protocol_.send_word(word);
              }
          }
          is_running_ = false;
      } catch (ClosedSocketException &e) {
          is_running_ = false;
          return;
      } catch (std::out_of_range &e) {}
  }

 public:
  explicit ClientThread(Socket peer, QueueManager<std::string> *manager)
      : keep_talking_(true), is_running_(true),
        protocol_(std::move(peer)), manager_(manager) {}

  void stop() {
      keep_talking_ = false;
      protocol_.shutdown();
  }

  bool isRunning() {
      return is_running_;
  }
};

class AcceptorThread : public Thread {
 private:
  std::vector<ClientThread*> clients_;
  Socket socket_;

  void accept_client_(QueueManager<std::string> *manager) {
      Socket peer = socket_.accept();
      auto* client = new ClientThread(std::move(peer), manager);
      client->start();
      clients_.push_back(client);
  }

  void clean_clients_() {
      for (auto i = clients_.begin(); i != clients_.end();) {
          if (!(*i)->isRunning()) {
              (*i)->stop();
              (*i)->join();
              delete (*i);
              i = clients_.erase(i);
          } else {
              ++i;
          }
      }
  }

  void stop_clients_() {
      for (auto i = clients_.begin(); i != clients_.end();) {
          (*i)->stop();
          (*i)->join();
          delete (*i);
          i = clients_.erase(i);
      }
  }

  void run_() override {
      QueueManager<std::string> manager;
      while (true) {
          try {
              accept_client_(&manager);
              clean_clients_();
          } catch (ClosedSocketException &e) {
              syslog(LOG_INFO, "[SERVER] %s", e.what());
              break;
          }
      }
      stop_clients_();
  }

 public:
  explicit AcceptorThread(const char *port) : clients_() {
      socket_.bind(port);
      socket_.listen(10);
  }

  ~AcceptorThread() override {
      stop();
      stop_clients_();
  }

  void stop() {
      socket_.shutdown();
  }
};

void run_server_(const char *port) {
    AcceptorThread acceptor(port);
    acceptor.start();

    while (getchar() != COMMAND_EXIT) continue;

    acceptor.stop();
    acceptor.join();
}

int run(const char *port) {
    try {
        run_server_(port);
    } catch (SocketException &e) {
        closelog();
        return 2;
    }

    closelog();
    return EXIT_SUCCESS;
}
