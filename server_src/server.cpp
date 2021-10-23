#include "server.h"

#include <cstdio>
#include <string>
#include <atomic>
#include <list>

#include "QueueManager.h"
#include "Socket.h"
#include "Protocol.h"
#include "Thread.h"

class ClientThread : public Thread {
  std::atomic<bool> keep_talking_;
  std::atomic<bool> is_running_;
  Socket peer_;
  Protocol protocol_;
  QueueManager<std::string>* manager_;

 public:
  explicit ClientThread(Socket peer, QueueManager<std::string> *manager)
      : keep_talking_(true), is_running_(true), peer_(peer),
        protocol_(peer), manager_(manager) {}

  void run() override {
      try {
          while (keep_talking_) {
              char command = protocol_.getCommand();
              std::string queue = protocol_.get_word();

              if (command == 'd') {
                  manager_->addQueue(queue);
                  continue;
              }

              if (command == 'u') {
                  manager_->getQueue(queue)->produce(protocol_.get_word());
                  continue;
              }

              if (command == 'o') {
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
      peer_.shutdown();
  }

  bool isRunning() {
      return is_running_;
  }
};

class AcceptorThread : public Thread {
  std::list<ClientThread*> clients;
  Socket socket_;

 public:
  explicit AcceptorThread(const char *port) {
      socket_.bind(port);
      socket_.listen(10);
  }

  void run() override {
      while (true) {
          try {
              Socket peer = socket_.accept();
              auto* manager = new QueueManager<std::string>;
              auto* client_thread = new ClientThread(peer, manager);

              clients.push_back(client_thread);

              for (auto i = clients.begin(); i != clients.end();) {
                  if (!(*i)->isRunning()) {
                      (*i)->join();
                      delete (*i);
                      i = clients.erase(i);
                  } else {
                      i++;
                  }
              }
          } catch (ClosedSocketException &e) {
              for (auto i = clients.begin(); i != clients.end();) {
                  (*i)->stop();
                  (*i)->join();
                  delete (*i);
              }
              return;
          }
      }
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
        if (c == 'q') {
            acceptor.stop();
            return EXIT_SUCCESS;  // todo error codes?
        }
    }
}
