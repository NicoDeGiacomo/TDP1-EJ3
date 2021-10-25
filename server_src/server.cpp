#include "server.h"

#include <sys/syslog.h>
#include <cstdio>
#include <string>
#include <atomic>
#include <list>
#include <utility>

#include "../common_src/QueueManager.h"
#include "../common_src/Socket.h"
#include "../common_src/Protocol.h"
#include "../common_src/Thread.h"

class ClientThread : public Thread {
  std::atomic<bool> keep_talking_;
  std::atomic<bool> is_running_;
  Protocol protocol_;
  QueueManager<std::string>* manager_;

 public:
  explicit ClientThread(Socket peer, QueueManager<std::string> *manager)
      : keep_talking_(true), is_running_(true),
        protocol_(std::move(peer)), manager_(manager) {}

  void run() override {
      try {
          while (keep_talking_) {
              char command = protocol_.getCommand();
              std::string queue = protocol_.get_word();

              if (command == COMMAND_DEFINE) {
                  manager_->addQueue(queue);
                  continue;
              }

              if (command == COMMAND_PUSH) {
                  manager_->getQueue(queue)->produce(protocol_.get_word());
                  continue;
              }

              if (command == COMMAND_POP) {
                  std::string word = manager_->getQueue(queue)->top();
                  manager_->getQueue(queue)->pop();
                  protocol_.send_word(word);
                  continue;
              }
          }
          is_running_ = false;
      } catch (ClosedSocketException &e) {
          is_running_ = false;
          return;
      }
  }

  void stop() {
      keep_talking_ = false;
      protocol_.shutdown();
  }

  bool isRunning() {
      return is_running_;
  }
};

class AcceptorThread : public Thread {
  std::list<ClientThread*> clients;
  Socket socket_;

 public:
  explicit AcceptorThread(const char *port) : clients() {
      socket_.bind(port);
      socket_.listen(10);
  }

  void run() override {
      auto* manager = new QueueManager<std::string>;
      while (true) {
          try {
              Socket peer = socket_.accept();
              auto* client = new ClientThread(std::move(peer), manager);
              client->start();

              clients.push_back(client);

              for (auto i = clients.begin(); i != clients.end();) {
                  if (!(*i)->isRunning()) {
                      (*i)->stop();
                      (*i)->join();
                      delete (*i);
                      i = clients.erase(i);
                  } else {
                      i++;
                  }
              }
          } catch (ClosedSocketException &e) {
              syslog(LOG_INFO, "[SERVER] %s", e.what());
              break;
          }
      }

      for (auto & client : clients) {
          client->stop();
          client->join();
          delete client;
      }

      delete manager;
      stop();
  }

  void stop() {
      socket_.shutdown();
  }
};

int run(const char *port) {
    AcceptorThread acceptor(port);
    acceptor.start();

    while (true) {
        int c = std::getchar();
        if (c == COMMAND_EXIT) {
            break;
        }
    }

    acceptor.stop();
    acceptor.join();
    closelog();
    return EXIT_SUCCESS;
}
