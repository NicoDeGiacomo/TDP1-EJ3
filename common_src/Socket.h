#ifndef COMMON_SRC_SOCKET_H_
#define COMMON_SRC_SOCKET_H_

#include <exception>
#include <string>
#include <utility>
class Socket {
 private:
  int fd;
  explicit Socket(int fd);
  static struct addrinfo* get_addresses(const char* port, const char* ip);

 public:
  Socket();
  ~Socket();
  Socket(Socket &&other) noexcept;
  Socket& operator=(Socket&& other) noexcept;

  int bind(const char* port);
  int listen(int size) const;
  Socket accept() const;
  unsigned int send(const char *buffer, unsigned int size) const;
  void receive(char* buffer, unsigned int size) const;
  void connect(const char *port, const char *name);
  void shutdown();
};

class SocketException;

struct ClosedSocketException;

/****************** IMPLEMENTATION ******************/
class SocketException: public std::exception {
 private:
  std::string msg_;
 public:
  explicit SocketException(std::string message) : msg_(std::move(message)) {}
  const char* what() const noexcept override {
      return msg_.c_str();
  }
};

struct ClosedSocketException : public SocketException {
  ClosedSocketException() : SocketException("The socket is closed.") {}
};

#endif  // COMMON_SRC_SOCKET_H_
